#ifndef _OPTIONS_H_
#define _OPTIONS_H_

// stl
#include <string>

// imports
using std::string;

class par
{
	public:
	// i/o related
	// input
	static string pedFile;
	static string mapFile;
	static string file;
	
	static string tpedFile;
	static string tfamfile;
	static string tfile;

	static string genomeFile;
	static bool genome;
	
	// output
	static bool recode;
	static bool transpose;
	static bool webDuo;
	
	static string out;
	
	// screen messages
	static bool verbose;
	
	// primary analysis
	static bool counts;
	static bool summary;
	static bool specified;
	static bool calculated;
	static bool oldCalculated;
	static bool conflicting;
	
	// Other options
	static bool map3;
	
 	static int snpCount;
};

void checkMinimumInput();

#endif
