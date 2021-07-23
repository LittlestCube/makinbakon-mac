/* -- control.h -- */

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
 The Control class lists the available lessons and passes the lesson to be
 executed to the lesson class. It is also responsible for the closing message.
 ---------------------------------------------------------------------------*/

#ifndef CONTROL_H
#define CONTROL_H

#include "display.h"
#include "scores.h"
#include "stats.h"
#include "menus.h"
#include "error.h"
#include "macros.h"
#include <string>
#include <vector>

class Control {
  private:
    Control(const Control& src);            // never defined
    void operator=(const Control& src);     // never defined
  private:
    Display* out;       // Display ptr
    Menus* menu;        // get the Makin' Bakon menu
    Scores* score;      // get default scores (wpm and speed)
    Stats* stats;       // get wpm and session target
  private:
    std::vector<std::string> scores;    // hold stats strings for close()
    char* message;                      // closing message for close()
  private:
    std::string introtext;              // the Makin' Bakon intro text
    std::string menuone[MENUSIZE];      // defines a menu
    std::string menutwo[MENUSIZE];      // defines a menu
  public:
    Control(Display* iout, Menus* imenu) throw(Error);  
    // default constructor
    ~Control();
    // default destructor
    int start();
    // check defaults and start the show
    int &showMenu(void);
    // sends the menu to display and returns lesson choice
    int execute(void);
    // executes the chosen lesson
    void close(void) throw(Error);
    // i is the wpm target speed for the session, if speed is
    // reached then print good message, otherwise not so good
    int choice;
    // lesson to execute
};

inline Control::~Control() { 
    if (message != 0) {
	delete[] message;
	message = 0;
    }
}

#endif
