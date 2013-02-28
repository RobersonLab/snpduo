// Standard
#include <string>

using namespace std;

#include "options.h"
#include "helper.h"

// i/o options
string par::pedfile = "null";
string par::mapfile = "null";
string par::file = "null";

string par::tpedfile = "null";
string par::tfamfile = "null";
string par::tfile = "null";

// string par::bpedfile = "null";
// string par::bmapfile = "null";
// string par::bfamfile = "null";
// string par::bfile = "null";

bool par::genome = false;
string par::genomefile = "null";

bool par::recode = false;
bool par::transpose = false;
// bool par::binary = false;

string par::outfile = "snpduo";
bool par::webduo = false;

// Screen printing
bool par::verbose = true;

// Primary analysis
bool par::counts = false;
bool par::summary = false;
bool par::specified = false;
bool par::calculated = false;
bool par::conflicting = false;

// Other options
// bool par::findblocks = false;
// bool par::makeucsc = false;
bool par::map3 = false;
bool par::version = false;

int par::snpcount = 0;
int par::personcount = 0;

void CheckMinimumInput()
{
	if (par::pedfile == "null" and par::tpedfile == "null"  and par::genomefile == "null" /*and par::bpedfile == "null"*/) error ("Must specify an input source");
	
	if ( (par::pedfile != "null" and (par::tpedfile != "null" or par::genomefile != "null")) or (par::tpedfile != "null" and par::genomefile != "null") )
	{
		error ("Cannot specify multiple input options. Please specify .ped/.map files, .tped/.tfam files, or a .genome file");
	}
	
	if (!par::counts and !par::summary and !par::specified and !par::calculated and !par::conflicting and !par::recode) error("Must specify an analysis method or recoding option. Use '--help' for more info");
	
	if (par::genome and (par::conflicting or par::specified)) error("Cannot specify a PLINK genome file and any options requiring pedigree information (specified relationships or conflicting relationships)");
	
	if (par::transpose and par::webduo) error("Please only specify one recoding option (--recode, --recode --tranpose, or --recode --webduo)");
	
// 	if (par::transpose and par::binary) error("Please only specify one recoding option (--recode or --recode --transpose or --recode --binary)");
}
