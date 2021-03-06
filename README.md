To run the program, you must `brew install ncurses` and `brew install fortune` if you haven't already.

Then you may simply `make` and `./bakon`.

Original README, prettied up with markdown:
                                                                              
#                         Makin' Bakon Typing Tutor                           
#                              version 0.3.2                                  
#                   Copyright (C) 2002 2003 Stephen Webster                   
                                                                              
   This program is free software; you can redistribute it and/or modify it    
   under the terms of the GNU General Public License as published by the      
   Free Software Foundation; either version 2 of the License, or (at your     
   option) any later version.                                                 
                                                                              
   This program is distributed in the hope that it will be useful, but         
   WITHOUT ANY WARRANTY; without even the implied warranty of                 
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       
                                                                              
   See the GNU General Public License for more details.                       
                                                                              
   You should have received a copy of the GNU General Public License along    
   with this program; if not, write to the Free Software Foundation, Inc.,    
   59 Temple Place, Suite 330, Boston, MA 0.1.2-1307 USA                      
                                                                              
   Please read the COPYING file for the complete license. The copyright       
   holder can be contacted via email: moonunit@ukonline.co.uk                 
                                                                              

* 1: Warning
* 2: A brief history
* 3: Installation
* 4: Usage
* 5: Command line options
* 6: Bugs and De-bugging
* 7: Getting fortune
* 8: Acknowledgements

# WARNING

Makin' Bakon uses Fortune epigrams for some of its text. The epigrams chosen
are sometimes of a politically or sexually insensative nature. If you are
worried about being offended then please use another typing tutor...

# A very brief history of Makin' Bakon

The Makin' Bakon Typing Tutor started life, like all worthy ideas, down the
pub. I was looking for a project and a close friend suggested I do a typing
tutor because he didn't think there were any worth using on the Linux/BSD
platforms. (There are, as it goes... Go to http://freshmeat.net/ and do a
search for "typing tutor")

We started talking about other, more famous typing tutors and how dull the
text was. Well, several pints and a few puns later (but before solving the
worlds problems) we hit upon the idea of Makin' Bakon, a typing tutor that
entertains and maybe even offends, as well as doing all the proper typing
tutor stuff.

If you like fortune and the idea of a typing tutor that uses them, and you
need to learn typing to a professional standard, then read on...

# Installation

If you want to, copy the BAKONRC file to .bakonrc in your home directory. If
you don't want then a copy will be created for you when you first start-up
the program. Read the INSTALL file for further instructions.

NOTE: You must have the fortune program and the curses development libs
installed before you can compile Makin' Bakon. 

# Usage

For the seriously impatient, type 'bakon' (minus the quotes, Doh!) and any
command-line options at the prompt and all will be revealed, though I would
ask you to read the following text, it wont take long, honest...

Makin' Bakon is really a game that you play in a terminal screen. You have
to rescue Pig from the clutches of the evil factory farmer by typing as fast
and as accurately as you can. Pig gets hurt if you leave a lesson early and
he gets sliced and diced and made into sausages if you don't reach your
words-per-minute target by the time you quit. Read the online help text for
more information.

As well as the default lessons (which you should do first, they don't take
long), there are five seperate fortune based lessons. Bear in mind that the 
default lessons will improve your typing speed much faster than the fortune
based lessons...

Makin bakon adds the following to your home directory...

~/.bakonrc         -  you can set defaults in this
~/.pigsty/scores   -  holds your usage details
~/.pigsty/epigram  -  temp file holds fortune epigrams

# Command line options

Color and keyboard type can be defined on the command line, or you can set
defaults in the ~/.bakonrc file. Command line options are as follows: 

-fg (foreground)
-bg (background)
-bd (bold)
-kb (keyboard)

The color choices are black, red, green, yellow, blue, magenta, cyan, white
and trans (transparent). Setting the trans option throughout will leave
terminal settings unchanged, which suits some transparent terminals like
Aterm and Eterm. However, if you choose this path then you may want to set
the terminals BOLD to a different color. See your terminal man page for
instructions.

