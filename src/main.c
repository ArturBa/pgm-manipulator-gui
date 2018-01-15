/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2018 Artur Bauer <Bauer.Artur@protonmail.com>
 * 
 * Pgm_Manipulator_Gui is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Pgm_Manipulator_Gui is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "graphic.h"
#define HISTOGRAM_HEIGHT 70
#define PICTURE_SIZE 500

struct data {
    bool file_open;
    GtkBuilder *builder;
    picture *picture_ptr;
    GtkWidget *window;
    GtkSpinButton *number;
    GtkImage *picture;
    GtkImage *histogram;
    GtkRadioButton *radio_button_x;
    bool radio_button_x_active;
};

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    picture *picture_ptr = NULL;
    GtkRadioButton *button_y;
    struct data *data_gtk = calloc(1, sizeof *data_gtk);
    data_gtk->file_open = false;
    data_gtk->picture_ptr = picture_ptr;
    data_gtk->radio_button_x_active = true;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/pgm_manipulator_gui.ui", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    data_gtk->window = window;
    data_gtk->number =
	GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "number"));
    data_gtk->picture =
	GTK_IMAGE(gtk_builder_get_object(builder, "picture"));
    data_gtk->histogram =
	GTK_IMAGE(gtk_builder_get_object(builder, "histogram"));
    gtk_spin_button_set_increments(data_gtk->number, 1, -1);
    data_gtk->radio_button_x =
	GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "x_radio"));
    gtk_radio_button_set_group(data_gtk->radio_button_x, NULL);
    button_y =
	GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "y_radio"));
    gtk_radio_button_join_group(button_y, data_gtk->radio_button_x);
    data_gtk->builder = builder;
    gtk_builder_connect_signals(builder, data_gtk);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

void reload_picture(struct data *data_ptr) {
    GdkPixbuf *buf;
    picture *histogram = create_histogram_pgm(data_ptr->picture_ptr, HISTOGRAM_HEIGHT);
    picture_negative(histogram);
    picture *photo = calloc(1, sizeof *photo);
    picture_copy(photo, data_ptr->picture_ptr);
    picture_resize(photo, PICTURE_SIZE);
    picture_save(photo, ".buffer.pgm");
    picture_save(histogram, ".histogram.pgm");
    buf = gdk_pixbuf_new_from_file(".buffer.pgm", NULL);
    gtk_image_set_from_pixbuf(data_ptr->picture, buf);
    buf = gdk_pixbuf_new_from_file(".histogram.pgm", NULL);
    gtk_image_set_from_pixbuf(data_ptr->histogram, buf);
    picture_delete(histogram);
    picture_delete(photo);
}

void gtk_spin_button_set_new_value_max(GtkSpinButton * button,
				       int new_value) {
    gtk_spin_button_set_range(button, (gdouble) 0, (gdouble) new_value);
    gtk_spin_button_set_value(button, new_value);
}

void on_salt_pepper_clicked(GtkWidget * button, gpointer * gpointer_ptr) {
    g_print("You pressed Salt&Pepper\n");
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if (data_ptr->file_open) {
	picture_salt_pepper_hum(data_ptr->picture_ptr);
	reload_picture(data_ptr);
    }
}

void on_gauss_clicked(GtkWidget * button, gpointer * gpointer_ptr) {
    g_print("You pressed Gauss\n");
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if (data_ptr->file_open) {
	picture_generic_filter(data_ptr->picture_ptr, true);
	reload_picture(data_ptr);
    }
}

void on_negative_clicked(GtkWidget * button, gpointer * gpointer_ptr) {
    g_print("You pressed Negative\n");
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if (data_ptr->file_open) {
	picture_negative(data_ptr->picture_ptr);
	reload_picture(data_ptr);
    }
}

void on_progging_clicked(GtkWidget * button, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    double value =
	(double) gtk_spin_button_get_value_as_int(data_ptr->number);
    gtk_spin_button_set_new_value_max(data_ptr->number, value);
    g_print("You pressed Progging %lf\n", value);
    if (data_ptr->file_open) {
	picture_proging(data_ptr->picture_ptr, (int)
			gtk_spin_button_get_value_as_int
			(data_ptr->number));
	reload_picture(data_ptr);
    }
}

void on_rotate_clockwise_clicked(GtkWidget * button,
				 gpointer * gpointer_ptr) {
    g_print("You pressed Rotate\n");
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if (data_ptr->file_open) {
	picture_rotate_clockwise(data_ptr->picture_ptr);
	reload_picture(data_ptr);
    }
}

void on_Save_activate(GtkWidget * button, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    g_print("You pressed Save\n");
	if(data_ptr->file_open)
    picture_save(data_ptr->picture_ptr, data_ptr->picture_ptr->name);
}

void on_Quit_activate(GtkWidget * button, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
	if(data_ptr->file_open)
    picture_delete(data_ptr->picture_ptr);
    gtk_main_quit();
}

void on_window_destroy(GtkWidget * button, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if(data_ptr->file_open)
	picture_delete(data_ptr->picture_ptr);
    gtk_main_quit();
}

void menu_dialog(GtkWidget * button, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    char name_buffer[256] = { 0 };
    GtkWidget *dialog;
    dialog =
	gtk_file_chooser_dialog_new("Chosse a file",
				    GTK_WINDOW(data_ptr->window),
				    GTK_FILE_CHOOSER_ACTION_OPEN,
				    GTK_STOCK_OK, GTK_RESPONSE_OK,
				    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				    NULL);
    gtk_widget_show_all(dialog);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
					g_get_home_dir());
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK) {
	strcpy(name_buffer,
	       gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
	if (strstr(name_buffer, ".pgm")) {
	    if (data_ptr->file_open)
		picture_delete(data_ptr->picture_ptr);
	    data_ptr->picture_ptr = (picture *)
		picture_add(name_buffer);
	    data_ptr->file_open = true;
	    reload_picture(data_ptr);
	    gtk_spin_button_set_new_value_max(data_ptr->number,
					      data_ptr->
					      picture_ptr->grey_value);
	}
    } else if (resp == GTK_RESPONSE_CANCEL)
	g_print("You pressed Cancel\n");
    gtk_widget_destroy(dialog);
}

void on_Open_activate(GtkWidget * builder, gpointer * data_ptr) {
    menu_dialog(builder, data_ptr);
}

void on_mirror_clicked(GtkWidget * builder, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    if (data_ptr->file_open) {
	if (data_ptr->radio_button_x_active) {
	    g_print("You pressed x\n");
	    picture_mirror_0X_axis(data_ptr->picture_ptr);
	} else {
	    g_print("You pressed y\n");
	    picture_mirror_0Y_axis(data_ptr->picture_ptr);
	}
	reload_picture(data_ptr);
    }
}
void on_y_radio_toggled(GtkWidget * builder, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    data_ptr->radio_button_x_active = false;
}
void on_x_radio_toggled(GtkWidget * builder, gpointer * gpointer_ptr) {
    struct data *data_ptr = (struct data *) gpointer_ptr;
    data_ptr->radio_button_x_active = true;
}
