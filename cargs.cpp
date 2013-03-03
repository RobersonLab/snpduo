// Standard Includes
#include <iostream>
#include <string>

// Namespace
using std::cout;
using std::string;

// Custom headers
#include "cargs.h"
#include "helper.h"
#include "options.h"
#include "output.h"

// Functions
CArgs::CArgs( int n, char *params[] ): parameterCount( n )
{
	fillVector(parsed, false, n);
	parameters.reserve( n );
	
	parsed[0] = true; // Filename automatically returned
	
	for (int i = 0; i < n; ++i) { addParameter( params[i] ); }
}

void CArgs::validity()
{
	vector<int> badOptions;
	
	for (unsigned int i = 0; i < getParCount(); ++i)
	{
		if (parsed[i] == false) { badOptions.push_back( i ); }
	}
	
	if (badOptions.size())
	{
		string badopt = "Unrecognized command-line switch(es):\n";
		for (unsigned int i = 0; i < badOptions.size(); ++i)
		{
			badopt += parameters[ badOptions[i] ] + "\n";
		}
		badopt += "Bad command-line options selected";
		
		screenError( badopt );
	}
}

bool CArgs::search( string const& s )
{
	for (unsigned int i = 0; i < getParCount(); ++i)
	{
		if (parameters[i] == s)
		{
			parsed[i] = true;
			return true;
		}
	}
	return false;
}

int CArgs::valueIndex( string const& s )
{
	for (unsigned int i = 0; i < getParCount(); ++i)
	{
		if (parameters[i] == s and i + 1 < getParCount())
		{
			parsed[i + 1] = true;
			return i + 1;
		}
	}
	
	screenError( "Missing argument for command [" + s + "]" );
	
	return -10;
}

void CArgs::write()
{
	writeLog( "Command-line Options Set:\n" );
	for (unsigned int i = 0; i < commandCount(); ++i)
	{
		writeLog( commandLineList[i] + " " + commandLineValue[i] + "\n");
	}
	
	writeLog("\n");
}

