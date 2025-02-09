# Template to be included from every makefile that creates libs
# $Id: mkmainlib.inc,v 1.2.2.2 2005/07/18 16:00:39 rjs Exp $
#

LIBLINKSELF     = $(MAINOBJ) $(INTERNALLIBS) $(EXTERNALLIBS)

all::	$(MAINLIBNAME)

$(MAINLIBNAME): $(MAINOBJ) $(INTERNALLIBS)
	$(LIBTOOL_LD) $(CC) $(LDFLAGS) -o $@ $(RPATH) $(LIBLINKSELF) -export-dynamic
	$(LIBTOOL_LD) $(CC) $(LDFLAGS) -o libs_$@ $(LIBLINKSELF)

clean::
	-$(RM) $(MAINLIBNAME)

depend::
	$(CC) $(CFLAGS) -MM $(MAINSRC) | sed 's/\.o:/$$(LO):/g' >>Makefile

install:: $(MAINLIBNAME)
	if test ! -d $(libdir) ; then 	\
	    mkdir -p $(libdir);		\
	    chmod 755 $(libdir);	\
	fi
	if test ! -d $(includedir) ; then 	\
	    mkdir -p $(includedir);		\
	    chmod 755 $(includedir);		\
	fi
	$(INSTALL_DATA) $(MAININC) $(includedir)
	@-if test "$(WITH_SHARED)" = "no"; then	\
	    cp .libs/$(MAINLIBNAME) .libs/$(MAINLIBNAME)i;\
	fi
	$(LIBTOOL) $(INSTALL_DATA) $(MAINLIBNAME) $(libdir);
	@-if test "$(WITH_SHARED)" = "yes"; then	\
	    $(LIBTOOL) -n --finish $(libdir);\
	else\
	    rm -f .libs/$(MAINLIBNAME)i;\
	fi

uninstall:: .dummy
	$(RM) $(includedir)/$(MAININC)
	-$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/$(MAINLIBNAME)

test::

smoke::

.PHONY: clean
