/* -- nCurses.cpp -- */

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
 These are the curses methods for the display class. They put a curses ui onto
 the terminal screen. See display.h for a class description.
 ---------------------------------------------------------------------------*/ 

#include "display.h"
#include "keyboard.h"
#include "macros.h"
#include "keyinfo.h"
#include "defaults.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

Kurses::Kurses() throw(Error) : firstitems(NULL), seconditems(NULL) {

    try {
	NEXTKEY = 0x0E;       // key macro: turn nextkey window on/off (CNTRL+N)
	STATS = 0x13;         // key macro: turn stats window on/off (CNTRL+S)
	INSTRUCT = 0x09;      // key macro: turn lesson instructions on/off (CNTRL+I0
	ENDLESSON = 0x05;     // key macro: escape lessons (CNTRL+E)
	REVS = KEY_LEFT;      // key macro: go back a screen
	FRWD = KEY_RIGHT;     // key macro: go forward a screen
	QUIT = 0x11;          // key macro: quit program (CNTRL+Q)
	
	Input = 0;            // keyboard input values
	
	Inlesson = false;     // true if doing a lesson
	Nextkey = true;       // true if nextkey window on
	Userin = true;        // true if userin wait message on
	Lintro = false;       // true if lesson instr's panel is on
	Setless = false;      // true if setLesson menu created
	Scrmenu = false;      // true if scrMenu menu created
	Erroron = false;      // true if error message in wpm window
	Opened = false;       // true if curses fully operational
	Built = false;        // true if virtual keybaord constructed
	
	Lastchar = '\0';      // last used keyboard char (scrKey()) 
	Lastshift = 0;        // last used shift value (scrKey())
	Spos[0] = 0;          // last used shift coords (scrKey())
	Cpos[0] = 0;          // last used key coords (scrKey())
	Keypos = 0;           // ptr to Key coordinates
	Lchoice = 0;          // return value of setLesson()
	
	Menuchoice = 0;       // menu item to return
	Columns = 58;         // num cols in arrayout subwin
	Middle = Columns/2;   // defines the midpoint of arrayout subwin
	Z = 0;                // count down from Columns to zero
	J = 0;                // num arrayout elements to print
    
	// make space for the menus
	if ((firstitems = (ITEM **)calloc(FMENUSIZE+1, sizeof(ITEM *))) == NULL)
	    throw Error("bad_alloc - can't create firstmenu: nCurses.cpp"); 
	if ((seconditems = (ITEM **)calloc(SMENUSIZE+1, sizeof(ITEM *))) == NULL) {
	    free(firstitems);
	    throw Error("bad_alloc - can't create secondmenu: nCurses.cpp"); 
	}
	
	// start curses screen
	if(initscr() == NULL) {
	    free(firstitems);
	    free(seconditems);
	    throw Error("can't initialize curses window: nCurses.cpp");
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (firstitems != NULL) {
	    free(firstitems);
	}
	if (seconditems != NULL) {
	    free(seconditems);
	}
	throw Error("unexpected exception during construction: nCurses.cpp");
    }
    
    try {
	raw();                     // disable line buffering
	keypad(stdscr, TRUE);      // get function keys etc.
	noecho();                  // no echo while doing getch()
	
	// start color only if the terminal is capable
	if (has_colors()) {
	    start_color();
	    // get color Defaults
	    Defaults defs;
	    // -1 to use terminal defaults 
	    assume_default_colors(-1, -1);
	    // get the default colors
	    fore = defs.getColor(FORE);
	    back = defs.getColor(BACK);
	    bold = defs.getColor(BOLD);
	    // initialise color pairs
	    init_pair(1, fore, back);
	    init_pair(2, bold, back);
	    init_pair(3, back, fore);
	    init_pair(4, -1, -1);
	}
	
	// set the screen size
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	if (rows < 25 || cols < 78)
	    throw Error("the minimum screen size is 25 x 78!");

	Height = rows > 25 ? 25 : rows;
	Width = cols > 80 ? 80 : cols;
	
	// set text position for message/titles windows
	Half = (Width-2)/2;
	
	int winpos = Width/2 - 59/2;      // x offset of arrayout win
	int boxpos = Width/2 - 61/2;      // x offset of arrayout boxwin
	int nextpos = Width/2 - 43/2;     // x offset of nextkey win
	int intropos = Width/2 - 71/2;    // x offset of lesson instr's win
	int vbpos = Width/2 - 65/2;       // x offset of virtual keyboard win
	int wpmpos = Width/2 - 55/2;      // x offset of stats win
	int wpmbox = Width/2 - 57/2;      // x offset of stats boxwin
	
	// establish subwindows
	if ((mainwin = derwin(stdscr, Height, Width, 0, 0)) == NULL)
	    throw Error("can't create vboard subwin: nCurses.cpp");
	if ((stitles = subwin(mainwin, 1, Width-2, 1, 1)) == NULL)
	    throw Error("can't create vboard subwin: nCurses.cpp");
	if ((ltitles = subwin(mainwin, 1, Width-2, 0, 1)) == NULL)
	    throw Error("can't create vboard subwin: nCurses.cpp");
	if ((vboard = subwin(mainwin, 15, 64, 8, vbpos)) == NULL)
	    throw Error("can't create vboard subwin: nCurses.cpp");
	if ((nextkey = subwin(mainwin, 1, 42, 21, nextpos)) == NULL)
	    throw Error("can't create nextkey subwin: nCurses.cpp");
	if ((arrayout = subwin(mainwin, 1, 58, 3, winpos)) == NULL)
	    throw Error("can't create arrayout subwin: nCurses.cpp");
	if ((wpm = subwin(stdscr, 1, 54, 6, wpmpos)) == NULL)
	    throw Error("can't create wpm subwin: nCurses.cpp");
	if ((smessage = subwin(mainwin, 1, Width-2, Height-2, 1)) == NULL)
	    throw Error("can't create message subwin: nCurses.cpp");
	if ((lmessage = subwin(mainwin, 1, Width-2, Height-1, 1)) == NULL)
	    throw Error("can't create message subwin: nCurses.cpp");
	if ((introwin = newwin(16, 70, 2, intropos)) == NULL)
	    throw Error("can't create introwin subwin: nCurses.cpp");
	if ((fmenuwin = subwin(mainwin, FMENUSIZE+4, 23, 3, 2)) == NULL)
	    throw Error("can't create fmenuwin subwin: nCurses.cpp");
	if ((smenuwin = subwin(mainwin, FMENUSIZE+4, 20, 3, 2)) == NULL)
	    throw Error("can't create smenuwin subwin: nCurses.cpp");
	if ((boxout = subwin(mainwin, 3, 60, 2, boxpos)) == NULL)
	    throw Error("can't create boxout subwin: nCurses.cpp");
	if ((boxwpm = subwin(stdscr, 3, 56, 5, wpmbox)) == NULL)
	    throw Error("can't create boxwpm subwin: nCurses.cpp");
	if ((finstruct = subwin(mainwin, FMENUSIZE+2, (Width-27), 4, 26)) == NULL)
	    throw Error("can't create finstruct subwin: nCurses.cpp");
	if ((sinstruct = subwin(mainwin, FMENUSIZE+2, (Width-24), 4, 23)) == NULL)
	    throw Error("can't create sinstruct subwin: nCurses.cpp");
	if ((textwin = subwin(mainwin, Height-6, Width-4, 3, 2)) == NULL)
	    throw Error("can't create default subwin: nCurses.cpp");
	
	// the cursor is on when Inlesson is true
	curs_set(0);
	
	// set some background color characteristics
	wbkgdset(mainwin, COLOR_PAIR(1));
	wbkgdset(stitles, COLOR_PAIR(2));
	wbkgdset(ltitles, COLOR_PAIR(1));
	wbkgdset(vboard, COLOR_PAIR(1));
	wbkgdset(nextkey, COLOR_PAIR(2));
	wbkgdset(arrayout, COLOR_PAIR(1));
	wbkgdset(wpm, COLOR_PAIR(1));
	wbkgdset(smessage, COLOR_PAIR(2));
	wbkgdset(lmessage, COLOR_PAIR(1));
	wbkgdset(introwin, COLOR_PAIR(1));
	wbkgdset(fmenuwin, COLOR_PAIR(1));
	wbkgdset(smenuwin, COLOR_PAIR(1));
	wbkgdset(boxout, COLOR_PAIR(1));
	wbkgdset(boxwpm, COLOR_PAIR(1));
	wbkgdset(finstruct, COLOR_PAIR(1));
	wbkgdset(sinstruct, COLOR_PAIR(1));
	wbkgdset(textwin, COLOR_PAIR(1));
	
	// set some window attributes
	wattrset(stitles, A_BOLD | A_UNDERLINE);
	wattrset(smessage, A_BOLD);
	wattrset(nextkey, A_BOLD);
	
	// flag curses as fully opened
	Opened = true;
    }
    catch (Error err) {
	free(firstitems);
	free(seconditems);
	clear(); 
	endwin();
	throw err;
    }
    catch (...) {
	free(firstitems);
	free(seconditems);
	clear(); 
	endwin();
	throw Error("unexpected exception during construction: nCurses.cpp");
    }
}

int Kurses::scrHelp(string str) { 

    // divide up the scrHelp string
    vector<string> help;
    // iterate through the strings
    vector<string>::iterator it;
    // create lines that are COLS wide
    scrFormat(Width-4, str);
    // clear screen
    werase(mainwin);
    box(mainwin, ACS_VLINE, ACS_HLINE);
    wnoutrefresh(mainwin);
    
    int lines = Height-6;      // n lines to print
    int cutpoint = 0;          // where to cut str
    bool endflag = false;      // end of string
    int size = str.size();     // dunno, do you?
	
    for (int j = 0; ; j++) {
	int linecount = 0;
	for (int k = cutpoint; k < size; k++) {
	    if (k == cutpoint && str[k] == '\n') {
		cutpoint += 1;
		j--;
		break;
	    }
	    else if (str[k] == '\n') {
		linecount++;
		if (k == size-1) {
		    string page(str, cutpoint, k-cutpoint);
		    help.push_back(page);
		    cutpoint = k;
		    endflag = true;
		    break;
		}
		else if (linecount == lines) {
		    string page(str, cutpoint, k-cutpoint);
		    help.push_back(page);
		    cutpoint = k;
		    break;
		}
	    }
	}
	if (endflag) break;
    }

    string n1("Pig's Destructions");
    string n2("Pig's Destructions Cont'd");
    string n3("Left and Right arrows to navigate, CNTRL+E for main menu");

    int pos1 = Half - n1.size()/2;
    int pos2 = (Half - n2.size()/2) -1;
    int pos3 = Half - n3.size()/2;
    
    // when to erase titles subwin
    bool updated = true;
    
    // only erase message subwin once
    werase(smessage);
    mvwaddstr(smessage, 0, pos3, n3.c_str());
    
    for (it = help.begin(); it != help.end(); it++) {
	
	if (it == help.begin()) { 
	    werase(stitles);
	    mvwaddstr(stitles, 0, pos1, n1.c_str());
	    updated = false;
	}
	else if (it > help.begin()) {
	    if (! updated) {
		werase(stitles);
		mvwaddstr(stitles, 0, pos2, n2.c_str());
		updated = true;
	    }
	}

	werase(textwin);
	string page(*it);
	mvwprintw(textwin, 0, 0, "%s", page.c_str());

	wnoutrefresh(smessage);
	wnoutrefresh(stitles);
	wnoutrefresh(textwin);
	doupdate();

	int ch;
	bool breakflag = false;
	
	while(1) {
	    ch = scrInput();
	    if (ch == QUIT) return 'Q';
	    else if (ch == ENDLESSON) {
		breakflag = true;
		break;
	    }
	    else if (ch == FRWD) {
		if (it != help.end()-1) {
		    break;
		}
	    }
	    else if (ch == REVS) {
		if (it != help.begin()) {
		    it -= 2;
		    break;
		}
	    }
	}
	if (breakflag) break;
    }
    return 0;
}

int Kurses::scrInstruct(string str) throw(Error) { 

    scrFormat(68, str);
    str.append(1, '\n');

    string quit("CNTRL+I to exit Lesson Help");

    // erase introwin for each call
    werase(introwin);
    // box the window
    box(introwin, ACS_VLINE, ACS_HLINE);

    // attach window to panel
    if ((intropanel = new_panel(introwin)) == NULL)
	throw Error("new_panel returned error in scrInstruct: nCurses.cpp");
    
    // print header and footer
    wattrset(introwin, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(introwin, 1, (70/2)-(24/2), "Makin' Bakon Lesson Help");
    mvwaddstr(introwin, 14, 1, quit.c_str());
    wattrset(introwin, 0);

    // string to hold Pig's tips
    string hmmm;
    // assign part of str to temp
    string temp;
    
    unsigned int next = 0;      // line counter
    unsigned int newline = 0;   // holds '\n' position
    unsigned int j = 0;         // for looping
    unsigned int k = 0;         // for looping
    
    // print lesson help text
    for (k = 0, j = 0; k < str.size(); k++, j++) {
	if ((newline = str.find('\n', k)) != str.npos) {
	    temp.assign(str, k, newline-k);
	    mvwprintw(introwin, j+3, 1, "%s", temp.c_str());
	    k = newline;
	}
	next = j+2;
    }
    
    hmmm = gettext->getTips();   // get the Pig tip
    scrFormat(68, hmmm);         // format the tip
    hmmm.append(1, '\n');        // append a newline
    
    for (k = 0, j = next; k < hmmm.size(); k++, j++) {
	if ((newline = hmmm.find('\n', k)) != hmmm.npos) {
	    temp.assign(hmmm, k, newline-k);
	    mvwprintw(introwin, j+3, 1, "%s", temp.c_str());
	    k = newline;
	}
    }
    wnoutrefresh(introwin);
    
    // draw line under heading
    mvwaddch(introwin, 2, 0, ACS_LTEE);
    mvwhline(introwin, 2, 1, ACS_HLINE, 68);
    mvwaddch(introwin, 2, 69, ACS_RTEE);
    // de-activate cursor
    curs_set(0);
    // update the secreen
    wnoutrefresh(introwin);
    update_panels();
    doupdate();

    Lintro = true;

    while (1) {
	int ch = scrInput();
	if (ch == QUIT) {
	    // hide panel and re-activate cursor
	    hide_panel(intropanel);
	    update_panels();
	    doupdate();
	    curs_set(1);
	    return 'Q';
	}
	else if (ch == INSTRUCT) {
	    // hide panel and re-activate cursor
	    curs_set(1);
	    int Cursor = Z <= Middle ? Z-1 : Middle;
	    hide_panel(intropanel);
	    update_panels();
	    wmove(arrayout, 0, Cursor);
	    wnoutrefresh(arrayout);
	    doupdate();
	    Lintro = false;
	    break;
	}
    }
    return 1;
}

int Kurses::scrMenu(string str[]) throw(Error) {
    
    Lastchar = '\0';      // last keyboard char (scrKey()) 
    Lastshift = 0;        // last key shift value (scrKey())
    Inlesson = false;     // well, we're not in a lesson
    Built = false;        // keyboard not constructed (scrKey())
    
    // copy of str[] for setting default lesson
    string copy[SMENUSIZE];
    
    // the menu screen prints its own titles
    string title("Makin' Bakon Typing Tutor. Type fast or the pig gets it!");
    string pigsd("Pig's Destructions");
    string bott1("Pig is happy to help. Just press \"H\"");

    // position of title string
    int post = Half - title.size()/2;
    int posp = (Width-27)/2 - pigsd.size()/2;
    int posb = Half - bott1.size()/2;
    
    // clear screen
    werase(mainwin);
    box(mainwin, ACS_VLINE, ACS_HLINE);
    wnoutrefresh(mainwin);
    
    // print titles
    mvwaddstr(stitles, 0, post, title.c_str());
    mvwaddstr(smessage, 0, posb, bott1.c_str());
    wnoutrefresh(stitles);
    wnoutrefresh(smessage);
    
    // zero out menuitems before (re)writing menu
    memset(firstitems, '\0', (FMENUSIZE+1) * sizeof(ITEM *));

    try {
	// put menu items in the memory we've reserved
	for(int i = 0; i < MENUSIZE; i++) {
	    if (i < FMENUSIZE) {
		if ((firstitems[i] = new_item(str[i].c_str(), str[i].c_str())) == NULL)
		    throw Error("new_item returned error in scrMenu: nCurses.cpp");
	    }
	    else copy[i-FMENUSIZE] = str[i];
	}
	// set the last ITEM to NULL
	firstitems[FMENUSIZE] = (ITEM *)NULL;
	
	// create menu
	if ((firstmenu = new_menu((ITEM **)firstitems)) == NULL)
	    throw Error("new_menu returned error in scrMenu: nCurses.cpp");
    }
    catch (Error err) {
	Opened = false;
	free(firstitems);
	free(seconditems);
	clear(); 
	endwin();
	throw err;
    }

    // turn off description (which would just repeat the string[])
    menu_opts_off(firstmenu, O_SHOWDESC);
    
    // set menu window and sub window
    set_menu_win(firstmenu, fmenuwin);
    set_menu_sub(firstmenu, derwin(fmenuwin, FMENUSIZE, 21, 3, 2));
    
    // set firstmenu colors
    set_menu_fore(firstmenu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(firstmenu, COLOR_PAIR(1));

    // set num rows and num (strlen)cols
    set_menu_format(firstmenu, FMENUSIZE, 1);
    
    // set menu marker to string " <symbol> "
    set_menu_mark(firstmenu, NULL);
    
    // draw box around window
    box(fmenuwin, ACS_VLINE, ACS_HLINE);
    
    wattrset(fmenuwin, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(fmenuwin, 1, 3, "Makin' Bakon Menu");
    wattrset(fmenuwin, 0);
    
    // draw a line under heading
    mvwaddch(fmenuwin, 2, 0, ACS_LTEE);
    mvwhline(fmenuwin, 2, 1, ACS_HLINE, 21);
    mvwaddch(fmenuwin, 2, 22, ACS_RTEE);
    
    // post menu
    post_menu(firstmenu);
    wnoutrefresh(fmenuwin);
    
    // scrMenu is now true
    Scrmenu = true;
    
    // string to hold lesson intro
    string intro;
    
    if ((current = current_item(firstmenu)) == NULL)
	throw Error("current_item returned error in scrMenu: nCurses.cpp");

    // print first menuitem intro
    intro = gettext->getIntro(item_index(current));
    scrFormat(Width-27, intro);
    wattrset(finstruct, A_BOLD | A_UNDERLINE | COLOR_PAIR(2));
    mvwaddstr(finstruct, 0, posp, pigsd.c_str());
    wattrset(finstruct, 0);
    mvwaddstr(finstruct, 2, 0, intro.c_str());
    wnoutrefresh(finstruct);
    doupdate();
    
    int count = 0;     // get return value from menu driver
    int check = 0;     // only update intro's if count is > or < check

    while(1) {
	
	// get user input
	int ch = scrInput();
	
	// down arrow for down one item
	if (ch == KEY_DOWN) { 
	    if (count != FMENUSIZE-1) {
	        menu_driver(firstmenu, REQ_DOWN_ITEM);
		count++;
	    }
	    // else continue;
	}
	// up arrow for up one item
	else if (ch == KEY_UP) {
	    if (count != 0) {
	       menu_driver(firstmenu, REQ_UP_ITEM);
	       count--;
	    }
	    // else continue;
        }
	// page down for last item in list
	else if (ch == KEY_NPAGE) {
	    menu_driver(firstmenu, REQ_LAST_ITEM);
	    count = FMENUSIZE-1;
	}
	// page up for first item in list
	else if (ch == KEY_PPAGE) {
	    menu_driver(firstmenu, REQ_FIRST_ITEM);
	    count = 0;
	}
	// return key for choice
	else if (ch == '\n') {
	    // check if we want to change default lesson
	    if ((Menuchoice = item_index(current)) == static_cast<unsigned int>(CHANGEDEF)) {
		Setless = true;
		int choice = setLesson(copy);
		if (choice == ENDLESSON) { 
		    // passing a junk/out of range number restarts the menu
		    Menuchoice = 1001;
		    break;
		}
		else {
		    Menuchoice = choice;
		    break;
		}
	    }
	    else break;
	}
	// test for menu shortcuts which fall between A-Z and a-z
	else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
	    bool breakflag = false;
	    for (int i = 0; i < FMENUSIZE; i++) {
                // get the shortcut name
		char match = str[i][str[i].size()-2];
		char keypress = static_cast<char>(ch);
		// compare shortcut and ch for upper and lower case
		if (match == keypress || match == (keypress - ' ')) {
		    // check if we want to change default lesson
		    if ((Menuchoice = i) == static_cast<unsigned int>(CHANGEDEF)) {
			Setless = true;
			int choice = setLesson(copy);
			if (choice == ENDLESSON) {
			    Menuchoice = 1000;
			    breakflag = true;
			    break;
			}
			else {
			    Menuchoice = choice;
			    breakflag = true;
			    break;
			}
		    }
		    else {
			breakflag = true;
			break;
		    }
		}
	    }
	    if (breakflag) break;
	}
	// test for QUIT
	else if (ch == QUIT) {
	    Menuchoice = 'Q';
	    break;
	}

	if ((current = current_item(firstmenu)) == NULL)
	    throw Error("current_item returned error in scrMenu: nCurses.cpp");

	// get the intro to print for each menuitem
	if (count != check) {
	    intro = gettext->getIntro(item_index(current));
	    scrFormat(Width-27, intro);
	    werase(finstruct);
	    wattrset(finstruct, A_BOLD | A_UNDERLINE | COLOR_PAIR(2));
	    mvwaddstr(finstruct, 0, posp, pigsd.c_str());
	    wattrset(finstruct, 0);
	    mvwaddstr(finstruct, 2, 0, intro.c_str());
	    wnoutrefresh(fmenuwin);
	    wnoutrefresh(finstruct);
	    doupdate();
	    check = count;
	}
    }
    return Menuchoice;
}

int Kurses::setLesson(string str[]) throw(Error) {
    
    // the menu screen prints its own titles
    string title("Change Lesson Group");
    string pigsd("Pig's Destructions");
    string bott1("Made a mistake? Press CNTRL+E for the main menu");
    
    // position of title string
    int post = Half - title.size()/2;
    int posp = (Width-24)/2 - pigsd.size()/2;
    int posb = (Half - bott1.size()/2) -1;

    // clear screen
    werase(mainwin);
    box(mainwin, ACS_VLINE, ACS_HLINE);
    wnoutrefresh(mainwin);

    // print titles
    mvwaddstr(stitles, 0, post, title.c_str());
    mvwaddstr(smessage, 0, posb, bott1.c_str());
    wnoutrefresh(stitles);
    wnoutrefresh(smessage);
    
    // zero out menuitems before (re)writing menu
    memset(seconditems, '\0', (SMENUSIZE+1) * sizeof(ITEM *));
    
    try {
	// put menu items in the memory we've reserved
	for(int i = 0; i < SMENUSIZE; i++) {
	    if ((seconditems[i] = new_item(str[i].c_str(), str[i].c_str())) == NULL)
		throw Error("new_item returned error in setLesson: nCurses.cpp");
	}
	// set the last ITEM to NULL
	seconditems[SMENUSIZE] = (ITEM *)NULL;
	
	// create menu
	if ((secondmenu = new_menu((ITEM **)seconditems)) == NULL)
	    throw Error("new_menu returned error in setLesson: nCurses.cpp");
    }
    catch (Error err) {
	Opened = false;
	unpost_menu(firstmenu);
	free_menu(firstmenu);
	for(int i = 0; i < FMENUSIZE; i++)
	    free_item(firstitems[i]);
	free(seconditems);
	clear(); 
	endwin();
	throw err;
    }
	
    // turn off description (which would just repeat the string[])
    menu_opts_off(secondmenu, O_SHOWDESC);
    
    // set menu window and sub window
    set_menu_win(secondmenu, smenuwin);
    set_menu_sub(secondmenu, derwin(smenuwin, FMENUSIZE, 18, 3, 2));
    
    // set num rows and num (strlen)cols
    set_menu_format(secondmenu, FMENUSIZE, 1);
    
    // set menu marker to string " <symbol> "
    set_menu_mark(secondmenu, NULL);
    
    // set secondmenu colors
    set_menu_fore(secondmenu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(secondmenu, COLOR_PAIR(1));

    // draw box around window
    box(smenuwin, ACS_VLINE, ACS_HLINE);
    
    wattrset(smenuwin, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(smenuwin, 1, 2, "Lesson Group: 1");
    wattrset(smenuwin, 0);
    
    // draw a line under heading
    mvwaddch(smenuwin, 2, 0, ACS_LTEE);
    mvwhline(smenuwin, 2, 1, ACS_HLINE, 18);
    mvwaddch(smenuwin, 2, 19, ACS_RTEE);
    
    // post menu
    post_menu(secondmenu);
    wnoutrefresh(smenuwin);
    
    // string to hold lesson intro
    string intro;
    
    if ((current = current_item(secondmenu)) == NULL)
	throw Error("current_item returned error in setLesson: nCurses.cpp");

    // print first menuitem intro
    intro = gettext->getIntro(item_index(current) + FMENUSIZE);
    scrFormat(Width-24, intro);
    wattrset(sinstruct, A_BOLD | A_UNDERLINE | COLOR_PAIR(2));
    mvwaddstr(sinstruct, 0, posp, pigsd.c_str());
    wattrset(sinstruct, 0);
    mvwaddstr(sinstruct, 2, 0, intro.c_str());
    wnoutrefresh(sinstruct);
    doupdate();
    
    int count = 0;     // get return value from menu driver
    int check = 0;     // only update intro's if count is > or < check
    
    while(1) {
	
	int ch = scrInput();
	
	// catch QUIT macro
	if (ch == QUIT) {
	    Lchoice = 'Q';
	    break;
	}
	// catch ENDLESSON escape macro
	else if (ch == ENDLESSON) {
	    Lchoice = ENDLESSON;
	    break;
	}
	// down arrow for down one item
	else if (ch == KEY_DOWN) { 
	    if (count != SMENUSIZE-1) {
	        menu_driver(secondmenu, REQ_DOWN_ITEM);
		count++;
	    }
	    // else continue;
	}
	// up arrow for up one item
	else if (ch == KEY_UP) {
	    if (count != 0) {
		menu_driver(secondmenu, REQ_UP_ITEM);
		count--;
	    }
	    // else continue;
        }
	// page down for last item in list
	else if (ch == KEY_NPAGE) {
	    menu_driver(secondmenu, REQ_LAST_ITEM);
	    count = SMENUSIZE-1;
	}
	// page up for first item in list
	else if (ch == KEY_PPAGE) {
	    menu_driver(secondmenu, REQ_FIRST_ITEM);
	    count = 0;
	}
	// return key for choice
	else if (ch == '\n') {
	    Lchoice = item_index(current) + FMENUSIZE;
	    break;
	}

	if ((current = current_item(secondmenu)) == NULL)
	    throw Error("current_item returned error in setLesson: nCurses.cpp");

	if (count != check) {
	    stringstream num;
	    if (count < 10) {
		num << "Lesson Group: " << count+1 << " ";
	    }
	    else {
		num << "Lesson Group: " << count+1;
	    }
	    wattrset(smenuwin, A_BOLD | COLOR_PAIR(2));
	    mvwaddstr(smenuwin, 1, 2, num.str().c_str());
	    wattrset(smenuwin, 0);
	    wnoutrefresh(smenuwin);
	    
	    // get the intro to print for each menuitem
	    intro = gettext->getIntro(item_index(current) + FMENUSIZE);
	    scrFormat(Width-24, intro);
	    werase(sinstruct);
	    wattrset(sinstruct, A_BOLD | A_UNDERLINE | COLOR_PAIR(2));
	    mvwaddstr(sinstruct, 0, posp, pigsd.c_str());
	    wattrset(sinstruct, 0);
	    mvwaddstr(sinstruct, 2, 0, intro.c_str());
	    wnoutrefresh(sinstruct);
	    doupdate();
	    check = count;
	}
    }
    return Lchoice;
}

void Kurses::scrWelcome(string str) {
    
    // This is the screen the user sees upon first starting the program.
    
    Lastchar = '\0';      // last keyboard char (scrKey()) 
    Lastshift = 0;        // last key shift value (scrKey())
    Inlesson = false;     // well, we're not in a lesson
    Built = false;        // keyboard not constructed (scrKey())

    string title("Welcome to Makin' Bakon");
    string next("Press spacebar for test screen");
    
    int post = (Half - title.size()/2) -1;
    int posb = (Half - next.size()/2) -1;
    
    // clear screen
    werase(mainwin);
    box(mainwin, ACS_VLINE, ACS_HLINE);
    wnoutrefresh(mainwin);

    mvwaddstr(stitles, 0, post, title.c_str());
    scrFormat(Width-4, str);
    mvwprintw(textwin, 0, 0, "%s", str.c_str());
    mvwaddstr(smessage, 0, posb, next.c_str());
    wnoutrefresh(stitles);
    wnoutrefresh(textwin);
    wnoutrefresh(smessage);
    doupdate();
}

void Kurses::scrKey(string finger, unsigned char ch, int shift) {
    
    Keyboard kboard; // get keyboard data for virtual keyboard

    // NOTE:- Keypos is set to zero in the boxKeys() function!

    if (! Built) {
	int askii = 0;      // ascii value of the key
	int top = 0;        // return key top-most corordinate
	int bottom = 0;     // return key bottom-most corordinate
	int left = 0;       // return key left-most corordinate
	int right = 0;      // return key right-most corordinate
	int chtype = 0;     // curses attribute
	string name;        // the key name
	
	for (int k = 0; k < kboard.getKsize()-1; k++) {
	    // only get the major keys
	    if (! kboard.index.getMajor(k)) {
		askii = kboard.index.getAskii(k);
		// now draw them on the screen
		Keypos = kboard.index.getKcoords(k);
		mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1, askii);
		boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_NORMAL);
	    }
	}
	// get function-key coordinates, draw clockwise from <- to tab
	for (int i = 0; i < 10; i++) {
	    Keypos = kboard.getFcoords(i);
	    name.assign(kboard.getFname(i));
	    mvwaddstr(vboard, *Keypos+1, *(Keypos+1)+1, name.c_str());
	    boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_NORMAL);
	}
	
	// draw the corners for the return key (which we treat as a
	// special case because of its awkward shape) working clock-
	// wise from upper right corner.
	
	for (int i = 0; i < 6; i++) {
	    Keypos = kboard.getRcoords(i);
	    switch (i) {
	      case 0: chtype = ACS_URCORNER; break;
	      case 1: chtype = ACS_LRCORNER; break;
	      case 2: chtype = ACS_LLCORNER; break;
	      case 3: chtype = ACS_URCORNER; break;
	      case 4: chtype = ACS_LLCORNER; break;
	      case 5: chtype = ACS_ULCORNER; break;
	    }
	    mvwaddch(vboard, *Keypos, *(Keypos+1), chtype | A_NORMAL);
	    // get extreme top, bottom, left and right coordinates
	    if (i == 0) {
		top = *Keypos;
		left = *(Keypos+1);
		bottom = right = 0;
	    }
	    if (*Keypos > bottom) bottom = *Keypos;
	    if (*(Keypos+1) > right) right = *(Keypos+1);
	    if (*Keypos < top) top = *Keypos;
	    if (*(Keypos+1) < left) left = *(Keypos+1);
	}
	// draw the horizontal lines, starting with the top-most line
	for(int i = left+1; i < right; i++) {
	    mvwaddch(vboard, top, i, ACS_HLINE | A_NORMAL);
	    if (i == left+1)  // middle left -> middle right
		mvwaddch(vboard, top+2, i, ACS_HLINE | A_NORMAL);
	    if (i > left+2)   // bottom left -> bottom right
		mvwaddch(vboard, bottom, i, ACS_HLINE | A_NORMAL);
	}
	// draw the vertical lines, starting at the right-most line
	for(int i = top+1; i < bottom; i++) {
	    mvwaddch(vboard, i, right, ACS_VLINE | A_NORMAL);
	    if ( i == top+1)  // left top -> left bottom
		mvwaddch(vboard, i, left, ACS_VLINE | A_NORMAL);
	    if (i > top+2)    // middle top -> middle bottom
		mvwaddch(vboard, i, left+2, ACS_VLINE | A_NORMAL);
	}
	name.assign(kboard.getRname());
	mvwaddstr(vboard, top+1, left+1, name.c_str());

	Built = true;
    }

    if (! Lintro) {
	
	int mkey = 0;    // is the character major (A) or minor (a)
	string name;     // the key name

	// update the virtual keyboard character keys
	if (ch != '\0' && ch != Lastchar) {
	    if (Lastchar != '\0') {
		Keypos = Cpos;
		if (Lastchar != ' ') {
		    // remove highlighting from last key name
		    if ((mkey = kboard.askii.getMajor(Lastchar)) != 0) {
			mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1, mkey | A_NORMAL | COLOR_PAIR(1));
		    }
		    else {
			mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1, Lastchar | A_NORMAL | COLOR_PAIR(1));
		    }
		}
		// remove highlighting from last key box
		boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_NORMAL | COLOR_PAIR(1));
	    }
	    if (ch == ' ') {
		Keypos = kboard.getFcoords(ch);
	    }
	    else {
		// highlight the new key name
		Keypos = kboard.askii.getKcoords(ch);
		mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1, ch | A_BOLD | COLOR_PAIR(2));
	    }
	    // make a copy of the new key coordinates
	    for (int i = 0; i < 4; i++) {
		*(Cpos+i) = *(Keypos+i);
	    }
	    // highlight the new key box
	    boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_BOLD | COLOR_PAIR(2));
	    Lastchar = ch;
	}
	// update the virtual keyboard shift keys
	if (shift != Lastshift) {
	    if (Lastshift != 0) {
		Keypos = Spos;
		name.assign(kboard.getFname(Lastshift));
		// remove highlighting from the last key name
		for (unsigned int i = 0; i < name.size(); i++) {
		    mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1+i, name.c_str()[i] | A_NORMAL | COLOR_PAIR(1));
		}
		// remove highlighting from the last key box
		boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_NORMAL | COLOR_PAIR(1));
	    }
	    if (shift != 0) {
		Keypos = kboard.getFcoords(shift);
		// make a copy of the coordinates
		for (int i = 0; i < 4; i++) {
		    *(Spos+i) = *(Keypos+i);
		}
		name.assign(kboard.getFname(shift));
		// highlight the new key name
		for (unsigned int i = 0; i < name.size(); i++) {
		    mvwaddch(vboard, *Keypos+1, *(Keypos+1)+1+i, name.c_str()[i] | A_BOLD | COLOR_PAIR(2));
		}
		// highlight the new key box
		boxKeys(*Keypos, *(Keypos+1), *(Keypos+2), *(Keypos+3), A_BOLD | COLOR_PAIR(2));
	    }
	}
	Lastshift = shift;

	// update the nextkey window
	if (Userin || finger.size() == 0) {
	    werase(nextkey);
	    string wstr("Type the letter under the cursor");
	    int posw = 42/2 - wstr.size()/2;
	    mvwaddstr(nextkey, 0, posw, wstr.c_str());
	}
	else {
	    werase(nextkey);
	    int fsize = finger.size();
	    int poss = 42/2 - fsize/2;
	    if ((fsize % 2) == 1) {
		poss -= 1;
	    }
	    mvwaddstr(nextkey, 0, poss, finger.c_str());
	}
	Nextkey = true;
    }

    int Cursor = Z <= Middle ? Z-1 : Middle;
    wmove(arrayout, 0, Cursor);
    wnoutrefresh(nextkey);
    wnoutrefresh(vboard);
    wnoutrefresh(arrayout);
    doupdate();
}

