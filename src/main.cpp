#include <iostream>
#include <gdkmm/screen.h>
#include <gtkmm/application.h>
#include <cairo.h>

using namespace std;

static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer user_data);
static gboolean screen_pressed(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean drawing_pressed(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data);
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data);

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    GtkWidget *window;
    GtkWidget *da;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Alpha Demo");

    // these don't let the events through
    // gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);
    // GdkWindow *gdk_window = gtk_widget_get_window(window);
    // gdk_window_set_pass_through(gdk_window, TRUE);
    g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);
    gtk_widget_add_events(window, GDK_ALL_EVENTS_MASK);
    g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(screen_pressed), NULL);

    gtk_widget_set_app_paintable(window, TRUE);
    // need this
    // gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);

    da = gtk_drawing_area_new();
    gtk_widget_add_events(da, GDK_ALL_EVENTS_MASK);
    g_signal_connect(G_OBJECT(da), "button-press-event", G_CALLBACK(drawing_pressed), NULL);
    g_signal_connect(da, "draw", G_CALLBACK(draw_cb), NULL);

    screen_changed(window, NULL, NULL);

    GtkWidget *overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER (window), overlay);

    gtk_container_add(GTK_CONTAINER (overlay), da);

    // gtk_overlay_set_overlay_pass_through(GTK_OVERLAY(overlay), window, TRUE);

    gtk_widget_show(da);
    gtk_widget_show(window);
    gtk_widget_show(overlay);
    gtk_widget_set_size_request(window, 0, 0);
    gtk_main();

    return 0;
}


gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    }
    else
    {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_visual(widget, visual);
}

static gboolean screen_pressed(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    cout << "Clicked window" << endl;
    return FALSE;
}

static gboolean drawing_pressed(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    cout << "Clicked drawing area" << endl;
    return FALSE;
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{   
  GdkRectangle workarea = {0};
  gdk_monitor_get_geometry(
      gdk_display_get_primary_monitor(gdk_display_get_default()),
      &workarea);

  /* draw horizontal line */
  cairo_set_source_rgb(cr, 0.77, 0.16, 0.13);
  cairo_set_line_width(cr, 6);
  
  /* top left */
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, 0, 160);
  cairo_stroke(cr);
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, 160, 0);
  cairo_stroke(cr);

  /* bottom left */
  cairo_move_to(cr, 0, workarea.height);
  cairo_line_to(cr, 0, workarea.height - 160);
  cairo_stroke(cr);
  cairo_move_to(cr, 0, workarea.height);
  cairo_line_to(cr, 160, workarea.height);
  cairo_stroke(cr);

  /* top right */
  cairo_move_to(cr, workarea.width, 0);
  cairo_line_to(cr, workarea.width, 160);
  cairo_stroke(cr);
  cairo_move_to(cr, workarea.width, 0);
  cairo_line_to(cr, workarea.width -160, 0);
  cairo_stroke(cr);

  /* bottom right */
  cairo_move_to(cr, workarea.width, workarea.height);
  cairo_line_to(cr, workarea.width, workarea.height - 160);
  cairo_stroke(cr);
  cairo_move_to(cr, workarea.width, workarea.height);
  cairo_line_to(cr, workarea.width -160, workarea.height);
  cairo_stroke(cr);

  return FALSE;
}