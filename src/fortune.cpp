/* -- fortune.cpp -- */

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
 class Fortune gets the fortune epigrams from the fortune program, formats
 them, and returns them back to lesson. If it can't get the fortune or open
 the epigram file in ~/.pigsty it throws an error.
 ---------------------------------------------------------------------------*/

#include "fortune.h"
#include "keyboard.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <new>
#include <cstring>
#include <cctype>

using namespace std;

Fortune::Fortune(const int i) throw(Error) : styloc(0), command(0), lnum(i) {
    
    try {
	sty = Pigsty::Instance();
    
	// set array size for command target ~/.pigsty/epigram
	int ssize = sty->getEsize() + 1;

	if ((styloc = new (std::nothrow) char[ssize]) == 0)
	    throw Error("bad_alloc - can't create styloc: fortune.cpp");
	
	strcpy(styloc, sty->getEpigram());
	
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (styloc != 0) {
	    delete[] styloc;
	    styloc = 0;
	}
	throw Error("unexpected exception during construction: fortune.cpp");
    }
}

string& Fortune::getFortune() throw(Error) {
    
    Keyboard kboard;           // check for valid characters
    ifstream insty;            // read our epigram
    int fs = 0;                // return value for system
    int size = 0;              // size of epigram
    bool breakflag = false;    // exit loop only for right type of epigram

    // create the command
    if (lnum > 2 && lnum < 7) {
	// set array size for program defined fortune command
	int csize = strlen(ARCHIVE) + strlen(styloc) + 34;
	
	if ((command = new (std::nothrow) char[csize]) == 0) {
	    delete[] styloc;
	    styloc = 0;
	    throw Error("bad_alloc - can't create command: fortune.cpp");
	}
	strcpy(command, "fortune ");
	strcat(command, ARCHIVE);
    }
    else if (lnum == 2) {
	// set array size for standard fortune command
	int csize = strlen(styloc) + 27;
	
	if ((command = new (std::nothrow) char[csize]) == 0) {
	    delete[] styloc;
	    styloc = 0;
	    throw Error("bad_alloc - can't create command: fortune.cpp");
	}
	strcpy(command, "fortune -as > ");
    }
    else {
	delete[] styloc;
	styloc = 0;
	throw Error("bad parameter in constructor: fortune.cpp");
    }
    if (lnum == 3)
	strcat(command, "/poetry > ");
    else if (lnum == 4)
	strcat(command, "/startrek > ");
    else if (lnum == 5)
	strcat(command, "/offensive > ");
    // command for "Distracted Pig"
    else if (lnum == 6) {
        srand(time(NULL));
        int n = (rand() % 3) + 1;
	
        switch (n) {
          case 1: strcat(command, "/offensive > ");
            break;
          case 2: strcat(command, "/startrek > ");
            break;
          case 3: strcat(command, "/poetry > ");
            break;
        }
    }    
    strcat(command, styloc);
    // send error to /dev/null
    strcat(command, " 2>/dev/null");
    
    int again = 0;    // check how many times we've gone round...
    
    while (1) {
	// always start loop with an empty string
	fortune = ""; 
	
	fs = system(command);
	if (fs < 0 || fs > 0)
	    throw Error("can't execute system command: fortune.cpp");

        // open file for reading (set stream ptr to beginning)
        insty.open(styloc, ios::in | ios::ate);
	
        if (insty.bad())
            throw Error("can't open epigram file for reading: fortune.cpp");

	if ((size = insty.tellg()) == 0) {
            // try going round ten times as some fortunes are buggy...
	    insty.close();
	    if (again > 9)
		throw Error("fortune command returned null: fortune.cpp");
	    else {
		again += 1;
		continue;
	    }
	}

	insty.seekg(0, ios::beg);
	
	// read the epigram into string
	while (!insty.eof()) {
	    
	    string temp;
	    
            getline(insty, temp);
	    temp += ' ';
	    
	    for (unsigned int i = 0; i < temp.size(); i++) { 
		unsigned char ch = static_cast<unsigned char>(temp[i]);
		// reject epigrams with characters we can't type
		if (isspace(ch) || ch == kboard.askii.getAskii(ch)) {
		    fortune += ch;
		    breakflag = true;
		}
		else {
		    breakflag = false;
		    break;
		}
	    }
	    if (!breakflag) break;
	}
        insty.close();
	
        if (breakflag) {
	    delete[] command;
	    command = 0;
            fformat(fortune);
            return fortune;
        }
    }
}

// remove unwanted \n's, \t's and spaces
void Fortune::fformat(string& str) {
    
    // if *str == '\t' or '\n' make *str == space, otherwise do nothing 
    for (unsigned int i = 0; i < str.size(); i++) {
        if (isspace(str[i])) {
            str[i] = ' ';
        }
    }
    // now remove unwanted spaces
    for (unsigned int i = 0; i < str.size(); i++) {
        if ((str[i] == ' ' && str[i+1] == ' ') || (i == 0 && str[i] == ' ')) {
	    str.erase(i, 1);
	    i--;
	}
    }
    // Display member function out->scrUI() expects last character == " "
    if (str[str.size()-1] != ' ') str.append(" ");
}