void Kurses::boxKeys(int r1, int c1, int r2, int c2, chtype ATTRIB) {
    
    // (r1,c1) = upper left  corner
    // (r2,c2) = lower right corner

    // draw the corners
    mvwaddch(vboard, r1, c1, ACS_ULCORNER | ATTRIB);
    mvwaddch(vboard, r1, c2, ACS_URCORNER | ATTRIB);
    mvwaddch(vboard, r2, c1, ACS_LLCORNER | ATTRIB);
    mvwaddch(vboard, r2, c2, ACS_LRCORNER | ATTRIB);
    
    // draw the horizontal lines
    for(int i = c1+1; i < c2; i++) {
	mvwaddch(vboard, r1, i, ACS_HLINE | ATTRIB);
	mvwaddch(vboard, r2, i, ACS_HLINE | ATTRIB);
    }
    
    // draw the vertical lines
    for(int i = r1+1; i < r2; i++) {
	mvwaddch(vboard, i, c1, ACS_VLINE | ATTRIB);
	mvwaddch(vboard, i, c2, ACS_VLINE | ATTRIB);
    }
    Keypos = 0;
}

void Kurses::scrKey() {

    // turn off the virtual keyboard
    
    int Cursor = Z <= Middle ? Z-1 : Middle;
    werase(nextkey);
    werase(vboard);
    wmove(arrayout, 0, Cursor);
    wnoutrefresh(nextkey);
    wnoutrefresh(vboard);
    wnoutrefresh(arrayout);
    doupdate();

    Lastchar = '\0';      // last keyboard char (scrKey()) 
    Lastshift = 0;        // last key shift value (scrKey())
    Nextkey = false;      // nextkey window is off
    Built = false;        // keyboard not constructed (scrKey())
    

}

