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
	int buffersize;	// the size of the current buffer
	GdkPoint *points;	// the point objects for this signal
	short vloc;	// how high or low to display the signal
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
	graph_drawing_area();
	~graph_drawing_area();
	void setinputfile ( string name, int signalnum );

private:
	/* Create a new backing pixmap of the appropriate size */
	int configure_event_impl (GdkEventConfigure * /* event */);
	/* Redraw the screen from the backing pixmap */
	int expose_event_impl (GdkEventExpose *event);
	/* Draw_Graph will load the file information from file if it
	   isn't at the same location */
	void draw_graph( int updatesignal, int xoffset, int yoffset );
	void move_graph( int updatesignal, int xoffset, int yoffset );
	gint button_press_event_impl (GdkEventButton *event);
	gint button_release_event_impl (GdkEventButton *event);
	gint motion_notify_event_impl (GdkEventMotion *event);

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
};

graph_drawing_area::graph_drawing_area() : pixmap(0)
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
		signal[i].location = 0;
		signal[i].buffer = new short[DefaultWidth];
		signal[i].buffersize = DefaultWidth;
		signal[i].points = new GdkPoint[DefaultWidth];
		signal[i].vloc = 6000;
		signal[i].vzoomfact = 3;
		signal[i].bg = colormap.black();

		// read the file
		signal[i].file.read( (char*)signal[i].buffer,
			DefaultWidth*sizeof(short));
		ExitOnTrue(!signal[i].file,
			"Error reading file" );

	}
	signal[testsignal].fg.set_rgb_p( 0,1,0);
	colormap.alloc( signal[testsignal].fg);
	signal[syncsignal].fg.set_rgb_p( 1, 0, 0);
	colormap.alloc( signal[syncsignal].fg);

	black.set_rgb_p( 0, 0, 0);
	colormap.alloc( black );
}

graph_drawing_area::~graph_drawing_area()
{
	for(int i = 0; i < numsignals; i++)
	{
		signal[i].file.close();
		delete [] signal[i].buffer;
		delete [] signal[i].points;
	}
}

void graph_drawing_area::setinputfile ( string name, int signalnum )
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

int graph_drawing_area::configure_event_impl (GdkEventConfigure * /* event */)
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

int graph_drawing_area::expose_event_impl (GdkEventExpose *event)
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

gint graph_drawing_area::button_release_event_impl (GdkEventButton *event)
{
	int updatesignal = (event->y >= height()/2);
	if( event->button == 1 && pixmap)
	{
		draw_graph( updatesignal, (int)(event->x - lastx),
			(int)(event->y - lasty));
	}
	return TRUE;
}

gint graph_drawing_area::motion_notify_event_impl (GdkEventMotion *event)
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

	int updatesignal = (y >= height()/2);
	if(state& GDK_BUTTON1_MASK && pixmap )
		move_graph( updatesignal, (int)(event->x - lastx),
			(int)(event->y - lasty));
	return TRUE;
}

gint graph_drawing_area::button_press_event_impl (GdkEventButton *event)
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

void graph_drawing_area::draw_graph( int updatesignal,
	int xoffset, int yoffset )
{
	int sig;
	int i;

	ExitOnTrue(signal[updatesignal].buffersize!=width(),
		"resizing not implimented yet");

	// update offsets
	signal[updatesignal].location += xoffset;
	if(signal[updatesignal].location < 0)
		signal[updatesignal].location = 0;
	signal[updatesignal].vloc += yoffset;

	// seek the file to location
	signal[updatesignal].file.seekg(signal[updatesignal].location,
		ios::beg );
	ExitOnTrue(!signal[updatesignal].file, "Error seeking file" );
	// read the file
	signal[updatesignal].file.read( (char *)signal[updatesignal].buffer,
		width()*sizeof(short));
	ExitOnTrue(!signal[updatesignal].file, "Error reading file" );


	// create the points
	for( sig = 0; sig < numsignals; sig++)
	for( i = 0; i < width(); i++)
	{
		signal[sig].points[i].x=i;
		signal[sig].points[i].y = (int)
			((signal[sig].buffer[i] + signal[sig].vloc)/
			signal[sig].vzoomfact);
	}

	// clear the drawing area

	gc = get_style()->gtkobj()->black_gc;
	gc.set_foreground( black );
	pixmap.draw_rectangle( gc,
		TRUE,
		0, 0,
		width(),
		height());

	// draw the points for the signals
	for( sig = 0; sig < numsignals; sig++)
	{
		gc.set_foreground( signal[sig].fg );
		pixmap.draw_points( gc, signal[sig].points, width() );
	}
}

void graph_drawing_area::move_graph( int updatesignal,
	int xoffset, int yoffset )
{
	int sig;
	int i;

	ExitOnTrue(signal[updatesignal].buffersize!=width(),
		"resizing not implimented yet");

	// create the points
	for( sig = 0; sig < numsignals; sig++)
	for( i = 0; i < width(); i++)
	{
		signal[sig].points[i].x=i+xoffset;
		signal[sig].points[i].y = (int)(yoffset +
			(signal[sig].buffer[i] + signal[sig].vloc)/
			signal[sig].vzoomfact);
	}

	// clear the drawing area

	gc = get_style()->gtkobj()->black_gc;
	gc.set_foreground( black );
	pixmap.draw_rectangle( gc,
		TRUE,
		0, 0,
		width(),
		height());

	// draw the points for the signals
	for( sig = 0; sig < numsignals; sig++)
	{
		gc.set_foreground( signal[sig].fg );
		pixmap.draw_points( gc, signal[sig].points, width() );
	}
}

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


