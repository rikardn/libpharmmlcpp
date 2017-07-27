AC_PREREQ([2.61])
AC_INIT(pharmml2mdl, 0.4)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])

LT_PREREQ([2.2])
LT_INIT

AM_SILENT_RULES([yes])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

PKG_CHECK_MODULES(LIBXML2, libxml-2.0 >= 2.4)

# Get paths to libpharmmlcpp
AC_ARG_VAR([PHARMMLCPPLIB], [The PATH wherein libpharmmlcpp.la can be found.])
AC_ARG_VAR([PHARMMLCPPINC], [The PATH wherein the includes for libpharmmlcpp can be found.])

if (test -n "$PHARMMLCPPLIB" && test -z "$PHARMMLCPPINC") || \
    (test -n "$PHARMMLCPPINC" && test -z "$PHARMMLCPPLIB"); then
    AC_MSG_ERROR([Specify both libpharmmlcpp library and include paths, or neither.])
fi

if test -z "$PHARMMLCPPLIB"; then
    if test -d "$srcdir/../libpharmmlcpp"; then
        PHARMMLCPPLIB='$(top_builddir)/../libpharmmlcpp/.libs'
        PHARMMLCPPINC='$(top_srcdir)/../libpharmmlcpp'
    fi
fi

#if test -z "$PHARMMLCPPLIB"; then
    #Check for installed lib and includes here.
#fi

if test -n "$PHARMMLCPPLIB"; then
    AC_SUBST([PHARMMLCPP_LDADD], ["-lpharmmlcpp -L$PHARMMLCPPLIB"])
    AC_SUBST([PHARMMLCPP_INCLUDE], ["-I$PHARMMLCPPINC"])
fi


# Detect the target system
AC_CANONICAL_HOST

build_linux=no
build_windows=no
build_mac=no

case "${host_os}" in
    linux*)
        AC_SUBST([AM_CPPFLAGS], ['$(LIBXML2_CFLAGS) -std=c++14'])
        build_linux=yes
        ;;
    cygwin*|mingw*)
        AC_SUBST([AM_CPPFLAGS], ['-std=c++14 -static-libgcc -static-libstdc++ -L../windep/lib -I../windep/include'])
        AC_SUBST([AM_LDFLAGS], ['-static-libgcc -static-libstdc++ -L../windep/lib'])
        build_windows=yes
        ;;
    darwin*)
        build_mac=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
