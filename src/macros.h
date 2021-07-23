/* -- macros.h -- */

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
 macros.h defines the macros and constatnts for the Makin' Bakon Typing Tutor
 ---------------------------------------------------------------------------*/

#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <curses.h>
#include <string>

/*-------------------------------- macros ----------------------------------*/

// get menu array index from lesson number
#define GETINDEX(x) (((x) + 10) / 2)
// get lesson number from menu array index
#define GETLESSON(x) (((x) * 2) - 10) 

/*------------------------------- constants --------------------------------*/

const int PINKY = 1;              // little finger
const int RING = 2;               // ring finger
const int MIDDLE = 3;             // middle finger
const int INDEX = 4;              // index finger
const int LEFT = 0;               // left hand
const int RIGHT = 1;              // right hand

const int KBSIZE = 96;            // sizeof Key array
const int NUMROWS = 5;            // number of rows on a keyboard

const int RSPEED = 4;             // number of speed lessons to search
const int TSPEED = 5;             // total number of speed lessons

const int FMENUSIZE = 10;         // sizeof main menu
const int SMENUSIZE = 20;         // sizeof sub menu
const int MENUSIZE = 30;          // main menu + sub menu
const int CHANGEDEF = 1;          // re-define default lesson
const int LESSONONE = 10;         // first lesson
const int TESTLESSON = 50;        // test lesson
const int LASTLESSON = 49;        // last default lesson
const int NOT_SET = 127;          // variable not set

const short COLOR_TRANS = -1;     // transparent theme

const std::string EMPTY("");                // an empty string
const std::string FORE("fore");             // forground name
const std::string BACK("back");             // background name
const std::string BOLD("bold");             // bold name
const std::string KBRD("keyboard");         // keyboard name
const std::string DEFCOL("trans");          // default color setting
const std::string DEFKBRD("uk");            // default keyboard setting
const std::string USTYPE("us");             // american keyboard type
const std::string UKTYPE("uk");             // british keyboard type

const char D_PIGSTY[] = "/.pigsty";         // /home/<user>/.pigsty/
const char D_SCORES[] = "/scores";          // /home/<user>/.pigsty/scores
const char D_EPIGRAM[] = "/epigram";        // /home/<user>/.pigsty/epigram
const char D_BAKONRC[] = "/.bakonrc";       // /home/<user>/.bakonrc
const char D_BAKONBAK[] = "/BAKONRC";       // <DOC>/BAKONRC

const char PIGSTY[] = ".pigsty";            // /home/<user>/.pigsty/
const char SCORES[] = "scores";             // /home/<user>/.pigsty/scores
const char EPIGRAM[] = "epigram";           // /home/<user>/.pigsty/epigram
const char BAKONRC[] = ".bakonrc";          // /home/<user>/.bakonrc

const char D_WELCOME[] = "/welcome.dat";    // <DATA>/welcome.dat
const char D_HELP[] = "/help.dat";          // <DATA>/help.dat
const char D_PIGTIPS[] = "/pigtips.dat";    // <DATA>/pigtips.dat
const char D_INTROS[] = "/intros.dat";      // <DATA>/intros.dat
const char D_LESSONS[] = "/lessons.dat";    // <DATA>/lessons.dat
const char D_SPEED[] = "/speed.dat";        // <DATA>/speed.dat

// win and lose closing screen strings
const char WIN[] = "Yipeeeeeeeeeeee. I'm a free pig!";
const char LOSE[] = "Mmmmmmmmmmmmmmmmmmmmmmmm. Bacon!";

// command-line option groups
const std::string OPTIONS[]={ "-h", "-kb", "-fg", "-bg", "-bd" };
// size of OPTIONS
const int OSIZE = sizeof(OPTIONS)/sizeof(*OPTIONS);

// curses color-map keys 
const std::string CURSKEYS[]={
    "black", "magenta", "green", "yellow",
    "red", "blue", "cyan", "white", "trans"
};
// size of CURSESKEYS
const int CSIZE = sizeof(CURSKEYS)/sizeof(*CURSKEYS);

// curses color-map values
const short CURSVALS[]={
    COLOR_BLACK, COLOR_MAGENTA, COLOR_GREEN, COLOR_YELLOW,
    COLOR_RED, COLOR_BLUE, COLOR_CYAN, COLOR_WHITE, COLOR_TRANS
};
// size of CURSESVALS
const int DSIZE = sizeof(CURSVALS)/sizeof(*CURSVALS);

// list of ALL default values
const std::string DEFKEYS[]={ FORE, BACK, BOLD, KBRD };
// size of DEFKEYS
const int KSIZE = sizeof(DEFKEYS)/sizeof(*DEFKEYS);

#endif
