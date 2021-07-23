/* -- pigsty.h -- */

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
 Checks if ~/.pigsty/scores, ~/.pigsty/epigram and ~/.bakonrc files exist. If
 they don't then they are created.
 ---------------------------------------------------------------------------*/

#ifndef PIGSTY_H
#define PIGSTY_H

#include "destroyer.h"
#include "error.h"
#include <dirent.h>
#include <sys/stat.h>

class Pigsty {
  private:
    Pigsty(const Pigsty& src);            // not defined
    void operator=(const Pigsty& src);    // not defined
  private:
    char* Home;        // return value of getenv (~/)
    char* Pigdir;      // holds location of ~/.pigsty
    char* Scores;      // holds location of ~/.pigsty/scores
    char* Epigram;     // holds location of ~/.pigsty/epigram
    char* Bakonrc;     // holds location of ~/.bakonrc
  private:
    int Psize;         // strlen(Pigdir)
    int Ssize;         // strlen(Scores)
    int Esize;         // strlen(Epigram)
    int Bsize;         // strlen(Bakonrc)
    DIR* dir;          // reads directory entries
  private:
    struct dirent* dp;                      // counts directory entries
    struct stat stystat;                    // checks file permissions.
    static Pigsty* pInstance;               // ptr to singleton
    static Destroyer<Pigsty> destroyer;     // destroy singletons
  protected:
    Pigsty() throw(Error);                  // construct Env and Def
    virtual ~Pigsty();                      // destructor calls closePig()
    void closePig();                        // delete arrays
    friend class Destroyer<Pigsty>;         // destroy singletons
  public:
    static Pigsty* Instance() throw(Error); // create singleton
  public:
    char* getScores();      // get location of ~/.pigsty/scores
    char* getEpigram();     // get location of ~/.pigsty/epigram
    char* getBakonrc();     // get location of ~/.bakonrc
  public:
    int& getSsize();        // return Psize
    int& getEsize();        // return Psize
    int& getBsize();        // return Psize
  private:
    void setPigsty() throw(Error);          // set ~/.pisty dir
    void setScores() throw(Error);          // set scores file
    void setEpigram() throw(Error);         // set epigram file
    void setBakonrc() throw(Error);         // set bakonrc file
};

inline Pigsty::~Pigsty() { closePig(); }

inline char* Pigsty::getScores() { return Scores; }

inline char* Pigsty::getEpigram() { return Epigram; }

inline char* Pigsty::getBakonrc() { return Bakonrc; }

inline int& Pigsty::getSsize() { return Ssize; }

inline int& Pigsty::getEsize() { return Esize; }

inline int& Pigsty::getBsize() { return Bsize; }

#endif
