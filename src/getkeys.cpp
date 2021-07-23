/* -- getkeys.cpp -- */

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
 Define the keyboard arrays for FUNCTION, RETURN and STANDARD keys.
 ---------------------------------------------------------------------------*/

#include "getkeys.h"
#include "macros.h"
#include "defaults.h"
#include <iostream>
#include <new>

using namespace std;

// initialise zInstance statics
Getkeys* Getkeys::zInstance = 0;
Destroyer<Getkeys> Getkeys::destroyer;

// create singleton instance
Getkeys* Getkeys::Instance() throw (Error) {
    if (zInstance == 0) {
	if ((zInstance = new (std::nothrow) Getkeys) == 0) {
	    throw Error("can't create getkeys instance: getkeys.cpp");
	}
	destroyer.setDoomed(zInstance);
    }
    return zInstance;
}

// fill the key arrays
Getkeys::Getkeys() throw (Error, bad_alloc) {
    
    try {
	// get default kbtype
	Defaults defaults;
	kbtype = defaults.getKbtype();
	// set sarray for kbtype
	if (kbtype == UKTYPE) {
	    kbsize = KBSIZE-1;
	    sarray = new Skey[kbsize];
	    // Skey[0] ascii 33
	    sarray[0] = Skey(0, 4,  2,  7,   '1', PINKY,   LEFT,  'R',  '!');
	    sarray[1] = Skey(0, 8,  2,  11,  '2', RING,    LEFT,  'R',  '"');
	    sarray[2] = Skey(6, 52, 8,  55,   0,  PINKY,   RIGHT,  0,   '#');
	    sarray[3] = Skey(0, 16, 2,  19,  '4', MIDDLE,  LEFT,  'R',  '$');
	    sarray[4] = Skey(0, 20, 2,  23,  '5', INDEX,   LEFT,  'R',  '%');
	    sarray[5] = Skey(0, 28, 2,  31,  '7', INDEX,   RIGHT, 'L',  '&');
	    sarray[6] = Skey(6, 48, 8,  51,   0,  PINKY,   RIGHT,  0,  '\'');
	    sarray[7] = Skey(0, 36, 2,  39,  '9', MIDDLE,  RIGHT, 'L',  '(');
	    sarray[8] = Skey(0, 40, 2,  43,  '0', RING,    RIGHT, 'L',  ')');
	    sarray[9] = Skey(0, 32, 2,  35,  '8', INDEX,   RIGHT, 'L',  '*');
	    sarray[10] = Skey(0, 48, 2,  51,  '=', PINKY,   RIGHT, 'L',  '+');
	    sarray[11] = Skey(9, 38, 11, 41,   0,  MIDDLE,  RIGHT,  0,   ',');
	    sarray[12] = Skey(0, 44, 2,  47,   0,  PINKY,   RIGHT,  0,   '-');
	    sarray[13] = Skey(9, 42, 11, 45,   0,  RING,    RIGHT,  0,   '.');
	    sarray[14] = Skey(9, 46, 11, 49,   0,  PINKY,   RIGHT,  0,   '/');
	    // Skey[15] ascii 48
	    sarray[15] = Skey(0, 40, 2, 43,  0, RING,   RIGHT, 0, '0');
	    sarray[16] = Skey(0, 4,  2,  7,  0, PINKY,  LEFT,  0, '1');
	    sarray[17] = Skey(0, 8,  2, 11,  0, RING,   LEFT,  0, '2');
	    sarray[18] = Skey(0, 12, 2, 15,  0, MIDDLE, LEFT,  0, '3');
	    sarray[19] = Skey(0, 16, 2, 19,  0, MIDDLE, LEFT,  0, '4');
	    sarray[20] = Skey(0, 20, 2, 23,  0, INDEX,  LEFT,  0, '5');
	    sarray[21] = Skey(0, 24, 2, 27,  0, INDEX,  LEFT,  0, '6');
	    sarray[22] = Skey(0, 28, 2, 31,  0, INDEX,  RIGHT, 0, '7');
	    sarray[23] = Skey(0, 32, 2, 35,  0, INDEX,  RIGHT, 0, '8');
	    sarray[24] = Skey(0, 36, 2, 39,  0, MIDDLE, RIGHT, 0, '9');
	    // Skey[25] ascii 59
	    sarray[25] = Skey(6, 44, 8,  47,  ';',  PINKY,  RIGHT, 'L', ':');
	    sarray[26] = Skey(6, 44, 8,  47,   0,   PINKY,  RIGHT,  0,  ';');
	    sarray[27] = Skey(9, 38, 11, 41,  ',',  MIDDLE, RIGHT, 'L', '<');
	    sarray[28] = Skey(0, 48, 2,  51,   0,   PINKY,  RIGHT,  0,  '=');
	    sarray[29] = Skey(9, 42, 11, 46,  '.',  RING,   RIGHT, 'L', '>');
	    sarray[30] = Skey(9, 46, 11, 49,  '/',  PINKY,  RIGHT, 'L', '?');
	    sarray[31] = Skey(6, 48, 8,  51,  '\'', PINKY,  RIGHT, 'L', '@');
	    // Skey[32] ascii 65
	    sarray[32] = Skey(6, 8,  8,  11,  0, PINKY,  LEFT,  'R', 'A');
	    sarray[33] = Skey(9, 26, 11, 29,  0, INDEX,  LEFT,  'R', 'B');
	    sarray[34] = Skey(9, 18, 11, 21,  0, MIDDLE, LEFT,  'R', 'C');
	    sarray[35] = Skey(6, 16, 8,  19,  0, MIDDLE, LEFT,  'R', 'D');
	    sarray[36] = Skey(3, 14, 5,  17,  0, MIDDLE, LEFT,  'R', 'E');
	    sarray[37] = Skey(6, 20, 8,  23,  0, INDEX,  LEFT,  'R', 'F');
	    sarray[38] = Skey(6, 24, 8,  27,  0, INDEX,  LEFT,  'R', 'G');
	    sarray[39] = Skey(6, 28, 8,  31,  0, INDEX,  RIGHT, 'L', 'H');
	    sarray[40] = Skey(3, 34, 5,  37,  0, MIDDLE, RIGHT, 'L', 'I');
	    sarray[41] = Skey(6, 32, 8,  35,  0, INDEX,  RIGHT, 'L', 'J');
	    sarray[42] = Skey(6, 36, 8,  39,  0, MIDDLE, RIGHT, 'L', 'K');
	    sarray[43] = Skey(6, 40, 8,  43,  0, RING,   RIGHT, 'L', 'L');
	    sarray[44] = Skey(9, 34, 11, 37,  0, INDEX,  RIGHT, 'L', 'M');
	    sarray[45] = Skey(9, 30, 11, 33,  0, INDEX,  RIGHT, 'L', 'N');
	    sarray[46] = Skey(3, 38, 5,  41,  0, RING,   RIGHT, 'L', 'O');
	    sarray[47] = Skey(3, 42, 5,  45,  0, PINKY,  RIGHT, 'L', 'P');
	    sarray[48] = Skey(3, 6,  5,  9,   0, PINKY,  LEFT,  'R', 'Q');
	    sarray[49] = Skey(3, 18, 5,  21,  0, INDEX,  LEFT,  'R', 'R');
	    sarray[50] = Skey(6, 12, 8,  15,  0, RING,   LEFT,  'R', 'S');
	    sarray[51] = Skey(3, 22, 5,  25,  0, INDEX,  LEFT,  'R', 'T');
	    sarray[52] = Skey(3, 30, 5,  33,  0, INDEX,  RIGHT, 'L', 'U');
	    sarray[53] = Skey(9, 22, 11, 25,  0, INDEX,  LEFT,  'R', 'V');
	    sarray[54] = Skey(3, 10, 5,  13,  0, RING,   LEFT,  'R', 'W');
	    sarray[55] = Skey(9, 14, 11, 17,  0, RING,   LEFT,  'R', 'X');
	    sarray[56] = Skey(3, 26, 5,  29,  0, INDEX,  RIGHT, 'L', 'Y');
	    sarray[57] = Skey(9, 10, 11, 13,  0, PINKY,  LEFT,  'R', 'Z');
	    // Skey[58] ascii 91
	    sarray[58] = Skey(3, 46, 5,  49,  0,  PINKY, RIGHT,  0,  '[');
	    sarray[59] = Skey(9, 6,  11, 9,   0,  PINKY, LEFT,   0, '\\');
	    sarray[60] = Skey(3, 50, 5,  53,  0,  PINKY, RIGHT,  0,  ']');
	    sarray[61] = Skey(0, 24, 2,  27, '6', INDEX, LEFT,  'R', '^');
	    sarray[62] = Skey(0, 44, 2,  47, '-', PINKY, RIGHT, 'L', '_');
	    sarray[63] = Skey(0, 0,  2,  3,   0,  PINKY, LEFT,   0,  '`');
	    // Skey[64] ascii 97
	    sarray[64] = Skey(6, 8,  8,  11,  'A', PINKY,  LEFT,  0, 'a');
	    sarray[65] = Skey(9, 26, 11, 29,  'B', INDEX,  LEFT,  0, 'b');
	    sarray[66] = Skey(9, 18, 11, 21,  'C', MIDDLE, LEFT,  0, 'c');
	    sarray[67] = Skey(6, 16, 8,  19,  'D', MIDDLE, LEFT,  0, 'd');
	    sarray[68] = Skey(3, 14, 5,  17,  'E', MIDDLE, LEFT,  0, 'e');
	    sarray[69] = Skey(6, 20, 8,  23,  'F', INDEX,  LEFT,  0, 'f');
	    sarray[70] = Skey(6, 24, 8,  27,  'G', INDEX,  LEFT,  0, 'g');
	    sarray[71] = Skey(6, 28, 8,  31,  'H', INDEX,  RIGHT, 0, 'h');
	    sarray[72] = Skey(3, 34, 5,  37,  'I', MIDDLE, RIGHT, 0, 'i');
	    sarray[73] = Skey(6, 32, 8,  35,  'J', INDEX,  RIGHT, 0, 'j');
	    sarray[74] = Skey(6, 36, 8,  39,  'K', MIDDLE, RIGHT, 0, 'k');
	    sarray[75] = Skey(6, 40, 8,  43,  'L', RING,   RIGHT, 0, 'l');
	    sarray[76] = Skey(9, 34, 11, 37,  'M', INDEX,  RIGHT, 0, 'm');
	    sarray[77] = Skey(9, 30, 11, 33,  'N', INDEX,  RIGHT, 0, 'n');
	    sarray[78] = Skey(3, 38, 5,  41,  'O', RING,   RIGHT, 0, 'o');
	    sarray[79] = Skey(3, 42, 5,  45,  'P', PINKY,  RIGHT, 0, 'p');
	    sarray[80] = Skey(3, 6,  5,  9,   'Q', PINKY,  LEFT,  0, 'q');
	    sarray[81] = Skey(3, 18, 5,  21,  'R', INDEX,  LEFT,  0, 'r');
	    sarray[82] = Skey(6, 12, 8,  15,  'S', RING,   LEFT,  0, 's');
	    sarray[83] = Skey(3, 22, 5,  25,  'T', INDEX,  LEFT,  0, 't');
	    sarray[84] = Skey(3, 30, 5,  33,  'U', INDEX,  RIGHT, 0, 'u');
	    sarray[85] = Skey(9, 22, 11, 25,  'V', INDEX,  LEFT,  0, 'v');
	    sarray[86] = Skey(3, 10, 5,  13,  'W', RING,   LEFT,  0, 'w');
	    sarray[87] = Skey(9, 14, 11, 17,  'X', RING,   LEFT,  0, 'x');
	    sarray[88] = Skey(3, 26, 5,  29,  'Y', INDEX,  RIGHT, 0, 'y');
	    sarray[89] = Skey(9, 10, 11, 13,  'Z', PINKY,  LEFT,  0, 'z');
	    // Skey[90] ascii 123
	    sarray[90] = Skey(3, 46, 5,  49,  '[',  PINKY, RIGHT, 'L', '{');
	    sarray[91] = Skey(9, 6,  11, 9,   '\\', PINKY, LEFT,  'R', '|');
	    sarray[92] = Skey(3, 50, 5,  53,  ']',  PINKY, RIGHT, 'L', '}');
	    sarray[93] = Skey(6, 52, 8,  55,  '#',  PINKY, RIGHT, 'L', '~');
	    // Skey[94] ascii 163
	    sarray[94] = Skey(0, 12, 2,  15,  '3',  MIDDLE, LEFT, 'R', '£');
	}
	else if (kbtype == USTYPE) {
	    kbsize = KBSIZE;
	    sarray = new Skey[kbsize];
	    // Skey[0] ascii 33
	    sarray[0] = Skey(0,  4, 2,  7,  '1',  PINKY,   LEFT,  'R', '!');
	    sarray[1] = Skey(6, 48, 8,  51, '\'', PINKY,   RIGHT, 'L', '"');
	    sarray[2] = Skey(0, 12, 2,  15, '3',  MIDDLE,  LEFT,  'R', '#');
	    sarray[3] = Skey(0, 16, 2,  19, '4',  MIDDLE,  LEFT,  'R', '$');
	    sarray[4] = Skey(0, 20, 2,  23, '5',  INDEX,   LEFT,  'R', '%');
	    sarray[5] = Skey(0, 28, 2,  31, '7',  INDEX,   RIGHT, 'L', '&');
	    sarray[6] = Skey(6, 48, 8,  51,  0,   PINKY,   RIGHT,  0, '\'');
	    sarray[7] = Skey(0, 36, 2,  39, '9',  MIDDLE,  RIGHT, 'L', '(');
	    sarray[8] = Skey(0, 40, 2,  43, '0',  RING,    RIGHT, 'L', ')');
	    sarray[9] = Skey(0, 32, 2,  35, '8',  INDEX,   RIGHT, 'L', '*');
	    sarray[10] = Skey(0, 48, 2,  51, '=',  PINKY,   RIGHT, 'L', '+');
	    sarray[11] = Skey(9, 38, 11, 41,  0,   MIDDLE,  RIGHT,  0,  ',');
	    sarray[12] = Skey(0, 44, 2,  47,  0,   PINKY,   RIGHT,  0,  '-');
	    sarray[13] = Skey(9, 42, 11, 45,  0,   RING,    RIGHT,  0,  '.');
	    sarray[14] = Skey(9, 46, 11, 49,  0,   PINKY,   RIGHT,  0,  '/');
	    // Skey[15] ascii 48
	    sarray[15] = Skey(0, 40, 2, 43,  0, RING,   RIGHT, 0, '0');
	    sarray[16] = Skey(0, 4,  2,  7,  0, PINKY,  LEFT,  0, '1');
	    sarray[17] = Skey(0, 8,  2, 11,  0, RING,   LEFT,  0, '2');
	    sarray[18] = Skey(0, 12, 2, 15,  0, MIDDLE, LEFT,  0, '3');
	    sarray[19] = Skey(0, 16, 2, 19,  0, MIDDLE, LEFT,  0, '4');
	    sarray[20] = Skey(0, 20, 2, 23,  0, INDEX,  LEFT,  0, '5');
	    sarray[21] = Skey(0, 24, 2, 27,  0, INDEX,  LEFT,  0, '6');
	    sarray[22] = Skey(0, 28, 2, 31,  0, INDEX,  RIGHT, 0, '7');
	    sarray[23] = Skey(0, 32, 2, 35,  0, INDEX,  RIGHT, 0, '8');
	    sarray[24] = Skey(0, 36, 2, 39,  0, MIDDLE, RIGHT, 0, '9');
	    // Skey[25] ascii 59
	    sarray[25] = Skey(6, 44, 8,  47,  ';',  PINKY,  RIGHT, 'L', ':');
	    sarray[26] = Skey(6, 44, 8,  47,   0,   PINKY,  RIGHT,  0,  ';');
	    sarray[27] = Skey(9, 38, 11, 41,  ',',  MIDDLE, RIGHT, 'L', '<');
	    sarray[28] = Skey(0, 48, 2,  51,   0,   PINKY,  RIGHT,  0,  '=');
	    sarray[29] = Skey(9, 42, 11, 46,  '.',  RING,   RIGHT, 'L', '>');
	    sarray[30] = Skey(9, 46, 11, 49,  '/',  PINKY,  RIGHT, 'L', '?');
	    sarray[31] = Skey(0, 8,  2,  11,  '2',  RING,   LEFT,  'R', '@');
	    // Skey[32] ascii 65
	    sarray[32] = Skey(6, 8,  8,  11,  0, PINKY,  LEFT,  'R', 'A');
	    sarray[33] = Skey(9, 26, 11, 29,  0, INDEX,  LEFT,  'R', 'B');
	    sarray[34] = Skey(9, 18, 11, 21,  0, MIDDLE, LEFT,  'R', 'C');
	    sarray[35] = Skey(6, 16, 8,  19,  0, MIDDLE, LEFT,  'R', 'D');
	    sarray[36] = Skey(3, 14, 5,  17,  0, MIDDLE, LEFT,  'R', 'E');
	    sarray[37] = Skey(6, 20, 8,  23,  0, INDEX,  LEFT,  'R', 'F');
	    sarray[38] = Skey(6, 24, 8,  27,  0, INDEX,  LEFT,  'R', 'G');
	    sarray[39] = Skey(6, 28, 8,  31,  0, INDEX,  RIGHT, 'L', 'H');
	    sarray[40] = Skey(3, 34, 5,  37,  0, MIDDLE, RIGHT, 'L', 'I');
	    sarray[41] = Skey(6, 32, 8,  35,  0, INDEX,  RIGHT, 'L', 'J');
	    sarray[42] = Skey(6, 36, 8,  39,  0, MIDDLE, RIGHT, 'L', 'K');
	    sarray[43] = Skey(6, 40, 8,  43,  0, RING,   RIGHT, 'L', 'L');
	    sarray[44] = Skey(9, 34, 11, 37,  0, INDEX,  RIGHT, 'L', 'M');
	    sarray[45] = Skey(9, 30, 11, 33,  0, INDEX,  RIGHT, 'L', 'N');
	    sarray[46] = Skey(3, 38, 5,  41,  0, RING,   RIGHT, 'L', 'O');
	    sarray[47] = Skey(3, 42, 5,  45,  0, PINKY,  RIGHT, 'L', 'P');
	    sarray[48] = Skey(3, 6,  5,  9,   0, PINKY,  LEFT,  'R', 'Q');
	    sarray[49] = Skey(3, 18, 5,  21,  0, INDEX,  LEFT,  'R', 'R');
	    sarray[50] = Skey(6, 12, 8,  15,  0, RING,   LEFT,  'R', 'S');
	    sarray[51] = Skey(3, 22, 5,  25,  0, INDEX,  LEFT,  'R', 'T');
	    sarray[52] = Skey(3, 30, 5,  33,  0, INDEX,  RIGHT, 'L', 'U');
	    sarray[53] = Skey(9, 22, 11, 25,  0, INDEX,  LEFT,  'R', 'V');
	    sarray[54] = Skey(3, 10, 5,  13,  0, RING,   LEFT,  'R', 'W');
	    sarray[55] = Skey(9, 14, 11, 17,  0, RING,   LEFT,  'R', 'X');
	    sarray[56] = Skey(3, 26, 5,  29,  0, INDEX,  RIGHT, 'L', 'Y');
	    sarray[57] = Skey(9, 10, 11, 13,  0, PINKY,  LEFT,  'R', 'Z');
	    // Skey[58] ascii 91
	    sarray[58] = Skey(3, 46, 5, 49,  0,  PINKY, RIGHT,  0,  '[');
	    sarray[59] = Skey(6, 52, 8, 55, '|', PINKY, RIGHT,  0, '\\');
	    sarray[60] = Skey(3, 50, 5, 53,  0,  PINKY, RIGHT,  0,  ']');
	    sarray[61] = Skey(0, 24, 2, 27, '6', INDEX, LEFT,  'R', '^');
	    sarray[62] = Skey(0, 44, 2, 47, '-', PINKY, RIGHT, 'L', '_');
	    sarray[63] = Skey(0, 0,  2, 3,   0,  PINKY, LEFT,   0,  '`');
	    // Skey[64] ascii 97
	    sarray[64] = Skey(6, 8,  8,  11,  'A', PINKY,  LEFT,  0, 'a');
	    sarray[65] = Skey(9, 26, 11, 29,  'B', INDEX,  LEFT,  0, 'b');
	    sarray[66] = Skey(9, 18, 11, 21,  'C', MIDDLE, LEFT,  0, 'c');
	    sarray[67] = Skey(6, 16, 8,  19,  'D', MIDDLE, LEFT,  0, 'd');
	    sarray[68] = Skey(3, 14, 5,  17,  'E', MIDDLE, LEFT,  0, 'e');
	    sarray[69] = Skey(6, 20, 8,  23,  'F', INDEX,  LEFT,  0, 'f');
	    sarray[70] = Skey(6, 24, 8,  27,  'G', INDEX,  LEFT,  0, 'g');
	    sarray[71] = Skey(6, 28, 8,  31,  'H', INDEX,  RIGHT, 0, 'h');
	    sarray[72] = Skey(3, 34, 5,  37,  'I', MIDDLE, RIGHT, 0, 'i');
	    sarray[73] = Skey(6, 32, 8,  35,  'J', INDEX,  RIGHT, 0, 'j');
	    sarray[74] = Skey(6, 36, 8,  39,  'K', MIDDLE, RIGHT, 0, 'k');
	    sarray[75] = Skey(6, 40, 8,  43,  'L', RING,   RIGHT, 0, 'l');
	    sarray[76] = Skey(9, 34, 11, 37,  'M', INDEX,  RIGHT, 0, 'm');
	    sarray[77] = Skey(9, 30, 11, 33,  'N', INDEX,  RIGHT, 0, 'n');
	    sarray[78] = Skey(3, 38, 5,  41,  'O', RING,   RIGHT, 0, 'o');
	    sarray[79] = Skey(3, 42, 5,  45,  'P', PINKY,  RIGHT, 0, 'p');
	    sarray[80] = Skey(3, 6,  5,  9,   'Q', PINKY,  LEFT,  0, 'q');
	    sarray[81] = Skey(3, 18, 5,  21,  'R', INDEX,  LEFT,  0, 'r');
	    sarray[82] = Skey(6, 12, 8,  15,  'S', RING,   LEFT,  0, 's');
	    sarray[83] = Skey(3, 22, 5,  25,  'T', INDEX,  LEFT,  0, 't');
	    sarray[84] = Skey(3, 30, 5,  33,  'U', INDEX,  RIGHT, 0, 'u');
	    sarray[85] = Skey(9, 22, 11, 25,  'V', INDEX,  LEFT,  0, 'v');
	    sarray[86] = Skey(3, 10, 5,  13,  'W', RING,   LEFT,  0, 'w');
	    sarray[87] = Skey(9, 14, 11, 17,  'X', RING,   LEFT,  0, 'x');
	    sarray[88] = Skey(3, 26, 5,  29,  'Y', INDEX,  RIGHT, 0, 'y');
	    sarray[89] = Skey(9, 10, 11, 13,  'Z', PINKY,  LEFT,  0, 'z');
	    // Skey[90] ascii 123
	    sarray[90] = Skey(3, 45, 5, 48,  '[',  PINKY, RIGHT, 'L', '{');
	    sarray[91] = Skey(6, 52, 8, 55,   0,   PINKY, RIGHT, 'R', '|');
	    sarray[92] = Skey(3, 49, 5, 52,  ']',  PINKY, RIGHT, 'L', '}');
	    sarray[93] = Skey(0, 0,  2, 3,   '`',  PINKY, LEFT,  'R', '~');
	    // Skey[94] ascii 61
	    sarray[94] = Skey(9, 6, 11, 9,  0,  PINKY, LEFT,  0,  '<');
	    // Skey[95] ascii 63
	    sarray[95] = Skey(9, 6, 11, 9, '<', PINKY, LEFT, 'R', '>');
	}
	else {
	    throw Error("defaults returned invalid kbtype: getkeys.cpp");
	}
	// construct an array of Function Keys
	farray = new Fkey[10];
	farray[0] = Fkey(0,  52, 2,  63, "BACKSPACE");    // backspace
	farray[1] = Fkey(9,  50, 11, 63, "SHIFT");        // right shift
	farray[2] = Fkey(12, 59, 14, 63, "CTL");          // right control
	farray[3] = Fkey(12, 54, 14, 58, "ALT");          // right alt
	farray[4] = Fkey(12, 10, 14, 53, "");             // space bar
	farray[5] = Fkey(12, 5,  14, 9,  "ALT");          // left alt
	farray[6] = Fkey(12, 0,  14, 4 , "CTL");          // left control
	farray[7] = Fkey(9,  0,  11, 5 , "SHFT");         // left shift
	farray[8] = Fkey(6,  0,  8,  7 , "CAPS");         // caps lock
	farray[9] = Fkey(3,  0,  5,  5 , "TAB");          // tab
	
	// construct the RETURN Key
	rarray = new Rkey(3,63,8,63,8,56,5,56,5,54,3,54, "RETURN");
    }
    catch (bad_alloc bad) {
	string badalloc(bad.what());
	throw Error(badalloc + " during construction: getkeys.cpp");
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: getkeys.cpp");
    }
}
