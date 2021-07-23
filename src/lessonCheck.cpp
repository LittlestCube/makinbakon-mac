/* -- LessonCheck.cpp -- */

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
 Lesson LessonCheck(), or check user input against lesson output. If wrong, 
 print error message. If right, remove error message if there is one.
 positFlag is for Keyinfo obj for displaying finger positions. statsFalg is
 for turning statistics display on/off. startFlag starts/stops the statistics
 timer.
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "keyboard.h"
#include "keyinfo.h"
#include <iostream>
#include <string>

using namespace std;

// initialise statics
bool Lesson::positFlag = true;
bool Lesson::statsFlag = true;
bool Lesson::startFlag = false;

int Lesson::lessonCheck(const string abc) {
    
    Keyboard kboard;    // check lesson array for valid input
    unsigned int i;     // loop count and incriment charCount
    
    for (i = 0; i < abc.size()-1; i++) {
        // get finger positioning
        if (positFlag) {
	    Keyinfo key(abc[i]);
            out->scrKey(key.finger, key.ch, key.shift);
        }
	
	// get the next input character
        int ch = out->scrInput();

	// quit signal quits the program
	if (ch == out->QUIT) {
	    return 'Q';
        }
        // switch finger position display on/off (on by default)
	else if (ch == out->NEXTKEY) {
            if (positFlag) {
                positFlag = false;
		out->scrKey();
            }
            else {
                positFlag = true;
            }
	    startFlag = false;
            i--;
        }
        // switch statistics display on/off (on by default)
        else if (ch == out->STATS) {
            if (statsFlag) {
                statsFlag = false;
                out->scrStats();
            }
            else {
		out->scrStats(stats->getScore());
                statsFlag = true;
            }
	    startFlag = false;
            i--;
        }
        // switch on lesson instruction display (off by default)
        else if (ch == out->INSTRUCT) {
	    while (1) {
		int rv = out->scrInstruct(intro);
		if (rv == 'Q') {
		    return 'Q';
		}
		else if (rv == 1) {
		    break;
		}
	    }
	    // re-set the timer
	    startFlag = false;
	    i--;
	}
        // break out if it all gets too much
        else if (ch == out->ENDLESSON) {
            if (error) {
                error = false;
		lessonError(error);
            }
	    startFlag = false;
            return 1;
        }
	else {
	    // it's a character that needs checking
	    unsigned char ch_copy = static_cast<unsigned char>(ch);
	    unsigned char in_copy = static_cast<unsigned char>(abc[i]);

	    // Pig hurts if you type the wrong key...
	    if (abc[i] == ' ' && ch != ' ') {
		// start the stats timer and count
		if (!startFlag) {
		    stats->start();
		    startFlag = true;
		}
		stats->error();
		error = true;
		lessonError(error);
		i--;
	    }
	    // ...but not if you type the correct key
	    else if ((abc[i] == ' ' && ch == ' ') ||
		     ch_copy == kboard.askii.getAskii(in_copy)) {
		// start the stats timer and count
		if (!startFlag) {
		    stats->start();
		    startFlag = true;
		}
		// unset flag and remove error message
		if (error) {
		    error = false;
		    lessonError(error);
		}
		// update lesson stats by one character
		stats->setScore(1);
		// print the stats
		if (statsFlag) {
		    out->scrStats(stats->getScore());
		}
		// finally, update the text display
		out->scrUI();
	    }
	    // any other letter must be wrong...
	    else {
		// start the stats timer and count
		if (!startFlag) {
		    stats->start();
		    startFlag = true;
		}
		stats->error();
		error = true;
		lessonError(error);
		i--;
	    }
	}
    }
    charCount += i;
    startFlag = false;
    return 0;
}
