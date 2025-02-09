PROGS_BUILD = $(LIBTOOL_LD) $(CC) -static $(FLAGS) -o $@ \
	                 $@.c $(EXTRA_SOURCES) $(LIBS)
CXXPROGS_BUILD = $(LIBTOOL_LD) $(CXX) -static $(CXXFLAGS) -o $@ \
	                 $@.cpp $(EXTRA_SOURCES) $(LIBS)

all::    $(PROGS)

clean::
	-$(RM) *.$(OBJ) *$(LO) *$(LA) $(PROGS) *.bak .libs

distclean:: clean
	-$(RM) m.out ms.out

depend::

ctags::

install::

uninstall::

test::

smoke::

.PHONY: clean

.dummy:


.SUFFIXES: .$(OBJ) $(LO)

.c$(LO):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.c.$(OBJ):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.cpp$(LO):
	$(LIBTOOL_CC) $(CXX) -c $(CXXFLAGS) $<

.cpp.$(OBJ):
	$(LIBTOOL_CC) $(CXX) -c $(CXXFLAGS) $<
