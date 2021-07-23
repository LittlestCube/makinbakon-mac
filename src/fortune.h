/* -- fortune.h -- */

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
 Fortune gets the fortune epigrams from the fortune program, formats them,
 and returns them back to lesson. If it can't get the fortune or open the
 epigram file in ~/.pigsty it throws an error.
 ---------------------------------------------------------------------------*/

#ifndef FORTUNE_H
#define FORTUNE_H

#include "error.h"
#include "pigsty.h"
#include <string>

class Fortune {
  private:
    Fortune(const Fortune& src);           // never defined
    void operator=(const Fortune& src);    // never defined
  private:
    Pigsty* sty;           // get ~/.pigsty address
    char* styloc;          // holds location of .pigsty dir
    char* command;         // holds system command
    std::string fortune;   // holds fortune epigram
    int lnum;              // the fortune lesson to do
  public:
    std::string &getFortune() throw(Error);
    // construct command and return reference to epigram
    Fortune(const int i) throw(Error);
    // construct styloc
    void fformat(std::string& str);
    // remove unwanted \t's and \n's etc.
    ~Fortune();
    // delete dynamic arrays
};

inline Fortune::~Fortune() { 
    if (command != 0) {
	delete[] command; command = 0;
    }
    delete[] styloc; styloc = 0;
}

#endif