If you get no colors in Aterm then you probably have the -tn (--termName)
switch set, unset it and all should be well.

You can choose between two keyboard types, American and British: us or uk.

For a complete list of choices, just type "bakon -h". Note that command line
options override ~/.bakonrc defaults.

# Bugs and De-bugging

a) If something goes horribly wrong and your terminal/console freezes, you
   can kill Makin' Bakon by opening up another terminal/console and typing
   'killall bakon'. You can quit Makin' Bakon at any time simply by typing
   CNTRL-Q, or just Q if in the menu screen.

b) Works fine in the standard Linux consol, Aterm, Eterm and Xterm. Doesn't
   scroll so good in KDE Konsole. If you get no colors in Aterm then you
   probably have the -tn (--termName) switch set, unset it and all should be
   well.

c) This program has been tested with curses 1.96 and 1.97. If you have ANY
   terminal/console problems then check /usr/include/curses.h for your
   version of curses. It's important that you let me know what version you're
   using when sending bug reports. (Also let me know about terminal type or
   $TERM setting in console (do 'echo $TERM' at the prompt). 

d) If the program exits with an exception then let me know what the exception
   was. Also include curses and TERM details as outlined above.

d) If you get a segmentation fault and your terminal isn't responding
   properly then type 'reset' at the prompt. You may not be able to see the
   letters as you type but do it anyway, and then press return.
   
   Send bug reports, with any shell output, curses and term details to the
   adress below.

e) Makin' Bakon can be compiled with the -g flag, which means that you can
   get sensible information out of gdb, the GNU debugger. If you want to do
   this, and you get a segmentation fault OR the program exits with "Aborted"
   then just change the line in src/Makefile that looks like this...
   
   CFLAGS = -O2 -Wall
   
   to look like this...
   
   CFLAGS = -g -Wall

   Once your terminal is behaving properly, enter 'gdb bakon' and press return.
   (gdb is a standard part of most linux/BSD distributions.) At the gdb prompt,
   type 'run' and then use the program as you did before. When you get the seg
   fault, gdb will catch it. Type 'where' or 'bt' at the prompt and cut'n'paste
   the stack trace that follows into a text file.

   Send me the text file, with curses and term details to the address below.
   
                                                                              
                  Send bug reports to: moonunit@ukonline.co.uk                
                                                                              

# Getting Fortune

You will need the Fortune program before you can install Makin' Bakon. It is
usually included as part of your distribution.

If you're running Linux and you haven't got fortune then...

You can get the fortune rpm from:
http://rpmfind.net/linux/RPM/contrib/libc6/i386/fortune-mod-1.2.1-1.i386.html

You'll also need the fortune database rpm:
http://rpmfind.net/linux/RPM/contrib/noarch/noarch/
fortune-mod-fortunes-1.2.1-1.noarch.html

You can install the Offensive rpm if you want: 
http://rpmfind.net/linux/RPM/contrib/noarch/noarch/
fortune-mod-offensive-1.2.1-1.noarch.html

Go to Chris Ausbrooks page at rpmfind.net for the latest versions:
http://rpmfind.net/linux/RPM/Chris_Ausbrooks__weed_bucket.pp.ualr.edu_.html

You can get the tar.gz source (which contains everything you'll need) from:
http://ibiblio.org/pub/Linux/games/amusements/fortune/

If you installed the tar.gz package from ibiblio then you may want to create
a link to the executable if it's not in your PATH. You create the link like
this...
    
    ln -s /usr/local/games/fortune /usr/local/bin/

Or... you can include it in your path by adding a line to your ~/.profile or
~/.bashrc, like this... 
    
    PATH=$PATH":/usr/local/games"

# Acknowledgements

Thanks to Steve Peck. It was all his idea. He made me do it! He's the real
brains behind this, no really...

Thanks to Xander Soldaat for testing it in KDE. He also made the case for
integrating color.

Thanks to Richard Corby-Siddens for his help with C++ exception handling.

------------------------------------------------------------------------------

Here's a parting epigram...

I think pop music has done more for oral intercourse than anything else 
that has ever happened, and vice versa. -- Frank Zappa

Happy Typing!
