/* -- stats.cpp -- */

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

#include "stats.h"
#include "macros.h"
#include "error.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <sstream>
#include <iomanip>

using namespace std;

// initialise sInstance statics
Stats* Stats::sInstance = 0;
Destroyer<Stats> Stats::destroyer;

// create stats instance
Stats* Stats::Instance() throw (Error) {
    if (sInstance == 0) {
	if ((sInstance = new (std::nothrow) Stats) == 0) {
	    throw Error("can't create stats instance: stats.cpp");
	}
	destroyer.setDoomed(sInstance);
    }
    return sInstance;
}

// create a scores instance and set target string
Stats::Stats() throw (Error)
    : c_misses(0), n_misses(0), c_time(0), n_time(0), t_time(0), c_wpm(0),
      n_wpm(0), n_error(100), c_char(0), n_char(0), w_count(0), t_wpm(0),
      s_wpm(0), bonus(0), in_word(false), t_start(0), t_now(0) 
{ 
    try {
	score = Scores::Instance();
	setTarget();
    }
    catch (Error err) { 
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: stats.cpp");
    }
}

// set the target string
void Stats::setTarget() {
	return;
    try {
	if ((t_wpm = score->getStats()) >= 1) {
	    t_wpm += 1;
	    stringstream rate;
	    rate << "  [ Target Score: ";
	    rate << setprecision(2) << setiosflags(ios::fixed) << t_wpm;
	    rate << " ]";
	    speed = rate.str();
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception in setTarget: stats.cpp");
    }
}

// record the stats at end of each lesson and set bonus, if session
// speed (sans bonus) is greater than target speed then update the
// scores file, but only if it is greater than the last entry made
void Stats::setStats() {
    c_char = n_char;
    c_time = n_time;
    c_misses = n_misses;
    c_wpm = n_wpm;
    
    if (c_wpm > t_wpm) {
	if (t_wpm > 0) {
	    bonus += 1;
	}
	if (c_wpm > s_wpm) {
	    s_wpm = c_wpm;
	    score->setStats(c_wpm);
	}
    }
}

// deduct one error point for one typo in every
// five characters, otherwise ignore the typo's
void Stats::error() { 
    if (!in_word) {
	w_count = n_char + 4;
	n_misses += 1;
	in_word = true;
    }
    else if (in_word) {
	if (n_char == w_count)
	    in_word = false;
    }
}

// calculate the lesson wpm and error
void Stats::setScore(int i) {
    
    // increment n_char if i > 0 
    if (i > 0) {
	n_char += 1;
	// update the error engine
	if (in_word) {
	    if (n_char == w_count) {
		in_word = false;
	    }
	}
	// get the current elapsed time
	time(&t_now);
	n_time = difftime(t_now, t_start) + t_time;
    }
	
    // Calculate the error adjusted wpm and %error. Make n_wpm and n_error
    // zero if misses/errors are so great they push them BELOW zero. Also,
    // results are not accurate until we have typed at least twelve chars.

    if (n_char > 12) {
	n_wpm = (n_char/5 - n_misses) / (n_time/60);
	if (n_wpm < 0.1) {
	    n_wpm = 0;
	}
	n_error = 100 - (n_misses/(n_char/100));
	if (n_error < 0.1) {
	    n_error = 0;
	}
    }
    else {
	n_wpm = 0;
	n_error = 100;
    }
}

// set the score string
string Stats::getScore() {

    stringstream speed;

    speed << "Speed ";
    speed << setprecision(2) << setiosflags(ios::fixed) << (n_wpm);
    speed << "wpm" << " :: " << "Accuracy ";
    speed << setprecision(2) << setiosflags(ios::fixed) << n_error;
    speed << "%" << " :: " << "Score ";
    speed << setprecision(2) << setiosflags(ios::fixed) << (n_wpm  + bonus);
    
    stats.assign(speed.str());

    return stats;
}
