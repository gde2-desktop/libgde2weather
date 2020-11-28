if BUILD_PYTHON
AM_CPPFLAGS =			\
	-I$(top_srcdir)		\
	$(PYTHON_INCLUDES)	\
	$(PYGOBJECT_CFLAGS)	\
	$(GTK_CFLAGS)

pyunstabledir = $(pyexecdir)/gde2weather/I_KNOW_THIS_IS_UNSTABLE
pyunstable_LTLIBRARIES = gde2weather.la
gde2weather_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgde2weather
gde2weather_la_LIBADD = ../libgde2weather/libgde2weather.la
gde2weather_la_SOURCES = gde2weathermodule.c
nodist_gde2weather_la_SOURCES = gde2weather.c
gde2weather.c: gde2weather.defs gde2weather.override

.defs.c:
	$(AM_V_GEN)(cd $(srcdir)\
	 && pygobject-codegen-2.0 \
		--register $(PYGTK_DEFS) \
		--override $*.override \
		--prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

install-exec-hook:
	$(MKDIR_P) "$(DESTDIR)$(pyunstabledir)"
	$(INSTALL_DATA) $(srcdir)/bad__init__.py "$(DESTDIR)$(pyexecdir)/gde2weather/__init__.py"
	touch $(DESTDIR)$(pyunstabledir)/__init__.py
	chmod a+x $(DESTDIR)$(pyunstabledir)/__init__.py

endif

EXTRA_DIST = gde2weather.defs gde2weather.override bad__init__.py demo.py

CLEANFILES = gde2weather.c

-include $(top_srcdir)/git.mk
