/* -- lessonMessage.cpp -- */

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
 lessonMessage defines the message for each lesson, completed or not.
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "macros.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

string Lesson::lessonMessage(int i, bool fin) {

    if (i == TESTLESSON) {
	if (fin)
	    message = "You've completed the test! Well done!";
	else
	    message = "You must complete the test before going any further!";
    }
    else {
	if (fin) {
	    // we finished the lesson
	    if (stats->getCwpm() < stats->getTwpm()) {
		if (stats->getBwpm() < stats->getTwpm()) {
		    message = "You must type faster if you want to save Pig's bacon!";
		}
		else {
		    message = "\"Eeeeek! I can hear the sausage machine!\"";
		}
	    }
	    else {
		message = "Bonus point awarded for beating your target! Well Done!";
	    }
	}
	else {
	    // we didn't start the lesson
	    if (charCount < 1) {
		message = "What's wrong with this lesson, you bored or something?";
	    }
	    // we didn't finish the lesson
	    else {
		srand(time(NULL));
		int n = (rand() % 4) + 1;
		if (i < LESSONONE) {
		    // fortune lessons
		    switch (n) {
		      case 1:
			message = "Line not completed - Pig gets a steroid injection!";
			break;
		      case 2:
			message = "Line not completed - Pig gets a cattle prodding!";
			break;
		      case 3:
			message = "Line not completed - Pig gets a brutal kick!";
			break;
		      case 4:
			message = "Line not completed - Pig misses a meal!";
			break;
		    }
		}
		else {
		    // default lessons
		    switch (n) {
		      case 1:
			message = "Lesson not completed - Pig gets a steroid injection!";
			break;
		      case 2:
			message = "Lesson not completed - Pig gets a cattle prodding!";
			break;
		      case 3:
			message = "Lesson not completed - Pig gets a brutal kick!";
			break;
		      case 4:
			message = "Lesson not completed - Pig misses a meal!";
			break;
		    }
		}
	    }
	}
    }
    return message;
}

