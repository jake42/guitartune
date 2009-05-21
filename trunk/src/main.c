/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) cchandel 2009 <cchandel@yahoo.com>
 * 
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <config.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <alsa/asoundlib.h>
#include <glib.h>
#include <pthread.h>
#include "callbacks.h"
#include "dsp.h"
#include "alsa_capture.h"

unsigned int rate = 44100;
float mycosine[N];
float max_amp = 0.0;
int new_freq = 0;

/*
 * Standard gettext macros.
 */

#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

/* For testing propose use the local (not installed) glade file */
/* #define GLADE_FILE PACKAGE_DATA_DIR"/guitar/glade/guitartune.glade" */
//#define GLADE_FILE "/usr/share/guitartune/guitartune.glade"
#define GLADE_FILE "guitar_alt.glade"

GtkWidget*
create_window (void)
{
	GtkWidget *window;
	GladeXML *gxml;

	gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	/* This is important */
	glade_xml_signal_autoconnect (gxml);
	window = glade_xml_get_widget (gxml, "win");
	n_note = glade_xml_get_widget(gxml, "n_note");
	det_freq = glade_xml_get_widget(gxml, "det_freq");
	tune = glade_xml_get_widget(gxml, "tune");
	
	gtk_widget_modify_font (n_note, pango_font_description_from_string("50"));
	gtk_widget_modify_font (det_freq, pango_font_description_from_string("50"));
	gtk_widget_modify_font (tune, pango_font_description_from_string("50"));
	
	label_0 = glade_xml_get_widget(gxml, "label_0"); 
	label_p = glade_xml_get_widget(gxml, "label_p");
	label_pp = glade_xml_get_widget(gxml, "label_pp");
	label_m = glade_xml_get_widget(gxml, "label_m");
	label_mm = glade_xml_get_widget(gxml, "label_mm");
		
	return window;
}

// (mainly) display the detected freq, nearest note and whether
// this is lower / higher than the nearest note
void display_freq(void)
{
	int i=0; char tmpstr[30]; float f,diff = 1.0, diff1,diff2;
    struct {
      char *label;
      float freq;
    } notes[] =      
    {
		{"A 0", 27.50},{"B 0", 29.13},
		{"C 1", 30.87},{"C# 1", 34.65},{"D 1", 36.70},
		{"D# 1", 38.89},{"E 1", 41.20},{"F 1", 43.65},
		{"F# 1", 46.25},{"G 1", 49.00},{"G# 1", 51.91},
		{"A 1", 55.00},{"A# 1", 58.27},{"B 1", 61.74},
		{"C 2", 65.41},{"C# 2", 69.30},{"D 2", 73.41},
		{"D# 2", 77.78},{"E 2", 82.41},{"F 2", 87.31},
		{"F# 2", 92.50},{"G 2", 98.00},{"G# 2", 103.83},
		{"A 2", 110.00 },{"A# 2", 116.54},{"B 2", 123.47},
		{"C 3", 130.81},{"C# 3", 138.59},{"D 3", 146.83},
		{"D# 3", 155.56},{"E 3", 164.81},{"F 3", 174.61},
		{"F# 3", 185.00},{"G 3", 195.00},{"G# 3", 207.65},
		{"A 3", 220.00},{"A# 3", 233.08},{"B 3", 246.94},
		{"C 4", 261.63},{"C# 4", 277.18},{"D 4", 293.66},
		{"D# 4", 311.12},{"E 4", 329.63},{"F 4", 349.23},
		{"F# 4", 370.00},{"G 4", 392.00},{"G# 4", 415.30},
		{"A 4", 440.00},{"A# 4", 466.16},{"B 4", 493.88},
		{"C 5", 523.25},{"C# 5", 554.36},{"D 5", 587.33},
		{"D# 5", 622.25},{"E 5", 659.26},{"F 5", 698.46},
		{"F# 5", 740.00},{"G 5", 784.00},{"G# 5", 830.61},
		{"A 5", 880.00},{"A# 5", 932.33},{"B 5", 987.77},
		{"C 6", 1046.50},{"C# 6", 1108.73},{"D 6", 1174.66},
		{"D# 6", 1244.51},{"E 6", 1318.51},{"F 6", 1397.00},
		{"F# 6", 1480.00},{"G 6", 1568.00},{"G# 6", 1661.22},
		{"A 6", 1760.00},{"A# 6", 1864.66},{"B 6", 1975.53},
		{"C 7", 2093},{"C# 7", 2217.46},{"D 7", 2349.32},
		{"D# 7", 2489.00},{"E 7", 2637.00},{"F 7", 2793.83},
		{"F# 7", 2960.00},{"G 7", 3136.00},{"G# 7", 3322.44},
		{"A 7", 3520.00},{"A# 7", 3729.31},{"B 7", 3951.00},
		{"C 8", 4186.00}
    };

  f = max_amp * rate/N;
  sprintf(tmpstr, "%.2f",f);
  gtk_label_set_text( GTK_LABEL(det_freq), tmpstr);

	
	if (f < 25.00 || f > 4200) {
		strcpy (tmpstr,"~");
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);
		strcpy (tmpstr,"Tune ~");
		gtk_label_set_text( GTK_LABEL(tune), tmpstr);
		return;

	}

	for (i = 0; i < 87; i++) {
		diff = f - notes[i].freq;
		if (diff < 0) break;
	}

	// if the freq is less than A0
	if (i == 0 && diff < 0) {
		strcpy (tmpstr,"A0");
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);
		strcpy (tmpstr,"Tune\nUp");
		gtk_label_set_text( GTK_LABEL(tune), tmpstr);
		set_incr(3);
		return;
	}
	// if freq is higer than C8	
	if (i == 87 && diff > 0) { i=86;
		strcpy (tmpstr,"C 8");
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);
		strcpy (tmpstr,"Tune\nDown");
		gtk_label_set_text( GTK_LABEL(tune), tmpstr);
		set_incr(1);
		return;
	}
  diff2 = diff * -1;
  diff1 = f - notes[i-1].freq;
	
	// if tuned to any note 
	if (diff1 == 0) {
		strcpy (tmpstr,notes[i-1].label);
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);	  
		set_incr(2);
	}
	// if freq is higher than the nearest note
	else if (diff1 < diff2) {
		strcpy (tmpstr,notes[i-1].label);
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);
		strcpy (tmpstr,"Tune\nDown");
		gtk_label_set_text( GTK_LABEL(tune), tmpstr);
		set_incr(1);
	 }
	// if freq is lower than nearest note
	else {
		strcpy (tmpstr,notes[i].label);
		gtk_label_set_text( GTK_LABEL(n_note), tmpstr);
		strcpy (tmpstr,"Tune\nUp");
		gtk_label_set_text( GTK_LABEL(tune), tmpstr);
		set_incr(3);
	}
}

