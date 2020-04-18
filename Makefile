# ####################################################################
# 
#			   C/C++ Makefile
# 
# Adapted from
#  Dimitris Floros <fcdimitr@auth.gr> and
#  http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
#  
# ####################################################################
# 
# 'make'        build executable file 'main'
# 'make lib'	build the libraries .a
# 'make clean'  removes all .o and executable files
#

# define the shell to bash
SHELL := /bin/bash

# define the C/C++ compiler to use,default here is clang
CC = gcc

# define compile-time flags
# CFLAGS = -O3 -fcilkplus -fopenmp -Wall -pg -fsanitize=address
CFLAGS = -O3 -Wall -g -fopenmp 

# define any directories containing header files
INCLUDES = -I./inc

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib specify
#   their path using -Lpath, something like:
# LDFLAGS = ../lib/
LDFLAGS = 

# define any libraries to link into executable:
#   To ling libusb-1.0 :
#   LIBS = -lusb-1.0
# LIBS = sort.a -lm
LIBS = -lm

# define the source file for the library
SRC = vptree

SRCDIR = src
LIBDIR = lib

# define the different possible executables
# TYPES = sequential openmp cilk pthreads
TYPES = sequential openmp

# define the executable file  name
MAIN = main
VPATH = src
#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above
#

# call everytime
.PRECIOUS: %.a

all: $(addprefix $(MAIN)_, $(TYPES))

lib: $(addsuffix .a, $(addprefix $(SRC)_, $(TYPES)))

$(MAIN)_%: $(MAIN).c $(SRC)_%.a
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(word 1,$^) $(LIBDIR)/$(word 2,$^) $(LDFLAGS)$(LIBS)
	# $(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)$(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .cpp file) and $@: the name of the target of the rule (a .o file)

.o.a:
	ar rcv $(LIBDIR)/$@ $<
	# ar rcs $@ $<

# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(SRCDIR)/*.o *~ $(addprefix $(MAIN)_, $(TYPES)) $(LIBDIR)/$(SRC)*.a
	# $(RM) *.o *~ $(addprefix $(MAIN)_, $(TYPES)) vptree_*.a

