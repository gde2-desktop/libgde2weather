
#define GDE2WEATHER_I_KNOW_THIS_IS_UNSTABLE
#include <gtk/gtk.h>
#include "location-entry.h"
#include "timezone-menu.h"

static void
deleted (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit ();
}

static void
location_changed (GObject *object, GParamSpec *param, gpointer tzmenu)
{
    Gde2WeatherLocationEntry *entry = GDE2WEATHER_LOCATION_ENTRY (object);
    Gde2WeatherLocation *loc;
    Gde2WeatherTimezone *zone;

    loc = gde2weather_location_entry_get_location (entry);
    g_return_if_fail (loc != NULL);
    zone = gde2weather_location_get_timezone (loc);
    if (zone)
	gde2weather_timezone_menu_set_tzid (tzmenu, gde2weather_timezone_get_tzid (zone));
    else
	gde2weather_timezone_menu_set_tzid (tzmenu, NULL);
    if (zone)
	gde2weather_timezone_unref (zone);
    gde2weather_location_unref (loc);
}

int
main (int argc, char **argv)
{
    Gde2WeatherLocation *loc;
    GtkWidget *window, *vbox, *entry;
    GtkWidget *combo;
    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "location");
    gtk_container_set_border_width (GTK_CONTAINER (window), 8);
    g_signal_connect (window, "delete-event",
		      G_CALLBACK (deleted), NULL);

#if GTK_CHECK_VERSION (3, 0, 0)
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
#else
    vbox = gtk_vbox_new (FALSE, 8);
#endif
    gtk_container_add (GTK_CONTAINER (window), vbox);

    loc = gde2weather_location_new_world (FALSE);
    entry = gde2weather_location_entry_new (loc);
    gtk_widget_set_size_request (entry, 400, -1);
    gtk_box_pack_start (GTK_BOX (vbox), entry, FALSE, TRUE, 0);

    combo = gde2weather_timezone_menu_new (loc);
    gde2weather_location_unref (loc);
    gtk_box_pack_start (GTK_BOX (vbox), combo, FALSE, TRUE, 0);

    g_signal_connect (entry, "notify::location",
		      G_CALLBACK (location_changed), combo);

    gtk_widget_show_all (window);

    gtk_main ();

    return 0;
}
