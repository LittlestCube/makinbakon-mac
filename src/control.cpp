/* -- control.cpp -- */

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
 The Control class lists the available lessons and passes the lesson to be
 executed to the lesson class. It is also responsible for the closing message.
 ---------------------------------------------------------------------------*/

#include "control.h"
#include "lesson.h"
#include <iostream>
#include <string>
#include <sstream>
#include <new>
#include <iomanip>

using namespace std;

Control::Control(Display* iout, Menus* imenu) throw (Error)
    : out(iout), menu(imenu), message(0), choice(0)
{
    try {
	// create a Scores instance
	score = Scores::Instance();
	// create a Stats instance
	stats = Stats::Instance();

	// fill Makin' Bakon menu arrays
	string* mainptr = const_cast<string*>(menu->getMain());
	string* scutptr = const_cast<string*>(menu->getScut());
	string* lessptr = const_cast<string*>(menu->getLess());
	
	for (int i = 0; i < FMENUSIZE; i++) {
	    // the menu sent to display
	    *(menuone+i) = *(mainptr+i);
	    // the menu sent to lessons
	    *(menutwo+i) = *(scutptr+i);
	}
	for (int i = 0; i < SMENUSIZE; i++) {
	    *(menuone+FMENUSIZE+i) = *(lessptr+i);
	    *(menutwo+FMENUSIZE+i) = *(lessptr+i);
	}
    }
    catch (Error err) { 
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: control.cpp");
    }
}    

// start test lesson if no target set
int Control::start() {
    double targ = 0;
    
    // set default lesson number
    int i = score->getLesson();
    if (i == 0)	score->setLesson(LESSONONE);

    // if no target set open with test lesson
    if ((targ = stats->getTwpm()) < 1) {
	// start the test lesson
	Lesson nextLesson(out);
	while (1) {
	    int ret = nextLesson.lessonTest(TESTLESSON);
	    if (ret) {
		if (ret == 'Q') return 'Q';
		else break;
	    }
	}
    }
    return 1;
}

// post the menu and get choice
int& Control::showMenu() {
    
    choice = out->scrMenu(menuone);
    
    // the last menu item is always quit!
    if (choice == FMENUSIZE-1) choice = 'Q';

    return choice;
}

// execute the menu choice
int Control::execute() {
    
    // start the lessons
    Lesson nextLesson(out); 

    int quit = nextLesson.lessonNumber(choice, menutwo);
    
    if (quit == 'Q') return 'Q';

    return 0;
}

// close down with score and message
void Control::close() throw (Error) {
    
    // defines the score dependant closing message
    if (stats->getBwpm() < stats->getTwpm() || stats->getTwpm() < 1) {
	if ((message = new (std::nothrow) char[strlen(LOSE) + 1]) == 0) {
	    throw Error("bad_alloc - can't create message: control.cpp");
	}
	strcpy(message, LOSE);
    }
    else {
	if ((message = new (std::nothrow) char[strlen(WIN) + 1]) == 0) {
	    throw Error("bad_alloc - can't create message: control.cpp");
	}
	strcpy(message, WIN);
    }
    
    // defines the scores and titles
    string titles[] = { "Target Score:", "Closing WPM:", "Final Score:" };
    
    // add titles to strings and set width
    int m_size = strlen(message);

    for (int i = 0; i < 3; i++) {
	string temp(titles[i]);
	stringstream stream(ios::in|ios::out|ios::trunc);
	if (i == 0) {
	    stream << setprecision(2)
		   << setiosflags(ios::fixed)
		   << stats->getTwpm();
	}
	else if (i == 1) {
	    stream << setprecision(2)
		   << setiosflags(ios::fixed)
		   << stats->getCwpm();
	}
	else if (i == 2) {
	    stream << setprecision(2)
		   << setiosflags(ios::fixed)
		   << stats->getBwpm();
	}
	int spaces = m_size - (temp.size() + stream.str().size());
	for (int j = 0; j < spaces; j++) {
	    temp.append(" ");
	}
	temp.append(stream.str());
	scores.push_back(temp);
    }
    // print the scores and message
    out->scrFinish(scores, message);
    // clean up and close down!
    out ->scrClose();
}
