AC_INIT([mrim-prpl], [0.2], [mrim-prpl@googlegroups.com],, [http://code.google.com/p/mrim-prpl/])
AM_INIT_AUTOMAKE([mrim-prpl], [0.2])
AC_CONFIG_SRCDIR([src/mrim.c])
AC_CONFIG_HEADERS([src/config_generated.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([sys/socket.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_REALLOC

PKG_CHECK_MODULES(PURPLE, [purple >= 2.7.0])

AC_ARG_ENABLE([gtk], AS_HELP_STRING([--enable-gtk], [enable gtk+ dialogs]), [with_gtk=${enableval}], [with_gtk=no])
if test "x${with_gtk}" = "xyes"; then
	AC_DEFINE([ENABLE_GTK], 1, [enable gtk+ dialogs])
	
fi

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_ARG_ENABLE([ft], AS_HELP_STRING([--enable-ft], [add support of the file transfer feature]), [with_ft=${enableval}], [with_ft=no])
if test "x${with_ft}" = "xyes"; then
	AC_DEFINE([ENABLE_FILES], 1, [support file transfer])
	PKG_CHECK_MODULES([GTK], [gtk+-2.0])
fi


AC_DEFINE([BUILD_NUMBER], ["null"], [TODO: add git revision])

AC_CONFIG_FILES([Makefile 
				src/Makefile 
				po/Makefile 
				pixmaps/Makefile])


AC_OUTPUT
