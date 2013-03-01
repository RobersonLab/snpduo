// Standard Includes
#include <string>
#include <vector>
#include <iostream>

// Namespace
using namespace std;

// Custom Headers
#include "options.h"
#include "helper.h"

void setOptions (CArgs &arg)
{
	// File reading options
	if (arg.Search("--file"))
	{
		par::file = arg.parameters[ arg.Index("--file") ];
		
		if (par::pedfile != "null" or par::mapfile != "null") error ("Cannot specify a map or pedfile while using the --file option");
		
		par::pedfile = par::file + ".ped";
		par::mapfile = par::file + ".map";
		
		arg.clinelist.push_back("--file");
		arg.clinevalue.push_back(par::file);
	}
	
	if (arg.Search("--ped"))
	{
		if (par::file != "null") error ("Cannot specify --file and --ped at the same time");
		
		par::pedfile = arg.parameters[ arg.Index("--ped") ];
		
		arg.clinelist.push_back("--ped");
		arg.clinevalue.push_back(par::pedfile);
	}
	
	if (arg.Search("--map"))
	{
		if (par::file != "null") error ("Cannot specify --file and --map at the same time");
		
		par::mapfile = arg.parameters[ arg.Index("--map") ];
		
		arg.clinelist.push_back("--map");
		arg.clinevalue.push_back( par::mapfile );
	}
	
	if (arg.Search("--tfile"))
	{
		par::tfile = arg.parameters[ arg.Index("--tfile") ];
		
		if (par::tpedfile != "null" or par::tfamfile != "null") error ("Cannot specify a tped file or tfam file while using the --tfile option");
		
		par::tpedfile = par::tfile + ".tped";
		par::tfamfile = par::tfile + ".tfam";
		
		arg.clinelist.push_back("--tfile");
		arg.clinevalue.push_back( par::tfile );
	}
	
	if (arg.Search("--tped"))
	{
		if (par::tfile != "null") error ("Cannot specify --tfile and --tped at the same time");
		
		par::tpedfile = arg.parameters[ arg.Index("--tped") ];
		
		arg.clinelist.push_back("--tped");
		arg.clinevalue.push_back(par::tfamfile);
	}
	
	if (arg.Search("--tfam"))
	{
		if (par::tfile != "null") error ("Cannot specify --tfile and --tfam at the same time");
		
		par::tfamfile = arg.parameters[ arg.Index("--tfam") ];
		
		arg.clinelist.push_back("--tfam");
		arg.clinevalue.push_back( par::tfamfile );
	}
	
// 	if (arg.Search("--bfile"))
// 	{
// 		par::bfile = arg.parameters[ arg.Index("--bfile") ];
// 		
// 		if (par::bpedfile != "null" or par::bmapfile != "null") error ("Cannot specify a bped or bmap file separately when using --bfile option");
// 		
// 		par::bpedfile = par::bfile + ".bped";
// 		par::bmapfile = par::bfile + ".bmap";
// 		par::bfamfile = par::bfile + ".bfam";
// 		
// 		arg.clinelist.push_back("--bfile");
// 		arg.clinevalue.push_back(par::bfile);
// 	}
// 	
// 	if (arg.Search("--bped"))
// 	{
// 		if (par::bfile != "null") error ("Cannot specify --bfile and --bped at the same time");
// 		
// 		par::bpedfile = arg.parameters[ arg.Index("--bped") ];
// 		
// 		arg.clinelist.push_back("--bped");
// 		arg.clinevalue.push_back(par::bpedfile);
// 	}
// 	
// 	if (arg.Search("--bmap"))
// 	{
// 		if (par::bfile != "null") error ("Cannot specify --bfile and --bmap at the same time");
// 		
// 		par::bmapfile = arg.parameters[ arg.Index("--bmap") ];
// 		
// 		arg.clinelist.push_back("--bmap");
// 		arg.clinevalue.push_back(par::bmapfile);
// 	}
// 	
// 	if (arg.Search("--bfam"))
// 	{
// 		if (par::bfile != "null") error ("Cannot specify --bfile and --bfam at the same time");
// 	}
	
	// File writing options
	if (arg.Search("--recode"))
	{
		par::recode = true;
		
		arg.clinelist.push_back("--recode");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--transpose"))
	{
		par::transpose = true;
		
		arg.clinelist.push_back("--transpose");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--webduo"))
	{
		par::webduo = true;
		
		arg.clinelist.push_back("--webduo");
		arg.clinevalue.push_back(" ");
	}
	
// 	if (arg.Search("--binary"))
// 	{
// 		par::binary = true;
// 		
// 		arg.clinelist.push_back("--binary");
// 		arg.clinevalue.push_back(" ");
// 	}

	if (arg.Search("--genome"))
	{
		par::genome = true;
		
		par::genomefile = arg.parameters[ arg.Index("--genome") ];
		
		arg.clinelist.push_back("--genome");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--out"))
	{
		par::outfile = arg.parameters[ arg.Index("--out") ];
		
		arg.clinelist.push_back("--out");
		arg.clinevalue.push_back(par::outfile);
	}
	
// 	// Analysis options	
// 	if (arg.Search("--findblocks"))
// 	{
// 		par::findblocks = true;
// 		
// 		arg.clinelist.push_back("--findblocks");
// 		arg.clinevalue.push_back(" ");
// 	}
// 	
// 	if (arg.Search("--makeucsc"))
// 	{
// 		par::makeucsc = true;
// 		
// 		arg.clinelist.push_back("--makeucsc");
// 		arg.clinevalue.push_back(" ");
// 	}
	
	if (arg.Search("--map3"))
	{
		par::map3 = true;
		
		arg.clinelist.push_back("--map3");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--silent"))
	{
		par::verbose = false;
		
		arg.clinelist.push_back("--silent");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--version"))
	{
		par::version = true;
		
		arg.clinelist.push_back("--version");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--counts"))
	{
		par::counts = true;
		
		arg.clinelist.push_back("--counts");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--summary"))
	{
		par::summary = true;
		
		arg.clinelist.push_back("--summary");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--specified"))
	{
		par::specified = true;
		
		arg.clinelist.push_back("--specified");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--calculated"))
	{
		par::calculated = true;
		
		arg.clinelist.push_back("--calculated");
		arg.clinevalue.push_back(" ");
	}
	
	if (arg.Search("--conflicting"))
	{
		par::conflicting = true;
		
		arg.clinelist.push_back("--conflicting");
		arg.clinevalue.push_back(" ");
	}
	
	// Help data 
	if (arg.Search("--help") or arg.Search("-h") or arg.parametercount == 1)
	{
		cout << ("\n"
		"snpduo --file <fileroot>    Specify .ped and .map files\n"
		"       --ped <ped file>     Specify .ped file name (requires --map)\n"
		"       --map <map file>     Specify .map file name\n"
		"       --tfile <fileroot>   Specify .tfam and .tped files\n"
		"       --tped <tped file>   Specify .tped file name (requires --tfam)\n"
		"       --tfam <tfam file>   Specify .tfam file name\n"
		"       --genome <filename>  Specify a PLINK .genome file for input\n"
		"                            **Warning: Cannot get expected relationships from this file\n"
		"\n"
		"       --out <fileroot>     Specify the root name for output files\n"
		"\n"
		"       --counts             Prints IBS 0,1,2 counts to .count file\n"
		"       --summary            Prints counts + Mean and Standard Deviation of IBS to .summary file\n"
		"       --specified          Prints relationships specified in input to .specified file\n"
		"       --calculated         Prints calculated relationships from Mean and SD to .theoretical file\n"
		"       --conflicting        Prints comparisons where specified relationship is different from calculated\n"
		"                            relationship to .conflicting file\n"
// 		"\n"
// 		"       --findblocks         Find IBS blocks\n"
// 		"       --makeucsc           Turn blocks into UCSC track (.bed)\n"
		"\n"
		"       --map3               Specifies no genetic distance column\n"
		"\n"
		"       --recode             Recodes data into ped/map files when used alone\n"
		"       --transpose          When used with --recode outputs a tped/tfam files\n"
		"       --webduo             When used with --recode outputs file compatible with\n"
		"                            the web version of SNPduo (Custom format)\n"
		"                            **Warning: Uses A/B coding but will NOT be the same\n"
		"                              as the platform's A/B coding scheme. Only for Web SNPduo!\n"
		"\n"
		"       --silent             No messages printed to screen\n"
		"       --version            Print version information and exit\n"
		"\n"
		"       --help / -h          Invokes this help menu\n");
		
		shutdown();
	}
}
