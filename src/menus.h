/* -- menus.h -- */

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
 Menus does as it's name suggests and defines the Makin' Bakon menu, which is
 Display type dependant. As there is only one Display type at present, there
 can only be one menu.
 ---------------------------------------------------------------------------*/

#ifndef MENUS_H
#define MENUS_H

#include "macros.h"
#include <iostream>
#include <string>

class Menus {
  private:
    void operator=(const Menus& src);          // not defined
    Menus(const Menus& src);                   // not defined
  public:
    std::string mainmenu[FMENUSIZE];           // main menu with shortcuts
    std::string mainscut[FMENUSIZE];           // main menu without shortcuts
    std::string lessonmenu[SMENUSIZE];         // the menu array
    Menus() { }                                // default constructor
    virtual ~Menus() { }                       // defualt destructor
  public:
    virtual const std::string* getMain() = 0;      // return main menu
    virtual const std::string* getScut() = 0;      // return main sans s/cuts
    virtual const std::string* getLess() = 0;      // return lesson menu
};

class Kmenu : public Menus {
  private:
    void operator=(const Kmenu& src);
    Kmenu(const Kmenu& src);
  public:
    Kmenu() {
	// main menu items
	mainmenu[0] = "Default Lesson  (L)";
	mainmenu[1] = "Change Default  (C)";
	mainmenu[2] = "Humorous Pig    (F)";
	mainmenu[3] = "Poetic Pig      (P)";
	mainmenu[4] = "StarTrek Pig    (S)";
	mainmenu[5] = "Offensive Pig   (O)";
	mainmenu[6] = "Distracted Pig  (D)";
	mainmenu[7] = "Speed Test      (T)";
	mainmenu[8] = "Can Pig Help?   (H)";
	mainmenu[9] = "Quit the Pig!   (Q)";
	// default lessons
	lessonmenu[0]  = "The Home Keys";
	lessonmenu[1]  = "Focus keys G/H";
	lessonmenu[2]  = "Focus keys E/O";
	lessonmenu[3]  = "Capitals and '.'";
	lessonmenu[4]  = "Focus keys R/I";
	lessonmenu[5]  = "Focus keys W/U";
	lessonmenu[6]  = "Focus keys Q/P";
	lessonmenu[7]  = "Focus keys T/Y";
	lessonmenu[8]  = "Focus keys N/V";
	lessonmenu[9]  = "Focus keys M/C";
	lessonmenu[10] = "Comma and Colon";
	lessonmenu[11] = "Focus keys B/X";
	lessonmenu[12] = "Focus keys Z/-";
	lessonmenu[13] = "Alphabet Soup";
	lessonmenu[14] = "Focus Keys 1/2/3";
	lessonmenu[15] = "Focus keys 4/5";
	lessonmenu[16] = "Focus keys 6/7";
	lessonmenu[17] = "Focus keys 8/9/0";
	lessonmenu[18] = "Symbol Madness 1";
	lessonmenu[19] = "Symbol Madness 2";
	
	// assign mainmenu to menuscut and remove shortcut information
	for (int n = 0; n < FMENUSIZE; n++) {
	    mainscut[n] = mainmenu[n];
	    // find the shortcut part of the string
	    int found = mainscut[n].find("(");
	    // search back for a character != space
	    for (int z = found-1; z > 0; z--) {
		if (mainscut[n][z] == ' ')
		    found -= 1;
		else
		    break;
	    }
	    // remove the unwanted characters
	    mainscut[n].erase(found, (mainscut[n].size() - found));
	}
    }
    ~Kmenu() { }
    const std::string* getMain() { return mainmenu; }
    const std::string* getScut() { return mainscut; }
    const std::string* getLess() { return lessonmenu; }
};    

#endif
