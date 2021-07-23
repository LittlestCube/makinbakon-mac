/* -- gettext.cpp -- */

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
 All lessons, lesson intro's and help texts are held in text files which are 
 waiting to have their dubious contents converted into strings by Gettext...
 ---------------------------------------------------------------------------*/

#include "gettext.h"
#include "macros.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <new>
#include <cstdlib>
#include <string>

using namespace std;

// initialise gInstance statics
Gettext* Gettext::gInstance = 0;
Destroyer<Gettext> Gettext::destroyer;

// create singleton instance
Gettext* Gettext::Instance() throw (Error) {
    if (gInstance == 0) {
	if ((gInstance = new (std::nothrow) Gettext) == 0) {
	    throw Error("can't create gettext instance: gettext.cpp");
	}
	destroyer.setDoomed(gInstance);
    }
    return gInstance;
}

// read part of the file into a string
void Gettext::readPart(string location, int n, string& text) throw(Error) {
    
    stringstream inum;        // lesson we search for
    stringstream next;        // find next lesson (i + 1)
    size_t found = 0;   // location in string
    bool gotya = false;       // if found, gotya = true
    string temp;              // holds getline strings
    ifstream in;              // for reading file at location
    
    text = "";    // always start with an empty string
    
    // open file for reading (set stream ptr to beginning)
    in.open(location.c_str(), ios::in);

    if (in.bad())
	throw Error("can't open " + location + " for reading: gettext.cpp");
    
    inum << "//" << n << " ";
    next << "//" << n+1 << " ";

    // just pull out the piece we want
    while (!in.eof()) {
	getline(in, temp);
	if (!gotya) {
	    if ((found = temp.find(inum.str(), 0)) != temp.npos) {
		gotya = true;
	    }
	}
	else {
	    if ((found = temp.find(next.str(), 0)) != temp.npos) {
		break;
	    }
	    text += temp + '\n';
	}
    }
    in.close();

    if (text.size() < 1) {
	throw Error(location + ": text string has zero length: gettext.cpp");
    }
}

// read the whole file into a string
void Gettext::readWhole(string location, string& text) throw(Error) {
    
    size_t comment = 0;    // position of comments
    size_t nline = 0;      // position of \n after comment
    string temp;                 // holds getline strings
    ifstream in;                 // for reading file at location
    
    text = "";    // always start with an empty string
    
    // open file for reading (set stream ptr to beginning)
    in.open(location.c_str(), ios::in);

    if (in.bad())
	throw Error("can't open " + location + " for reading: gettext.cpp");
    
    while (!in.eof()) {
	getline(in, temp);
	// getline removes '\n' so we append
	text += temp + '\n';
    }
    in.close();

    // remove comments from str
    comment = str.find("//");
    while (comment != str.npos) {
	if ((nline = str.find('\n', comment)) != str.npos) {
	    // if comment = 0 and nline == 2 erase nline + 1
	    str.erase(comment, nline+1);
	}
	comment++;
	comment = str.find("//");
    }
    if (text.size() < 1) {
	throw Error(location + ": text string has zero length: gettext.cpp");
    }
}

// return welcome screen text
string& Gettext::getWelcome() { 
    
    // where to get the string
    string location(DATA);
    location += D_WELCOME;
    // get the welcome text
    readWhole(location, str);
    // remove unwanted whitespace != '\n\n'
    formatOne(str);

    return str;
}

// return help text
string& Gettext::getHelp() { 
    
    // where to get the text
    string location(DATA);
    location += D_HELP;
    // get the help text
    readWhole(location, str);
    // remove unwanted whitespace != '\n\n'
    formatOne(str);

    return str;
}

// return the Pig Tip
string& Gettext::getTips() { 
    
    // where to get the string
    string location(DATA);
    location += D_PIGTIPS;
    // get the tips sting
    readWhole(location, str);
    // remove unwanted space characters
    formatTwo(str);

    return str;
}

// return 'i' lesson intro text
string& Gettext::getIntro(int i) {

    // where to get the string    
    string location(DATA);
    location += D_INTROS;
    
    // lesson passes parameter 'i' as const
    int n = i;
    
    // get lesson index if n == 0: useful
    // if menu index is zero in scrMenu()
    if (n == 0x0) {
	n = gscore->getLesson();
	n = GETINDEX(n);
    }
    // get the intro text
    readPart(location, n, str);
    // remove unwanted space characters
    formatTwo(str);

    return str;
}

// return 'i' lesson text
string& Gettext::getLesson(int i) { 
    
    // where to get the string
    string location(DATA);
    location += D_LESSONS;
    // get the lesson string
    readPart(location, i, str);
    // remove unwanted whitespace != '\n\n'
    for (unsigned int i = 0; i < str.size(); i++) {
	if (str[i] == '\n' && str[i+1] == '\n') {
	    str.erase(i, 1);
	    i += 1;
	}
	else if (str[i] == '\n' && str[i+1] != '\n')
	    str[i] = ' ';
    }
    // now remove any surplus space characters
    for (unsigned int i = 0; i < str.size(); i++) {
        if ((str[i] == ' ' && str[i+1] == ' ') || (i == 0 && str[i] == ' ')) {
	    str.erase(i, 1);
	    i--;
	}
    }
    return str;
}

// return randomly selected speed test text
string& Gettext::getSpeed() { 

    // where to get the string
    string location(DATA);
    location += D_SPEED;
    
    // get one of the tests
    if (!gotnum) {
	srand(time(NULL));
	snum = (rand() % RSPEED) + 1;
	gotnum = true;
    }
    // get the speed string
    readPart(location, snum, str);

    // remove any surplus space characters
    for (unsigned int i = 0; i < str.size(); i++) {
        if ((str[i] == ' ' && str[i+1] == ' ') || (i == 0 && str[i] == ' ')) {
	    str.erase(i, 1);
	    i--;
	}
    }
    // give two spaces between each substring and remove the last whitespace
    for (unsigned int i = 0; i < str.size(); i++) {
	if (isspace(str[i]))
	    str[i] = ' ';
	if (i == str.size()-1 && isspace(str[i]))
	    str.erase(i, 1);
    }
    // finally, increment snum
    if (snum == TSPEED) snum = 1;
    else snum++;
    
    return str;
}

// to keep layout style only remove surplus whitespace != '\n\n'
void Gettext::formatOne(string& str) {

    for (unsigned int i = 0; i < str.size(); i++) {
	if (str[i] == '\n' && str[i+1] == '\n')
	    i += 1;
	else if (str[i] == '\n' && str[i+1] != '\n')
	    str[i] = ' ';
    }
    for (unsigned int i = 0; i < str.size(); i++) {
        if ((str[i] == ' ' && str[i+1] == ' ') || (i == 0 && str[i] == ' ')) {
	    str.erase(i, 1);
	    i--;
	}
    }
}

// remove ALL surplus whitespace (newlines, tabs...)
void Gettext::formatTwo(string& str) {

    for (unsigned int i = 0; i < str.size(); i++) {
	if (isspace(str[i])) {
	    str[i] = ' ';
	}
    }
    for (unsigned int i = 0; i < str.size(); i++) {
        if ((str[i] == ' ' && str[i+1] == ' ') || (i == 0 && str[i] == ' ')) {
	    str.erase(i, 1);
	    i--;
	}
    }
}
