#ifndef _HELPER_H_
#define _HELPER_H_

// stl
#include <fstream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>

// imports
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

////////////////
//  constants //
////////////////
// program returns
int const RETURN_GOOD = 0;
int const RETURN_ERROR = 1;

// program information
const string VERSION = "2.00"; // 4 char
const string RELEASE = "a"; // 1 char. Blank for full release. a for alpha. b for beta
const string DATE = "2011/Feb/15"; // 10 char. In YYYY/MMM/DD format
const string CRIGHTLINE = "(c) 2007-2011 Roberson and Pevsner"; // copyright blurb
const string PROGNAME = "snpduo"; // program name

// numeric coding/decoding for variables
const string RELATIONSHIP[] = {"Unrelated","Identical","ParentChild","Sibling","OtherRelated"};
const string CHROM_STRINGS[] = {"NULL", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "X", "Y", "XY", "M"};
int const XINT = 23;
int const YINT = 24;
int const XYINT = 25;
int const MTINT = 26;

// for creating associative array key (separates fid and iid)
const string UNIQUEIDSEP = "%%";

///////////////
// Functions //
///////////////
// miscellaneous error and splash
void error( string const&  );
void screenError( string const&  );
void shutdown();
void writeHeader();

// misc helper functions
inline string makeUniqueId( string const& fid, string const& iid )
{
	string s = fid;
	s += UNIQUEIDSEP;
	s += iid;
	return s;
}

// file reading
bool readString( ifstream &, string & );
bool readWhiteSpacePaddedString( ifstream &, string &, char & );
bool readChar( ifstream &, char & );
void readCommentLine( ifstream & );

// conversions
int string2Int( string const&  );
double string2Double( string const&  );
string string2Filename( string const&  );

// templated fillVector
// ATTN may change back to just a resize and push_back, relying on the other code to treat it correctly
template <class T>
void fillVector( vector<T> &v, const T& t, int i )
{
	v.reserve( i );
	if (v.size())
	{
		for (int j=0; j < i; ++j)
		{
			v[j] = t;
		}
	}
	else
	{
		for (int j=0; j < i; ++j)
		{
			v.push_back(t);
		}
	}
}

// templated convert to string
template <class T>
string convertToString( const T& t )
{
	stringstream ss;
	if ( (ss << t).fail() )
	{
		string s = typeid(t).name();
		error("Program died converting " + s + " to string");
	}
	return ss.str();
};

#endif
