/* -- lessonFortune.cpp -- */

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
 LessonFortune(), or fortune epigrams based lesson.
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "fortune.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int Lesson::lessonFortune(int i, const string menu[]) {

    unsigned long usecs = 10000;    // timer setting between epigrams
    int quit = 0;                   // quit sig from lessonCheck()
    int length = 250;               // number of characters in a lesson
    
    // get lesson title, intro and epigram
    title = menu[i];
    title += stats->getTarget();
    intro = gettext->getIntro(i);
    out->scrTitle(title);
    Fortune nextFortune(i);

    while (1) {
	
	charCount = 0;           // set character count to zero
	bool exitflag = false;   // true if we exit the lesson
	bool fin = true;         // true if we finish the lesson

	// refresh the lesson title and quit
	// meaasge and print out the stats
	out->scrTitle(title);
	if (statsFlag) out->scrStats(stats->getScore());

	while (1) {

	    // if !statsFlag turn off the stats window
	    if (!statsFlag) {
		out->scrStats();
	    }
	    // print the epigram out and test it
	    epigram = nextFortune.getFortune();
	    out->scrLesson(epigram);
	    quit = lessonCheck(epigram);
	    
	    if (quit) {
		if (quit == 'Q') {
		    return 'Q';
		}
		else {
		    // reset the stats to those
		    // of last completed lesson
		    stats->setLast();
		    fin = false;
		}
		break;
	    }
	    else  {
		// set the stats if we finished the lesson
		if (charCount >= length) {
		    stats->setStats();
		    break;
		}
	    }
	    usleep(usecs);
	}
	// print out the stats and end of lesson message
	out->scrStats(stats->getScore());
	out->scrMessage(i, fin, lessonMessage(i, fin));
	
	while(1) {
	    int ch = out->scrInput();
	    if (ch == out->QUIT) return 'Q';
	    // return to main menu
	    else if (ch == out->ENDLESSON) {
		exitflag = true;
		break;
	    }
	    // next epigram
	    else if (ch == 'N' || ch == 'n') break;
	}
	if (exitflag) break;
    }
    return 0;
}
