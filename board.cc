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

int main( int argc, char ** argv)
{
	Display * dpy;
	Window win;
	XSetWindowAttributes xswa;
	XGCValues gcvalues;
	GC gc;

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

	gcvalues.foreground = white;
	gcvalues.background = black;
	gcvalues.line_width = 1;

	win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10, 10, 640, 480, 1,
		CopyFromParent, CopyFromParent, CopyFromParent,
		CWBackPixel | CWBorderPixel | CWOverrideRedirect
		| CWColormap , &xswa);
	XMapWindow( dpy, win);

	gc = XCreateGC( dpy, win, GCForeground | GCBackground | GCLineWidth,
		&gcvalues );
	
	XClearWindow( dpy, win);
	XFlush( dpy );
	for( int x = 0; x < 600; x+=10)
	{
		XDrawLine( dpy, win, gc, x, 0, x, 480 );
		XFlush( dpy );
	}
	for( int y = 0; y < 400; y+=10)
	{
		XDrawLine( dpy, win, gc, 0, y, 640, y );
		XFlush( dpy );
	}
	XFlush( dpy );
	
	sleep(10);
	
	return 0;
}
