#include <gtk--.h>
#include <gdk--.h>
#include <gtk/gtk.h>
#include <gtk--/fileselection.h>
#include <fstream.h>
#include <iostream>

#define ExitOnTrue( file, msg ) if(file){ cout << msg ;\
	cout << __FUNCTION__ << "() " << __FILE__ << ':' << __LINE__ << endl;\
	exit(1); }


struct signalinfo
{
	ifstream file;	// file to read from
	int location;	// where in the file we are, -1 initially
	short * buffer;	// where to put the temporary information
	GdkPoint *points;	// the point objects for this signal
	double vloc;	// how high or low to display the signal
	double vzoomfact;	// how far to zoom vertically, higher is zoomout
	Gdk_Color fg;	// the foreground color
	Gdk_Color bg;	// the background color
};

const int testsignal = 0;
const int syncsignal = 1;
const int numsignals = 2;
const char* syncfile = "/tmp/sync";

const int DefaultWidth = 400;
const int DefaultHeight = 400;

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
		colormap = colormap.get_system();
		for( int i = 0; i < numsignals; i++)
		{
			signal[i].file.open(syncfile, ios::in);
			ExitOnTrue( !signal[i].file, "Error opening " 
				<< syncfile);
			signal[i].location = -1;
			signal[i].vloc = 6000;
			signal[i].vzoomfact = 3;
			signal[i].bg = colormap.black();
		}
		signal[testsignal].fg.set_rgb_p( 0,1,0);
		colormap.alloc( signal[testsignal].fg);
		signal[syncsignal].fg.set_rgb_p( 1, 0, 0);
		colormap.alloc( signal[syncsignal].fg);

		black.set_rgb_p( 0, 0, 0);
		colormap.alloc( black );

		numpoints = DefaultWidth;
		points = new GdkPoint[numpoints];
	}

	~graph_drawing_area()
	{
	}

	void setinputfile ( string name, int signalnum )
	{
		if( signalnum < 0 || signalnum >= numsignals )
		{
			cerr << "Trying to set the file " << name
				<< " to a nonexisting signal " << signalnum
				<< endl;
		}

		signal[signalnum].file.close();
		signal[signalnum].file.open( name.c_str() );
		ExitOnTrue( !signal[signalnum].file, "Error opening "
			<< name );
		signal[signalnum].location = 0;
	}
private:

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
		gc.set_foreground( black );
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
		gc.set_foreground( black );
		// Same like above + Gtk_Widget has set_state function but
		// no get_state function.
		win.draw_pixmap( gc,
			pixmap,
			event->area.x, event->area.y,
			event->area.x, event->area.y,
			event->area.width, event->area.height);
		return FALSE;
	}

	/* Draw the signals on the screen */
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
		int updatesignal = (event->y >= height()/2);
		if(event->button == 2 && pixmap)
			draw_graph( updatesignal, -width(), 0);
		if(event->button == 3 && pixmap)
			draw_graph( updatesignal, width(), 0);
		if(event->button == 1 && pixmap)
		{
			lastx = (int)event->x;
			lasty = (int)event->y;
		}
		return TRUE;
	}

	gint button_release_event_impl (GdkEventButton *event)
	{
		int updatesignal = (event->y >= height()/2);
		if( event->button == 1 && pixmap)
		{
			draw_graph( updatesignal, (int)(event->x - lastx),
				(int)(event->y - lasty));
		}
	}

	gint motion_notify_event_impl (GdkEventMotion *event)
	{
		GdkModifierType state;
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
			move_graph( updatesignal, (int)(event->x - lastx),
				(int)(event->y - lasty));
		return TRUE;
	}

	/* Backing pixmap for drawing area */

	Gdk_Pixmap pixmap;
	Gdk_GC gc;
	Gdk_Window win;
	Gdk_Visual visual;
	int lastx;
	int lasty;
	Gdk_Color black;
	Gdk_Colormap colormap;
	signalinfo signal[numsignals];
	GdkPoint *points;
	int numpoints;

};


class graph_window : public Gtk_Window
{
public:
	graph_window() : Gtk_Window(GTK_WINDOW_TOPLEVEL),
		vbox( FALSE, 0), hbox( FALSE, 0), FileSelectButton("Load File"),
		button ("quit"), fs(0)
	{
		add(&vbox);

		/* Creaet the drawing area */
		drawing_area.size(DefaultWidth,DefaultHeight);
		vbox.pack_start(drawing_area, TRUE, TRUE, 0);

		/* Add the hbox to the buttom */
		vbox.pack_start(hbox, FALSE, FALSE, 0);

		/* Add the button */
		hbox.pack_start( button, FALSE, FALSE, 0);

		hbox.pack_start( FileSelectButton, FALSE, FALSE, 0);
		connect_to_method ( FileSelectButton.clicked, this,
			&showfileselection);
		FileSelectButton.show();

		connect_to_method ( button.clicked, this, &quit );
		connect_to_method ( destroy, this, &quit );

		drawing_area.show();
		button.show();
		vbox.show();
		hbox.show();

	}

private:

	int FSselectionclosed(GdkEventAny*)
	{
		cout << "Selection closed\n";
		// according the the example program it is deleted by
		// now so we don't bother
		// fs->hide();
		// fs=0;
		return TRUE;
	}
	void FSbuttoncanceled()
	{
		cout << "File NOT selected " << fs->get_filename() << endl;
		fs->hide();
	}

	void FSbuttonpressed()
	{
		cout << "File Selected " << fs->get_filename() << endl;
		drawing_area.setinputfile( fs->get_filename(), testsignal);
		fs->hide();
	}
	void showfileselection()
	{
		if(!fs)
		{
			fs=new Gtk_FileSelection("File Selection");
			connect_to_method(fs->get_ok_button()->clicked,
				this, &FSbuttonpressed);
			connect_to_method(fs->get_cancel_button()->clicked,
				this, &FSbuttoncanceled);
			connect_to_method(fs->delete_event,
				this, &FSselectionclosed);
		}
		fs->show();
	}
	void quit()
	{
		gtk_exit(0);
	}

	Gtk_VBox vbox;
	Gtk_HBox hbox;
	graph_drawing_area drawing_area;
	Gtk_Button FileSelectButton;
	Gtk_Button button;
	Gtk_FileSelection *fs;

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


