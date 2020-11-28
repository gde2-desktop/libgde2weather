/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* timezone-menu.c - Timezone-selecting menu
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define GDE2WEATHER_I_KNOW_THIS_IS_UNSTABLE
#include "timezone-menu.h"
#include "weather-priv.h"

#include <string.h>

/**
 * SECTION:timezone-menu
 * @Title: Gde2WeatherTimezoneMenu
 *
 * A #GtkComboBox subclass for choosing a #Gde2WeatherTimezone
 */

G_DEFINE_TYPE (Gde2WeatherTimezoneMenu, gde2weather_timezone_menu, GTK_TYPE_COMBO_BOX)

enum {
    PROP_0,

    PROP_TOP,
    PROP_TZID,

    LAST_PROP
};

static void set_property (GObject *object, guint prop_id,
			  const GValue *value, GParamSpec *pspec);
static void get_property (GObject *object, guint prop_id,
			  GValue *value, GParamSpec *pspec);

static void changed      (GtkComboBox *combo);

static GtkTreeModel *gde2weather_timezone_model_new (Gde2WeatherLocation *top);
static gboolean row_separator_func (GtkTreeModel *model, GtkTreeIter *iter,
				    gpointer data);
static void is_sensitive (GtkCellLayout *cell_layout, GtkCellRenderer *cell,
			  GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer data);

static void
gde2weather_timezone_menu_init (Gde2WeatherTimezoneMenu *menu)
{
    GtkCellRenderer *renderer;

    gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (menu),
					  row_separator_func, NULL, NULL);

    renderer = gtk_cell_renderer_text_new ();
    gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (menu), renderer, TRUE);
    gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (menu), renderer,
				    "markup", 0,
				    NULL);
    gtk_cell_layout_set_cell_data_func (GTK_CELL_LAYOUT (menu),
					renderer, is_sensitive, NULL, NULL);
}

static void
finalize (GObject *object)
{
    Gde2WeatherTimezoneMenu *menu = GDE2WEATHER_TIMEZONE_MENU (object);

    if (menu->zone)
	gde2weather_timezone_unref (menu->zone);

    G_OBJECT_CLASS (gde2weather_timezone_menu_parent_class)->finalize (object);
}

