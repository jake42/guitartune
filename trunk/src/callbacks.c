/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.c
 * Copyright (C) cchandel 2009 <cchandel@yahoo.com>
 * 
 * callbacks.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * callbacks.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"

// indicate whether the freq detected is lower / higher than
// the nearest note
void 
set_incr(int i)
{
	gtk_widget_modify_bg (label_0, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_bg (label_p, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_bg (label_pp, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_bg (label_m, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_bg (label_mm, GTK_STATE_NORMAL, NULL);
	
	switch (i) {
		case 0 : gdk_color_parse ("blue", &color);
			gtk_widget_modify_bg (label_pp, GTK_STATE_NORMAL, &color);
			return;
		case 1 : gdk_color_parse ("blue", &color);
		gtk_widget_modify_bg (label_p, GTK_STATE_NORMAL, &color);
			return;
		case 2 : gdk_color_parse ("green", &color);
		gtk_widget_modify_bg (label_0, GTK_STATE_NORMAL, &color);
			return;
		case 3 : gdk_color_parse ("red", &color);
		gtk_widget_modify_bg (label_m, GTK_STATE_NORMAL, &color);
			return;
		case 4 : gdk_color_parse ("red", &color);
		gtk_widget_modify_bg (label_mm, GTK_STATE_NORMAL, &color);
			return;
		default : return;		
	}
}

// play a note of appropriate freq when the appropriate button is pressed
// pause freq detection at that time

	// generate a sine wave
	/*short wave[N];
	//short sin = 0;
	wave[0] = 0;
	short cos = 5000;
	int i;
	float frq; 

	for (i=1; i<N-1; i++) {
		freq = 2.0 * (M_PI / i);
		wave[i] = wave[i-1] + (short) (cos * frq);
		printf ("%d ",wave[i]);
	}*/

