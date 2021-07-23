/* -- error.h -- */

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
 Base class for handling errors using exceptions. Contains a message string.
 ---------------------------------------------------------------------------*/

#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>

class Error {
  private:
    // not defined
    void operator=(const Error& src);
  private:
    // message to print
    std::string message;
  public:
    // construct error message
    Error(const Error& src) : message(src.message) { }
    // default copy constructor
    Error(const std::string m) : message(m) { }
    // defualt destructor
    ~Error() { }
    // return error message
    const std::string getMessage() { return message; }
};

inline std::ostream& operator<<(std::ostream& out, Error& error) {
    out << error.getMessage();
    return out;
}

#endif
