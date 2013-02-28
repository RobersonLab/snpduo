#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>

class par
{
	public:
	// i/o related
	// input
	static string pedfile;
	static string mapfile;
	static string file;
	
	static string tpedfile;
	static string tfamfile;
	static string tfile;
	
// 	static string bpedfile;
// 	static string bmapfile;
// 	static string bfamfile;
// 	static string bfile;

	static string genomefile;
	static bool genome;
	
	// output
	static bool recode;
	static bool transpose;
// 	static bool binary;
	static bool webduo;
	
	static string outfile;
	
	// screen messages
	static bool verbose;
	
	// primary analysis
	static bool counts;
	static bool summary;
	static bool specified;
	static bool calculated;
	static bool conflicting;
	
	// Other options
// 	static bool findblocks;
// 	static bool makeucsc;
	static bool map3;
	static bool version;
	
 	static int snpcount;
 	static int personcount;
};


void CheckMinimumInput();

#endif