void Kurses::scrStats(string str) {
    
    // update the statistics window

    int Cursor = Z <= Middle ? Z-1 : Middle;

    if (! Lintro) {
	 
	int ssize = str.size();
	int poss = (54/2) - (ssize/2);
	if ((ssize % 2) == 1) {
	    poss -= 1;
	}
        box(boxwpm, ACS_VLINE, ACS_HLINE);
	
	// don't print over error messages in wpm window 
	// i.e. if we turn boxwpm back on after an error
	// message has just been printed...

	if (! Erroron) {
	    werase(wpm);
	    mvwaddstr(wpm, 0, poss, str.c_str());
	    wnoutrefresh(wpm);
	}
    }
    wmove(arrayout, 0, Cursor);
    wnoutrefresh(boxwpm);
    wnoutrefresh(arrayout);
    doupdate();
}

void Kurses::scrStats() {

    // turn off the statistics window

    int Cursor = Z <= Middle ? Z-1 : Middle;
    
    // don't print over error message in wpm window i.e.
    // if there is an error message then re-print it if
    // we turn off boxwpm
	
    if (! Erroron) {
	werase(boxwpm);
    }
    else {
	int ssize = Scopy.size();
	int posc = (54/2) - (ssize/2);
	if ((ssize % 2) == 1) {
	    posc -= 1;
	}
	werase(boxwpm);
	mvwaddstr(wpm, 0, posc, Scopy.c_str());
	wnoutrefresh(wpm);
    }
    wmove(arrayout, 0, Cursor);
    wnoutrefresh(boxwpm);
    wnoutrefresh(arrayout);
    doupdate();
}

