# Makefile for Safe Cracker door for Unix.
# Copyright (c) 2003 by M. Broek.
# $Id$

include ../Makefile.global

OTHER		= Makefile safe-config safeopen.ans safeopen.asc safewel.ans safewel.asc

#############################################################################################################

all:

clean: 
		rm -f filelist Makefile.bak

install:

filelist:	Makefile
		BASE=`pwd`; \
		BASE=`basename $${BASE}`; \
		(for f in ${SRCS} ${HDRS} ${OTHER} ; do echo ${PACKAGE}-${VERSION}/$${BASE}/$$f; done) >filelist

depend:
