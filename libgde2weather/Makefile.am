lib_LTLIBRARIES = libgde2weather.la

noinst_PROGRAMS = test_metar test_locations test_sun_moon

AM_CPPFLAGS = 			\
	$(GTK_CFLAGS)		\
	-I$(top_srcdir)		\
	-I$(srcdir)

AM_CFLAGS = $(WARN_CFLAGS)

gde2weather_old_headers = \
	weather.h gde2weather-prefs.h gde2weather-xml.h
gde2weather_new_headers = \
	gde2weather-location.h location-entry.h \
	gde2weather-timezone.h timezone-menu.h
libgde2weatherincdir = $(includedir)/libgde2weather
libgde2weatherinc_HEADERS = \
	$(gde2weather_old_headers)		\
	$(gde2weather_new_headers)		\
	gde2weather-enum-types.h

noinst_HEADERS = weather-priv.h

libgde2weather_la_SOURCES = \
	weather.c weather.h weather-priv.h \
	weather-metar.c weather-iwin.c weather-met.c \
	weather-bom.c weather-wx.c \
	weather-sun.c weather-moon.c \
	gde2weather-enum-types.c \
	gde2weather-prefs.c gde2weather-prefs.h \
	gde2weather-xml.c gde2weather-xml.h \
	gde2weather-location.c gde2weather-location.h \
	gde2weather-timezone.c gde2weather-timezone.h \
	location-entry.c location-entry.h \
	timezone-menu.c timezone-menu.h \
	parser.c parser.h

libgde2weather_la_CPPFLAGS = \
	$(AM_CPPFLAGS)			\
	$(LIBXML_CFLAGS)		\
	$(LIBSOUP_CFLAGS)		\
	$(GIO_CFLAGS)			\
	-DG_LOG_DOMAIN=\"Gde2Weather\"	\
	-DGDE2LOCALEDIR=\""$(datadir)/locale"\" \
	-DGDE2WEATHER_XML_LOCATION_DIR=\""$(pkgdatadir)"\"

libgde2weather_la_LIBADD = \
	-lm		\
	$(GTK_LIBS)	\
	$(LIBXML_LIBS)	\
	$(LIBSOUP_LIBS)	\
	$(GIO_LIBS) \
	$(REGEX_LIBS)

libgde2weather_la_LDFLAGS = \
	-version-info $(LT_VERSION) -no-undefined

test_metar_SOURCES = test_metar.c
test_metar_CPPFLAGS = $(AM_CPPFLAGS) $(LIBSOUP_CFLAGS)
test_metar_LDADD = libgde2weather.la $(GTK_LIBS)

test_locations_SOURCES = test_locations.c
test_locations_LDADD = libgde2weather.la $(GTK_LIBS)

test_sun_moon_SOURCES = test_sun_moon.c
test_sun_moon_CPPFLAGS = $(AM_CPPFLAGS) $(LIBSOUP_CFLAGS)
test_sun_moon_LDADD = libgde2weather.la $(GTK_LIBS) $(LIBM)

gde2weather-enum-types.h: $(gde2weather_new_headers)
	$(AM_V_GEN)( cd $(srcdir) && $(GLIB_MKENUMS) --template gde2weather-enum-types.h.tmpl \
		$(gde2weather_new_headers) ) > gde2weather-enum-types.h.tmp \
	&& mv gde2weather-enum-types.h.tmp gde2weather-enum-types.h \
	|| rm -f gde2weather-enum-type.h.tmp

gde2weather-enum-types.c: $(libgde2weatherinclude_HEADERS)
	$(AM_V_GEN)( cd $(srcdir) && $(GLIB_MKENUMS) --template gde2weather-enum-types.c.tmpl \
		$(gde2weather_new_headers) ) > gde2weather-enum-types.c.tmp \
	&& mv gde2weather-enum-types.c.tmp gde2weather-enum-types.c \
	|| rm -f gde2weather-enum-type.c.tmp


BUILT_SOURCES = gde2weather-enum-types.c gde2weather-enum-types.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gde2weather.pc

@INTLTOOL_XML_NOMERGE_RULE@
gsettingsschema_in_files = org.gde2.weather.gschema.xml.in
gsettings_SCHEMAS = $(gsettingsschema_in_files:.xml.in=.xml)
@GSETTINGS_RULES@

EXTRA_DIST = gde2weather.pc.in gde2weather-uninstalled.pc.in $(gsettingsschema_in_files)

CLEANFILES = $(noinst_PROGRAMS) $(BUILT_SOURCES) $(gsettings_SCHEMAS) *.gschema.valid

-include $(top_srcdir)/git.mk
