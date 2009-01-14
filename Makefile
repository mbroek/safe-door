# Top-level makefile for Safe Cracker door for Unix.
# $Id$

include Makefile.global

OTHER		= ChangeLog COPYING FILE_ID.DIZ.in INSTALL.in Makefile \
		  Makefile.global.in README aclocal.m4 config.h.in \
		  configure configure.in
TARFILE		= ${PACKAGE}-${VERSION}.tar.bz2

###############################################################################


all depend:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done;

install:
		@if [ "`id -un`" != "root" ] ; then \
			echo; echo " Must be root to install!"; echo; exit 3; \
		fi
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done;
		mkdir -p ${DESTDIR}${VARDIR} ${DESTDIR}${VARDIR}/log ${DESTDIR}${VARDIR}/safe
		chmod 0777 ${DESTDIR}${VARDIR}/log
		chmod 0777 ${DESTDIR}${VARDIR}/safe

dist tar:	${TARFILE}

clean:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done;
		rm -f .filelist core ${TARFILE} config.h Makefile.global FILE_ID.DIZ INSTALL

${TARFILE}:	.filelist
		cd ..; ln -s ${PACKAGE} ${PACKAGE}-${VERSION} ; rm -f ${TARFILE}; \
		${TAR} cvTf ./${PACKAGE}-${VERSION}/.filelist - | bzip2 >${TARFILE} ; \
		rm -f ${PACKAGE}-${VERSION}

.filelist filelist:
		(for f in ${OTHER} ;do echo ${PACKAGE}-${VERSION}/$$f; done) >.filelist
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} filelist && cat filelist >>../.filelist) || exit; done;