static void
gde2weather_timezone_menu_class_init (Gde2WeatherTimezoneMenuClass *timezone_menu_class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (timezone_menu_class);
    GtkComboBoxClass *combo_class = GTK_COMBO_BOX_CLASS (timezone_menu_class);

    object_class->finalize = finalize;
    object_class->set_property = set_property;
    object_class->get_property = get_property;

    combo_class->changed = changed;

    /* properties */
    g_object_class_install_property (
	object_class, PROP_TOP,
	g_param_spec_pointer ("top",
			      "Top Location",
			      "The Gde2WeatherLocation whose children will be used to fill in the menu",
			      G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property (
	object_class, PROP_TZID,
	g_param_spec_string ("tzid",
			     "TZID",
			     "The selected TZID",
			     NULL,
			     G_PARAM_READWRITE));
}

static void
set_property (GObject *object, guint prop_id,
	      const GValue *value, GParamSpec *pspec)
{
    GtkTreeModel *model;

    switch (prop_id) {
    case PROP_TOP:
	model = gde2weather_timezone_model_new (g_value_get_pointer (value));
	gtk_combo_box_set_model (GTK_COMBO_BOX (object), model);
	g_object_unref (model);
	gtk_combo_box_set_active (GTK_COMBO_BOX (object), 0);
	break;

    case PROP_TZID:
	gde2weather_timezone_menu_set_tzid (GDE2WEATHER_TIMEZONE_MENU (object),
					 g_value_get_string (value));
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	break;
    }
}

static void
get_property (GObject *object, guint prop_id,
	      GValue *value, GParamSpec *pspec)
{
    Gde2WeatherTimezoneMenu *menu = GDE2WEATHER_TIMEZONE_MENU (object);

    switch (prop_id) {
    case PROP_TZID:
	g_value_set_string (value, gde2weather_timezone_menu_get_tzid (menu));
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	break;
    }
}

enum {
    GDE2WEATHER_TIMEZONE_MENU_NAME,
    GDE2WEATHER_TIMEZONE_MENU_ZONE
};

static void
changed (GtkComboBox *combo)
{
    Gde2WeatherTimezoneMenu *menu = GDE2WEATHER_TIMEZONE_MENU (combo);
    GtkTreeIter iter;

    if (menu->zone)
	gde2weather_timezone_unref (menu->zone);

    gtk_combo_box_get_active_iter (combo, &iter);
    gtk_tree_model_get (gtk_combo_box_get_model (combo), &iter,
			GDE2WEATHER_TIMEZONE_MENU_ZONE, &menu->zone,
			-1);

    if (menu->zone)
	gde2weather_timezone_ref (menu->zone);

    g_object_notify (G_OBJECT (combo), "tzid");
}

static void
append_offset (GString *desc, int offset)
{
    int hours, minutes;

    hours = offset / 60;
    minutes = (offset > 0) ? offset % 60 : -offset % 60;

    if (minutes)
	g_string_append_printf (desc, "GMT%+d:%02d", hours, minutes);
    else if (hours)
	g_string_append_printf (desc, "GMT%+d", hours);
    else
	g_string_append (desc, "GMT");
}

static char *
get_offset (Gde2WeatherTimezone *zone)
{
    GString *desc;

    desc = g_string_new (NULL);
    append_offset (desc, gde2weather_timezone_get_offset (zone));
    if (gde2weather_timezone_has_dst (zone)) {
	g_string_append (desc, " / ");
	append_offset (desc, gde2weather_timezone_get_dst_offset (zone));
    }
    return g_string_free (desc, FALSE);
}

static void
insert_location (GtkTreeStore *store, Gde2WeatherTimezone *zone, const char *loc_name, GtkTreeIter *parent)
{
    GtkTreeIter iter;
    char *name, *offset;

    offset = get_offset (zone);
    name = g_strdup_printf ("%s <small>(%s)</small>",
                            loc_name ? loc_name : gde2weather_timezone_get_name (zone),
                            offset);
    gtk_tree_store_append (store, &iter, parent);
    gtk_tree_store_set (store, &iter,
                        GDE2WEATHER_TIMEZONE_MENU_NAME, name,
                        GDE2WEATHER_TIMEZONE_MENU_ZONE, gde2weather_timezone_ref (zone),
                        -1);
    g_free (name);
    g_free (offset);
}

static void
insert_locations (GtkTreeStore *store, Gde2WeatherLocation *loc)
{
    int i;

    if (gde2weather_location_get_level (loc) < GDE2WEATHER_LOCATION_COUNTRY) {
	Gde2WeatherLocation **children;

	children = gde2weather_location_get_children (loc);
	for (i = 0; children[i]; i++)
	    insert_locations (store, children[i]);
	gde2weather_location_free_children (loc, children);
    } else {
	Gde2WeatherTimezone **zones;
	GtkTreeIter iter;

	zones = gde2weather_location_get_timezones (loc);
	if (zones[1]) {
	    gtk_tree_store_append (store, &iter, NULL);
	    gtk_tree_store_set (store, &iter,
				GDE2WEATHER_TIMEZONE_MENU_NAME, gde2weather_location_get_name (loc),
				-1);

	    for (i = 0; zones[i]; i++) {
                insert_location (store, zones[i], NULL, &iter);
	    }
	} else if (zones[0]) {
            insert_location (store, zones[0], gde2weather_location_get_name (loc), NULL);
	}

	gde2weather_location_free_timezones (loc, zones);
    }
}

static GtkTreeModel *
gde2weather_timezone_model_new (Gde2WeatherLocation *top)
{
    GtkTreeStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *unknown;
    Gde2WeatherTimezone *utc;

    store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
    model = GTK_TREE_MODEL (store);

    unknown = g_markup_printf_escaped ("<i>%s</i>", C_("timezone", "Unknown"));

    gtk_tree_store_append (store, &iter, NULL);
    gtk_tree_store_set (store, &iter,
			GDE2WEATHER_TIMEZONE_MENU_NAME, unknown,
			GDE2WEATHER_TIMEZONE_MENU_ZONE, NULL,
			-1);

    utc = gde2weather_timezone_get_utc ();
    if (utc) {
        insert_location (store, utc, NULL, NULL);
        gde2weather_timezone_unref (utc);
    }

    gtk_tree_store_append (store, &iter, NULL);

    g_free (unknown);

    insert_locations (store, top);

    return model;
}

static gboolean
row_separator_func (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
    char *name;

    gtk_tree_model_get (model, iter,
			GDE2WEATHER_TIMEZONE_MENU_NAME, &name,
			-1);
    if (name) {
	g_free (name);
	return FALSE;
    } else
	return TRUE;
}

static void
is_sensitive (GtkCellLayout *cell_layout, GtkCellRenderer *cell,
	      GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer data)
{
    gboolean sensitive;

    sensitive = !gtk_tree_model_iter_has_child (tree_model, iter);
    g_object_set (cell, "sensitive", sensitive, NULL);
}

/**
 * gde2weather_timezone_menu_new:
 * @top: the top-level location for the menu.
 *
 * Creates a new #Gde2WeatherTimezoneMenu.
 *
 * @top will normally be a location returned from
 * gde2weather_location_new_world(), but you can create a menu that
 * contains the timezones from a smaller set of locations if you want.
 *
 * Return value: the new #Gde2WeatherTimezoneMenu
 **/
GtkWidget *
gde2weather_timezone_menu_new (Gde2WeatherLocation *top)
{
    return g_object_new (GDE2WEATHER_TYPE_TIMEZONE_MENU,
			 "top", top,
			 NULL);
}

typedef struct {
    GtkComboBox *combo;
    const char  *tzid;
} SetTimezoneData;

static gboolean
check_tzid (GtkTreeModel *model, GtkTreePath *path,
	    GtkTreeIter *iter, gpointer data)
{
    SetTimezoneData *tzd = data;
    Gde2WeatherTimezone *zone;

    gtk_tree_model_get (model, iter,
			GDE2WEATHER_TIMEZONE_MENU_ZONE, &zone,
			-1);
    if (!zone)
	return FALSE;

    if (!strcmp (gde2weather_timezone_get_tzid (zone), tzd->tzid)) {
	gtk_combo_box_set_active_iter (tzd->combo, iter);
	return TRUE;
    } else
	return FALSE;
}

/**
 * gde2weather_timezone_menu_set_tzid:
 * @menu: a #Gde2WeatherTimezoneMenu
 * @tzid: (allow-none): a tzdata id (eg, "America/New_York")
 *
 * Sets @menu to the given @tzid. If @tzid is %NULL, sets @menu to
 * "Unknown".
 **/
void
gde2weather_timezone_menu_set_tzid (Gde2WeatherTimezoneMenu *menu,
				 const char           *tzid)
{
    SetTimezoneData tzd;

    g_return_if_fail (GDE2WEATHER_IS_TIMEZONE_MENU (menu));

    if (!tzid) {
	gtk_combo_box_set_active (GTK_COMBO_BOX (menu), 0);
	return;
    }

    tzd.combo = GTK_COMBO_BOX (menu);
    tzd.tzid = tzid;
    gtk_tree_model_foreach (gtk_combo_box_get_model (tzd.combo),
			    check_tzid, &tzd);
}

/**
 * gde2weather_timezone_menu_get_tzid:
 * @menu: a #Gde2WeatherTimezoneMenu
 *
 * Gets @menu's timezone id.
 *
 * Return value: (allow-none): @menu's tzid, or %NULL if no timezone
 * is selected.
 **/
const char *
gde2weather_timezone_menu_get_tzid (Gde2WeatherTimezoneMenu *menu)
{
    g_return_val_if_fail (GDE2WEATHER_IS_TIMEZONE_MENU (menu), NULL);

    if (!menu->zone)
	return NULL;
    return gde2weather_timezone_get_tzid (menu->zone);
}

