#include <gtk--.h>
#include <gdk--.h>
#include <gtk/gtk.h>
#include <iostream>

class graph_drawing_area : public Gtk_DrawingArea
{
public:
	graph_drawing_area() : pixmap(0)
	{
		set_events( GDK_EXPOSURE_MASK
			| GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_POINTER_MOTION_MASK
			| GDK_POINTER_MOTION_HINT_MASK);
		lastx = -1;
		lasty = -1;
		//fgcolor.set_rgb( 30000, 0, 10000);
		fgcolor.set_rgb_p( 0, 0, 1);
		colormap = colormap.get_system();
		//fgcolor = colormap.black();
		colormap.alloc( fgcolor );
	}
private:
	/* Backing pixmap for drawing area */

	Gdk_Pixmap pixmap;
	Gdk_GC gc;
	Gdk_Window win;
	Gdk_Visual visual;
	int lastx;
	int lasty;
	Gdk_Color fgcolor;
	Gdk_Colormap colormap;

	/* Create a new backing pixmap of the appropriate size */
	int configure_event_impl (GdkEventConfigure * /* event */)
	{
		win = get_window();
		visual = win.get_visual();

		if (pixmap)
		{
			pixmap.free();
		}
		//gc = get_style()->gtkobj()->white_gc;
		gc = get_style()->gtkobj()->black_gc;
		pixmap.create(get_window(), width(), height());

		pixmap.draw_rectangle( gc,
			TRUE,
			0, 0,
			width(),
			height());
		return TRUE;
	}

	/* Redraw the screen from the backing pixmap */
	int expose_event_impl (GdkEventExpose *event)
	{
		gc = get_style()->gtkobj()->fg_gc
			[GTK_WIDGET_STATE (GTK_WIDGET(gtkobj()))];
		// Same like above + Gtk_Widget has set_state function but
		// no get_state function.
		win.draw_pixmap( gc,
			pixmap,
			event->area.x, event->area.y,
			event->area.x, event->area.y,
			event->area.width, event->area.height);
		return FALSE;
	}

	/* Draw a line on the screen */
	void draw_brush ( gdouble x, gdouble y)
	{
		GdkRectangle update_rect;
		int xcur = (int)x;
		int ycur = (int)y;
		if( lastx == -1 )
		{
			lastx = xcur;
			lasty = ycur;
		}
		//gc = get_style()->gtkobj()->white_gc;
		gc.set_foreground( fgcolor );
		gc.set_background( fgcolor );
			
		pixmap.draw_line(
			gc,
			lastx,lasty,
			xcur,ycur );
		/* -1 and +2 makes the box one pixel bigger on all sides */
		update_rect.x = ((lastx < xcur ) ? lastx : xcur)-1;
		update_rect.y = ((lasty < ycur ) ? lasty : ycur)-1;
		update_rect.width = 
			((lastx < xcur) ? xcur-lastx : lastx-xcur)+2;
		update_rect.height =
			((lasty < ycur) ? ycur-lasty : lasty-ycur)+2;
		draw( &update_rect);
		lastx = xcur;
		lasty = ycur;
	}

	gint button_press_event_impl (GdkEventButton *event)
	{
		if(event->button == 1 && pixmap)
			draw_brush(event->x, event->y);
		return TRUE;
	}

	gint motion_notify_event_impl (GdkEventMotion *event)
	{
		int x, y;
		GdkModifierType state;
		if (event->is_hint)
			gdk_window_get_pointer( event->window, &x, &y, &state);
		else
		{
			x=(int)event->x;
			y=(int)event->y;
			state = (GdkModifierType) event->state;
		}

		if(state& GDK_BUTTON1_MASK && pixmap )
			draw_brush(x,y);
		return TRUE;
	}
};


class graph_window : public Gtk_Window
{
public:
	graph_window() : Gtk_Window(GTK_WINDOW_TOPLEVEL),
		vbox( FALSE, 0),
		button ("quit")
	{
		add(&vbox);

		/* Creaet the drawing area */
		drawing_area.size(400,400);
		vbox.pack_start(drawing_area, TRUE, TRUE, 0);

		/* Add the button */
		vbox.pack_start( button, FALSE, FALSE, 0);

		connect_to_method ( button.clicked, this, &quit );
		connect_to_method ( destroy, this, &quit );

		drawing_area.show();
		button.show();
		vbox.show();
	}

private:
	Gtk_VBox vbox;
	graph_drawing_area drawing_area;
	Gtk_Button button;

	void quit()
	{
		gtk_exit(0);
	}

};

int main ( int argc, char ** argv)
{
	graph_window *window;
	Gtk_Main myapp(&argc, &argv);

	window = new graph_window;
	window->show();

	myapp.run();

	return 0;
}


