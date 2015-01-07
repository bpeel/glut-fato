AC_INIT([glut-fato], 0.1)

AC_CONFIG_SRCDIR([src/fato-triangle.c])
AM_CONFIG_HEADER([config.h])
AC_GNU_SOURCE

AC_PROG_CC

AM_INIT_AUTOMAKE([1.9 foreign no-dist-gzip dist-xz tar-ustar])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

FATO_EXTRA_CFLAGS=""
FATO_EXTRA_LIBS=""

dnl     ============================================================
dnl     Enable strict compiler flags
dnl     ============================================================

# use strict compiler flags only when building from git; the rules for
# distcheck will take care of turning this on when making a release
m4_define([maintainer_default], [yes])
AC_ARG_ENABLE(
  [maintainer-flags],
  [AC_HELP_STRING([--enable-maintainer-flags=@<:@no/yes/error@:>@],
                  [Use strict compiler flags @<:@default=]maintainer_default[@:>@])],
  [],
  enable_maintainer_flags=maintainer_default
)

MAINTAINER_COMPILER_FLAGS="-Wall -Wcast-align -Wuninitialized
                           -Wno-strict-aliasing -Wempty-body -Wformat
                           -Wformat-security -Winit-self -Wundef
                           -Wdeclaration-after-statement
                           -Wpointer-arith -Wmissing-declarations"

AS_CASE(
  [$enable_maintainer_flags],
  [yes],
  [
    AS_COMPILER_FLAGS([MAINTAINER_CFLAGS], [$MAINTAINER_COMPILER_FLAGS])
  ],
  [no],
  [
  ],
  [error],
  [
    MAINTAINER_COMPILER_FLAGS="$MAINTAINER_COMPILER_FLAGS -Werror"
    AS_COMPILER_FLAGS([MAINTAINER_CFLAGS], [$MAINTAINER_COMPILER_FLAGS])
  ],
  [*],
  [AC_MSG_ERROR([Invalid option for --enable-maintainer-flags])]
)

# strip leading spaces
FATO_EXTRA_CFLAGS="$FATO_EXTRA_CFLAGS ${MAINTAINER_CFLAGS#*  }"

dnl Check for freeglut
AC_CHECK_HEADER([GL/freeglut.h],
                [have_freeglut=yes],
                [have_freeglut=no])
AC_CHECK_LIB([glut], [glutInit], [], [have_freeglut=no])
AS_IF([test "x$have_freeglut" != "xyes"],
      [AC_MSG_ERROR([freeglut is required])])
FATO_EXTRA_LIBS="$FATO_EXTRA_LIBS -lfreeglut"

PKG_CHECK_MODULES([GL], [gl])
PKG_CHECK_MODULES([EPOXY], [epoxy])

AC_SUBST(FATO_EXTRA_CFLAGS)
AC_SUBST(FATO_EXTRA_LIBS)

AC_CONFIG_FILES([
        Makefile
        build/Makefile
        src/Makefile
])

AC_OUTPUT
