/* -- gettext.h -- */

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
 All lessons, lesson intro's and help texts are held in text files which are 
 waiting to have their dubious contents converted into strings by Gettext...
 ---------------------------------------------------------------------------*/

#ifndef GETTEXT_H
#define GETTEXT_H

#include "error.h"
#include "destroyer.h"
#include "scores.h"
#include <string>

class Gettext {
  private:
    Gettext(const Gettext& src);            // never defined
    void operator=(const Gettext& src);     // never defined
  private:
    Scores* gscore;     // get lesson number
    bool gotnum;        // true if we have speed test random number
    int snum;           // the speed test ramdom number
    std::string str;    // string to return
  private:
    static Gettext* gInstance;               // ptr to singleton instance
    static Destroyer<Gettext> destroyer;     // destroy singleton instance
    void formatOne(std::string& str);        // make all whitespace == ' '
    void formatTwo(std::string& str);        // remove surplus ' ' characters
  private:
    // get part of file
    void readPart(std::string, int, std::string&) throw(Error);
    // gets all of file
    void readWhole(std::string, std::string&) throw(Error);
  protected:
    Gettext() throw(Error);                  // default constructor
    virtual ~Gettext() { }                   // default destructor
    friend class Destroyer<Gettext>;         // destroy singleton
  public:
    static Gettext* Instance() throw(Error);     // create singleton instance
  public:
    std::string& getWelcome();         // return welcome screen text
    std::string& getHelp();            // return help text
    std::string& getTips();            // return Pig's Tips
    std::string& getIntro(int i);      // return lesson 'i' intro text 
    std::string& getLesson(int i);     // return lesson 'i' text
    std::string& getSpeed();           // return speed lesson text
};

inline Gettext::Gettext() throw (Error) : gotnum(false), snum(0) { 
    try {
	gscore = Scores::Instance(); 
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: gettext.h");
    }
}

#endif
