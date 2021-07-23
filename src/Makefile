SHELL = /bin/sh
##############################################################################
#                          Makin' Bakon Typing Tutor                         #
#		                version 0.3.2                                #
#		    Copyright (C) 2002 2003 Stephen Webster                  #
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

CC = g++
CURSES_PATH = /usr/local/Cellar/ncurses/*
CFLAGS = -O2 -Wall -I$(CURSES_PATH)/include/ncursesw -I$(CURSES_PATH)/include
LDFLAGS = -L$(CURSES_PATH)/lib -lmenu -lpanel -lncurses
DEFINES = -DARCHIVE=\"../archive\" -DDATA=\"../data\" -DDOC=\"../\"

#---------------------------------- make ------------------------------------#

.SILENT:

all: bakon

SRCS = bakon.cpp pigsty.cpp defaults.cpp scores.cpp getkeys.cpp \
	keyboard.cpp keyinfo.cpp stats.cpp gettext.cpp format.cpp \
	nCurses.cpp fortune.cpp lessonNumber.cpp lessonCheck.cpp \
	lessonMessage.cpp lessonFortune.cpp lessonDefault.cpp \
	lessonSpeed.cpp lessonTest.cpp lessonError.cpp control.cpp
	
OBJECTS = $(SRCS:.cpp=.o)

.cpp.o:
	echo "$(CC) $(CFLAGS) -c $<"
	$(CC) $(CFLAGS) $(DEFINES) -c $<
	
bakon: $(OBJECTS)
	
	echo ""
	echo "$(CC) $(LDFLAGS) -o $@ $(OBJECTS)"
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

#------------------------------ dependencies --------------------------------#

bakon.o: bakon.cpp display.h error.h gettext.h destroyer.h scores.h \
	pigsty.h control.h stats.h menus.h macros.h defaults.h makemap.h

control.o: control.cpp control.h display.h error.h gettext.h destroyer.h \
	scores.h pigsty.h stats.h menus.h macros.h lesson.h

defaults.o: defaults.cpp defaults.h error.h pigsty.h destroyer.h makemap.h \
	macros.h

format.o: format.cpp display.h error.h gettext.h destroyer.h scores.h \
	pigsty.h

fortune.o: fortune.cpp fortune.h error.h pigsty.h destroyer.h keyboard.h \
	key.h getkeys.h

getkeys.o: getkeys.cpp getkeys.h key.h error.h destroyer.h macros.h \
	defaults.h pigsty.h makemap.h

gettext.o: gettext.cpp gettext.h error.h destroyer.h scores.h pigsty.h \
	macros.h

keyboard.o: keyboard.cpp keyboard.h key.h error.h getkeys.h destroyer.h

keyinfo.o: keyinfo.cpp keyinfo.h keyboard.h key.h error.h getkeys.h \
	destroyer.h

lessonCheck.o: lessonCheck.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h keyboard.h key.h getkeys.h \
	keyinfo.h

lessonDefault.o: lessonDefault.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h macros.h

lessonError.o: lessonError.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h

lessonFortune.o: lessonFortune.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h fortune.h

lessonMessage.o: lessonMessage.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h macros.h

lessonNumber.o: lessonNumber.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h macros.h defaults.h makemap.h

lessonSpeed.o: lessonSpeed.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h

lessonTest.o: lessonTest.cpp lesson.h display.h error.h gettext.h \
	destroyer.h scores.h pigsty.h stats.h macros.h

nCurses.o: nCurses.cpp display.h error.h gettext.h destroyer.h scores.h \
	pigsty.h keyboard.h key.h getkeys.h macros.h keyinfo.h defaults.h \
	makemap.h

pigsty.o: pigsty.cpp pigsty.h destroyer.h error.h macros.h

scores.o: scores.cpp scores.h error.h pigsty.h destroyer.h macros.h

stats.o: stats.cpp stats.h destroyer.h scores.h error.h pigsty.h macros.h


#----------------------------------- END ------------------------------------#
