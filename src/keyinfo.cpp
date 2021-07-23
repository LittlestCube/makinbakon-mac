/* -- Keyinfo.cpp -- */

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
 Keyinfo class returns finger position information for Display class.
----------------------------------------------------------------------------*/

#include "keyinfo.h"
#include "keyboard.h"
#include <iostream>

using namespace std;

Keyinfo::Keyinfo(unsigned char askii) {
    
    Keyboard kboard;    // check ascii values against Key array

    if (askii == ' ') {
	finger = "Hit the space bar with a thumb!";
	ch = askii;
	shift = 0;
    }
    else if (askii == kboard.askii.getAskii(askii)) {
	
	ch = askii;
	
	finger = "\"";
	finger += askii;
	finger += "\"";
	
	int digit = kboard.askii.getFinger(askii);
	
	if (digit == 1)
	    finger += " Pinky ";
	else if (digit == 2)
	    finger += " Ring ";
	else if (digit == 3)
	    finger += " Middle ";
	else if (digit == 4)
	    finger += " Index ";
	
	if (kboard.askii.getHand(askii)) 
	    finger += ": Right Hand ";
	else 
	    finger += ": Left Hand ";
	
	// '0' not shifted, 'R' right shifted...
	if ((shift = kboard.askii.getShift(askii)) != 0) {
	    if (shift == 'R')
		finger += ": (R)Shift";
	    else if (shift == 'L')
		finger += ": (L)Shift";
	}
    }
}
