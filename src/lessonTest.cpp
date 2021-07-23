/* -- lessonTest.cpp -- */

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
 LessonTest(), or test the users typing speed, parameter 'i' lesson num
 This lesson displays a brief intro to Makin' Bakon before the test....
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "macros.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int Lesson::lessonTest(int i) {

    string temp;                     // holds the complete lesson text
    unsigned long usecs = 10000;     // timer setting between text lines
    string wtext;                    // holds the Makin' Bakon welcome
    int quit = 0;                    // quit sig from lessonCheck()
    size_t found = 0;          // holds position in lesson string

    // display the intro text
    wtext = gettext->getWelcome();
    out->scrWelcome(wtext);

    while (1) {
	int ch = out->scrInput();
	if (ch == out->QUIT) return 'Q';
	else if (ch == ' ') break;
    }
    
    // get the lesson title and intro
    title = "Test Lesson";
    intro = gettext->getIntro(LESSONONE);
    
    while (1) {
	
	// true if we finished the lesson
	bool fin = true;

	// refresh the lesson title and quit
	// message and print out the stats
	out->scrTitle(title);
	temp = gettext->getLesson(i);
	if (statsFlag) out->scrStats(stats->getScore());
	
	while (1) {
	    // split lesson on each newline
	    if ((found = temp.find('\n', 0)) != temp.npos) {
		nlesson.assign(temp, 0, found);
		nlesson.replace(found, 1, " ");
		temp.erase(0, found+1);
	    }
	    // gettest sorts out the '\n' for us...
	    else break;
	    
	    // if !statsFlag turn off stats window
	    if (!statsFlag) {
		out->scrStats();
	    }
	    // print the text and test it
	    out->scrLesson(nlesson);
	    quit = (lessonCheck(nlesson));

	    if (quit) {
		if (quit == 'Q') return 'Q';
		else {
		    // reset stats to zero
		    stats->setLast();
		    fin = false;
		}
		break;
	    }
	    usleep(usecs);
	}

	if (fin) {
	    // set the final stats
	    stats->setStats();
	    // set the target speed
	    if (stats->getCwpm() > stats->getTwpm()) {
		stats->setTarget();
	    }
	    // do test again 'cos score is zero
	    else { fin = false; }
	}
	// print out end of stats and end of lesson message
	out->scrStats(stats->getScore());
	out->scrMessage(i, fin, lessonMessage(i, fin));
	// reset the stats to zero before beginning lessons proper
	stats->reset();

	bool exitflag = false;
	
	while (1) {
	    int ch = out->scrInput();
	    
	    if (ch == out->QUIT) return 'Q';
	    // goto main menu
	    else if (ch == out->ENDLESSON) {
		if (fin) return 1;
	    }
	    // repeat lesson
	    else if (ch == 'R' || ch == 'r') {
		if (! fin) break;
	    }
	    // return to welcome screen
	    else if (ch == ' ') {
		if (! fin) {
		    exitflag = true;
		    break;
		}
	    }
	}
	if (exitflag) break;
    }
    return 0;
}
