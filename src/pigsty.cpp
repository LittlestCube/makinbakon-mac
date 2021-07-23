/* -- pigsty.cpp -- */

/*****************************************************************************
 *                         Makin' Bakon Typing Tutor                         *
 *		               version 0.3.1                                 *
 *		      Copyright (C) 2002 Stephen Webster                     *
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

#include "pigsty.h"
#include "macros.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <new>
#include <cstring>
#include <unistd.h>

using namespace std;

// initialise pInstance static
Pigsty* Pigsty::pInstance = 0;
Destroyer<Pigsty> Pigsty::destroyer;

Pigsty* Pigsty::Instance() throw (Error) {
    if (pInstance == 0) {
	if ((pInstance = new (std::nothrow) Pigsty) == 0) {
	    throw Error("can't create pigsty instance: pigsty.cpp");
	}
	destroyer.setDoomed(pInstance);
    }
    return pInstance;
}

Pigsty::Pigsty() throw(Error)
    : Home(0), Pigdir(0), Scores(0), Epigram(0), Bakonrc(0), Psize(0),
      Ssize(0), Esize(0), Bsize(0), dir(NULL)
{
    try {
	// set array for ~/ path
	if ((Home = new (std::nothrow) char[strlen(getenv("HOME")) + 1]) == 0)
	    throw Error("bad_alloc - can't create Home: pigsty.cpp");

	strcpy(Home, getenv("HOME")); 
	
	// set array for ~/.pigsty path
	Psize = strlen(Home) + strlen(D_PIGSTY) + 1;

	if ((Pigdir = new (std::nothrow) char[Psize]) == 0)
	    throw Error("bad_alloc - can't create Pigsty: pigsty.cpp");
	
	strcpy(Pigdir, Home);
	strcat(Pigdir, D_PIGSTY);
    
	// set array for ~/.pigsty/scores path
	Ssize = strlen(Pigdir) + strlen(D_SCORES) + 1;
	
	if ((Scores = new (std::nothrow) char[Ssize]) == 0)
	    throw Error("bad_alloc - can't create Scores: pigsty.cpp");
	
	strcpy(Scores, Pigdir);
	strcat(Scores, D_SCORES);
    
	// set array for ~/.pigsty/epigram path
	Esize = strlen(Pigdir) + strlen(D_EPIGRAM) + 1;
	
	if ((Epigram = new (std::nothrow) char[Esize]) == 0)
	    throw Error("bad_alloc - can't create Epigram: pigsty.cpp");
	
	strcpy(Epigram, Pigdir);
	strcat(Epigram, D_EPIGRAM);
    
	// set array for ~/.bakonrc path
	Bsize = strlen(Home) + strlen(D_BAKONRC) + 1;
	
	if ((Bakonrc = new (std::nothrow) char[Bsize]) == 0)
	    throw Error("bad_alloc - can't create Bakonrc: pigsty.cpp");
	
	strcpy(Bakonrc, Home);
	strcat(Bakonrc, D_BAKONRC);
    }
    catch (Error err) { 
	closePig();
	throw err;
    }
    catch (...) {
	closePig();
	throw Error("unexpected exception during construction: pigsty.cpp");
    }
    
    try {
	// test/create files
	setPigsty();
	setScores();
	setEpigram();
	setBakonrc();
    }
    catch (Error err) {
	closePig();
	throw err;
    }
}

// check/create ~/.pigsty
void Pigsty::setPigsty() throw(Error) {
    
    try {
	int pigdir = 0;        // return value of mkdir
	bool found = false;    // true if ~/.pigsty exists
	
	if ((dir = opendir(Home)) == NULL)
	    throw Error("opendir returned error in setPigsty: pigsty.cpp");
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
	    if (!strcmp(PIGSTY, dp->d_name)) {
		stat(Pigdir, &stystat);
		if (stystat.st_mode & S_IFDIR) {
		    if ((stystat.st_mode & S_IRUSR) &&
			(stystat.st_mode & S_IWUSR) && 
			(stystat.st_mode & S_IXUSR)) {
			closedir(dir);
			dir = NULL;
			found = true;
			break;
		    }
		    else {
			closedir(dir);
			dir = NULL;
			throw Error("you must have wrx permission for ~/.pigsty: pigsty.cpp");
		    }
		}
		else {
		    closedir(dir);
		    dir = NULL;
		    throw Error("there is already a .pigsty file of some type in "
				"your home directory. You'll need to rename/move "
				"it before you can use Makin' Bakon");
		}
	    }
	    
	}
	if (!found) {
	    closedir(dir);
	    dir = NULL;
	    if ((pigdir = mkdir(Pigdir, 0700)) == -1) {
		throw Error("mkdir returned error: pigsty.cpp");
	    }
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (dir != NULL) {
	    closedir(dir);
	}
	throw Error("unexpected exception in setPigsty: pigsty.cpp");
    }
}

// check/create ~/.pigsty/scores
void Pigsty::setScores() throw (Error) {
	
    try {
	if ((dir = opendir(Pigdir)) == NULL)
	    throw Error("opendir returned error in setScores: pigsty.cpp");
	
	bool found = false;
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
	    if (!strcmp(SCORES, dp->d_name)) {
		stat(Scores, &stystat);
		if (stystat.st_mode & S_IFREG) {
		    if ((stystat.st_mode & S_IRUSR) &&
			(stystat.st_mode & S_IWUSR)) {
			closedir(dir);
			dir = NULL;
			found = true;
			break;
		    }
		    else {
			closedir(dir);
			dir = NULL;
			throw Error("you must have read and write permissions for "
				    "~/.pigsty/scores: pigsty.cpp");
		    }
		}	
		else {
		    closedir(dir);
		    dir = NULL;
		    throw Error("there is already a scores file of some type in "
				"~/.pigsty. You'll need to rename/move it before "
				"you can use Makin' Bakon");
		}
	    }
	}
	if (!found) {
	    closedir(dir);
	    dir = NULL;
	    ofstream out(Scores, ios::out);
	    if (out.is_open()) {
		out.close();
	    }
	    else throw Error("setScores can't create Scores file: pigsty.cpp");
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (dir != NULL) {
	    closedir(dir);
	}
	throw Error("unexpected exception in setScores: pigsty.cpp");
    }
}

// check/unlink ~/.pigsty/epigram
void Pigsty::setEpigram() throw (Error) {
    
    try {
	if ((dir = opendir(Pigdir)) == NULL)
	    throw Error("opendir returned error in setEpigram: Pigsty.cpp");
    
	bool found = false;
	
	// if an epigram file exists, check that it has
	// both read and write access, otherwise unlink

	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
	    if (!strcmp(EPIGRAM, dp->d_name)) {
		stat(Epigram, &stystat);
		if (stystat.st_mode & S_IFREG) {
		    if ((stystat.st_mode & S_IRUSR) &&
			(stystat.st_mode & S_IWUSR)) {
			closedir(dir);
			dir = NULL;
			found = true;
			break;
		    }
		    else {
			closedir(dir);
			dir = NULL;
			int fs = unlink(Epigram);
			if (fs < 0) {
			    throw Error("unlink command failed: pigsty.cpp");
			}
		    }
		}	
		else {
		    closedir(dir);
		    dir = NULL;
		    throw Error("there is already an epigram file of some type in "
				"~/.pigsty. You'll need to rename/move it before "
				"you can use Makin' Bakon");
		}
	    }
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (dir != NULL) {
	    closedir(dir);
	}
	throw Error("unexpected exception in setEpigram: pigsty.cpp");
    }
}

// check/create ~/.bakonrc
void Pigsty::setBakonrc() throw (Error) {
    
    try {
	if ((dir = opendir(Home)) == NULL)
	    throw Error("opendir returned error in setBakonrc: pigsty.cpp");
	
	bool found = false;
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
	    if (!strcmp(BAKONRC, dp->d_name)) {
		stat(Bakonrc, &stystat);
		if (stystat.st_mode & S_IFREG) {
		    if (stystat.st_mode & S_IRUSR) {
			closedir(dir);
			dir = NULL;
			found = true;
			break;
		    }
		    else {
			closedir(dir);
			dir = NULL;
			throw Error("You must have read permission for ~/.bakonrc: "
				    "pigsty.cpp");
		    }
		}	
		else {
		    closedir(dir);
		    dir = NULL;
		    throw Error("there is already a .bakonrc file of some type in "
				"your home directory. You'll need to rename/move "
				"it before you can use Makin' Bakon");
		}
	    }
	}
	if (!found) {
	    closedir(dir);
	    dir = NULL;
	    
	    // default copy resides in DOC directory
	    string bakonbak(DOC);
	    bakonbak += D_BAKONBAK;
	    // make a copy of DOC/BAKONRC
	    ifstream in(bakonbak.c_str(), ios::in);
	    if (in.bad()) {
		throw Error("setBakonrc can't open DOC/BAKONRC: pigsty.cpp");
	    }
	    
	    string temp;   // get a line of file
	    string bako;   // hold file contents 
	    
	    while (!in.eof()) {
		getline(in, temp);
		bako += temp;
		bako.append(1, '\n');
	    }
	    in.close();
	    
	    // now create new copy of ~/.bakonrc
	    ofstream out(Bakonrc, ios::out);
	    if (out.is_open()) {
		out << bako; 
		out.close();
	    }
	    else throw Error("setBakonrc can't create Bakonrc file: pigsty.cpp");
	}
    }
    catch (Error err) {
	throw err;
    }
    catch (...) {
	if (dir != NULL) {
	    closedir(dir);
	}
	throw Error("unexpected exception in setBakonrc: pigsty.cpp");
    }
}
    
// destroy dynamic arrays
void Pigsty::closePig() {

    if (Home != 0) {
	delete[] Home; 
	Home = 0;
    }
    if (Pigdir != 0) {
	delete[] Pigdir; 
	Pigdir = 0;
    }
    if (Scores != 0) {
	delete[] Scores; 
	Scores = 0;
    }
    if (Epigram != 0) {
	delete[] Epigram;
	Epigram = 0;
    }
    if (Bakonrc != 0) {
	delete[] Bakonrc;
	Bakonrc = 0;
    }
}
