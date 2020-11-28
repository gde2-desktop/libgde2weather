/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* location-entry.h - Location-selecting text entry
 *
 * Copyright 2008, Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef GDE2WEATHER_LOCATION_ENTRY_H
#define GDE2WEATHER_LOCATION_ENTRY_H 1

#include <gtk/gtk.h>
#include <libgde2weather/gde2weather-location.h>

#define GDE2WEATHER_TYPE_LOCATION_ENTRY            (gde2weather_location_entry_get_type ())
#define GDE2WEATHER_LOCATION_ENTRY(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDE2WEATHER_TYPE_LOCATION_ENTRY, Gde2WeatherLocationEntry))
#define GDE2WEATHER_LOCATION_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDE2WEATHER_TYPE_LOCATION_ENTRY, Gde2WeatherLocationEntryClass))
#define GDE2WEATHER_IS_LOCATION_ENTRY(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDE2WEATHER_TYPE_LOCATION_ENTRY))
#define GDE2WEATHER_IS_LOCATION_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDE2WEATHER_TYPE_LOCATION_ENTRY))
#define GDE2WEATHER_LOCATION_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDE2WEATHER_TYPE_LOCATION_ENTRY, Gde2WeatherLocationEntryClass))

typedef struct {
    GtkEntry parent;

    /*< private >*/
    Gde2WeatherLocation *location, *top;
    guint custom_text : 1;
} Gde2WeatherLocationEntry;

typedef struct {
    GtkEntryClass parent_class;

} Gde2WeatherLocationEntryClass;

GType             gde2weather_location_entry_get_type     (void);

GtkWidget        *gde2weather_location_entry_new          (Gde2WeatherLocation      *top);

void              gde2weather_location_entry_set_location (Gde2WeatherLocationEntry *entry,
							Gde2WeatherLocation      *loc);
Gde2WeatherLocation *gde2weather_location_entry_get_location (Gde2WeatherLocationEntry *entry);

gboolean          gde2weather_location_entry_has_custom_text (Gde2WeatherLocationEntry *entry);

gboolean          gde2weather_location_entry_set_city     (Gde2WeatherLocationEntry *entry,
							const char            *city_name,
							const char            *code);

#endif
