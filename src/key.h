/* -- key.h -- */

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

#ifndef KEY_H
#define KEY_H

#include <string>

/*----------------------------------------------------------------------------
 The Key base class represents all the keys on the Keyboard. It holds only the
 key coordinates.
 ---------------------------------------------------------------------------*/

class Key {
  public:
    int r1;                // top left row coordinate
    int c1;                // top left column coordinate
    int r2;                // bottom right row coordinate
    int c2;                // bottom right column coordinate
  public:
    Key() { }
    Key& operator=(const Key& src);
    Key(const Key& src);
    Key(int, int, int, int);
    virtual ~Key() { }
};

// construct key coordinates
inline Key::Key(int tr, int tc, int br, int bc)
    : r1(tr), c1(tc), r2(br), c2(bc) { }

// construct operator= for Key class
inline Key& Key::operator=(const Key& src) {
    if (&src != this) {
	r1 = src.r1;
	c1 = src.c1;
	r2 = src.r2;
	c2 = src.c2;
    }
    return *this;
}

// copy construct Key class
inline Key::Key(const Key& src)
    : r1(src.r1), c1(src.c1), r2(src.r2), c2(src.c2) { }

/*----------------------------------------------------------------------------
 The Skey class represents the standard keys on the Keyboard. It holds key
 coordinates, identification (major and minor key) whether the key is shifted,
 which finger of which hand presses the key and the key name.
 ---------------------------------------------------------------------------*/

class Skey : public Key {
  public:
    int mkey;              // 'A' is major key to 'a'
    int finger;            // finger used to press the key
    int hand;              // side of the keyboard the key is on
    int shift;             // shifted key (left shift, right shift, ! shifted)
    unsigned char askii;   // the value of the key
  public:
    Skey() { }
    Skey& operator=(const Skey& src);
    Skey(const Skey& src);
    Skey(int, int, int, int, int, int, int, int, unsigned char);
    ~Skey() { }
};

// construct the standard keys
inline Skey::Skey(int tr, int tc, int br, int bc, int m, int f, int h, int s, 
		  unsigned char a)
    : Key(tr, tc, br, bc), mkey(m), finger(f), hand(h), shift(s), askii(a) { }

// construct operator= for Skey class
inline Skey& Skey::operator=(const Skey& src) {
    if (&src != this) {
	Key::operator=(src);
	mkey = src.mkey;
	finger = src.finger;
	hand = src.hand;
	shift = src.shift;
	askii = src.askii;
    }
    return *this;
}

// copy construct the Skey class
inline Skey::Skey(const Skey& src)
    : Key(src), mkey(src.mkey), finger(src.finger), hand(src.hand),
      shift(src.shift), askii(src.askii) { }

/*----------------------------------------------------------------------------
 The Fkey class represents the functional keys on the Keyboard, like shift
 and tab. It holds corner coordinates and the key name.
 ---------------------------------------------------------------------------*/

class Fkey : public Key {
  public:
    std::string name;           // functional key name
  public:
    Fkey() { }
    Fkey& operator=(const Fkey& src);
    Fkey(const Fkey& src);
    Fkey(int, int, int, int, std::string);
    virtual ~Fkey() { }
};

// construct the functional keys
inline Fkey::Fkey(int tr, int tc, int br, int bc, std::string n)
    : Key(tr, tc, br, bc), name(n) { }

// construct operator= for Fkey class
inline Fkey& Fkey::operator=(const Fkey& src) {
    if (&src != this) {
	Key::operator=(src);
	name = src.name;
    }
    return *this;
}

// copy construct the Fkey class
inline Fkey::Fkey(const Fkey& src)
    : Key(src), name(src.name) { }

/*----------------------------------------------------------------------------
 The Rkey class represents the RETURN key, It holds coordinates for each of
 the six corners and the key name.
 ---------------------------------------------------------------------------*/

class Rkey : public Fkey {
  public:
    int trr;               // top right row coordinate
    int trc;               // top right column coordinate
    int blr;               // bottom left row coordinate
    int blc;               // bottom left column coordinate
    int mrr;               // middle right row coordinate
    int mrc;               // middled right column coordinate
    int mlr;               // middle left row coordinate
    int mlc;               // middle left column coordinate
  public:
    Rkey() { }
    Rkey& operator=(const Rkey& src);
    Rkey(const Rkey& src);
    Rkey(int, int, int, int, int, int, int, int, int, int, int, int, std::string);
    ~Rkey() { }
};

// construct the RETURN key
inline Rkey::Rkey(int ar, int ac, int br, int bc,  int cr, int cc, int dr,
		  int dc, int er, int ec, int fr, int fc, std::string n) 
    : Fkey(ar, ac, cr, cc, n), trr(br), trc(bc), blr(dr), blc(dc), mrr(er),
      mrc(ec), mlr(fr), mlc(fc) { }

// construct operator= for Rkey class
inline Rkey& Rkey::operator=(const Rkey& src) {
    if (&src != this) {
	Fkey::operator=(src);
	trr = src.trr;
	trc = src.trc;
	blr = src.blr;
	blc = src.blc;
	mrr = src.mrr;
	mrc = src.mrc;
	mlr = src.mlr;
	mlc = src.mlc;
    }
    return *this;
}

// copy construct the Rkey class
inline Rkey::Rkey(const Rkey& src)
    : Fkey(src), trr(src.trr), trc(src.trc),  blr(src.blr), blc(src.blc),
      mrr(src.mrr), mrc(src.mrc), mlr(src.mlr), mlc(src.mlc) { }

#endif
