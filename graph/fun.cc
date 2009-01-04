#include <gtk--.h>
#include <gdk--.h>
#include <gtk/gtk.h>
#include <gtk--/fileselection.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sigc++/slot.h>

using namespace std;

#define ExitOnTrue( file, msg ) if(file){ cout << msg ;\
	cout << __FUNCTION__ << "() " << __FILE__ << ':' << __LINE__ << endl;\
	exit(1); }


using SigC::slot;
using SigC::bind;

class signalinfo
{
public:
	signalinfo();
	~signalinfo();
	// initialize with the specific file and width
	void Init(const char *filename, int width);
	// resize the data structures to handle a different width
	void Resize(int width);
	ifstream file;	// file to read from
	int location;	// where in the file we are, -1 initially
	double fraction; // the fractional part of the location
	short * buffer;	// where to put the temporary information
	int buffersize;	// the size of the current buffer
	GdkPoint *points;	// the point objects for this signal
	short vloc;	// how high or low to display the signal
	double vzoomfact;	// how far to zoom vertically, higher is zoomout
	Gdk_Color fg;	// the foreground color
	Gdk_Color bg;	// the background color
};

signalinfo::signalinfo()
{
	location = 0;
	fraction = 0;
	buffer = NULL;
	buffersize = 0;
	points = NULL;
	vloc = 165;
	vzoomfact = 36;
}

signalinfo::~signalinfo()
{
	file.close();
	delete [] buffer;
	delete [] points;
}

void signalinfo::Init(const char *filename, int width)
{
	cout << "Opening file " << filename << endl;
	file.open(filename, ios::in);
	ExitOnTrue( !file, "Error opening " << filename);
	Resize(width);
	// read the file
	file.read( (char*)buffer, width*sizeof(short));
	ExitOnTrue(!file, "Error reading file" );
}

void signalinfo::Resize(int width)
{
	delete [] buffer;
	buffer = new short[width];
	buffersize = width;
	delete [] points;
	points = new GdkPoint[width];
}

const int testsignal = 0;
const int syncsignal = 1;
const int numsignals = 2;
//const char* syncfile = "/tmp/sync";
//const char* syncfile = "/tmp/data/121blank";
const char * syncfile = "sync";
const char * filename[2] = {"sync", "sync"};

const int DefaultWidth = 550;
const int DefaultHeight = 550;

const int DefaultMinMotion = 5;

class graph_drawing_area : public Gtk::DrawingArea
{
public:
	graph_drawing_area();
	~graph_drawing_area();
	void setinputfile ( string name, int signalnum );
	void setflipamount( double amount );
	class graph_window *graph_pointer;

private:
	/* Create a new backing pixmap of the appropriate size */
	int configure_event_impl (GdkEventConfigure * /* event */);
	/* Redraw the screen from the backing pixmap */
	int expose_event_impl (GdkEventExpose *event);
	/* Draw_Graph will load the file information from file if it
	   isn't at the same location */
	void draw_graph( int updatesignal, double xoffset, int yoffset );
	void move_graph( int updatesignal, double xoffset, int yoffset );
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
	int lastsignal;
	int lastxflip;
	double flipamount;
	Gdk_Color black;
	Gdk_Colormap colormap;
	signalinfo signal[numsignals];
	GdkPoint *points;
};

class graph_window : public Gtk::Window
{
public:
	graph_window();
	void setfileoffset( int signal, int offset);
private:

	void changeflip();
	int FSselectionclosed(GdkEventAny*);
	void FSbuttoncanceled();
	void FSbuttonpressed();
	void showfileselection();
	void quit();

	Gtk::VBox vbox;
	Gtk::HBox hbox;
	graph_drawing_area drawing_area;
	Gtk::Button FileSelectButton;
	Gtk::Button button;
	Gtk::FileSelection *fs;
	Gtk::Label changeflipL;
	Gtk::Entry changeflipE;
	Gtk::Label fileoffset[numsignals];
};

graph_drawing_area::graph_drawing_area() : pixmap(0)
{
	set_events( GDK_EXPOSURE_MASK
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK
		| GDK_POINTER_MOTION_HINT_MASK);
	lastx = -1;
	lasty = -1;
	colormap = colormap.get_system();
	for( int i = 0; i < numsignals; i++)
	{
		signal[i].Init(filename[i], DefaultWidth);
		signal[i].bg=colormap.black();
	}
	signal[testsignal].fg.set_rgb_p( 0, 1, 0);
	colormap.alloc( signal[testsignal].fg);
	signal[syncsignal].fg.set_rgb_p( 1, 0, 0);
	colormap.alloc( signal[syncsignal].fg);

	black.set_rgb_p( 0, 0, 0);
	colormap.alloc( black );
}

graph_drawing_area::~graph_drawing_area()
{
}

void graph_drawing_area::setflipamount( double amount )
{
	flipamount = amount;
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
	ExitOnTrue( !signal[signalnum].file, "Error opening \""
		<< name << "\" " );
	signal[signalnum].location = 0;
	signal[signalnum].fraction = 0;
}

