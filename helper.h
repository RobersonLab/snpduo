#ifndef _HELPER_H_
#define _HELPER_H_

// Standard Libraries
#include <vector>
#include <string>

// Classes
class CArgs
{
	public:
	CArgs(int, char**);
	
	// Class Members
	int parametercount;
	vector<string> parameters;
	vector<bool> parsed;
	vector<string> clinelist;
	vector<string> clinevalue;
	
	// Search functions
	bool Search( string );
	int Index( string );
	
	// Consistency check
	void CheckSwitches();
	
	// Writing functions
	void print();
	void printArgs();
	
	// Sizing functions
	int size() { return parametercount; }
	unsigned int clineSize() { return clinelist.size(); }
	
	// Constructor
	CArgs(): parametercount( 0 ), parameters( 0 ), parsed( 0 ), clinelist( 0 ), clinevalue( 0 ) {}
};

// Functions
void setOptions (CArgs &);
void error (string );
void shutdown();

bool getString (ifstream &, string &);
bool getWhiteSpacePaddedString (ifstream &, string &, char &);
bool getChar (ifstream &, char &);

string int2String( const int );
string dbl2String( const double );

#endif
