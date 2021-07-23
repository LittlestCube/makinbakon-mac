/* -- stats.h -- */

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
 Class Stats is a singleton that calculates the total number of correctly
 typed words per minute.
 ---------------------------------------------------------------------------*/

#ifndef STATS_H
#define STATS_H

#include "destroyer.h"
#include "scores.h"
#include <string>
#include <ctime>

class Stats {
  private:
    Stats(const Stats& src);               // not defined
    void operator=(const Stats& src);      // not defined
  private:
    static Stats* sInstance;               // ptr to singleton
    static Destroyer<Stats> destroyer;     // detroy singletons
  private:
    Scores* score;           // get scores (wpm and speed)
    std::string speed;       // formatted string for target wpm
    std::string stats;       // return stats info string
  private:
    double c_misses;         // copy n_misses
    double n_misses;         // incorrectly typed characters
    double c_time;           // copy n_time
    double n_time;           // difftime(tnow, tstart)
    double t_time;           // cumulative time
    double c_wpm;            // copy n_wpm
    double n_wpm;            // wpm for each character typed
    double n_error;          // percent lesson errors
    double c_char;           // copy n_char
    double n_char;           // number of characters typed
    double w_count;          // count words (five n_char's)
    double t_wpm;            // the target speed
    double s_wpm;            // last saved speed
    double bonus;            // bonus points for beating target speed
    bool in_word;            // allow only one miss for every word
    time_t t_start;          // time lesson started
    time_t t_now;            // present time
  public:
    void start();            // start clock and word count
    void error();            // counts errors (one only for every five chars)
    void reset();            // set all values to zero
    void setScore(int);      // increments n_char and sets n_wpm
    void setStats();         // make a copy of the stats at end lesson
    void setLast();          // sets stats to last stats
  public:
    static Stats* Instance() throw(Error);      // instantiate singleton
  public:
    double getCwpm();                 // returns last n_wpm
    double getBwpm();                 // returns last n_wpm plus bonus
    std::string getScore();           // returns lesson n_wpm and acc'y
    std::string getTarget();          // returns formatted string speed
    double getTwpm();                 // returns the target wpm
    void setTarget();                 // set target wpm and speed
  protected:
    Stats() throw(Error);             // default constructor
    virtual ~Stats() { }              // default destructor
    friend class Destroyer<Stats>;    // destroy singletons
};

// return the target speed
inline double Stats::getTwpm() { return t_wpm; }

// return the target speed string 
inline std::string Stats::getTarget() { return speed; }

// set clock
inline void Stats::start() {
    t_time = n_time;
    time(&t_start);
}

// reset everything to zero
inline void Stats::reset() { 
    n_time = 0;
    n_char = 0; 
    n_misses = 0;
    n_wpm = 0;
    n_error = 100;
    bonus = 0;
    c_wpm = 0;
}

// return the session wpm score
inline double Stats::getCwpm() { return c_wpm; }

// return the session wpm score plus any bonuses
inline double Stats::getBwpm() { return (c_wpm + bonus); }

// set stats to those of last completed lesson
inline void Stats::setLast() { 
    n_char = c_char;
    n_time = c_time;
    n_misses = c_misses;
    setScore(0);
}

#endif