int graph_drawing_area::configure_event_impl (GdkEventConfigure * /* event */)
{
	win = get_window();
	visual = win.get_visual();

	/*
	if (pixmap)
	{
		pixmap.free();
	}
	*/
	//gc = get_style()->gtkobj()->white_gc;
	gc = get_style()->gtkobj()->black_gc;
	gc.set_foreground( black );
	pixmap.create(get_window(), width(), height());

	pixmap.draw_rectangle( gc,
		TRUE,
		0, 0,
		width(),
		height());
	draw_graph(0, 0, 0);
	draw_graph(1, 0, 0);
	flipamount = width();
	return TRUE;
}

int graph_drawing_area::expose_event_impl (GdkEventExpose *event)
{
	gc = get_style()->gtkobj()->fg_gc
		[GTK_WIDGET_STATE (GTK_WIDGET(gtkobj()))];
	gc.set_foreground( black );
	// Same like above + Gtk::Widget has set_state function but
	// no get_state function.
	win.draw_pixmap( gc,
		pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);
	return FALSE;
}

gint graph_drawing_area::button_press_event_impl (GdkEventButton *event)
{
	int updatesignal = (event->y >= height()/2);
	lastsignal = updatesignal;
	// right mouse button move the graph left
	if(event->button == 3 && pixmap)
		draw_graph( updatesignal, flipamount, 0);
	// left mouse button move the graph right
	if(event->button == 2 && pixmap)
		draw_graph( updatesignal, -flipamount, 0);
	// nothing to do when button one is pressed
	lastx = (int)event->x;
	lasty = (int)event->y;
	lastxflip = lastx;
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

	if(state& GDK_BUTTON1_MASK && pixmap )
		move_graph( lastsignal, x - lastx, y - lasty);
	if((state& GDK_BUTTON2_MASK||state& GDK_BUTTON3_MASK) && pixmap )
	{
		int xx = (x-lastx)/DefaultMinMotion;
		int diff = (lastxflip - lastx)/DefaultMinMotion;
		draw_graph( lastsignal, -flipamount*(diff-xx),0);
		lastxflip = x;
	}
	return TRUE;
}

gint graph_drawing_area::button_release_event_impl (GdkEventButton *event)
{
	if( event->button == 1 && pixmap)
	{
		draw_graph( lastsignal, (event->x - lastx),
			(int)(event->y - lasty));
	}
	if( (event->button == 2 || event->button == 3)&& pixmap )
	{
		int xx = ((int)event->x-lastx)/DefaultMinMotion;
		int diff = (lastxflip - lastx)/DefaultMinMotion;
		draw_graph( lastsignal, -flipamount*(diff-xx),0);
	}
	return TRUE;
}


