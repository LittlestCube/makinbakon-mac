/* -- format.cpp -- */

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
 protected Display base class function formats text to int cols paremeter 
 ---------------------------------------------------------------------------*/

#include "display.h"
#include <string>

using namespace std;

void Display::scrFormat(int i, string& str) {
            
    int j = 0;       // counts strlen
    int k = 0;       // count int i chunks
    int flag = 0;    // the '\n' marker
    
    // If i is 20 and flag is 15 (the nearest space to 20) and j reaches
    // 20, k will be set to 5, so only counting 15 the next time around,
    // when flag sets the position of the next '\n'.

    for (j = 0, k = 0; j < static_cast<int>(str.size()); j++, k++) {
        if (k >= i) {
            k = j - flag;
            str[flag] = '\n';
        }
        if (str[j] == ' ')
            flag = j;
        if (str[j] == '\n')
            k = 0;
    }
    // remove the final '\n' if there is one
    if (str[str.size()-1] == '\n') str.erase(str.size()-1, 1);
}
