/* -- scores.cpp -- */

/*****************************************************************************
 *                         Makin' Bakon Typing Tutor                         *
 *		               version 0.3.2                                 *
 *		    Copyright (C) 2002 2003 Stephen Webster                  *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU General Public License as published by the    *
 *  Free Software Foundation; either version 2 of the License, or (at your   *
 *  option) any later version.                                               *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful, but      * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 *  See the GNU General Public License for more details.                     *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License along  *
 *  with this program; if not, write to the Free Software Foundation, Inc.,  *
 *  59 Temple Place, Suite 330, Boston, MA 0.1.2-1307 USA                    *
 *                                                                           *
 *  Please read the COPYING file for the complete license. The copyright     *
 *  holder can be contacted via email: moonunit@ukonline.co.uk               *
 *****************************************************************************/

/*----------------------------------------------------------------------------
 Scores class sets and gets the default lesson num and stats. Scores string
 looks like this:- "L:10 S:33" This translates to: last completed Lesson = 10,
 and last session Stats = 33
 ---------------------------------------------------------------------------*/

#include "scores.h"
#include "macros.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <new>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// initialise tInstance statics
Scores* Scores::sInstance = 0;
Destroyer<Scores> Scores::destroyer;

Scores* Scores::Instance() throw (Error) {
	if (sInstance == 0) {
	if ((sInstance = new (std::nothrow) Scores) == 0) {
		throw Error("can't create scores instance: scores.cpp");
	}
	destroyer.setDoomed(sInstance);
	}
	return sInstance;
}

Scores::Scores() throw (Error) : Gotya(false), Home(0), Snum(0), Lnum(0) { 

	try {
	sty = Pigsty::Instance();
	
	// size of ~/.pigsty/scores address and a null
	int size = 0; size = sty->getSsize() + 1;
	// allocate memory
	if ((Home = new (std::nothrow) char[size]) == 0)
		throw Error("bad_alloc - can't create Home: scores.cpp");
	// get the ~/.pigsty/scores address
	strcpy(Home, sty->getScores());
	}
	catch (Error err) {
	if (Home != 0) {
		delete[] Home;
		Home = 0;
	}
	throw err;
	}
	catch (...) {
	if (Home != 0) {
		delete[] Home;
		Home = 0;
	}
	throw Error("unexpected exception during construction: scores.cpp");
	}
}    

// get Scores string from ~/.pigsty/scores file
void Scores::readScores() throw(Error) {

	string temp;     // store a line of scores file
	Scorestr = "";   // ensure Scorestr is empty
	
	// open Scores file for reading
	ifstream in(Home, ios::in);
	
	if (in.bad()) {
		throw Error("can't open scores file for reading: scores.cpp");
	}
	
	// we only get the one line!
	while (!in.eof()) {
	getline(in, temp);
	size_t found = 0;
	if ((found = temp.find("#")) == temp.npos) {
		Scorestr.append(temp);
	}
	}
	in.close();
	
	// get rid of unwanted whitespace as it has a habbit of accumulating
	string::iterator it;
	
	for (it = Scorestr.begin(); it != Scorestr.end(); it++) {
	if (it == Scorestr.begin() && isspace(*it)) {
		Scorestr.erase(it);
		it--;
	}
	else if (it == (Scorestr.end()-1) && isspace(*it)) {
		Scorestr.erase(it);
		it--;
	}
	}
	Gotya = true;
}

// write Scores string to ~/.pigsty/scores file
void Scores::writeScores() throw(Error) {
	
	// add warning to beginning of Scorestr
	string str("#--- Don't edit this file! ---#\n");
	Scorestr.insert(0, str);
	
	// open file for writing
	ofstream out(Home, ios::out | ios::trunc);
	
	if (out.is_open()) {
	out << Scorestr; 
	out.close();
	}
	else {
	throw Error("can't open scores file for writing: scores.cpp");
	}
	Gotya = false;
}	

int Scores::getLesson()
{
	Lnum = atoi(getAttribute("L").c_str());
	
	// reset to zero if out of bounds
	if (Lnum < LESSONONE || Lnum > LASTLESSON)
	{
		Lnum = 0;
	}
	return Lnum;
}

double Scores::getStats()
{
	Snum = atof(getAttribute("S").c_str());
	
	// reset to zero if < one
	if (Snum < 1) {
		Snum = 0;
	}
	return Snum;
}

void Scores::setLesson(int lesson)
{
	string lessonStr = to_string(lesson);
	setAttribute("L", lessonStr);
}

void Scores::setStats(double stats) {

	string statsStr = to_string(stats);
	setAttribute("S", statsStr);
}

string Scores::getAttribute(string attributeKey)
{
	attributeKey.append(":");
	
	if (!Gotya) readScores();
	
	size_t found = 0;           // find substring
	size_t space = 0;           // find space
	string temp;                // holds attribute to return
	
	if ((found = Scorestr.find(attributeKey, 0)) != Scorestr.npos)
	{
		space = Scorestr.find(" ", found);
		// extract the attribute and assign to temp
		temp.assign(Scorestr, found+2, space-(found+2));
	}
	
	else
	{
		temp = "-1";
	}
	
	return temp;
}

void Scores::setAttribute(string attributeKey, string value)
{
	attributeKey.append(":");
	
	if (!Gotya) readScores();

	size_t found = 0;           // find substring
	size_t space = 0;           // find space
	stringstream temp;          // change num to char
	
	// if lesson score exists then amend
	if ((found = Scorestr.find(attributeKey, 0)) != Scorestr.npos) {
	space = Scorestr.find(" ", found);
	Scorestr.erase(found+2, space-(found+2));
	temp << value;
	Scorestr.insert(found+2, temp.str());
	}
	// if it doesn't exist then insert
	else {
	temp << attributeKey << value << " ";
	Scorestr.insert(0, temp.str());
	}
	writeScores();
}