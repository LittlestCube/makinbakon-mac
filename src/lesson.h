/* -- lesson.h -- */

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
 Lesson class. All the lessons are defined here.
 ---------------------------------------------------------------------------*/

#ifndef LESSON_H
#define LESSON_H

#include "display.h"
#include "scores.h"
#include "stats.h"
#include "gettext.h"
#include "error.h"
#include <string>

class Lesson {
  private:
    Lesson(const Lesson& src);            // never defined
    void operator=(const Lesson& src);    // never defined
  private:
    Display* out;              // output to display
    Scores* score;             // get/set scores (wpm and lesson number)
    Stats* stats;              // update the stats engine
    Gettext* gettext;          // get the lesson strings and intro's
  private:
    std::string intro;         // holds the lesson instructions
    std::string estring;       // return value of lessonError()
    std::string epigram;       // holds fortune epigram
    std::string title;         // holds lesson title
    std::string nlesson;       // holds the default lesson text
    std::string message;       // holds test lesson messages
    std::string prefs;         // holds the keyboard type
  private:
    bool error;                // error message on/off
    int charCount;             // count characters typed
  private:
    static bool positFlag;     // finger Position window on/off
    static bool statsFlag;     // statistics window on/off
    static bool startFlag;     // timer on/off
  public:
    Lesson(Display* lout) throw(Error);
    ~Lesson() { }
  public:
    // Lessons of Fortune epigrams, int menu index and string menu
    int lessonFortune(int i, const std::string menu[]);
    // Which lesson to execute, int menu index and string menu
    int lessonNumber(int i, const std::string menu[]);
    // Get default lesson, int lesson number, int menu index and string menu
    int lessonDefault(int lnum, int i, const std::string menu[]);
    // Speed Test a long fortune epigram, int menu index and string menu
    int lessonSpeed(int i, const std::string menu[]);
    // Test lesson for first use of Makin' Bakon, int menu index
    int lessonTest(int i);
    // Check user input against lesson output
    int lessonCheck(const std::string abc);
    // Define the lesson messages, int menu index, bool true if completed
    std::string lessonMessage(int i, bool fin);
  private:
    // Pass typing error messages to Display, true if error
    void lessonError(bool error);
    // Increment default lesson, int menu index and int lesson number
    void increment(int& index, int& lesson);
};

inline Lesson::Lesson(Display* lout) throw (Error) 
    : out(lout), error(false), charCount(0)
{
    try {
	score = Scores::Instance();
	stats = Stats::Instance();
	gettext = Gettext::Instance();
    }
    catch (Error err) { 
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: lesson.h");
    }
}

#endif
