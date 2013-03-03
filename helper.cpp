// Standard Includes
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cerrno>
#include <sstream>
#include <cstdlib>
#include <string>

// Namespace
using std::cerr;
using std::cout;
using std::exit;
using std::ofstream;
using std::string;
using std::stringstream;

// Custom headers
#include "helper.h"
#include "output.h"

/////////////////////////////////////////////
// Accessory functions for handling errors //
/////////////////////////////////////////////
void error (string const& s)
{
	writeLog( "\n[Error] " +  s + "\n");
	
	exit( RETURN_ERROR );
}

void screenError (string const& s)
{
	cerr << "\n[screen only] Error: " << s << "\n";
	exit( RETURN_ERROR );
}

void shutdown()
{
	exit( RETURN_GOOD );
}

////////////////////////////////////////////////////////
// Nicely print the program information on invocation //
////////////////////////////////////////////////////////
void writeHeader()
{
	// size of copyright
	unsigned int cRightSize = CRIGHTLINE.length() + 2;
	
	// size of prog line
	unsigned int progLineSize = 2 + PROGNAME.length() + 3 + VERSION.length() + RELEASE.length() + 2 + DATE.length() + 2;
	// PROGNAME with space padding on internal side, VERSION & RELEASE space padded + 'v' for version
	// 2 padding + DATE and two additional chars for '|' separators
	
	unsigned int myLength = 0;
	
	if (cRightSize > progLineSize)
	{
		unsigned int myLength = cRightSize;
		
		// make spacer
		string spacer = "|";
		for (unsigned int i=0; i<myLength; ++i)
		{
			spacer += "-";
		}
		spacer += "|";
		
		// make pad
		string pad = "";
		for (unsigned int i = 0; i < (myLength - progLineSize); ++i)
		{
			pad += " ";
		}
		
		// writeLog in output. LOG declared in main file
		writeLog(
		"\n"
		"" + spacer + "\n"
		"| " + PROGNAME + " | v" + VERSION + RELEASE + " | " + DATE + pad + " |\n"
		"" + spacer + "\n"
		"| " + CRIGHTLINE + " |\n"
		"" + spacer + "\n\n");
	}
	else
	{
		myLength = progLineSize;
		
		// make spacer
		string spacer = "|";
		for (unsigned int i=0; i<myLength; ++i)
		{
			spacer += "-";
		}
		spacer += "|";
		
		// make pad
		string pad = "";
		for (unsigned int i = 0; i < (myLength - cRightSize); ++i)
		{
			pad += " ";
		}
		
		// writeLog in output. LOG declared in main file
		writeLog(
		"\n"
		"" + spacer + "\n"
		"| " + PROGNAME + " | v" + VERSION + RELEASE + " | " + DATE + " |\n"
		"" + spacer + "\n"
		"| " + CRIGHTLINE + pad + " |\n"
		"" + spacer + "\n\n");
	}
}

//////////////////////////////////////////////////
// Accessory function for file reading:         //
// Reads a character at a time. Puts characters //
// together in a string and returns a bool for  //
// whether a string was actually read           //
//////////////////////////////////////////////////
bool readString (ifstream& f, string& s)
{
	s = "";
	char c = ' ';
	
	while (!f.eof())
	{
		f.get( c );
		
		if (c == ' ' or c == '\t' or c == '\n' or f.eof()) { break; }
		else if (c == '\r') { continue; }
		else { s += c; }
	}
	
	return s.length() != 0 ? true : false;
}

bool readWhiteSpacePaddedString( ifstream& f, string& s, char& c )
{
	s = "";
	c = ' ';
	
	while (!f.eof())
	{
		f.get( c );
		
		if (c == '\n' or f.eof()) { break; }
		
		else if (c == '\r') { continue; }
		
		else if (c == ' ' or c == '\t')
		{
			if (s.length() != 0) { return true; } // short circuits to return b/c a break would perform the same test again
			else { continue; }
		}
		
		else { s += c; }
	}
	
	return s.length() != 0 ? true : false;
}

bool readChar( ifstream& f, char& c )
{
	c = ' ';
	while (!f.eof())
	{
		f.get( c );
		
		if (c == '\n' or f.eof()) { return false; }
		else if (c == ' ' or c == '\t' or c == '\r') { continue; }
		else { return true; }
	}
	return false;
}

void readCommentLine( ifstream& f )
{
	char c = ' ';
	while (c != '\n' and !f.eof()) { f.get( c ); }
}

int string2Int( string const& s )
{
	stringstream ss( s );
	int i = 0;
	if ( (ss >> i).fail() ) error( "Program died converting string to int" );
	return i;
}

double string2Double( string const& s )
{
	stringstream ss( s );
	double d = 0.0;
	if ( (ss >> d).fail() ) error( "Program died converting string to double" );
	return d;
}

string string2Filename( string const& s )
{
	return "[ " + s + " ]";
}
