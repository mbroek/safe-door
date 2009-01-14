# Makefile for Safe Cracker door for Unix.
# Copyright (c) 2003 by M. Broek.
# $Id$

include ../Makefile.global

DATA		= safe-config safeopen.ans safeopen.asc safewel.ans safewel.asc
OTHER		= Makefile

#############################################################################################################

all:

clean: 
		rm -f filelist Makefile.bak

install:
		@if [ ! -f ${DESTDIR}${ETCDIR}/safe/safe-config ]; then \
			mkdir -p ${DESTDIR}${ETCDIR}/safe ; \
			cp ${DATA} ${DESTDIR}${ETCDIR}/safe ; \
		else  \
			cp safe-config  ${DESTDIR}${ETCDIR}/safe/safe-config.new ; \
			echo "Installed ${DESTDIR}${ETCDIR}/safe/safe-config.new" ; \
		fi

filelist:	Makefile
		BASE=`pwd`; \
		BASE=`basename $${BASE}`; \
		(for f in ${SRCS} ${HDRS} ${OTHER} ${DATA}; do echo ${PACKAGE}-${VERSION}/$${BASE}/$$f; done) >filelist

depend:
