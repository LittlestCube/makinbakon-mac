/* -- lessonError.cpp -- */

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
 Pass typing error messages to the screen. These are random "Ouch" style...
 ---------------------------------------------------------------------------*/

#include "lesson.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

void Lesson::lessonError(bool error) {
    
    if (error) {
        string prod("\"Eeeeeeeeek\"");
        string kick("\"Squeeeeeel\"");
        string steroid("\"Ouuuuuuuch\"");
        
        srand(time(NULL));

        int n = (rand() % 3) + 1;

        switch (n) {
          case 1: estring = prod;
            break;
          case 2: estring = kick;
            break;
          case 3: estring = steroid;
            break;
        }
	out->scrError(estring);
    }
    else {
	estring = "";
	out->scrError(estring);
    }
}
