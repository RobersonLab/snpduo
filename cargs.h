#ifndef _CARGS_H_
#define _CARGS_H_

// stl
#include <string>
#include <vector>

// namespace imports
using std::vector;
using std::string;

// classes
class CArgs
{
	private:
		// Class Members
		unsigned int parameterCount;
		vector<string> parameters;
		vector<bool> parsed;
		vector<string> commandLineList;
		vector<string> commandLineValue;
	
	public:
		CArgs( int, char** );
		
		// Functions
		void addParameter( string const& s ) { parameters.push_back( s ); }
		void addToList( string const& s ) { commandLineList.push_back( s ); }
		void addToValue( string const& s ) { commandLineValue.push_back( s ); }
		bool search( string const&  );
		int valueIndex( string const&  );
		void validity();
		void parse();
		
		// accessors
		unsigned int getParCount() { return parameterCount; }
		unsigned int commandCount() { return commandLineList.size(); }
		
		// Writing functions
		void write();
};

#endif
