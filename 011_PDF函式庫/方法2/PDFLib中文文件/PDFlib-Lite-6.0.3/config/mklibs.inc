# Template to be included from every makefile that creates libs
# $Id: mklibs.inc,v 1.2 2004/04/01 14:10:32 rjs Exp $
#

all::	$(LIBNAME)

$(LIBNAME): $(OBJS)
	$(LIBTOOL_LD) $(CC) -o $@ $(OBJS) $(LIBTOOL_LDOPT)

clean::
	-$(RM) *.$(OBJ) *$(LO) *$(LA) *.bak .libs

distclean:: clean
	-$(RM) m.out ms.out

depend:: .dummy
	cp Makefile Makefile.bak
	sed '/^# Automatically generated dependencies/q' Makefile.bak >Makefile
	$(CC) $(CFLAGS) -MM $(SRC) | sed 's/\.o:/$$(LO):/g' >>Makefile

install::

uninstall::

test::

smoke::

.PHONY: clean

.dummy:

# suffixes
.SUFFIXES: .c .$(OBJ) $(LO)

.c$(LO):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.c.$(OBJ):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<
