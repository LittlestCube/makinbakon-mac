/* -- keyboard.h -- */

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
 class Keyboard :- The Keyboard class is the keys on the Keyboard. If we don't
 'assume' anything about the keyboard then it makes sense to place everything 
 we need to know in here, thus making it easy to include different types of 
 keyboard layout simply by defining them in class Getkeys.

 class Askii:- return the char value of a particular key. The unsigned char
 parameter needs to be translated into an array element. Subracting 33 will
 do for most values, except for the British £, where we'd need to subtract 69.

 class Index:- return the key array index of a particluar key.
 ---------------------------------------------------------------------------*/
    
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "key.h"
#include "error.h"
#include "getkeys.h"
#include <string>

class Keyarray {
  private:
    Keyarray(const Keyarray& src);          // never defined
    void operator=(const Keyarray& src);    // never defined
  public:
    Keyarray() throw(Error);                // fill key arrays and kbsize
    virtual ~Keyarray() { }                 // default destructor
  protected:
    Getkeys* getkeys;                       // start a Getkeys instance
  protected:
    int kbsize;                             // num keys in Key array
    int skey[4];                            // Key coordinates
    int rkey[2];                            // the RETURN key coordinates
  protected:
    Fkey* farray;                           // array of function keys
    Rkey* rarray;                           // the return key
    Skey* sarray;                           // an array of keys
};

class Askii : public Keyarray {
  private:
    Askii(const Askii& src);                // never defined
    void operator=(const Askii& src);       // never defined
  public:
    Askii() { }                             // default constructor
    ~Askii() { }                            // default destructor
  public:
    const unsigned char getAskii(unsigned char c);  // key ascii value
    const int getMajor(unsigned char m);            // 'A' is major key to 'a'
    const int* getKcoords(unsigned char x);         // returns key coordinates
    const int getFinger(unsigned char f);           // key belongs to a finger
    const bool getHand(unsigned char h);            // key belongs to a hand
    const int getShift(unsigned char s);            // is the key shifted
};

class Index : public Keyarray {
  private:
    Index(const Index& src);                // never defined
    void operator=(const Index& src);       // never defined
  public:
    Index() { }                             // default constructor
    ~Index() { }                            // default destructor
  public:
    const int getAskii(int c);              // key ascii value
    const int getMajor(int m);              // 'A' is major key to 'a'
    const int* getKcoords(int x);           // return the key coordinates
    const int getFinger(int f);             // key belongs to a finger
    const bool getHand(int h);              // key belongs to a hand
    const int getShift(int s);              // is the key shifted
};

class Keyboard : public Keyarray {
  private:
    Keyboard(const Keyboard& src);             // never defined
    void operator=(const Keyboard& src);       // never defined
  public:
    Askii askii;         // access skey elements with char ascii values
    Index index;         // access skey elements with array index values
  public:
    Keyboard() { }       // default constructor
    ~Keyboard() { }      // default destructor
  public:
    const int* getFcoords(int k);        // return function-key coordinates
    const std::string getFname(int k);   // return function key name
    const int* getRcoords(int r);        // return return-key coordinates
    const int getKsize();                // return size of the Skey array
    const std::string getRname();        // return return key name
};

/*------------------------- Askii class functions --------------------------*/

// key ascii value
inline const unsigned char Askii::getAskii(unsigned char c) {
    if (c == 163) c -= 69;
    else c -= 33;
    return sarray[c].askii;
}
// 'A' is major key to 'a'
inline const int Askii::getMajor(unsigned char m) { 
    if (m == 163) m -= 69;
    else m -= 33;
    return sarray[m].mkey; 
}
// key belongs to a finger
inline const int Askii::getFinger(unsigned char f) { 
    if (f == 163) f -= 69;
    else f -= 33;
    return sarray[f].finger;
}
// key belongs to a hand
inline const bool Askii::getHand(unsigned char h) { 
    if (h == 163) h -= 69;
    else h -= 33;
    return sarray[h].hand;
}
// is the key shifted
inline const int Askii::getShift(unsigned char s) {
    if (s == 163) s -= 69;
    else s -= 33;
    return sarray[s].shift;
}

/*------------------------- Index class functions --------------------------*/

// key ascii value
inline const int Index::getAskii(int c) { return sarray[c].askii; }
// 'A' is major key to 'a'
inline const int Index::getMajor(int m) { return sarray[m].mkey; }
// key belongs to a finger
inline const int Index::getFinger(int f) { return sarray[f].finger; }
// key belongs to a hand
inline const bool Index::getHand(int h) { return sarray[h].hand; }
// is the key shifted
inline const int Index::getShift(int s) { return sarray[s].shift; }

/*----------------------- Keyboard class functions -------------------------*/

// get the size of the Skey array
inline const int Keyboard::getKsize() { return kbsize; }

// get return key name
inline const std::string Keyboard::getRname() { return rarray->name; }

/*--------------------------------------------------------------------------*/

#endif
