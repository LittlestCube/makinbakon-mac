/* -- makemap.h -- */

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
 Create a map of any <KEY, VALUE> type
 ---------------------------------------------------------------------------*/

#ifndef MAKEMAP_H
#define MAKEMAP_H

#include "macros.h"
#include <iostream>
#include <map>

using namespace std;

template<typename KEY, typename VALUE>
class MakeMap : public std::map<KEY, VALUE> {
  private:
    VALUE value;
    int key;
    bool nokeys;
  public:
    // initialize the map
    MakeMap() { }
    // add an item to the map
    void additem(KEY k, VALUE v) { this->insert(make_pair(k, v)); }
    // set elements in the map
    void setval(KEY k, VALUE v) { this->operator[](k) = v; }
    // return true if the map empty
    bool isempty() { 
	nokeys = this->empty();
	return nokeys;
    }
    // find values in the map
    VALUE& findval(KEY k) {
	typename std::map<KEY, VALUE>::iterator it = this->find(k);
	if (it != this->end()) {
	    value = it->second;
	}
	return value;
    }
    // find if a key exists
    int findkey(KEY k) {
	key = this->count(k);
	return key;
    }
    // print out the map
    void printmap(std::ostream& out) {
	for (typename std::map<KEY, VALUE>::iterator it = this->begin(); it != this->end(); it++) {
	    out << it->first << " = " << it->second << endl;
	}
    }
};

#endif
