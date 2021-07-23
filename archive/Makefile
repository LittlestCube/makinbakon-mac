SHELL = /bin/sh
##############################################################################
#                         Makin' Bakon Typing Tutor                          #
#		               version 0.3.2                                 #
#		   Copyright (C) 2002 2003 Stephen Webster                   #
#                                                                            #
#  This program is free software; you can redistribute it and/or modify it   #
#  under the terms of the GNU General Public License as published by the     #
#  Free Software Foundation; either version 2 of the License, or (at your    #
#  option) any later version.                                                #
#                                                                            #
#  This program is distributed in the hope that it will be useful, but       # 
#  WITHOUT ANY WARRANTY; without even the implied warranty of                #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                      #
#                                                                            #
#  See the GNU General Public License for more details.                      #
#                                                                            #
#  You should have received a copy of the GNU General Public License along   #
#  with this program; if not, write to the Free Software Foundation, Inc.,   #
#  59 Temple Place, Suite 330, Boston, MA 0.1.2-1307 USA                     #
#                                                                            #
#  Please read the COPYING file for the complete license. The copyright      #
#  holder can be contacted via email: moonunit@ukonline.co.uk                #
##############################################################################

#--------------------------------- macros -----------------------------------#

# fortune archives
SRCS = offensive poetry startrek

# path to strfile command
DODATS = strfile

#---------------------------------- make ------------------------------------#

.SILENT:

all: dotdat

dotdat:
	for i in $(SRCS) ; do \
	    echo -n "Checking archive for $$i.dat... " ; \
	    if ls `pwd` | grep "$$i.dat" > /dev/null 2>&1 ; then \
		echo "Found" ; \
	    else \
		echo "Creating $$i.dat" ; \
	        $(DODATS) -rs $$i ; \
	    fi ; \
	done

#----------------------------------- END ------------------------------------#
