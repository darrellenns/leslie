AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
leslie_CPPFLAGS = $(gl_CFLAGS) $(glu_CFLAGS) $(freetype2_CFLAGS) $(jack_CFLAGS) $(alsa_CFLAGS) $(liblo_CFLAGS) $(ftgl_CFLAGS) $(boost_signals_CFLAGS) $(glut_CFLAGS)
bin_PROGRAMS = leslie
leslie_SOURCES = src/gui.cpp src/leslie.cpp src/main.cpp src/osc.cpp src/widget.cpp src/gui.h src/leslie.h src/main.h src/osc.h src/widget.h
leslie_LDADD = $(gl_LIBS) $(glu_LIBS) $(freetype2_LIBS) $(jack_LIBS) $(alsa_LIBS) $(liblo_LIBS) $(ftgl_LIBS) $(boost_signals_LIBS) $(glut_LIBS) $(BOOST_LDFLAGS) $(BOOST_SIGNALS_LIB)
dist_noinst_SCRIPTS = autogen.sh

.PHONY: full-clean
full-clean: maintainer-clean
	-rm -f aclocal.m4 config.h config.h.in config.h.in~ configure depcomp install-sh Makefile.in missing src/Makefile.in stamp-h1
