/* -- display.h -- */

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
 The display class is pure virtual. With this set-up, it is possible to add 
 different display types. At present there is only one display type, nCurses. 
 Character input for the program is defined by the display type, primarily to
 make use of the type's input routines. Output is sent from the lessons and
 control routines and translated here into a screen image.

 Note: the key macro data members, defined in the base class, are initialized
       according to display type (see the derived class constructors).
 ---------------------------------------------------------------------------*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "error.h"
#include "gettext.h"
#include <string>
#include <vector>
#include <curses.h>
#include <menu.h>
#include <panel.h>

class Display {
  private:
    Display(const Display& src) { }            // never defined
    void operator=(const Display& src) { }     // never defined
  public:
    int NEXTKEY;              // key macro: turn nextkey window on/off
    int STATS;                // key macro: turn stats window on/off
    int INSTRUCT;             // key macro: turn lesson instructions on/off
    int ENDLESSON;            // key macro: escape lessons/screens
    int REVS;                 // key macro: go back a screen
    int FRWD;                 // key macro: go forward a screen
    int QUIT;                 // key macro: quit program
  public:
    int Input;                // return value of scrInput
    Gettext* gettext;         // get lesson intro's and strings
  public:
    Display() throw(Error);   // default constructor
    virtual ~Display() { }    // virtual destructor
  public:
    virtual int scrMenu(std::string[]) = 0;
    virtual int &scrInput(void) = 0;
    virtual int scrInstruct(std::string) = 0;
    virtual int scrHelp(std::string) = 0;
    virtual void scrWelcome(std::string) = 0;
    virtual void scrTitle(std::string) = 0;
    virtual void scrKey(std::string, unsigned char, int) = 0;
    virtual void scrKey() = 0;
    virtual void scrStats(std::string) = 0;
    virtual void scrStats() = 0;
    virtual void scrMessage(int, bool, std::string) = 0;
    virtual void scrLesson(std::string) = 0;
    virtual void scrUI(void) = 0;
    virtual void scrError(std::string) = 0;
    virtual void scrFinish(std::vector<std::string>, char*) = 0;
    virtual void scrClose(void) = 0;
    virtual void sizeEvent(void) = 0;
  protected:
    void scrFormat(int, std::string&);
};

inline Display::Display() throw(Error) {
    try {
	gettext = Gettext::Instance();
    }
    catch (Error err) { 
	throw err;
    }
    catch (...) {
	throw Error("unexpected exception during construction: display.h");
    }
}

/*----------------------------------------------------------------------------
 The Curses class controls the nCurses screen. 
 ---------------------------------------------------------------------------*/ 

