/* -- bakon.cpp -- */

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
 Here starts the typing tutor program. Enjoy...
 ---------------------------------------------------------------------------*/

#include "display.h"
#include "control.h"
#include "error.h"
#include "menus.h"
#include "defaults.h"
#include "macros.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <csignal>
#include <new>
#include <curses.h>
#include <unistd.h>

using namespace std;

void badopt(string error, char* argv);    // handle command-line errors
void usage(void);                         // print helpful message
void screensig(int sig);                  // catch changes in screen size
void closeError(void);                    // knock objects off the heap
void not_expected(void);                    // handle unexpected errors

static Display* out = 0;                  // display type
static Menus* menu = 0;                   // menu type
static int interrupted = 0;               // true of sigwinch
static int waiting = 0;                   // wait for sigwinch

int main(int argc, char *argv[]) {
    
    // catch unexpected errors
    set_unexpected(not_expected);
		   
    //-------------------- set command-line options ----------------------//
    
    try {
	// check for a help request
	if (argc == 2) {
	    if (!strcmp(argv[1], "-h")) {
		usage();
		exit (0);
	    }
	    else badopt("bad option: ", argv[1]);
	}
	// cycle through the options
	else if (argc > 2) {
	    
	    // set bakon Defaults
	    Defaults defs;
	    // for storing seen options
	    vector<string> args;
	    
	    for(int i = 1; i < argc; i++) {
		
		// string compare true if zero
		unsigned int found = 0;
		// true if not a bad option
		bool gotarg = false;
		
		// exit if option entered twice
		for (unsigned int z = 0; z < args.size(); z++) {
		    if (argv[i] == args[z]) {
			badopt("option entered twice: ", argv[i]);
		    }
		}
		// check for arguments without an option string
		if (argv[i][0] != '-') {
		    if (argv[i-1][0] != '-') {
			badopt("bad option: ", argv[i]);
		    }
		}
		// check options for correct arguments
		else if (argv[i][0] == '-') {
		    // -kb
		    if (!strcmp(argv[i], "-kb")) {
			if (argv[i+1] != NULL) {
			    if (((found = UKTYPE.compare(argv[i+1])) == 0) ||
				((found = USTYPE.compare(argv[i+1])) == 0)) {
				args.push_back(argv[i]);
				defs.setDefault(KBRD, argv[i+1]);
			    }
			    else badopt("not a default kb type : ", argv[i+1]);
			}
			else badopt("kb type expected after ", argv[i]);
		    }
		    // -fg
		    else if (!strcmp(argv[i], "-fg")) {
			if (argv[i+1] != NULL) {
			    args.push_back(argv[i]);
			    for (int z = 0; z < CSIZE; z++) {
				if ((found = CURSKEYS[z].compare(argv[i+1])) == 0) {
				    defs.setDefault(FORE, CURSKEYS[z]);
				    gotarg = true;
				    break;
				}
			    }
			    if (!gotarg) badopt("not a default color : ", argv[i+1]);
			}
			else badopt("color expected after : ", argv[i]);
		    }
		    // -bg
		    else if (!strcmp(argv[i], "-bg")) {
			if (argv[i+1] != NULL) {
			    args.push_back(argv[i]);
			    for (int z = 0; z < CSIZE; z++) {
				if ((found = CURSKEYS[z].compare(argv[i+1])) == 0) {
				    defs.setDefault(BACK, CURSKEYS[z]);
				    gotarg = true;
				    break;
				}
			    }
			    if (!gotarg) badopt("not a default color : ", argv[i+1]); 
			}
			else badopt("color expected after : ", argv[i]);
		    }
		    // -bd
		    else if (!strcmp(argv[i], "-bd")) {
			if (argv[i+1] != NULL) {
			    args.push_back(argv[i]);
			    for (int z = 0; z < CSIZE; z++) {
				if ((found = CURSKEYS[z].compare(argv[i+1])) == 0) {
				    defs.setDefault(BOLD, CURSKEYS[z]);
				    gotarg = true;
				    break;
				}
			    }
			    if (!gotarg) badopt("not a default color : ", argv[i+1]);
			}
			else badopt("color expected after : ", argv[i]);
		    }
		    else badopt("bad option: ", argv[i]);
		}
	    }
	}
    }
    catch (Error err) {
	cerr << "exception (1): " << err << endl;
	exit (1);
    }
    catch (...) { 
	cerr << "unexpected exception (1)" << endl;
	exit(1);
    }

    //------------------------ start makin' bakon ----------------------//
    
    // catch changes in screen size
    signal(SIGWINCH, screensig);

    try {
	// initialize the display type
	if ((out = new (std::nothrow) Kurses()) == 0)
	    throw Error("bad_alloc - can't create curses screen");
	if ((menu = new (std::nothrow) Kmenu()) == 0)
	    throw Error("bad_alloc - can't create menu");
    } 
    catch (Error err) { 
	closeError();
	cerr << "exception (2): " << err << endl;
	exit (1);
    }
    catch (...) { 
	closeError();
	cerr << "unexpected exception (2)" << endl;
	exit(1);
    }

    try {
	// start the show
	Control getLesson(out, menu);
	bool doneTest = false;
	
	while(1) {
		
	    if (interrupted) {
		screensig(0);
	    }
	    waiting = 1;
	    
	    getLesson.showMenu();

	    if (getLesson.choice == 'Q') {
		break;
	    }
	    if ((getLesson.execute()) == 'Q') {
		break;
	    }

	    waiting = 0;
	}
	// final curtain
	getLesson.close();
    }
    catch (Error err) {
	out->scrClose();
	closeError();
	cerr << "exception (3): " << err << endl;
	exit(1);
    }
    catch (...) { 
	out->scrClose();
	closeError();
	cerr << "unexpected exception (3)" << endl;
	exit(1);
    }
    delete out;
    out = 0;
    delete menu;
    menu = 0;
    exit(0);
}

