SHELL = /bin/sh
##############################################################################
#                         Makin' Bakon Typing Tutor                          #
#		               version 0.3.2                                 #
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

# where to put stuff (data, docs, fortune archives and executable)
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
BAKONDIR = $(PREFIX)/makinbakon
DATADIR = $(BAKONDIR)/data
DOCDIR = $(BAKONDIR)/docs
ARCHIVE = $(BAKONDIR)/archive

# compiler path defines
DEFINES = -DARCHIVE="\"$(ARCHIVE)\"" -DDATA="\"$(DATADIR)\"" \
	-DDOC="\"$(DOCDIR)\""

# fortune dat files
DATFILES = offensive offensive.dat poetry poetry.dat startrek startrek.dat

# Makin' Bakon data files
BAKFILES = help.dat intros.dat pigtips.dat lessons.dat welcome.dat speed.dat

# Makin' Bakon documentation
DOCFILES = BAKONRC CHANGES COPYING HELP INSTALL README TODO

# error message if fortune command not found
FORTUNE_ERR = "You must install fortune before compiling Makin' Bakon!"
# error message for install
INSTALL_ERR = "You must be root to install Makin' Bakon!"
# error message for uninstall
UNINSTALL_ERR = "You must be root to uninstall Makin' Bakon!"
# error message for uninstall
CURSES_ERR = "Please install the curses development package first!"
# error message if strfile command not found
STRFILE_ERR = "'strfile' not found. Do 'make MY_PATH=/path/to/strfile'"
# error message for make
MAKE_ERR = "Do 'make' first!"
# error message to print on exit
ERR =

# define curses include files and path
CURSES_INCS = curses.h panel.h menu.h

TMP_CURSES = /usr/local/Cellar/ncurses
CURSES_PATH = $(TMP_CURSES)/`ls $(TMP_CURSES)`

# define path to strfile command (edit this if your path is different)
POSS_PATHS = /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin \
             /usr/games /usr/share/games /usr/local/share/games

#---------------------------------- make ------------------------------------#

# search curses include files
FOUND = $(foreach name,$(CURSES_INCS),$(findstring \
        $(name),$(CURSES_PATH)/include/ncursesw/$(name)))

# search path to strfile command
MY_PATH = $(firstword $(wildcard $(addsuffix /strfile,$(POSS_PATHS))))

.SILENT:

all: have_fortune have_curses make_archive make_bakon

have_fortune:
	echo -n "Testing for the fortune command... " 
	if `fortune > /dev/null 2>&1` ; then \
	    echo "Working" ; \
	else \
	    echo "Not Working!" ; \
	    $(MAKE) err ERR=$(FORTUNE_ERR) -j8 ; \
	fi

have_curses:
	echo -n "Searching for Curses include files in $(CURSES_PATH)... "
	echo "$(FOUND)"
	for i in $(CURSES_INCS) ; do \
	    if [ -z "`echo $(FOUND) | grep $$i`" ] ; then \
                echo "($$i) Not Found! " ; \
		$(MAKE) err ERR=$(CURSES_ERR) -j8 ; \
	        exit ; \
	    fi ; \
	done ; \
        echo "Found"

make_archive:
	echo -n "Searching for the strfile command... "
	if [ -z "`echo $(MY_PATH) | grep '/strfile'`" ] ; then \
	    echo "Not Found!" ; \
	    $(MAKE) err ERR=$(STRFILE_ERR) -j8 ; \
	else \
	    echo "Found" ; \
	    cd `pwd`/archive && $(MAKE) DODATS='$(MY_PATH)' -j8 ; \
	fi

make_bakon:
	echo -n "Checking for object files and executable... " ; \
	cd `pwd`/src ; \
	if $(MAKE) -q bakon ; then \
	    echo "Done" ; \
	    echo "Makin' Bakon is up to date!" ; \
	else \
	    echo "Done" ; \
	    echo "Makin' Bakon. This could take a while..." ; \
	    echo "" ; \
	    $(MAKE) CURSES_PATH='$(CURSES_PATH)' -j8 ; \
	fi
	
	echo "#!/bin/bash\ncd \c" > bakon
	cd src && echo "`pwd`\c" >> ../bakon
	echo " && ./bakon" >> bakon
	chmod +x bakon
	
	echo ""
	echo "Made Bakon! Run './bakon' to start."
	
#------------------------------- make install -------------------------------#