static void quit( GtkWidget *widget,
                     gpointer   data )
{
	int err=0;
    err = system("alsactl -f /usr/share/openmoko/scenarios/stereoout.state restore");
    gtk_main_quit ();
}

int check_freq_det(void)
{
	if (new_freq) {
		new_freq = 0;
		display_freq();
	}
	return 1;
}

int
main (int argc, char *argv[])
{
	g_thread_init(NULL);
	gdk_threads_init();
 	GtkWidget *window;
	int i=0, err=0;
	GdkPixbuf *pixbuf;
	GError *error=NULL;
	GdkPixmap *pixmap;
	//GdkBitmap *mask;
	GtkStyle *style;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif
	
	gtk_set_locale ();
	gtk_init (&argc, &argv);

	window = create_window ();
	
	// set the background image
	//pixbuf = gdk_pixbuf_new_from_file("guitar.png", &error);
	//gdk_pixbuf_render_pixmap_and_mask (pixbuf, &pixmap, NULL, 0);
	//style = gtk_style_new();
	//style->bg_pixmap[0]=pixmap;
	//gtk_widget_set_style(GTK_WIDGET(window),GTK_STYLE(style));

	gtk_widget_show(window);
	// set callbacks
	g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (quit), NULL);
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (quit), NULL);
	g_timeout_add (200,(GSourceFunc)check_freq_det, NULL);
	// initialise alsa
	init_alsa(rate);
	// make the mycosine array
	for(i=0;i<N;i++)  {
    	mycosine[i]= 0.54 - (0.46 * cos(2.0 * M_PI * (float)i/N));
  	}
	
	// set alsa state
	err = system("alsactl -f /usr/share/openmoko/scenarios/voip.state restore");
	// create a continuously looping thread that calculates freq detected
	g_thread_create((GThreadFunc) get_max_amp,NULL,0,NULL);
	gdk_threads_enter();
	gtk_main ();
	gdk_threads_leave();
	return 0;
}
