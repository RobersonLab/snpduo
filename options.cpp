// stl
#include <string>

// imports
using std::string;

// custom headers
#include "options.h"
#include "helper.h"

// i/o options
string par::pedFile = "null";
string par::mapFile = "null";
string par::file = "null";

string par::tpedFile = "null";
string par::tfamfile = "null";
string par::tfile = "null";

bool par::genome = false;
string par::genomeFile = "null";

bool par::recode = false;
bool par::transpose = false;

string par::out = "snpduo";
bool par::webDuo = false;

// Screen printing
bool par::verbose = true;

// Primary analysis
bool par::counts = false;
bool par::summary = false;
bool par::specified = false;
bool par::calculated = false;
bool par::oldCalculated = false;
bool par::conflicting = false;

// Other options
bool par::map3 = false;

int par::snpCount = 0;

void checkMinimumInput()
{
	if (par::pedFile == "null" and par::tpedFile == "null"  and par::genomeFile == "null" /*and par::bpedFile == "null"*/) error ("Must specify an input source");
	
	if ( (par::pedFile != "null" and (par::tpedFile != "null" or par::genomeFile != "null")) or (par::tpedFile != "null" and par::genomeFile != "null") )
	{
		error ("Cannot specify multiple input options. Please specify .ped/.map files, .tped/.tfam files, or a .genome file");
	}
	
	if (!par::counts and !par::summary and !par::specified and !par::calculated and !par::conflicting and !par::recode) error("Must specify an analysis method or recoding option. Use '--help' for more info");
	
	if (par::genome and (par::conflicting or par::specified)) error("Cannot specify a PLINK genome file and any options requiring Pedigree information (specified relationships or conflicting relationships)");
	
	if (par::calculated and par::oldCalculated) error("Cannot perform relationship calculation by both old and new method simultaneously\nPlease specifiy --calculated OR --oldCalculated");
	
	if (par::transpose and par::webDuo) error("Please only specify one recoding option (--recode, --recode --tranpose, or --recode --webDuo)");
}