.PHONEY: install
install:
	if [ -z "`ls $$(pwd)/src | grep '^bakon$$'`" ] ; then \
	    $(MAKE) err ERR=$(MAKE_ERR) ; \
	elif [ -z "`ls $$(pwd)/archive | grep '\.dat'`" ] ; then \
	    $(MAKE) err ERR=$(MAKE_ERR) ; \
	elif [ -z "`id | grep 'uid=.*(root)'`" ] ; then \
	    $(MAKE) err ERR=$(INSTALL_ERR) ; \
	fi

	if [ ! -d $(BAKONDIR) ] ; then \
	    echo "Creating $(BAKONDIR)" ; \
	    install -d -m 755 $(BAKONDIR) ; \
	fi

	if [ ! -d $(DATADIR) ] ; then \
	    echo "Creating $(DATADIR)" ; \
	    install -d -m 755 $(DATADIR) ; \
	fi

	echo "Installing data files to $(DATADIR)/"

	for i in $(BAKFILES) ; do \
	    install -m 644 `pwd`/data/$$i $(DATADIR)/$$i ; \
	done

	if [ ! -d $(ARCHIVE) ] ; then \
	    echo "Creating $(ARCHIVE)" ; \
	    install -d -m 755 $(ARCHIVE) ; \
	fi

	echo "Installing fortune archives to $(ARCHIVE)/"

	for i in $(DATFILES) ; do \
	    install -m 644 `pwd`/archive/$$i $(ARCHIVE)/$$i ; \
	done

	if [ ! -d $(DOCDIR) ] ; then \
	    echo "Creating $(DOCDIR)" ; \
	    install -d -m 755 $(DOCDIR) ; \
	fi

	echo "Installing documentation to $(DOCDIR)/"

	for i in $(DOCFILES) ; do \
	    install -m 644 `pwd`/$$i $(DOCDIR)/$$i ; \
	done

	echo "Installing executable to $(BINDIR)/"

	install -s -m 755 `pwd`/src/bakon $(BINDIR)/bakon 

	echo "Finished installing Makin' Bakon!"

#------------------------------ make uninstall ------------------------------#

.PHONEY: uninstall
uninstall:
	if [ -z "`id | grep 'uid=.*(root)'`" ] ; then \
	    $(MAKE) err ERR=$(UNINSTALL_ERR) ; \
	fi
	
	if [ -d $(BAKONDIR) ] ; then \
	    echo "Removing $(BAKONDIR)" ; \
	    rm -rf $(BAKONDIR) ; \
	else \
	    echo "$(BAKONDIR) does not exist!" ; \
	fi
	
	if [ -x $(BINDIR)/bakon ] ; then \
	    echo "Removing $(BINDIR)/bakon" ; \
	    rm -f $(BINDIR)/bakon ; \
	else \
	    echo "$(BINDIR)/bakon does not exist!" ; \
	fi
	
#-------------------------------- make clean --------------------------------#

.PHONEY: clean
clean:
	if [ -z "`ls archive | grep '\.dat'`" ] ; then \
	    echo "No fortune.dat files to remove from archive directory!" ; \
	else \
	    echo "Removing fortune.dat files from archive directory" ; \
	    rm -f archive/*.dat ; \
	fi
	if [ -z "`ls src | grep '\.o'`" ] ; then \
	    echo "No object files to remove from src directory!" ; \
	else \
	    echo "Removing object files from src directory" ; \
	    rm -f src/*.o ; \
	fi
	if [ -z "`ls src | grep '^bakon$$'`" ] ; then \
	    echo "No executable to remove from src directory!" ; \
	else \
	    echo "Removing executable from src directory" ; \
	    rm -f src/bakon ; \
	fi
	
	rm -f bakon
	
#-------------------------------- exit make ---------------------------------#

.PHONY: err
err:
	$(error $(ERR))

#-------------------------------- make help ---------------------------------#

.PHONEY: help
help:
	echo "---------------------------------------------------------------"
	echo "Command        : User : Description"
	echo "---------------------------------------------------------------"
	echo "make           : user : build a brand new copy of Makin' Bakon"
	echo "make uninstall : root : remove an old copy of Makin' Bakon"
	echo "make install   : root : install the new copy if Makin' Bakon"
	echo "make clean     : user : clean src directory of unwanted files"
	echo "make help      : both : this help text..."
	echo "---------------------------------------------------------------"
	echo "Please read the INSTALL document for more detailed instructions"
	
#----------------------------------- END ------------------------------------#
