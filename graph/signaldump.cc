#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <iomanip.h>
#include <unistd.h>

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
	cout << setw(4) << "#" << setw(7) << "value" << setw(6) << "diff"
		<< setw(6) << "sum" << endl;
	// zero is a special case
	cout << setw(4) << 0 << setw(7) << buf[0] << endl;
	// stop early so we don't overrun the buffers
	for( i = 1; i < size-2; i++)
	{
		cout
		// display sample number
		<< setw(4) << i
		// display the sample
		<< setw(7) << buf[i] 
		// display difference between this one and last one
		<< setw(6) << (buf[i] - buf[i-1])
		// display sum of this and next two samples
		<< setw(6) << (buf[i+2]-buf[i-1])
		<< endl;
	}
	cout << setw(4) << size-2 << setw(7) << buf[size-2]
		<< setw(6) << (buf[size-2] - buf[size-2-1]) << endl;
	cout << setw(4) << size-1 << setw(7) << buf[size-1]
		<< setw(6) << (buf[size-1] - buf[size-2-1]) << endl;
}

int main( int argc, char ** argv)
{
	if( argc < 4 )
	{
		cout << "Usage: signaldump file offset amount\n";
		exit(1);
	}

	int offset = atoi( *(argv+2));
	if( offset%2 )
	{
		cout << "offset must be divisible by two, it was " << offset;
		cout << endl;
		exit(1);
	}
	int amount = atoi(*(argv+3));
	short * buf = new short[amount];
	int * nice = new int[amount];

	ifstream infile(*(argv+1));
	ExitOnTrue( !infile, "Error opening " << *(argv+1));
	infile.seekg( offset, ios::beg);
	ExitOnTrue( !infile, "Error seeking " << *(argv+1));
	infile.read( (char*)buf, 2*amount );
	ExitOnTrue( !infile, "Error reading " << *(argv+1));

	makenice( buf, nice, amount );
	dumpdata( nice, amount);

	delete [] buf;
	infile.close();
	return 0;
}