void Kurses::scrLesson(string str) {
    
    // set positional values
    Z = 1;
    J = 1;
    // copy str
    Lcopy = str;
    // wait message on
    Userin = true;
    // sleep in milliseconds
    unsigned long usecs = 1000;
    // position counter
    int count = 0;
    // make sure the curser is active
    curs_set(1);
    // yes, we're doing a lesson now
    Inlesson = true;
    
    // start arrayout box first
    box(boxout, ACS_VLINE, ACS_HLINE);
    wnoutrefresh(boxout);
    
    // If Nextkey is true then display wait message and initialize keyboard
    if (Nextkey) {
	Keyinfo key(str[0]);
	scrKey(key.finger, key.ch, key.shift);
    }

    // put cursor on the right of window
    wmove(arrayout, 0, (Columns-1));
    wnoutrefresh(arrayout);
    doupdate();
    
    // for the length of the string, print array from far
    // right of subwin arrayout until text reaches element
    // zero. Make the cursor shadow the first element...

    for (count = Columns; count > -1; count--) {
        mvwaddnstr(arrayout, 0, count, str.c_str(), J);
        wmove(arrayout, 0, count);
        wrefresh(arrayout);
        usleep(usecs);
        J++;
    }
}

void Kurses::scrUI() {

    if (Userin) Userin = false;
    
    unsigned long usecs = 10000;

    // move the cursor towards the Middle from the left and move
    // strings that are < Middle characters long off to the left

    if (Z <= Middle) {
        if (Z == Lcopy.size()-1) {

            // If Lcopy.size() is 30 then a " " sits at element 29. We delete
            // the first character, effectively makeing the strlen 29, and
            // move the cursur to element 28, thus shadowing the " ".
            
            for (unsigned int k = Lcopy.size(); k > 0;  k--) {
                mvwdelch(arrayout, 0, 0);
                wmove(arrayout, 0, (k-2));
                wrefresh(arrayout);
                usleep(usecs);
            }
        }
        else wmove(arrayout, 0, Z);
    }

    // When Z > Middle we make element zero a null and increment a ptr
    // by one, thus deleting the first character of the string. To do 
    // this operation in C, we'd make *Lcopy = '\0' and then ++Lcopy
    
    else if (Z > Middle) {
	// remove first character of string
	Lcopy.erase(0, 1);
	
        // We only print Cols elements at most
        if (Lcopy.size() > Columns) {
	    mvwaddnstr(arrayout, 0, 0, Lcopy.c_str(), Columns);
	}
        else {
            mvwaddnstr(arrayout, 0, 0, Lcopy.c_str(), Lcopy.size());
	}
        // keep cursor in Middle
	wmove(arrayout, 0, Middle);

        // Scroll text off to the left with cursor shadowing the space
        if (Lcopy.size() == Middle+1) {
            for (int k = Lcopy.size(); k > 0;  k--) {
                mvwdelch(arrayout, 0, 0);
                wmove(arrayout, 0, (k-2));
                wrefresh(arrayout);
                usleep(usecs);
            } 
	}
    }
    wrefresh(arrayout);
    Z++;
}

