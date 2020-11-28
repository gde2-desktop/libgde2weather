/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* gde2weather-location.h - Location-handling code
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

#ifndef __GDE2WEATHER_LOCATIONS_H__
#define __GDE2WEATHER_LOCATIONS_H__

#ifndef GDE2WEATHER_I_KNOW_THIS_IS_UNSTABLE
#error "libgde2weather should only be used if you understand that it's subject to change, and is not supported as a fixed API/ABI or as part of the platform"
#endif

#include <glib.h>
#include <libgde2weather/gde2weather-timezone.h>
#include <libgde2weather/weather.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Gde2WeatherLocation Gde2WeatherLocation;

typedef enum { /*< underscore_name=gde2weather_location_level >*/
    GDE2WEATHER_LOCATION_WORLD,
    GDE2WEATHER_LOCATION_REGION,
    GDE2WEATHER_LOCATION_COUNTRY,
    /* ADM1 = first-order administrative division = state/province, etc */
    GDE2WEATHER_LOCATION_ADM1,
    /* ADM2 = second-order division = county, etc */
    GDE2WEATHER_LOCATION_ADM2,
    GDE2WEATHER_LOCATION_CITY,
    GDE2WEATHER_LOCATION_WEATHER_STATION
} Gde2WeatherLocationLevel;

GType gde2weather_location_get_type (void);
#define GDE2WEATHER_TYPE_LOCATION (gde2weather_location_get_type ())

Gde2WeatherLocation      *gde2weather_location_new_world      (gboolean           use_regions);
Gde2WeatherLocation      *gde2weather_location_ref            (Gde2WeatherLocation  *loc);
void                   gde2weather_location_unref          (Gde2WeatherLocation  *loc);

const char            *gde2weather_location_get_name       (Gde2WeatherLocation  *loc);
const char            *gde2weather_location_get_sort_name  (Gde2WeatherLocation  *loc);
Gde2WeatherLocationLevel  gde2weather_location_get_level      (Gde2WeatherLocation  *loc);
Gde2WeatherLocation      *gde2weather_location_get_parent     (Gde2WeatherLocation  *loc);

Gde2WeatherLocation     **gde2weather_location_get_children   (Gde2WeatherLocation  *loc);
void                   gde2weather_location_free_children  (Gde2WeatherLocation  *loc,
							 Gde2WeatherLocation **children);

gboolean               gde2weather_location_has_coords     (Gde2WeatherLocation  *loc);
void                   gde2weather_location_get_coords     (Gde2WeatherLocation  *loc,
							 double            *latitude,
							 double            *longitude);
double                 gde2weather_location_get_distance   (Gde2WeatherLocation  *loc,
							 Gde2WeatherLocation  *loc2);

const char            *gde2weather_location_get_country    (Gde2WeatherLocation  *loc);

Gde2WeatherTimezone      *gde2weather_location_get_timezone   (Gde2WeatherLocation  *loc);
Gde2WeatherTimezone     **gde2weather_location_get_timezones  (Gde2WeatherLocation  *loc);
void                   gde2weather_location_free_timezones (Gde2WeatherLocation  *loc,
							 Gde2WeatherTimezone **zones);

const char            *gde2weather_location_get_code       (Gde2WeatherLocation  *loc);
char                  *gde2weather_location_get_city_name  (Gde2WeatherLocation  *loc);

WeatherInfo           *gde2weather_location_get_weather    (Gde2WeatherLocation  *loc);

#ifdef __cplusplus
}
#endif

#endif /* __GDE2WEATHER_LOCATIONS_H__ */
