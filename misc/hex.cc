#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>

int main( int argc, char **argv)
{
	ifstream input( *(argv+1) );
	int byte;
	int linefeed = 0;
	if( !input )
	{
		cerr << "Error\n";
		exit(1);
	}
	byte = input.get();
	while( !input.eof() )
	{
		cout << hex << setfill('0') << setw(2) << byte;
		if( linefeed )
		{
			cout << '\n';
			linefeed = 0;
		}
		else
		{
			linefeed = 1;
		}
		byte = input.get();
	}
	cout << endl;
	return 0;
}
