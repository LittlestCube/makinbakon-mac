/* -- keyboard.cpp -- */

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

/*---------------------------------------------------------------------------
 Get the keybaord type for the lesson and return key characteristics.
 --------------------------------------------------------------------------*/

#include "keyboard.h"
#include <iostream>
#include <string>

using namespace std;

/*----------------------- Keyarray class functions -------------------------*/

// fill arrays and get keyboard size
Keyarray::Keyarray() throw (Error) {

    try {
	// start a Getkeys instance
	getkeys = Getkeys::Instance();
	// set Skey total
	kbsize = getkeys->getSize();
	// set ptrs to key arrays
	farray = getkeys->getFkey();
	rarray = getkeys->getRkey();
	sarray = getkeys->getSkey();
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: keyboard.cpp");
    }
}

/*----------------------- Keyboard class functions -------------------------*/

// get the function-key coordinates: the array maps function
// keys clockwise around the keyboard from backspace to tab
const int* Keyboard::getFcoords(int k) {
    if (k > 9) {
	switch (k) {
	  case 'R': k = 1; break;
	  case ' ': k = 4; break;
	  case 'L': k = 7; break;
	}
    }
    skey[0] = farray[k].r1;
    skey[1] = farray[k].c1;
    skey[2] = farray[k].r2;
    skey[3] = farray[k].c2;
    return skey;
}

// get function key name
const string Keyboard::getFname(int k) { 
    if (k > 9) {
	switch (k) {
	  case 'R': k = 1; break;
	  case 'L': k = 7; break;
	}
    }
    return farray[k].name;
}

// get the return-key coordinates. We treat the return key as a special
// case because of its awkward shape, work clockwise from upper right
// corner: r1, c1, trr, trc, r2, c2, blr, blc, mrr, mrc, mlr, mlc.
const int* Keyboard::getRcoords(int r) {
    switch (r) {
      case 0: rkey[0] = rarray->r1;  rkey[1] = rarray->c1;  break;
      case 1: rkey[0] = rarray->trr; rkey[1] = rarray->trc; break;
      case 2: rkey[0] = rarray->r2;  rkey[1] = rarray->c2;  break;
      case 3: rkey[0] = rarray->blr; rkey[1] = rarray->blc; break;
      case 4: rkey[0] = rarray->mrr; rkey[1] = rarray->mrc; break;
      case 5: rkey[0] = rarray->mlr; rkey[1] = rarray->mlc; break;
    }
    return rkey;
}

/*------------------------- Askii class functions --------------------------*/

// get the key coordinates
const int* Askii::getKcoords(unsigned char x) {
    if (x == 163) x -= 69;
    else x -= 33;
    skey[0] = sarray[x].r1;
    skey[1] = sarray[x].c1;
    skey[2] = sarray[x].r2;
    skey[3] = sarray[x].c2;
    return skey;
}

/*------------------------- Index class functions --------------------------*/

// get the key coordinates
const int* Index::getKcoords(int x) { 
    skey[0] = sarray[x].r1; 
    skey[1] = sarray[x].c1;
    skey[2] = sarray[x].r2; 
    skey[3] = sarray[x].c2;

    return skey;
}

/*--------------------------------------------------------------------------*/
