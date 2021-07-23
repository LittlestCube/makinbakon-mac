/* -- lessonSpeed.cpp -- */

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
 LessonSpeed(), or speed typing lessons with Fortune epigrams.
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include <iostream>
#include <string>

using namespace std;

int Lesson::lessonSpeed(int i, const string menu[]) {

    int quit = 0;              // quit sig from lessonCheck()
    bool exitflag = false;     // quit the lesson
    bool newspeed = true;      // get new speed lesson text
	
    // get the lesson title and intro
    title = menu[i];
    title += stats->getTarget();
    intro = gettext->getIntro(i);

    while (1) {
	
	// true if we finished the lesson
	bool fin = true;

	// refresh the lesson title and quit
	// message and print out the stats
	out->scrTitle(title);
	if (statsFlag) out->scrStats(stats->getScore());

	// get the (random) speed test
	if (newspeed) {
	    nlesson = gettext->getSpeed();
	    newspeed = false;
	}
	// if !statsFlag turn off the stats window
	if (!statsFlag) {
	    out->scrStats();
	}
	// print the text and test it
	out->scrLesson(nlesson);
	quit = lessonCheck(nlesson);

	if (quit) {
	    if (quit == 'Q') {
		return 'Q';
	    }
	    else {
		// reset the stats to those
		// of last completed lesson
		stats->setLast();
	    }
	    fin = false;
	}
	if (fin) {
	    // set the stats if we finished the lesson
	    stats->setStats();
	}
	// print out the stats and end of lesson message
	out->scrStats(stats->getScore());
	out->scrMessage(i, fin, lessonMessage(i, fin));

	while(1) {
	    int ch = out->scrInput();
	    
	    if (ch == out->QUIT) return 'Q';
	    // goto main menu
	    else if (ch == out->ENDLESSON) { 
		exitflag = true;
		break;
	    }
	    // repeat lesson 
	    else if (ch == 'R' || ch == 'r') {
		if (! fin) break;
	    }
	    // do next lesson
	    else if (ch == 'N' || ch == 'n') {
		if (fin) {
		    newspeed = true;
		    break;
		}
	    }
	}
	if (exitflag) break;
    }
    return 0;
}
