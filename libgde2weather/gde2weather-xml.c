/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* gde2weather-xml.c - Locations.xml parsing code
 *
 * Copyright (C) 2005 Ryan Lortie, 2004 Gareth Owen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <math.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <libxml/xmlreader.h>

#define GDE2WEATHER_I_KNOW_THIS_IS_UNSTABLE
#include "gde2weather-xml.h"
#include "weather-priv.h"

/**
 * SECTION:gde2weather-xml
 * @Title: gde2weather-xml
 */

static gboolean
gde2weather_xml_parse_node (Gde2WeatherLocation *gloc,
			 GtkTreeStore *store, GtkTreeIter *parent)
{
    GtkTreeIter iter, *self = &iter;
    Gde2WeatherLocation **children, *parent_loc;
    Gde2WeatherLocationLevel level;
    WeatherLocation *wloc;
    const char *name;
    int i;

    name = gde2weather_location_get_name (gloc);
    children = gde2weather_location_get_children (gloc);
    level = gde2weather_location_get_level (gloc);

    if (!children[0] && level < GDE2WEATHER_LOCATION_WEATHER_STATION) {
	gde2weather_location_free_children (gloc, children);
	return TRUE;
    }

    switch (gde2weather_location_get_level (gloc)) {
    case GDE2WEATHER_LOCATION_WORLD:
    case GDE2WEATHER_LOCATION_ADM2:
	self = parent;
	break;

    case GDE2WEATHER_LOCATION_REGION:
    case GDE2WEATHER_LOCATION_COUNTRY:
    case GDE2WEATHER_LOCATION_ADM1:
	/* Create a row with a name but no WeatherLocation */
	gtk_tree_store_append (store, &iter, parent);
	gtk_tree_store_set (store, &iter,
			    GDE2WEATHER_XML_COL_LOC, name,
			    -1);
	break;

    case GDE2WEATHER_LOCATION_CITY:
	/* If multiple children, treat this like a
	 * region/country/adm1. If a single child, merge with that
	 * location.
	 */
	gtk_tree_store_append (store, &iter, parent);
	gtk_tree_store_set (store, &iter,
			    GDE2WEATHER_XML_COL_LOC, name,
			    -1);
	if (children[0] && !children[1]) {
	    wloc = gde2weather_location_to_weather_location (children[0], name);
	    gtk_tree_store_set (store, &iter,
				GDE2WEATHER_XML_COL_POINTER, wloc,
				-1);
	}
	break;

    case GDE2WEATHER_LOCATION_WEATHER_STATION:
	gtk_tree_store_append (store, &iter, parent);
	gtk_tree_store_set (store, &iter,
			    GDE2WEATHER_XML_COL_LOC, name,
			    -1);

	parent_loc = gde2weather_location_get_parent (gloc);
	if (parent_loc && gde2weather_location_get_level (parent_loc) == GDE2WEATHER_LOCATION_CITY)
	    name = gde2weather_location_get_name (parent_loc);
	wloc = gde2weather_location_to_weather_location (gloc, name);
	gtk_tree_store_set (store, &iter,
			    GDE2WEATHER_XML_COL_POINTER, wloc,
			    -1);
	break;
    }

    for (i = 0; children[i]; i++) {
	if (!gde2weather_xml_parse_node (children[i], store, self)) {
	    gde2weather_location_free_children (gloc, children);
	    return FALSE;
	}
    }

    gde2weather_location_free_children (gloc, children);
    return TRUE;
}

GtkTreeModel *
gde2weather_xml_load_locations (void)
{
    Gde2WeatherLocation *world;
    GtkTreeStore *store;

    world = gde2weather_location_new_world (TRUE);
    if (!world)
	return NULL;

    store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);

    if (!gde2weather_xml_parse_node (world, store, NULL)) {
	gde2weather_xml_free_locations ((GtkTreeModel *)store);
	store = NULL;
    }

    gde2weather_location_unref (world);

    return (GtkTreeModel *)store;
}

static gboolean
free_locations (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data)
{
	WeatherLocation *loc = NULL;

	gtk_tree_model_get (model, iter,
			    GDE2WEATHER_XML_COL_POINTER, &loc,
			    -1);

	if (loc) {
		weather_location_free (loc);
		gtk_tree_store_set ((GtkTreeStore *)model, iter,
			    GDE2WEATHER_XML_COL_POINTER, NULL,
			    -1);
	}

	return FALSE;
}

/* Frees model returned from @gde2weather_xml_load_locations. It contains allocated
   WeatherLocation-s, thus this takes care of the freeing of that memory. */
void
gde2weather_xml_free_locations (GtkTreeModel *locations)
{
	if (locations && GTK_IS_TREE_STORE (locations)) {
		gtk_tree_model_foreach (locations, free_locations, NULL);
		g_object_unref (locations);
	}
}