void graph_drawing_area::draw_graph( int updatesignal,
	double xoffset, int yoffset )
{
	int sig;
	int i;

	/*
	ExitOnTrue(signal[updatesignal].buffersize!=width(),
		"resizing not implimented yet");
	*/
	if(signal[updatesignal].buffersize!=width())
		signal[updatesignal].Resize(width());

	// the x offset is the number of pixels to move it
	// there are two bytes per short and one short per pixel
	// so we need to move the file offset 2*xoffset
	int xint = (int)xoffset;
	xint *=2;
	// update offsets
	signal[updatesignal].location -= (int)xint;
	signal[updatesignal].fraction -= xoffset*2-(int)xint;
	// only update location in multiples of two
	if((int)signal[updatesignal].fraction/2 )
	{
		// discard the odd or fractional portion
		int f = (int)(signal[updatesignal].fraction/2);
		f *= 2;

		signal[updatesignal].location += f;
		signal[updatesignal].fraction -= f;
	}
	if(signal[updatesignal].location < 0)
	{
		signal[updatesignal].location = 0;
		signal[updatesignal].fraction =0;
	}
	signal[updatesignal].vloc += yoffset;

	// seek the file to location
	signal[updatesignal].file.seekg(signal[updatesignal].location,
		ios::beg );
	ExitOnTrue(!signal[updatesignal].file, "Error seeking file" );
	// read the file
	signal[updatesignal].file.read( (char *)signal[updatesignal].buffer,
		width()*sizeof(short));
	/*
	ExitOnTrue(!signal[updatesignal].file, "Error reading file" );
	*/
	if(!signal[updatesignal].file)
	{
		cout << "Warning, error on file read\n";
		signal[updatesignal].file.clear();
	}
	graph_pointer->setfileoffset(updatesignal,
		signal[updatesignal].location);


	// only need to update the modified signal
	// create the points
	for( i = 0; i < width(); i++)
	{
		signal[updatesignal].points[i].x=i;
		signal[updatesignal].points[i].y = (int)
			(signal[updatesignal].buffer[i]/
			signal[updatesignal].vzoomfact+
			signal[updatesignal].vloc);
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
	/*
	win.draw_pixmap( gc,
		pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);
	*/
	win.draw_pixmap( gc, pixmap, 0,0,0,0, width(), height() );
}

void graph_drawing_area::move_graph( int updatesignal,
	double xoffset, int yoffset )
{
	int sig;
	int i;

	ExitOnTrue(signal[updatesignal].buffersize!=width(),
		"resizing not implimented yet");

	// only need to create the moved updated signal
	// create the points
	for( i = 0; i < width(); i++)
	{
		signal[updatesignal].points[i].x=i+(int)xoffset;
		signal[updatesignal].points[i].y = yoffset+(int)
			(signal[updatesignal].buffer[i]/
			signal[updatesignal].vzoomfact+
			signal[updatesignal].vloc);
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
	win.draw_pixmap( gc, pixmap, 0,0,0,0, width(), height() );
}

graph_window::graph_window() : Gtk::Window(GTK_WINDOW_TOPLEVEL),
	vbox( FALSE, 0), hbox( FALSE, 0), FileSelectButton("Load File"),
	button ("quit"), fs(0), changeflipL("Flip amount")
{
	add(vbox);
	//pack_start(&vbox);

	/* Create the drawing area */
	drawing_area.size(DefaultWidth,DefaultHeight);
	vbox.pack_start(drawing_area, TRUE, TRUE, 0);
	drawing_area.graph_pointer = this;

	/* Add the hbox to the buttom */
	vbox.pack_start(hbox, FALSE, FALSE, 0);

	hbox.pack_start( fileoffset[0], FALSE, FALSE, 0);
	fileoffset[0].show();
	hbox.pack_start( fileoffset[1], FALSE, FALSE, 10);
	fileoffset[1].show();
	setfileoffset(0, 0);
	setfileoffset(1, 0);
	
	/* Add the button */
	hbox.pack_start( button, FALSE, FALSE, 0);
	button.clicked.connect(slot(this,&graph_window::quit));

	hbox.pack_start( FileSelectButton, FALSE, FALSE, 0);
	FileSelectButton.clicked.connect(slot(this,
		&graph_window::showfileselection));
	FileSelectButton.show();

	hbox.pack_start( changeflipL, FALSE, FALSE, 0);
	changeflipL.show();
	hbox.pack_start( changeflipE, FALSE, FALSE, 0);
	changeflipE.show();
	changeflipE.activate.connect(slot(this,&graph_window::changeflip));

	// set the text for the flip amount
	stringstream membuff;
	// stress that a floating point value is acceptable
	membuff << DefaultWidth << ".0" << (char)0 << flush;
	int zero = 0;
	string buffer=membuff.str();
	changeflipE.insert_text(buffer.c_str(), buffer.size(), &zero);

	destroy.connect(slot(this, &graph_window::quit));

	drawing_area.show();
	button.show();
	vbox.show();
	hbox.show();

}

void graph_window::changeflip()
{
	cout << changeflipE.get_text() << endl;
	drawing_area.setflipamount(
		atof(changeflipE.get_text().c_str()));
}

int graph_window::FSselectionclosed(GdkEventAny*)
{
	cout << "Selection closed\n";
	// according the the example program it is deleted by
	// now so we don't bother
	// fs->hide();
	// fs=0;
	return TRUE;
}
void graph_window::FSbuttoncanceled()
{
	cout << "File NOT selected " << fs->get_filename() << endl;
	fs->hide();
}

void graph_window::FSbuttonpressed()
{
	cout << "File Selected " << fs->get_filename() << endl;
	drawing_area.setinputfile( fs->get_filename(), testsignal);
	fs->hide();
}
void graph_window::showfileselection()
{
	if(!fs)
	{
		fs=new Gtk::FileSelection("File Selection");
		fs->set_filename(".");
		fs->get_ok_button()->clicked.connect(slot(this,
			&graph_window::FSbuttonpressed));
		fs->get_cancel_button()->clicked.connect(slot(this,
			&graph_window::FSbuttoncanceled));
		fs->delete_event.connect(slot(this,
			&graph_window::FSselectionclosed));
	}
	fs->show();
}
void graph_window::quit()
{
	gtk_exit(0);
}

void graph_window::setfileoffset( int signal, int offset)
{
	// set the text for the flip amount
	stringstream membuff;
	membuff << (signal ? "Sync " : "Test ") << offset
		<< (char)0 << flush;
	string buffer=membuff.str();
	fileoffset[signal].set_text(buffer.c_str());
}

int main ( int argc, char ** argv)
{
	if(argc==2)
	{
		filename[0] = *(argv+1);
		filename[1] = filename[0];
	}
	else if( argc > 2 )
	{
		filename[0] = *(argv+1);
		filename[1] = *(argv+2);
	}
		
	graph_window *window;
	Gtk::Main myapp(&argc, &argv);

	window = new graph_window;
	window->show();

	myapp.run();

	return 0;
}


