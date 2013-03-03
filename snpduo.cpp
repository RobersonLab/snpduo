// stl
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

// imports
using std::cout;
using std::ofstream;
using std::string;
using std::time_t;

// Custom includes
#include "cargs.h"
#include "duo.h"
#include "helper.h"
#include "input.h"
#include "options.h"
#include "output.h"
#include "ped.h"

// Externalized LOG declaration
ofstream LOG;

// Functions
int main (int argc, char *argv[])
{
	// Set up variables to time the program
	time_t sysTime;
	
	// Set Options before continuing
	CArgs options(argc, argv);
	options.parse();
	options.validity();
	checkMinimumInput();
	
	LOG.open( string(par::out + ".log").c_str() );
	if (!LOG.good()) { screenError("Unable to write log file " + string2Filename( par::out + ".log")); }
	
	// show splash screen
	writeHeader();
	// Print log text
	writeLog( "Logging text to " + string2Filename(par::out + ".log") + "\n" );
	options.write(); // write options to screen
	
	// initialize variables
	DuoMap myDuo;
	LocusMap myMap;
	Ped myPed;
	
	// keep track of analysis time
	sysTime = time( 0 );
	writeLog( "\nAnalysis started: " + convertToString( ctime(&sysTime) ) + "\n");
	
	// file import
	if (par::pedFile != "null")
	{
		myMap.read();
		myPed.read(myMap);
	}
	else if (par::tpedFile != "null")
	{
		readTpedFile( myPed, myMap );
	}
	else if (par::genomeFile != "null")
	{
		readPlinkGenome( myDuo, myPed );
	}
	
	// file recoding
	if (par::recode and par::genomeFile == "null")
	{
		if (par::transpose) 
		{
			writeTranspose( myPed, myMap );
		}
		else if (par::webDuo)
		{
			writeForWeb( myPed, myMap );
		}
		else
		{
			myPed.write(myMap);
			myMap.write();
		}
	}
	
	// calculate everything first. print results last
	// get ibs counts
	if ((par::counts or par::summary or par::calculated or par::conflicting) and !par::genome) { myDuo.getCounts( myPed ); } // ibs counts
	if (par::summary or par::calculated or par::conflicting) { myDuo.getMeanAndSDFromCounts(); } // mean and sd of ibs
	if ( (par::specified or par::calculated or par::conflicting) and !par::genome) { myDuo.specifiedRelationships( myPed ); } // parse known info
	if (par::oldCalculated and par::conflicting) { myDuo.oldCalculatedRelationships(); } // calculate relationships -- old algorithm
	else if (par::calculated or par::conflicting) { myDuo.calculatedRelationships(); } // calculate relationships -- new algorithm
	
	// print results
	if (par::counts) { myDuo.printCounts(); }
	if (par::summary) { myDuo.printMeanSD(); }
	if (par::specified and not par::genome) { myDuo.printRelationships(); }
	if (par::calculated and not par::genome) { myDuo.printSpecifiedAndCalculated(); }
	if (par::calculated and par::genome) { myDuo.printCalculatedOnly(); }
	if (par::conflicting and !par::genome) { myDuo.printConflicted(); }
	
	// when did it end?
	sysTime = time( 0 );
	writeLog( "\nAnalysis ended: " + convertToString( ctime( &sysTime ) ));
	
	return RETURN_GOOD; // constant string from helper.h
}
