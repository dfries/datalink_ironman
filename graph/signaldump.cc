#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <iomanip.h>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#define ExitOnTrue( file, msg ) if(file){ cout << msg ;\
	cout << __FUNCTION__ << "() " << __FILE__ << ':' << __LINE__ << endl;\
	exit(1); }

// invert the graph and make it positive so it is nicer to work with
void makenice( short buf[], int nice[], int size)
{
	int i;
	for( i = 0; i < size; i++)
		nice[i] = -buf[i]+32768;
}

void dumpdata( int buf[], int size )
{
	int i;
	cout.setf(ios::right);
	cout << setw(4) << "#" << setw(6) << "sum"
		<< setw(7) << "value" << setw(6) << "diff" << endl;
	// zero is a special case
	cout << setw(4) << 0 << setw(7) << buf[0] << endl;
	// stop early so we don't overrun the buffers
	for( i = 1; i < size-2; i++)
	{
		cout
		// display sample number
		<< setw(4) << i
		// display sum of this and next two samples
		<< setw(6) << (buf[i+2]-buf[i-1])
		// display the sample
		<< setw(7) << buf[i] 
		// display difference between this one and last one
		<< setw(6) << (buf[i] - buf[i-1])
		<< endl;
	}
	cout << setw(4) << size-2 << "      " << setw(7) << buf[size-2]
		<< setw(6) << (buf[size-2] - buf[size-2-1]) << endl;
	cout << setw(4) << size-1 << "      " << setw(7) << buf[size-1]
		<< setw(6) << (buf[size-1] - buf[size-2-1]) << endl;
}

const int minpeakvalue = 250;
const int minbitspacing = 18;
const double averagespacing = 21.75;
const int maxbytelength = 200;
const int framelength = 737;
// find first peak that is over 250 high, using the diff as the sum of
// the change from the previous sample of three samples
// return -1 if the peak was not found
int findpeak( int buf[], int start, int end )
{
	int diff;
	int i;
	for( i = start; i < end-3; i++)
	{
		diff = buf[i+3]-buf[i];
		if( diff > minpeakvalue )
			return i+1;
	}
	return -1;
}

int findsignal( int buf[], int size )
{
	int diff;
	int i;
	int lastfail = 0;

	for( i = 0; i < size-3; i++)
	{
		diff = buf[i+3]-buf[i];
		if( diff > minpeakvalue )
		{
			if( (i-3)-(maxbytelength+50) > lastfail )
				return i+1;
			lastfail = i;
		}
	}
	return -1;
}

// round, >= .5 up, else down
int round( double value)
{
	int integer = (int)floor(value);
	if( value - integer >= .5)
		return integer+1;
	return integer;
}

unsigned char decodebyte( int buf[], int bytestart )
{
	int location = bytestart;
	int i;
	unsigned char byte = 0;
	int bit;
	for( i = 0; i < 8 ; i++ )
	{
		location = findpeak( buf, location+minbitspacing,
			bytestart+maxbytelength);
		// next peak not found, return what we have
		if(location == -1 )
			return byte;
		bit = round( (location - bytestart)/averagespacing);
		byte |= ( 1<<(bit-1));
	}
	return byte;
}

// decode a frame worth of data
unsigned short decodeframe( int buf[], int &lastlocation, int size )
{
	unsigned short byte = 0;
	int location = findpeak( buf, lastlocation,
		lastlocation+2*maxbytelength);
	if( location == -1 )
	{
		lastlocation += framelength;
		return 0;
	}
	byte = (unsigned short) decodebyte( buf, location );
	location = findpeak( buf, location+maxbytelength,
		location+maxbytelength);
	lastlocation = location + maxbytelength;
	byte |= decodebyte( buf, location) << 8*sizeof(char);
	return byte;
}

void decodestream( int buf[], int size )
{
	int current = findsignal( buf, size );
	while( current < size-2*framelength )
	{
		cout << "0x" << setbase(16) << setw(4) << setfill('0')
			<< decodeframe( buf, current, size);
		cout << setbase(10) << ", position is now "
			<< current << endl;
	}
}

int main( int argc, char ** argv)
{
	if( argc < 2 || strcmp( *(argv+1), "--help") == 0 )
	{
		cout << "Usage: signaldump file offset amount\n";
		exit(1);
	}

	/*
	int offset = atoi( *(argv+2));
	if( offset%2 )
	{
		cout << "offset must be divisible by two, it was " << offset;
		cout << endl;
		exit(1);
	}
	*/

	//int amount = atoi(*(argv+3));
	short * buf;
	int * nice;
	int filesize;
	int infile = open( *(argv+1), O_RDONLY);
	ExitOnTrue( infile == -1, "Error opening file");

	filesize = lseek(infile, 0, SEEK_END );
	ExitOnTrue( filesize == -1, "Error opening file");
	// the file is signed short so there are two bytes per data item
	// so we only need half as many ints
	nice = new int[filesize/2];

	buf = (short*)mmap( 0, filesize, PROT_READ, MAP_SHARED, infile, 0);
	close(infile);

	makenice( buf, nice, filesize/2 );
	munmap( buf, filesize);

//	dumpdata( nice, filesize/2);
	decodestream( nice, filesize/2 );

	delete [] nice;
	return 0;
}
