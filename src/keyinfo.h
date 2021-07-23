/* -- keyinfo.h --*/

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
 Keyinfo class return finger position information to Display class, comprising
 a string, an unsigned char and an int.
----------------------------------------------------------------------------*/

#ifndef KEYINFO_H
#define KEYINFO_H

#include <string>

class Keyinfo {
  private:
    Keyinfo(const Keyinfo& src);           // never defined
    void operator=(const Keyinfo& src);    // never defined
  public:
    Keyinfo(unsigned char askii);          // construct finger positions
    ~Keyinfo() { }                         // default destructor
  public:
    std::string finger;   // holds finger position string
    unsigned char ch;     // holds the character
    int shift;            // holds the shift value
};

#endif
