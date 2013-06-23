AC_INIT([Leslie], [0.1], [darrell@darrellenns.com],
        [leslie], [http://www.darrellenns.com])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])
AC_CONFIG_MACRO_DIR([m4])

#AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_PROG_CC

AC_ARG_ENABLE([gui],
    AS_HELP_STRING([--disable-gui], [Disable feature foo]))

AS_IF([test "x$enable_gui" != "xno"], [
	PKG_CHECK_MODULES([gl], [gl])
	PKG_CHECK_MODULES([glu], [glu])
	PKG_CHECK_MODULES([freetype2], [freetype2])
	PKG_CHECK_MODULES([ftgl], [ftgl])
	AC_SEARCH_LIBS(glutMainLoop,glut,,[AC_MSG_ERROR([glut not found])])
	AX_BOOST_BASE([1.46])
	AX_BOOST_SIGNALS
	AC_DEFINE(ENABLE_GUI)
])

PKG_CHECK_MODULES([jack], [jack])
PKG_CHECK_MODULES([alsa], [alsa])
PKG_CHECK_MODULES([liblo], [liblo])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
