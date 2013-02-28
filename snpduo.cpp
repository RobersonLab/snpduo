// Standard Includes
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>

// namespace
using namespace std;

// Custom includes
#include "snpduo.h"
#include "helper.h"
#include "options.h"
#include "input.h"
#include "Duo.h"
#include "output.h"

// Macros
const string VERSION = "1.00"; // 4 char
const string RELEASE = "b"; // 1 char. Blank for actual release. p for prelease or test. a for alpha. b for beta
const string DATE = "2009/Feb/18"; // 10 char. In YYYY/MM/DD format

// Externalized LOG declaration
ofstream LOG;

// Functions
int main (int argc, char *argv[])
{
	// Set up variables to time the program
	clock_t start_time, end_time;
	start_time = clock();
	
	// Set Options before continuing
	CArgs options(argc, argv);
	setOptions( options );
	
	if (par::version)
	{
		cout << "\nsnpduo v" + VERSION + RELEASE + "\n";
		shutdown();
	}
	
	LOG.open( string(par::outfile + ".log").c_str() );
	
	// Make objects
	Ped ped;
	Map map;
	Duo duo;
	stringstream ss;
	
	printLog("\n" 
	"|---------------------------------------|\n" 
	"|   snpduo   |   v"+VERSION+RELEASE+"   | "+DATE+" |\n" 
	"|---------------------------------------|\n" 
	"| (c) 2007-2009 Roberson and Pevsner    |\n" 
	"|---------------------------------------|\n\n");
	
	printLog("This text is being written to [ " + par::outfile + ".log" + " ]\n");
	
	options.CheckSwitches();
	CheckMinimumInput();
	
	options.printArgs();
	
	if (par::pedfile != "null")
	{
		readMapFile(map);
		readPedFile(ped);
	}
	else if (par::tpedfile != "null")
	{
		readTpedFile( ped, map );
	}
// 	else if (par::bpedfile != "null")
// 	{
// 		readBpedFile( ped, map);
// 		readBfamFile( ped );
// 		readBmapFile( ped, map );
// 	}
	else if (par::genomefile != "null")
	{
		readPLINKGenome( duo );
	}
	
	if (par::recode)
	{
		if (par::transpose) 
		{
			writeTranspose( ped, map );
		}
// 		else if (par::binary)
// 		{
// 			writeBinary( ped, map );
// 		}
		else if (par::webduo)
		{
			writeForWeb( ped, map );
		}
		else
		{
			ped.print();
			map.print();
		}
	}
	
	if ( (par::counts or par::summary or par::calculated or par::conflicting) and !par::genome) duo.getCounts( ped );
	if (par::counts) duo.printCounts();
	
	if (par::summary or par::calculated or par::conflicting) duo.getMeanAndSDFromCounts();
	if (par::summary) duo.printMeanSD();
	
	if ( (par::specified or par::calculated or par::conflicting) and !par::genome) duo.specifiedRelationships( ped );
	if (par::specified and !par::genome) duo.printRelationships();

	if (par::calculated or par::conflicting) duo.calculatedRelationships();
	if (par::calculated and !par::genome) duo.printSpecifiedAndCalculated();
	else if (par::calculated and par::genome) duo.printCalculatedOnly();
	
	if (par::conflicting and !par::genome) duo.printConflicted();
	
	end_time = clock();
	
	ss << (end_time - start_time) / CLOCKS_PER_SEC;
	
	printLog("\nAnalysis completed in " + ss.str() + " seconds\n");
	
	return 0;
}

//////////////////////////////////////////////////
// Ped functions. May make sense to put in a
// separate file so main functions are untainted
// by unrelated code
//////////////////////////////////////////////////
int Ped::findPerson(string &famID, string &indID)
{
	for (int i = 0; i < par::personcount; ++i)
	{
		if (samples[i]->fid == famID and samples[i]->iid == indID) return i;
	}
	
	error("Unable to find individual with family ID " + famID + " and individual ID " + indID);
}

string Ped::fileGenotypeString( int person, int snp )
{
	string s = " ";
	
	if (samples[person]->hasGenotype[snp])
	{
		if (samples[person]->a1[snp])
		{
			if (samples[person]->a2[snp])
			{
				s += allele2[snp];
				s += " ";
				s += allele2[snp];
			}
			else
			{
				if (allele1[snp] < allele2[snp])
				{
					s += allele1[snp];
					s += " ";
					s += allele2[snp];
				}
				else
				{
					s += allele2[snp];
					s += " ";
					s += allele1[snp];
				}
			}
		}
		else
		{
			if (samples[person]->a2[snp])
			{
				if (allele1[snp] < allele2[snp])
				{
					s += allele1[snp];
					s += " ";
					s += allele2[snp];
				}
				else
				{
					s += allele2[snp];
					s += " ";
					s += allele1[snp];
				}
			}
			else
			{
				s += allele1[snp];
				s += " ";
				s += allele1[snp];
			}
		}
		
		return s;
	}
	else return " 0 0";	
}