void Kurses::scrError(string str) {
   
    // copy str (for scrStats())
    Scopy = str;

    if (!Lintro) {
	// return cursor when we're done
	int Cursor = Z <= Middle ? Z-1 : Middle;
	
	if (Userin) Userin = false;

	int ssize = Scopy.size();

	if (ssize > 0) {
	    int pos = (54/2) - (str.size())/2;
	    if ((ssize % 2) == 1) {
		pos -= 1;
	    }
	    werase(wpm);
	    wattrset(wpm, A_BOLD | COLOR_PAIR(2));
	    mvwaddstr(wpm, 0, pos, str.c_str());
	    beep();
	    Erroron = true;
	}
	else {
	    wattrset(wpm, 0);
	    werase(wpm);
	    Erroron = false;
	}
	wmove(arrayout, 0, Cursor);
	wnoutrefresh(wpm);
	wnoutrefresh(arrayout);
	doupdate();
    }
}

void Kurses::scrTitle(string str) {

    // print title and lesson number
    if (!Inlesson) {
	// clear screen
	werase(mainwin);
	// draw left hand line and corners of lesson screen
	mvwaddch(mainwin, 0, 0, ACS_ULCORNER);
	mvwvline(mainwin, 1, 0, ACS_VLINE, Height-1);
	mvwaddch(mainwin, Height-1, 0, ACS_LLCORNER);
	// draw right hand line and corners of lesson screen
	mvwaddch(mainwin, 0, Width-1, ACS_URCORNER);
	mvwvline(mainwin, 1, Width-1, ACS_VLINE, Height-1);
	mvwaddch(mainwin, Height-1, Width-1, ACS_LRCORNER);
	wnoutrefresh(mainwin);
	// flag virtual keyboard as not built
	Built = false;
    }
    int tsize = str.size();
    int poss = Half - tsize/2;
    if ((tsize % 2) == 1) {
	poss -= 1;
    }
    werase(ltitles);
    // finish box in ltitles segment of lesson screen
    mvwhline(ltitles, 0, 0, ACS_HLINE, poss-1);
    mvwhline(ltitles, 0, poss+tsize+1, ACS_HLINE, Width-2);
    wattrset(ltitles, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(ltitles, 0, poss, str.c_str());
    wattrset(ltitles, A_NORMAL | COLOR_PAIR(1));
    wnoutrefresh(ltitles);

    // print standard start of lesson message
    string quit("CNTRL+I for lesson help, CNTRL+E to quit lesson early");
    int qsize = quit.size();
    int posq = Half - qsize/2;
    if ((qsize % 2) == 1) {
	posq -= 1;
    }
    werase(lmessage);
    // finish box in lmessage segment of lesson screen
    mvwhline(lmessage, 0, 0, ACS_HLINE, posq-1);
    mvwhline(lmessage, 0, posq+qsize+1, ACS_HLINE, Width-2);
    wattrset(lmessage, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(lmessage, 0, posq, quit.c_str());
    wattrset(lmessage, A_NORMAL | COLOR_PAIR(1));
    wnoutrefresh(lmessage);

    doupdate();
}

void Kurses::scrMessage(int i, bool fin, string str) {
    
    // put standard end of lesson message onto the lesson screen
	
    string quit;

    if (i == 50) {
	// must be the test lesson then
	if (fin)
	    quit = "CNTRL+E for the main menu";
	else
	    quit = "Spacebar for welcome screen, 'R' to repeat test";
    }
    else if (i == 6) {
	// must be a speed lesson then
	if (fin)
	    quit = "CNTRL+E for main menu, 'N' for another speed test";
	else
	    quit = "CNTRL+E for main menu, 'R' to repeat speed test";
    }
    else if (i >= 2 && i <= 5) {
	// must be a fortune lesson then
	quit = "CNTRL+E for the main menu, 'N' for more epigrams";
    }
    else {
	// must be a default lesson then
	if (fin)
	    quit = "CNTRL+E for main menu, 'N' for next lesson, 'R' to repeat lesson";
	else
	    quit = "CNTRL+E for main menu, 'R' to repeat lesson";
    }
	    
    int qsize = quit.size();
    int posq = Half - qsize/2;
    if ((qsize % 2) == 1) {
	posq -= 1;
    }
    int ssize = str.size();
    int poss = Columns/2 - ssize/2;
    if ((ssize % 2) == 1) {
	poss -= 1;
    }
	
    // de-activate cursor
    curs_set(0);

    werase(arrayout);
    wattrset(arrayout, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(arrayout, 0, poss, str.c_str());
    wattrset(arrayout, 0);
    wnoutrefresh(arrayout);

    werase(lmessage);
    mvwhline(lmessage, 0, 0, ACS_HLINE, posq-1);
    mvwhline(lmessage, 0, posq+qsize+1, ACS_HLINE, Width-2);
    wattrset(lmessage, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(lmessage, 0, posq, quit.c_str());
    wattrset(lmessage, A_NORMAL | COLOR_PAIR(1));
    wnoutrefresh(lmessage);
    
    doupdate();
}

void Kurses::scrFinish(vector<string> str, char* message) {

    // print closing message and statistics
    
    int poss = Width/2 - strlen(message)/2;
    
    curs_set(0);
    clear();
    wbkgdset(mainwin, COLOR_PAIR(4));
    wattron(mainwin, A_BOLD);
    
    for (unsigned int i = 0; i < str.size(); i++) {
	mvwaddstr(mainwin, 8+i, poss, str[i].c_str());
    }
    wrefresh(mainwin);
    
    for (unsigned int i = 0; i < strlen(message); i++) {
	mvwaddch(mainwin, 12, poss+i, message[i]);
	wrefresh(mainwin);
	usleep(25000);
    }
    wattrset(mainwin, 0);
    usleep(2000000);
}

void Kurses::scrClose() { 

    if (!Opened) return;
    
    // free() the menus before closing 
    if (Scrmenu) {
	unpost_menu(firstmenu);
	free_menu(firstmenu);
	for(int i = 0; i < FMENUSIZE; i++)
	    free_item(firstitems[i]);
    }
    else free(firstitems);
	
    if (Setless) {
	free_menu(secondmenu);
	unpost_menu(secondmenu);
	for(int i = 0; i < SMENUSIZE; i++)
	    free_item(seconditems[i]);
    }
    else free(seconditems);

    endwin(); 

    // if scrClose() forced then flush stdout
    cout.flush();

    // we're no longer opened
    Opened = false;
}

void Kurses::sizeEvent() throw(Error) {
    
    struct winsize size;
    
    try {
	if (ioctl(fileno(stdout), TIOCGWINSZ, &size) == 0) {
	    if (size.ws_col < Width || size.ws_row < Height)
		throw Error("please don't lessen the screen size in future, okay!");
	    else {
		endwin();
		if (!Inlesson) {
		    // we're not doing a lesson
		    curs_set(0);
		}
		else {
		    // we're doing a lesson
		    int Cursor = Z <= Middle ? Z-1 : Middle;
		    wmove(arrayout, 0, Cursor);
		    wnoutrefresh(arrayout);
		}
		wnoutrefresh(curscr);
		doupdate();
	    }
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("sizeEvent returned an unhandled exception: nCurses.cpp");
    }
}