void CArgs::parse()
{
	// Help data 
	if (search("--help") or search("-h") or getParCount() == 1)
	{
		cout << "\nsnpduo v" + VERSION + RELEASE + "\n" + DATE + "\n\n";
		
		cout << ("\n"
		"snpduo --file <fileroot>    Specify .ped and .map files\n"
		"       --ped <ped file>     Specify .ped file name (requires --map)\n"
		"       --map <map file>     Specify .map file name\n"
		"       --tfile <fileroot>   Specify .tfam and .tped files\n"
		"       --tped <tped file>   Specify .tped file name (requires --tfam)\n"
		"       --tfam <tfam file>   Specify .tfam file name\n"
		"       --genome <filename>  Specify a PLINK .genome file for input\n"
		"                            **Warning: Cannot get expected relationships from\n"
		"                              this file\n"
		"\n"
		"       --out <fileroot>     Specify the root name for output files\n"
		"\n"
		"       --counts             Prints IBS 0,1,2 and 2* (AB->AB) counts to .count\n"
		"                            file\n"
		"       --summary            Prints counts + Mean and Standard Deviation of IBS\n"
		"                            and informative percents to .summary file\n"
		"       --specified          Prints relationships specified in input to\n"
		"                            the .specified file\n"
		"       --calculated         Prints the calculated relationships using IBS*,\n"
		"                            Mean, and SD to .theoretical file\n"
		"       --oldcalculated      Prints calculated relationships from Mean and SD to\n"
		"                            .theoretical file (deprecated)*\n"
		"       --conflicting        Prints comparisons where specified relationship is\n"
		"                            different from calculated\n"
		"                            relationship to .conflicting file\n"
		"                            * Note: to get conflicting relationships by the old\n"
		"                            algorithm you MUST specify --oldcalculated AND\n"
		"                            --conflicting\n"
		"\n"
		"       --map3               Specifies no genetic distance column\n"
		"\n"
		"       --recode             Recodes data into ped/map files when used alone\n"
		"       --transpose          When used with --recode outputs a tped/tfam files\n"
		"       --webDuo             When used with --recode outputs file compatible with\n"
		"                            the web version of SNPDuo (Custom format)\n"
		"                            **Warning: Uses A/B coding but will NOT be the same\n"
		"                              as the platform's A/B coding scheme. Only for\n"
		"                              Web SNPDuo!\n"
		"\n"
		"       --silent             No messages printed to screen\n"
		"       --version            Print version information and exit\n"
		"\n"
		"       --help / -h          Invokes this help menu\n");
		
		shutdown();
	}
	
	// Print version information and exit
	if (search("--version"))
	{
		cout << "\nsnpduo v" + VERSION + RELEASE + "\n" + DATE + "\n\n";
		shutdown();
	}
	
	// File reading options
	if (search("--file"))
	{
		par::file = parameters[ valueIndex("--file") ];
		
		if (par::pedFile != "null" or par::mapFile != "null") error ("Cannot specify a map or pedFile while using the --file option");
		
		par::pedFile = par::file + ".ped";
		par::mapFile = par::file + ".map";
		
		addToList("--file");
		addToValue(par::file);
	}
	
	if (search("--ped"))
	{
		if (par::file != "null") error ("Cannot specify --file and --ped at the same time");
		
		par::pedFile = parameters[ valueIndex("--ped") ];
		
		addToList("--ped");
		addToValue(par::pedFile);
	}
	
	if (search("--map"))
	{
		if (par::file != "null") error ("Cannot specify --file and --map at the same time");
		
		par::mapFile = parameters[ valueIndex("--map") ];
		
		addToList("--map");
		addToValue( par::mapFile );
	}
	
	if (search("--tfile"))
	{
		par::tfile = parameters[ valueIndex("--tfile") ];
		
		if (par::tpedFile != "null" or par::tfamfile != "null") error ("Cannot specify a tped file or tfam file while using the --tfile option");
		
		par::tpedFile = par::tfile + ".tped";
		par::tfamfile = par::tfile + ".tfam";
		
		addToList("--tfile");
		addToValue( par::tfile );
	}
	
	if (search("--tped"))
	{
		if (par::tfile != "null") error ("Cannot specify --tfile and --tped at the same time");
		
		par::tpedFile = parameters[ valueIndex("--tped") ];
		
		addToList("--tped");
		addToValue(par::tfamfile);
	}
	
	if (search("--tfam"))
	{
		if (par::tfile != "null") error ("Cannot specify --tfile and --tfam at the same time");
		
		par::tfamfile = parameters[ valueIndex("--tfam") ];
		
		addToList("--tfam");
		addToValue( par::tfamfile );
	}
	
	// File writing options
	if (search("--recode"))
	{
		par::recode = true;
		
		addToList("--recode");
		addToValue(" ");
	}
	
	if (search("--transpose"))
	{
		par::transpose = true;
		
		addToList("--transpose");
		addToValue(" ");
	}
	
	if (search("--webDuo"))
	{
		par::webDuo = true;
		
		addToList("--webDuo");
		addToValue(" ");
	}

	if (search("--genome"))
	{
		par::genome = true;
		
		par::genomeFile = parameters[ valueIndex("--genome") ];
		
		addToList("--genome");
		addToValue(" ");
	}
	
	if (search("--out"))
	{
		par::out = parameters[ valueIndex("--out") ];
		
		addToList("--out");
		addToValue(par::out);
	}
	
	if (search("--map3"))
	{
		par::map3 = true;
		
		addToList("--map3");
		addToValue(" ");
	}
	
	if (search("--silent"))
	{
		par::verbose = false;
		
		addToList("--silent");
		addToValue(" ");
	}
	
	if (search("--counts"))
	{
		par::counts = true;
		
		addToList("--counts");
		addToValue(" ");
	}
	
	if (search("--summary"))
	{
		par::summary = true;
		
		addToList("--summary");
		addToValue(" ");
	}
	
	if (search("--specified"))
	{
		par::specified = true;
		
		addToList("--specified");
		addToValue(" ");
	}
	
	if (search("--calculated"))
	{
		par::calculated = true;
		
		addToList("--calculated");
		addToValue(" ");
	}
	
	if (search("--oldcalculated"))
	{
		par::oldCalculated = true;
		
		addToList("--oldCalculated");
		addToValue(" ");
	}
	
	if (search("--conflicting"))
	{
		par::conflicting = true;
		
		addToList("--conflicting");
		addToValue(" ");
	}
}