/*----------------------------------------------------------------------------
 This function prints command-line error and exits program
 ---------------------------------------------------------------------------*/

void badopt(string error, char* argv) {
    cout << error << argv << endl << endl;
    usage();
    exit(1);
}

/*----------------------------------------------------------------------------
 This function prints help message if requested or if args are wrong...
 ---------------------------------------------------------------------------*/

void usage(void) {
    cout << "Makin' Bakon <version 0.3.2> Type fast or the pig gets it!!!\n";
    cout << "usage: bakon [options]\n";
    cout << "   -kb  <uk/us>  set keyboard type\n";
    cout << "   -bg  <color>  background color\n";
    cout << "   -fg  <color>  foreground color\n";
    cout << "   -bd  <color>  bold print color\n";
    cout << "   -h            this help message\n";
    cout << "colors: blue red yellow magenta black green cyan white trans\n";
}

/*----------------------------------------------------------------------------
 This function exits Makin' Bakon if the screen size is changed beyond display
 defined limits...
 ---------------------------------------------------------------------------*/

void screensig(int sig) {
    
    unsigned long usecs = 300000;

    if (waiting || sig == 0) {
	try {
	    out->sizeEvent();
	}
	catch (Error err) {
	    out->scrClose();
	    cerr << endl << "sig handler: trying to shut down gracefully... ";
	    usleep(usecs);
	    cerr << "done" << endl;
	    usleep(usecs);
	    closeError();
	    cerr << err << endl;
	    usleep(usecs);
	    exit(1);
	}
	interrupted = 0;
    }
    else interrupted = 1;
    
    signal(SIGWINCH, screensig);
}

/*----------------------------------------------------------------------------
 This function closes Makin' Bakon when an unexpected exception is caught
 ---------------------------------------------------------------------------*/

void not_expected() {
    closeError();
    cerr << "unexpected exception (4)" << endl;
    exit(1);
}

/*----------------------------------------------------------------------------
 This function closes down the Makin' Bakon screen
 ---------------------------------------------------------------------------*/

void closeError() {
    
    if (out != 0) {
	delete out;
	out = 0;
    }
    if (menu != 0) {
	delete menu;
	menu = 0;
    }
    return;
}
