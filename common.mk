include $(top_srcdir)/common-rules.mk
AM_CPPFLAGS+=-DSS_DOCID_BITS=256
AM_CPPFLAGS+=-Wdeprecated-declarations
AM_CPPFLAGS+=-DBUILDTIME="\"`date '+%Y-%m-%d %H:%M:%S'`\""
AM_CPPFLAGS+=-DSVNURL="\"`export LC_ALL=en_US.utf8 ; svn info |grep URL|cut -d' ' -f2`\""
AM_CPPFLAGS+=-DREVERSION="\"`export LC_ALL=en_US.utf8 ; svn info |grep Revision|cut -d' ' -f2`\""
AM_CPPFLAGS+=-DBVERSION="\"1.0.0.1\""

collectlib_DIR=$(top_builddir)/_lib
collectbin_DIR=$(top_builddir)/_bin
collectinclude_DIR=$(top_builddir)/_include
include $(top_srcdir)/collect.mk

