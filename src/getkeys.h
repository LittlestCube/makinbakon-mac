/* -- getkeys.h -- */

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
 Define the key arrays for FUNCTION, RETURN and STANDARD key characteristics.
 ---------------------------------------------------------------------------*/

#ifndef GETKEYS_H
#define GETKEYS_H

#include "key.h"
#include "error.h"
#include "destroyer.h"
#include <string>
#include <new>

class Getkeys {
  private:
    Getkeys(const Getkeys& src);              // never defined
    void operator=(const Getkeys& src);       // never defined
  private:
    Fkey* farray;           // an array of Function key characteristics
    Rkey* rarray;           // an array of Return key characteristics
    Skey* sarray;           // an array of Standard key characteristics
    std::string kbtype;     // the keyboard type (us or uk...)
    int kbsize;             // the size of the standard keyboard
  private:
    static Getkeys* zInstance;                // ptr to singleton instance
    static Destroyer<Getkeys> destroyer;      // destroy singleton instance
  protected:
    Getkeys() throw(Error, std::bad_alloc);   // default constructor
    virtual ~Getkeys();                       // default destroy arrays
    friend class Destroyer<Getkeys>;          // destroy singleton
  public:
    static Getkeys* Instance() throw(Error);  // create singleton instance
  public:
    Fkey* getFkey();         // return array of Function key characteristics
    Rkey* getRkey();         // return array of Return key characteristics
    Skey* getSkey();         // return array of Standard key characteristics
    int& getSize();          // return the size of standard keyboard
};

// return array of Function key characteristics
inline Fkey* Getkeys::getFkey() { return farray; }

// return array of Return key characteristics
inline Rkey* Getkeys::getRkey() { return rarray; }

// return array of Standard key characteristics
inline Skey* Getkeys::getSkey() { return sarray; }

// return the size of standard keyboard
inline int& Getkeys::getSize() { return kbsize; }

inline Getkeys::~Getkeys() { 
    delete[] farray; farray = 0;
    delete rarray; rarray = 0;
    delete[] sarray; sarray = 0;
}    

#endif
