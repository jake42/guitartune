## Process this file with automake to produce Makefile.in

## Created by Anjuta

gladedir = $(datadir)/guitartune/glade
glade_DATA = guitartune.glade

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GUITARTUNE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = guitartune

guitartune_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c \
	alsa_capture.c \
	alsa_capture.h \
	dsp.c \
	dsp.h \
	dbus.c \
	dbus.h

guitartune_LDFLAGS = \
	-Wl,--export-dynamic

guitartune_LDADD = $(GUITARTUNE_LIBS)

EXTRA_DIST = $(glade_DATA)
