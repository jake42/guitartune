dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(guitartune, 0.36)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

AM_PROG_LIBTOOL

PKG_CHECK_MODULES(GUITARTUNE, [gtk+-2.0 >= 2.8 libglade-2.0 >= 2.6.0 fftw3 alsa dbus-1 gthread-2.0 dbus-glib-1])

AC_OUTPUT([
Makefile
src/Makefile
])
