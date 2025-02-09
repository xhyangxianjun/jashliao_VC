all::    $(BASEPROGS) $(LIBNAME)

# The actual shared library for the language support
$(LIBNAME):: $(CONVENIENT) $(OBJ)
	$(RM) $(LIBNAME)
	$(LIBTOOL_LD) $(CC) -o $@ $(OBJ) -module -rpath $(LANG_LIBDIR) \
	    $(CONVENIENT) $(BINDEXTERNALLIBS) $(EXTERNALLIBS)
	@-if test -f .libs/libplop_java.dylib; then                      \
	    ln .libs/libplop_java.dylib .libs/libplop_java.jnilib;        \
	fi
	@-if test -f .libs/libpdf_java.dylib; then                      \
	    ln .libs/libpdf_java.dylib .libs/libpdf_java.jnilib;        \
	fi

clean::
	-$(RM) *.$(OBJ) *$(LO) *$(LA) $(LIBNAME) *.bak .libs *.pdf

distclean:: clean
	-$(RM) m.out ms.out

depend:: .dummy
	cp Makefile Makefile.bak
	sed '/^# Automatically generated dependencies/q' Makefile.bak >Makefile
	$(CC) $(CFLAGS) -MM $(SRC) | sed 's/\.o:/$$(LO):/g' >>Makefile

ctags::

install::

uninstall::

.PHONY: clean

.dummy:

.SUFFIXES: .c .$(OBJ) $(LO)

.c$(LO):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.c.$(OBJ):
	$(LIBTOOL_CC) $(CC) -c $(CPPFLAGS) $(CFLAGS) $<
