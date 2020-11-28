/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* timezone-menu.h - Timezone-selecting menu
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

#ifndef GDE2WEATHER_TIMEZONE_MENU_H
#define GDE2WEATHER_TIMEZONE_MENU_H 1

#include <gtk/gtk.h>
#include <libgde2weather/gde2weather-location.h>

#define GDE2WEATHER_TYPE_TIMEZONE_MENU            (gde2weather_timezone_menu_get_type ())
#define GDE2WEATHER_TIMEZONE_MENU(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDE2WEATHER_TYPE_TIMEZONE_MENU, Gde2WeatherTimezoneMenu))
#define GDE2WEATHER_TIMEZONE_MENU_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDE2WEATHER_TYPE_TIMEZONE_MENU, Gde2WeatherTimezoneMenuClass))
#define GDE2WEATHER_IS_TIMEZONE_MENU(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDE2WEATHER_TYPE_TIMEZONE_MENU))
#define GDE2WEATHER_IS_TIMEZONE_MENU_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDE2WEATHER_TYPE_TIMEZONE_MENU))
#define GDE2WEATHER_TIMEZONE_MENU_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDE2WEATHER_TYPE_TIMEZONE_MENU, Gde2WeatherTimezoneMenuClass))

typedef struct {
    GtkComboBox parent;

    /*< private >*/
    Gde2WeatherTimezone *zone;
} Gde2WeatherTimezoneMenu;

typedef struct {
    GtkComboBoxClass parent_class;

} Gde2WeatherTimezoneMenuClass;

GType       gde2weather_timezone_menu_get_type         (void);

GtkWidget  *gde2weather_timezone_menu_new              (Gde2WeatherLocation     *top);

void        gde2weather_timezone_menu_set_tzid         (Gde2WeatherTimezoneMenu *menu,
						     const char           *tzid);
const char *gde2weather_timezone_menu_get_tzid         (Gde2WeatherTimezoneMenu *menu);

#endif
