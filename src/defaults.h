/* -- defaults.h -- */

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
 Defaults class reads the ~/.bakonrc file for keyboard and color settings.
 ---------------------------------------------------------------------------*/

#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "error.h"
#include "pigsty.h"
#include "makemap.h"
#include <string>

class Defaults {
  private:
    Defaults(const Defaults& src);           // never defined
    void operator=(const Defaults& src);     // never defined
  private:
    static MakeMap<std::string, short> colors;        // colors map
    static MakeMap<std::string, std::string> defmap;  // defaults map
  private:
    Pigsty* sty;                             // get ~/HOME address
    char* Home;                              // hold ~/HOME address
    std::string Kbtype;                      // return the keyboard setting
    short Color;                             // return a color setting
  private:
    void readDefaults() throw(Error);        // read ~/.bakonrc defaults
  public:
    Defaults() throw(Error);                 // default constructor
    ~Defaults() { }                          // default destructor
  public:
    // change defaults map
    void setDefault(std::string key, std::string value) throw(Error);
    // get short int value
    short& getColor(std::string key) throw(Error);
    // get string value
    std::string& getKbtype() throw(Error);
};

#endif
