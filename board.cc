#include <iostream.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Text.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <time.h>

int main( int argc, char ** argv)
{
	Display * dpy;
	Window win;
	XSetWindowAttributes xswa;
	XGCValues gcwhitevalues, gcblackvalues;
	GC gcwhite, gcblack;

	dpy = XOpenDisplay(NULL);

	int screen = DefaultScreen(dpy);
	int white;
	int black;
	Colormap cmap = XDefaultColormap(dpy, screen );
	XColor color;

	if( XParseColor( dpy, cmap, "White", &color ) )
		if( XAllocColor( dpy, cmap, &color ) )
			white  = color.pixel;
		else
			cerr << "Color white could not be found or set\n";

	if( XParseColor( dpy, cmap, "Black", &color ) )
		if( XAllocColor( dpy, cmap, &color ) )
			black  = color.pixel;
		else
			cerr << "Color black could not be found or set\n";

	xswa.background_pixel = black;
	xswa.border_pixel = 10;
	xswa.colormap = cmap;
	xswa.override_redirect = True;

	gcwhitevalues.foreground = white;
	gcwhitevalues.background = black;
	gcwhitevalues.line_width = 1;

	gcblackvalues.foreground = black;
	gcblackvalues.background = white;
	gcblackvalues.line_width = 1;

	win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10, 10, 640, 480, 1,
		CopyFromParent, CopyFromParent, CopyFromParent,
		CWBackPixel | CWBorderPixel | CWOverrideRedirect
		| CWColormap , &xswa);
	XMapWindow( dpy, win);

	gcwhite = XCreateGC( dpy, win, GCForeground | GCBackground |
		GCLineWidth, &gcwhitevalues );
	gcblack = XCreateGC( dpy, win, GCForeground | GCBackground |
		GCLineWidth, &gcblackvalues );
	
	XClearWindow( dpy, win);
	XFlush( dpy );
	int x;
	/*
	for( y = 0; y < 400; y+=10)
	{
		XDrawLine( dpy, win, gcwhite, 0, y, 640, y );
		XFlush( dpy );
	}
	for( x = 0; x < 600; x+=10)
	{
		XDrawLine( dpy, win, gcblack, x, 0, x, 480 );
		XFlush( dpy );
	}
	*/
	timeval start, end;
	int xvalues [] = {
		90,
		120,
		150,
		180,
		210,
		270,
		300,
		330,
		360,
		390,
		0};

	int time = 0;
		for( int i = 0; xvalues[i] != 0; i++ )
			XDrawLine( dpy, win, gcwhite, 0,
				xvalues[i], 640, xvalues[i] );
		XFlush( dpy );
	/*
	while ( time++ < 10000 )
	{
		for( int i = 0; xvalues[i] != 0; i++ )
			XDrawLine( dpy, win, gcwhite, 0,
				xvalues[i], 640, xvalues[i] );
		XFlush( dpy );
		nanosleep( &sleeptime, NULL );
		for( int i = 0; xvalues[i] != 0; i++ )
			XDrawLine( dpy, win, gcblack, 0,
				xvalues[i], 640, xvalues[i] );
		XFlush( dpy );
		nanosleep( &sleeptime, NULL );
	}
	*/
	cout << time << endl;

	XFlush( dpy );
	
	char exit;
	cin >> exit;
	
	return 0;
}