class Kurses : public Display {
  private:
    Kurses(const Kurses& src);             // never defined
    void operator=(const Kurses& src);     // never defined
  private: 
    bool Inlesson;                 // true if cursor on and in lesson
    bool Nextkey;                  // true if nextkey window on
    bool Userin;                   // true if userin wait message on
    bool Lintro;                   // true if lesson instr's panel is on
    bool Setless;                  // true if setLesson menu created
    bool Scrmenu;                  // true if scrMenu menu created
    bool Erroron;                  // true if error message in wpm window
    bool Opened;                   // true if curses fully operational
    bool Built;                    // true if virtual keyboard built
  private:
    short fore;                    // foreground color setting
    short back;                    // background color setting
    short bold;                    // bold color setting
  private:
    int Half;                      // half width of message/titles windows
    unsigned char Lastchar;        // last used keyboard char (scrKey()) 
    int Lastshift;                 // last used shift value (scrKey())
    int Spos[4];                   // last used key coordinates (scrKey())
    int Cpos[4];                   // last used shift coordinates (scrKey())
    const int* Keypos;             // ptr to Key coordinates (scrKey())
    int Lchoice;                   // return value of setLesson()
  private:
    std::string Lcopy;                  // makes copy of scrLesson string
    std::string Scopy;                  // makes copy of scrError string
  private:
    unsigned int Menuchoice;       // menu item to return (scrMenu)
    unsigned int Columns;          // number of columns in arrayout subwin
    unsigned int Middle;           // half way house (Columns/2)
    unsigned int Height;           // Height == LINES
    unsigned int Width;            // Width == COLS
    unsigned int Z;                // to count down from Columns to zero
    unsigned int J;                // number of array elements to print
  private:
    WINDOW* mainwin;               // derive all windows from this window
    WINDOW* stitles;               // put titles onto the screen
    WINDOW* ltitles;               // put lesson titles onto the screen
    WINDOW* vboard;                // displays the virtual keyboard
    WINDOW* nextkey;               // window for next key press
    WINDOW* arrayout;              // window for lesson output
    WINDOW* wpm;                   // window for displaying stats
    WINDOW* smessage;              // window for quit messages
    WINDOW* lmessage;              // window for quit lesson messages
    WINDOW* introwin;              // panel window for lesson instructions
    WINDOW* fmenuwin;              // window for the main menu
    WINDOW* smenuwin;              // window for the secondary menu
    WINDOW* boxout;                // window for lesson arrayout box
    WINDOW* boxwpm;                // window for stats box
    WINDOW* finstruct;             // window for firstmenu lesson instros
    WINDOW* sinstruct;             // window for secondmenu lesson instros
    WINDOW* textwin;               // displays text for help, prefs
    PANEL* intropanel;             // panel holds lesson instr's windows
    ITEM** firstitems;             // ptr to array of (first)menu items
    ITEM** seconditems;            // ptr to array of (second)menu items
    MENU* firstmenu;               // the menu holds the (first)menu items
    MENU* secondmenu;              // the menu holds the (second)menu items
    ITEM* current;                 // the current item displayed
  public:
    // Initialise the curses screen and define color theme and sub windows.
    Kurses() throw(Error); 
    // Destroy the curses screen
    ~Kurses();
  public:
    // Put the Makin' Bakon help text onto the screen. Function takes a string
    // of any size, which is split at the nearest '\n' to LINES-n characters.
    int scrHelp(std::string str);
    // returns next keyboard input character
    int &scrInput(void);
    // Add/remove the lesson introductions to/from the lesson screen.
    int scrInstruct(std::string str) throw(Error);
    // Puts the main menu onto the screen where str[] is the menu.
    int scrMenu(std::string str[]) throw(Error);
    // Put welcome text onto the screen
    void scrWelcome(std::string str);
    // Put lesson and function titles on the screen.
    void scrTitle(std::string str);
    // Put next key position text onto screen and highlight said key position
    // on the virtual keyboard.
    void scrKey(std::string finger, unsigned char ch, int shift);
    // Remove the virtual keyboard from the screen.
    void scrKey();
    // Put lesson statistics onto the screen.
    void scrStats(std::string str);
    // Remove lesson statistics window.
    void scrStats();
    // Put lesson array onto the screen.
    void scrLesson(std::string str);
    // For correct key presses, increment cursor in arrayout from left of
    // window to midpoint, then scroll text off to the left keeping cursor at
    // midpoint. When end of text is reached remaining text to left of cursor
    // is scrolled off to left with cursor shadowing final space character.
    void scrUI(void);
    // Puts an error message onto stats window when the user makes a mistake
    // and remove message when mistake is corrected.
    void scrError(std::string str);
    // Put end of lesson message onto the screen. Parameter i: lesson
    // number, fin: lesson finished (true/false), str: stats message.
    void scrMessage(int i, bool fin, std::string str);
    // Close down the curses screen.
    void scrClose(void);
    // Print the target, final wpm, final score and closing message.
    void scrFinish(std::vector<std::string> str, char* message);
    // handle SIGWINCH screen resizing events
    void sizeEvent(void) throw(Error);
  private:
    // Change the default lesson using menu options.
    int setLesson(std::string str[]) throw(Error);
    // put a box around each virtual key
    void boxKeys(int r1, int c1, int r2, int c2, chtype ATTRIB);
};

inline int &Kurses::scrInput() { Input = getch(); return Input; }

inline Kurses::~Kurses() { scrClose(); }

#endif
