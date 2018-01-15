## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = pgm_manipulator_gui.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(PGM_MANIPULATOR_GUI_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = pgm_manipulator_gui

pgm_manipulator_gui_SOURCES = \
	main.c \
	graphic.c \
	graphic.h


pgm_manipulator_gui_LDFLAGS = \
	-Wl,--export-dynamic \
	-lm

pgm_manipulator_gui_LDADD = $(PGM_MANIPULATOR_GUI_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

