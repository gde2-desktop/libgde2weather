/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* gde2weather-timezone.c - Timezone handling
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

#ifndef __GDE2WEATHER_TIMEZONE_H__
#define __GDE2WEATHER_TIMEZONE_H__

#ifndef GDE2WEATHER_I_KNOW_THIS_IS_UNSTABLE
#error "libgde2weather should only be used if you understand that it's subject to change, and is not supported as a fixed API/ABI or as part of the platform"
#endif

#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Gde2WeatherTimezone Gde2WeatherTimezone;

GType gde2weather_timezone_get_type (void);
#define GDE2WEATHER_TYPE_TIMEZONE (gde2weather_timezone_get_type ())

const char       *gde2weather_timezone_get_name       (Gde2WeatherTimezone *zone);
const char       *gde2weather_timezone_get_tzid       (Gde2WeatherTimezone *zone);
int               gde2weather_timezone_get_offset     (Gde2WeatherTimezone *zone);
gboolean          gde2weather_timezone_has_dst        (Gde2WeatherTimezone *zone);
int               gde2weather_timezone_get_dst_offset (Gde2WeatherTimezone *zone);

Gde2WeatherTimezone *gde2weather_timezone_ref            (Gde2WeatherTimezone *zone);
void              gde2weather_timezone_unref          (Gde2WeatherTimezone *zone);

Gde2WeatherTimezone *gde2weather_timezone_get_utc        (void);

#ifdef __cplusplus
}
#endif

#endif /* __GDE2WEATHER_TIMEZONE_H__ */
