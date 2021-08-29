/* -- lessonDefault.cpp -- */

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
 LessonDefault(), or the default structured lessons, parameters 'lnum' lesson
 number, 'i' menu index and menu array.
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "macros.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int Lesson::lessonDefault(int lnum, int i, const string menu[]) {

    string temp;                    // holds the complete lesson text
    unsigned long usecs = 10000;    // timer setting between text lines
    int quit = 0;                   // quit sig from lessonCheck()
    size_t found = 0;               // holds position in lesson string
    int n = 0;                      // less than 'lnum' goto next lesson
    
    while (1) {
	
	bool fin = true;          // true if we finished the lesson
	charCount = 0;            // set character count to zero
	
	// get the lesson title and intro and set n to lnum
	if (lnum > n || lnum == LESSONONE) {
	    title = menu[i];
	    title += stats->getTarget();
	    intro = gettext->getIntro(i);
	    n = lnum;
	}
	
	// refresh titles and get the lesson string
	out->scrTitle(title);
	temp = gettext->getLesson(lnum);
	
	// print out the stats even when zero
	if (statsFlag) out->scrStats(stats->getScore());
	
	while (1) {
	    // split lesson on each newline
	    if ((found = temp.find('\n', 0)) != temp.npos) {
		nlesson.assign(temp, 0, found);
		nlesson.replace(found, 1, " ");
		// check for unwanted spaces at end of line
		for (unsigned int i = nlesson.size()-1; i > 0 ; i--) {
		    if (nlesson[i] == ' ') {
			if (nlesson[i-1] == ' ') {
			    nlesson.erase(i, 1);
			}
		    }
		    else break;
		}
		temp.erase(0, found+1);
	    }
	    // gettext sorts out the '\n' for us...
	    else break;

	    // if !statsFlag turn off the stats window
	    if (!statsFlag) {
		out->scrStats();
	    }
	    // print out the text and test it
	    out->scrLesson(nlesson);
	    quit = (lessonCheck(nlesson));

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
		break;
	    }
	    usleep(usecs);
	}

	if (fin) {
	    // set the final lesson stats
	    stats->setStats();
	}
	// print out stats and end of lesson message
	out->scrStats(stats->getScore());
	out->scrMessage(i, fin, lessonMessage(i, fin));
	
	bool exitflag = false;
	
	while(1) {
	    int ch = out->scrInput();

	    if (ch == out->QUIT) return 'Q';
	    // goto main menu
	    else if (ch == out->ENDLESSON) {
		if (fin) {
		    increment(i, lnum);
		}
		exitflag = true;
		break;
	    }
	    // repeat lesson
	    else if (ch == 'R' || ch == 'r') break;
	    // do next lesson
	    else if (ch == 'N' || ch == 'n') {
		if (fin) {
		    increment(i, lnum);
		    break;
		}
	    }
	}
	if (exitflag) break;
    }
    return 0;
}

void Lesson::increment(int &index, int& lesson) {
    
    // increment the default lesson
    if (lesson == LASTLESSON) {
	lesson = LESSONONE;
	score->setLesson(lesson);
    }
    else {
	score->setLesson(++lesson);
    }
    // increment index
    index = GETINDEX(lesson);
}

