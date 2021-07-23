/* -- scores.h -- */

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
 Scores class sets and gets the default lesson num and stats. Scores string
 looks like this:- "L:10 S:33" This translates to: last completed Lesson = 10,
 and last session Stats = 33
 ---------------------------------------------------------------------------*/

#ifndef SCORES_H
#define SCORES_H

#include "error.h"
#include "pigsty.h"
#include "destroyer.h"
#include <string>

using namespace std;

class Scores {
  private:
    Scores(const Scores& src);             // never defined
    void operator=(const Scores& src);     // never defined
  private:
    string Scorestr;             // holds all the scores
    bool Gotya;                       // true if scores held in scores string
    Pigsty* sty;                      // get ~/.pigsty directory address
    char* Home;                       // hold ~/.pigsty address
  private:
    double Snum;                      // return wpm rate
    int Lnum;                         // return lesson number
  private:
    static Scores* sInstance;              // ptr to singleton instance
    static Destroyer<Scores> destroyer;    // instantiate singleton destroyer
  protected:
    Scores() throw(Error);                 // default constructor
    virtual ~Scores();                     // default destructor
    friend class Destroyer<Scores>;        // destroy singleton
  public:
    static Scores* Instance() throw(Error);     // create singleton
  public:
    int getLesson();                  // return default lesson number
    void setLesson(int lesson);            // set the default lesson number
    double getStats();                // return default wpm rate
    void setStats(double i);          // set the default wpm rate
    string getAttribute(string attributeKey);   // return attribute or "-1" if non-existing
    void setAttribute(string attributeKey, string);   // return attribute or "-1" if non-existing
  private:
    void readScores() throw(Error);        // read scores file into Scorestr
    void writeScores() throw(Error);       // write Scorestr to scores file
};

inline Scores::~Scores() { delete[] Home; Home = 0; }

#endif
