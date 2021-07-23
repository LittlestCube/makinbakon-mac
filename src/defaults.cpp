/* -- defaults.cpp -- */

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
 Defaults class reads the ~/.bakonrc file for keyboard and color settings.
 ---------------------------------------------------------------------------*/

#include "defaults.h"
#include "macros.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <new>

using namespace std;

// static colors map
MakeMap<string, short> Defaults::colors;
// static defmap map
MakeMap<string, string> Defaults::defmap;

// store address of and read ~/.bakonrc
Defaults::Defaults() throw(Error) : Home(0), Color(0) { 

    try {
	// only fill empty maps
	if (colors.isempty() || defmap.isempty()) {

	    // start a Pigsty instance
	    sty = Pigsty::Instance();
	    
	    // get size of ~/.bakonrc address and add one for the null
	    int size = sty->getBsize() + 1;
	    
	    if ((Home = new (std::nothrow) char[size]) == 0)
		throw Error("bad_alloc - can't create Home: defaults.cpp");
	    
	    strcpy(Home, sty->getBakonrc());
	    
	    // create the colors key value map
	    for (int i = 0; i < CSIZE; i++) {
		colors.additem(CURSKEYS[i], CURSVALS[i]);
	    }
	    readDefaults();
	    delete[] Home;
	    Home = 0;
	}
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
	throw Error("unexpected exception during construction: defaults.cpp");
    }
}

// get Defaults string from ~/.bakonrc file
void Defaults::readDefaults() throw(Error) {

    try {
	// hold ~/.bakonrc defaults
	vector<string> Defstr;

	// open Defaults file for reading
	ifstream in(Home, ios::in); 
	
	if (in.bad())
	    throw Error("can't open ~/.bakonrc file for reading: defaults.cpp");
	
	// fill the defaults string
	while (!in.eof()) {
	    string temp;
	    getline(in, temp);
	    string line;
	    if (temp.size() > 0) {
		for (unsigned int i = 0; i < temp.size(); i++) {
		    if (temp[i] != '#') {
			if (temp[i] != ' ') {
			    line += temp[i];
			}
		    }
		    else break;
		}
		if (line.size() > 0) {
		    Defstr.push_back(line);
		}
	    }
	}
	in.close();
	
	// test default strings and put them into a map
	for (unsigned int i = 0; i < Defstr.size(); i++) {
	    
	    bool gotkey = false;
	    bool gotvalue = false;
	    unsigned int found = 0;
	    
	    int equal = Defstr[i].find('=');
	    string key = Defstr[i].substr(0, equal);
	    // check name is a proper default key
	    for (int z = 0; z < KSIZE; z++) {
		if ((found = key.compare(DEFKEYS[z])) == 0) {
		    gotkey = true;
		    break;
		}
	    }
	    string value = Defstr[i].substr(equal+1);
	    // check value is a proper default value
	    for (int z = 0; z < CSIZE+2; z++) {
		if ((found = value.compare(CURSKEYS[z])) == 0) {
		    gotvalue = true;
		    break;
		}
		else if (((found = UKTYPE.compare(CURSKEYS[z])) == 0) ||
			 ((found = USTYPE.compare(CURSKEYS[z])) == 0)) {
		    gotvalue = true;
		    break;
		}
	    }
	    if (gotkey && gotvalue) {
		defmap.additem(key, value);
	    }
	}
	// set program default if ~/.bakonrc has an invalid/missing entry
	for (int z = 0; z < KSIZE; z++) {
	    
	    int found = 0;
	    
	    if ((found = defmap.findkey(DEFKEYS[z])) == 0) {
		if (DEFKEYS[z] == KBRD) {
		    defmap.setval(DEFKEYS[z], DEFKBRD);
		}
		else defmap.setval(DEFKEYS[z], DEFCOL);
	    }
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception in readDefaults: defaults.cpp");
    }
}

// change an item in the map
void Defaults::setDefault(string key, string value) throw (Error) {
    
    int found = 0;         // return value of compare()
    bool gotya = false;    // true if found == zero
    
    // check for valid key and value
    if ((found = defmap.findkey(key)) != 0) {
	if (key == FORE || key == BACK || key == BOLD) {
	    for (int z = 0; z < CSIZE; z++) {
		if ((found = value.compare(CURSKEYS[z])) == 0) {
		    gotya = true;
		    break;
		}
	    }
	    if (!gotya)	{
		throw Error("invalid value parameter in setDefault: defaults.cpp");
	    }
	}
	else if (key == KBRD) {
	    if ((found = value.compare(USTYPE)) != 0 && (found = value.compare(UKTYPE)) != 0) {
		throw Error("invalid value parameter in setDefault: defaults.cpp");
	    }
	}
    }
    else {
	throw Error("invalid key parameter in setDefault: defaults.cpp");
    }
    defmap.setval(key, value);
}

// get default keyboard type
string& Defaults::getKbtype() throw (Error) { 

    Kbtype = defmap.findval(KBRD);

    // check for valid keyboard type
    if (Kbtype == USTYPE || Kbtype == UKTYPE) {
	return Kbtype;
    }
    else {
	throw Error("makemap returned invalid value to getKbtype: defaults.cpp");
    }
}

// get default color settings
short& Defaults::getColor(string key) throw(Error) {

    // check for valid color type
    if (key == FORE || key == BACK || key == BOLD) {
	string str = defmap.findval(key);
	Color = colors.findval(str);
	if (Color < COLOR_TRANS || Color > COLOR_WHITE) {
	    throw Error("makemap returned invalid value to getColor: defaults.cpp");
	}
	else { return Color; }
    }
    else { throw Error("invalid key parameter in getColor: defaults.cpp"); }
}

