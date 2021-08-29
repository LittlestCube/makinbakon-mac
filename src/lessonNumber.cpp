/* -- lessonNumber.cpp -- */

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
 Which lesson to do next. int menu index and string menu array. Menu passes
 the following lesson codes...

 0x0 = Default Lesson     0xB = Focus keys G/H       0x16 = Focus keys Z
 0x1 = Change Default     0xC = Focus keys E/O       0x17 = Alphabet Soup
 0x2 = Humorous Pig       0xD = Capitals and '.'     0x18 = Focus Keys 1/2/3
 0x3 = Poetic Pig         0xE = Focus keys R/I       0x19 = Focus keys 4/5
 0x4 = StarTrek Pig       0xF = Focus keys W/U       0x1A = Focus keys 6/7
 0x6 = Offensive Pig      0x10 = Focus keys Q/P      0x1B = Focus keys 8/9/0
 0x5 = Distracted Pig     0x11 = Focus keys T/Y      0x1C = Symbol Madness 1
 0x7 = Speed Test         0x12 = Focus keys N/V      0x1D = Symbol Madness 2
 0x8 = Can Pig Help?      0x13 = Focus keys M/C      0x1E = Test Lesson
 0x9 = Quit the Pig!      0x14 = Comma and Colon     
 0xA = The Home Keys      0x15 = Focus keys B/X      

 Note:- Default lessons are split into two parts starting from lesson 10.
 A macro calculates the actual lesson number from the menu index and the
 menu index from the lesson number. 
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include "macros.h"
#include "defaults.h"
#include <iostream>
#include <string>

using namespace std;

int Lesson::lessonNumber(int i, const string menu[]) {
    
    int quit = 0;     // return value from lesons
    int lnum = 0;     // the lesson number
    
    // start the default lesson
    
    if (i == 0) {
	lnum = score->getLesson();
	
	
	if (lnum == 0) {
	    lnum = LESSONONE;
	    score->setLesson(lnum);
	}
	// get array index for lesson intros
	i = GETINDEX(lnum);
	
	if ((quit = lessonDefault(lnum, i, menu)) == 'Q')
	    return 'Q';
    }
    // choose new default and start
    else if (i >= 10 && i <= 29) {
	lnum = GETLESSON(i);
	score->setLesson(lnum);
	if ((quit = lessonDefault(lnum, i, menu)) == 'Q')
	    return 'Q';
    }
    // do fortune lesson
    else if (i >= 2 && i <= 6) {
	if ((quit = lessonFortune(i, menu)) == 'Q')
	    return 'Q';
    }
    // do speed test
    else if (i == 7) {
	if ((quit = lessonSpeed(i, menu)) == 'Q')
	    return 'Q';
    }
    // read the help screen
    else if (i == 8) {
	if ((quit = out->scrHelp(gettext->getHelp())) == 'Q')
	    return 'Q';
    }
    return 0;
}
