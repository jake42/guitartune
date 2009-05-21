/***************************************************************************
 *            dbus.c
 *
 *  Mon Feb  9 21:55:19 2009
 *  Copyright  2009  cchandel
 *  <cchandel@yahoo.com>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#include "dbus.h"
#include <glib.h>
#include <dbus/dbus-glib.h>

int occupy_display_and_cpu(void) 
{
	DBusGConnection *connection;
  	GError *error;
  	DBusGProxy *proxy;
	DBusGProxyCall* call;
	char *resource = "CPU";

  	g_type_init ();

  	error = NULL;
  	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
  	if (connection == NULL) {
      g_printerr ("Failed to open connection to bus: %s\n",
                  error->message);
      g_error_free (error);
      exit (1);
    }

	// Create a proxy object 
    proxy = dbus_g_proxy_new_for_name (connection,
                                     "org.freesmartphone.ousaged",
                                     "/org/freesmartphone/Usage",
                                     "org.freesmartphone.Usage");
	call = dbus_g_proxy_begin_call (proxy, "RequestResource", NULL, NULL, NULL, G_TYPE_STRING, resource, G_TYPE_INVALID);
	dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
	if (error != NULL) {
		printf("dbus error: %s\n", error->message);
	}
	
	resource = "Display";
	call = dbus_g_proxy_begin_call (proxy, "RequestResource", NULL, NULL, NULL, G_TYPE_STRING, resource, G_TYPE_INVALID);
	dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
	if (error != NULL) {
		printf("dbus error: %s\n", error->message);
	}

  	g_object_unref (proxy);
  	return 0;

}

int release_display_and_cpu(void) 
{
	DBusGConnection *connection;
  	GError *error;
  	DBusGProxy *proxy;
	DBusGProxyCall* call;
	char *resource = "CPU";

  	g_type_init ();

  	error = NULL;
  	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
  	if (connection == NULL) {
      g_printerr ("Failed to open connection to bus: %s\n",
                  error->message);
      g_error_free (error);
      exit (1);
    }

	// Create a proxy object
    proxy = dbus_g_proxy_new_for_name (connection,
                                     "org.freesmartphone.ousaged",
                                     "/org/freesmartphone/Usage",
                                     "org.freesmartphone.Usage");
	call = dbus_g_proxy_begin_call (proxy, "ReleaseResource", NULL, NULL, NULL, G_TYPE_STRING, resource, G_TYPE_INVALID);
	dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
	if (error != NULL) {
		printf("dbus error: %s\n", error->message);
	}
	
	resource = "Display";
	call = dbus_g_proxy_begin_call (proxy, "ReleaseResource", NULL, NULL, NULL, G_TYPE_STRING, resource, G_TYPE_INVALID);
	dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
	if (error != NULL) {
		printf("dbus error: %s\n", error->message);
	}

  	g_object_unref (proxy);
  	return 0;

}

int check_in_call(void)
{
	DBusGConnection *connection;
  	GError *error;
  	DBusGProxy *proxy;
	DBusGProxyCall* call;

  	g_type_init ();

  	error = NULL;
  	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
  	if (connection == NULL) {
      g_printerr ("Failed to open connection to bus: %s\n",
                  error->message);
      g_error_free (error);
      exit (1);
    }

	// Create a proxy object
    proxy = dbus_g_proxy_new_for_name (connection,
                                     "org.freesmartphone.ophoned",
                                     "/org/freesmartphone/Phone/Call",
                                     "org.freesmartphone.Phone.Call");
	call = dbus_g_proxy_begin_call (proxy, "ReleaseResource", NULL, NULL, NULL, G_TYPE_INVALID);
	dbus_g_proxy_end_call (proxy, call, &error, G_TYPE_INVALID);
	if (error != NULL) {
		printf("dbus error: %s\n", error->message);
	}
	
}

int listen_incoming(void) 
{
	
}
