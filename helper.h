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
	
	// Functions
	bool Search( string );
	int Index( string );
	void CheckSwitches();
	void print();
	void printArgs();
};

// Functions
void setOptions (CArgs &);
void error (string );
void shutdown();

bool getString (ifstream &, string &);
bool getWhiteSpacePaddedString (ifstream &, string &, char &);

#endif
