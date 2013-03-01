// Standard Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <bitset>

// Namespace
using namespace std;

// Custom headers
#include "options.h"
#include "duo.h"
#include "snpduo.h"
#include "input.h"  // Gives chromosome macros

// RELATIONSHIP const string comes from duo.h
// Used to maintain flexibility in adding relationship classes

// LOG declared in snpduo.cpp
extern ofstream LOG;

const int PRECISE = 7; // used to specify precision for output

//////////////////////////////////////////////////
//
// Log function
//
//////////////////////////////////////////////////
void printLog( string s )
{
	LOG << s;
	LOG.flush();
	
	if (par::verbose)
	{
		cout << s;
		cout.flush();
	}
}

//////////////////////////////////////////////////
//
// Duo Class output functions
//
//////////////////////////////////////////////////
void Duo::printCounts()
{
	string file = par::outfile + ".count";
	
	ofstream COUNTS;
	COUNTS.open( file.c_str(), ios::out );
	
	if ( !COUNTS.is_open() or !COUNTS.good() ) error("Could not create count file [ " + file + " ]");
	
	printLog( "Writing pair-wise IBS counts to [ " + file + " ]\n");
	
	COUNTS << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*" << "\n";
	
	for (unsigned int i = 0; i < numCounts(); ++i)
	{		
		COUNTS 
		<< ind1Index[i]->fid << "," << ind1Index[i]->iid << "," 
		<< ind2Index[i]->fid << "," << ind2Index[i]->iid << "," 
		<< ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "," << ibs2starCount[i] << "\n";
	}
	
	COUNTS.close();
}

void Duo::printMeanSD()
{
	string file = par::outfile + ".summary";
	
	ofstream MEANSD;
	MEANSD.open( file.c_str(), ios::out );
	
	if (!MEANSD.is_open() or !MEANSD.good()) error( "Could not create Mean / SD file [ " + file + " ]\n" );
	
	MEANSD.setf( ios::fixed );
	
	printLog( "Writing pair-wise Mean and SD summary information to [ " + file + " ]\n" );
	
	MEANSD.setf( ios::fixed );
	
	MEANSD << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%" << "\n";
	
	for (unsigned int i = 0; i < numMean(); ++i)
	{
		MEANSD
		 << ind1Index[i]->fid << "," << ind1Index[i]->iid << ","
		 << ind2Index[i]->fid << "," << ind2Index[i]->iid << ","
		 << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "," << ibs2starCount[i] << ","
		 << setprecision( PRECISE ) << meanIbs[i] << "," << setprecision( PRECISE ) << sdIbs[i] << ","
		 << setprecision( PRECISE ) << ibs2starpercent[i] << "," << setprecision( PRECISE ) << informativepercent[i] << "\n"; 
	}
	
	MEANSD.close();
}

void Duo::printRelationships()
{
	string file = par::outfile + ".specified";
	
	ofstream RELATE;
	RELATE.open( file.c_str(), ios::out );
	
	if (!RELATE.is_open() or !RELATE.good()) error( "Couldn't create file [ " + file + " ]\n" );
	
	printLog("Writing pair-wise relationships specified from input pedigrees to [ " + file + " ]\n");
	
	if (numSpecified() == 0) error( "No specified relationships found!" );
	
	RELATE << "FID1,IID1,FID2,IID2,SpecifiedRelationship" << "\n";
	
	for (unsigned int i = 0; i < numSpecified(); ++i)
	{
		RELATE 
		 << ind1Index[i]->fid << "," << ind1Index[i]->iid << ","
		 << ind2Index[i]->fid << "," << ind2Index[i]->iid << ","
		 << RELATIONSHIP[ specifiedRelationship[i] ] << "\n";
	}
	
	RELATE.close();
}

void Duo::printSpecifiedAndCalculated()
{
	string file = par::outfile + ".theoretical";
	
	ofstream THEORY;
	THEORY.open( file.c_str(), ios::out );
	
	if (!THEORY.is_open() or !THEORY.good()) error( "Could not write to file [ " + file + " ]\n" );
	
	THEORY.setf( ios::fixed );
	
	printLog("Writing theoretical relationships to [ " + file + " ]\n");	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,SpecifiedRelationship,CalculatedRelationship" << "\n";
	
	for (unsigned int i = 0; i < numCalculated(); ++i)
	{
		THEORY
		 << ind1Index[i]->fid << "," << ind1Index[i]->iid << ","
		 << ind2Index[i]->fid << "," << ind2Index[i]->iid << ","
		 << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "," << ibs2starCount[i] << ","
		 << setprecision( PRECISE ) << meanIbs[i] << "," << setprecision( PRECISE ) << sdIbs[i] << ","
		 << setprecision( PRECISE ) << ibs2starpercent[i] << "," << setprecision( PRECISE ) << informativepercent[i] << ","
		 << RELATIONSHIP[ specifiedRelationship[i] ] << "," << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
	}
	
	THEORY.close();
}

void Duo::printCalculatedOnly()
{
	string file = par::outfile + ".calculated";
	
	ofstream THEORY;
	THEORY.open( file.c_str(), ios::out );
	
	if (!THEORY.is_open() or !THEORY.good()) error( "Could not create file [ " + file + " ]\n" );
	
	THEORY.setf( ios::fixed );
	
	printLog( "Writing theoretical relationships to [ " + file + " ]\n" );	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,CalculatedRelationship" << "\n";
	
	for (unsigned int i = 0; i < numCalculated(); ++i)
	{
		THEORY
		 << ind1Index[i]->fid << "," << ind1Index[i]->iid << ","
		 << ind2Index[i]->fid << "," << ind2Index[i]->iid << ","
		 << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "," << ibs2starCount[i] << ","
		 << setprecision( PRECISE ) << meanIbs[i] << "," << setprecision( PRECISE ) << sdIbs[i] << ","
		 << setprecision( PRECISE ) << ibs2starpercent[i] << "," << informativepercent[i] << "," 
		 << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
	}
	
	THEORY.close();
}

void Duo::printConflicted()
{
	string file = par::outfile + ".conflicted";
	
	ofstream CONFLICTED;
	CONFLICTED.open( file.c_str(), ios::out );
	
	if (!CONFLICTED.is_open() or !CONFLICTED.good()) error( "Could not create file [ " + file + " ]\n" );
	
	CONFLICTED.setf( ios::fixed );
	
	printLog( "Writing relationships that conflict with specified pedigrees to [ " + file + " ]\n" );
	
	CONFLICTED << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,SpecifiedRelationship,CalculatedRelationship" << "\n"; 
	
	for (unsigned int i = 0; i < numSpecified(); ++i)
	{
		if (specifiedRelationship[i] != calculatedRelationship[i])
		{
			CONFLICTED
			 << ind1Index[i]->fid << "," << ind1Index[i]->iid << ","
			 << ind2Index[i]->fid << "," << ind2Index[i]->iid << ","
			 << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "," << ibs2starCount[i] << ","
			 << setprecision( PRECISE ) << meanIbs[i] << "," << setprecision( PRECISE ) << sdIbs[i] << ","
			 << setprecision( PRECISE ) << ibs2starpercent[i] << "," << setprecision( PRECISE ) << informativepercent[i] << ","
			 << RELATIONSHIP[ specifiedRelationship[i] ] << ","
			 << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
		}
	}
	
	CONFLICTED.close();
}

//////////////////////////////////////////////////
//
// Ped Class output functions
//
//////////////////////////////////////////////////
void Ped::print()
{
	string file = par::outfile + ".ped";
	
	ofstream PED;
	PED.open( file.c_str(), ios::out );
	
	if (!PED.is_open() or !PED.good()) error("Could not write to file [ " + file + " ]\n");
	
	printLog("Writing ped file [ " + file + " ]\n");
	
	for (unsigned int i = 0; i < numPeople(); ++i)
	{
		PED
		 << samples[i]->fid << " " << samples[i]->iid << " "
		 << samples[i]->pid << " " << samples[i]->mid << " "
		 << samples[i]->sex << " " << samples[i]->pheno;
		
		// REQUIRES REWRITE FOR NEW BOOLEAN EXPRESSION
		for (unsigned int j = 0; j < par::snpcount; ++j)
		{
			PED << fileGenotypeString( i, j );
		}
		
		PED << "\n";
	}
	
	PED.close();
}

//////////////////////////////////////////////////
//
// Map Class output functions
//
//////////////////////////////////////////////////
void Map::print()
{
	string file = par::outfile + ".map";
	
	ofstream MAP;
	MAP.open( file.c_str(), ios::out );
	
	if (!MAP.is_open() or !MAP.good()) error( "Couldn't write map file [ " + file + " ]\n" );
	
	printLog("Writing map file [ " + file + " ]\n");
	
	for (unsigned int i = 0; i < numMarkers(); ++i)
	{
		MAP
		 << rsid[i] << " " << chr[i] << " "
		 << genetic[i] << " "
		 << position[i] << "\n";
	}
	
	MAP.close();
}

//////////////////////////////////////////////////
//
// Transposed format writing
//
//////////////////////////////////////////////////
void writeTranspose( Ped &ped, Map &map )
{
	// Write the tfam file
	string file = par::outfile + ".tfam";
	
	ofstream TFAM;
	TFAM.open( file.c_str(), ios::out );
	
	if (!TFAM.is_open() or !TFAM.good()) error( "Couldn't write TFAM file [ " + file + " ]\n" );
	
	printLog( "Writing tfam file [ " + file + " ]\n" );
	
	for (unsigned int i = 0; i < ped.numPeople(); ++i)
	{
		TFAM
		 << ped.samples[i]->fid << " " << ped.samples[i]->iid << " "
		 << ped.samples[i]->pid << " " << ped.samples[i]->mid << " "
		 << ped.samples[i]->sex << " " << ped.samples[i]->pheno << "\n";
	}
	
	TFAM.close();
	
	// Write the TPED file
	file = par::outfile + ".tped";
	
	ofstream TPED;
	TPED.open( file.c_str(), ios::out );
	
	if (!TPED.is_open() or !TPED.good()) error( "Couldn't write TPED file [ " + file + " ]\n" );
	
	printLog( "Writing tped file [ " + file + " ]\n" );
	
	for (unsigned int i = 0; i < map.numMarkers(); ++i)
	{
		TPED
		 << map.chr[i] << " "
		 << map.rsid[i] << " "
		 << map.genetic[i] << " "
		 << map.position[i];
		
		for (int j = 0; j < par::personcount; ++j)
		{
			TPED << ped.fileGenotypeString( j, i );
		}
		
		TPED << "\n";
	}
}

//////////////////////////////////////////////////
// Write to binary file -- Development
//////////////////////////////////////////////////

// void writeBinary( Ped &ped, Map &map )
// {
// 	// Write the BFAM file
// 	string file = par::outfile + ".bfam";
// 	
// 	ofstream BFAM;
//	BFAM.open( file.c_str(), ios::in );
// 	
// 	printLog("Writing bfam file [ " + file + " ]\n");
// 	
// 	for (int i = 0; i < par::personcount; ++i)
// 	{
// 		BFAM << ped.samples[i]->fid << " " << ped.samples[i]->iid << " ";
// 		BFAM << ped.samples[i]->pid << " " << ped.samples[i]->mid << " ";
// 		BFAM << ped.samples[i]->sex << " " << ped.samples[i]->pheno << "\n";
// 	}
// 	
// 	BFAM.close();
// 	
// 	// Write the BMAP file
// 	file = par::outfile + ".bmap";
// 	
// 	ofstream BMAP;
//	BMAP.open( file.c_str(), ios::in );
// 	
// 	printLog("Writing bmap file [ " + file + " ]\n");
// 	
// 	for (int i = 0; i < par::snpcount; ++i)
// 	{
// 		BMAP << map.rsid[i] << " " << map.chr[i] << " ";
// 		BMAP << map.genetic[i] << " ";
// 		BMAP << map.position[i] << " ";
// 		BMAP << ped.allele1[i] << " ";
// 		BMAP << ped.allele2[i] << "\n";
// 	}
// 	
// 	BMAP.close();
// 	
// 	// Write BPED file
// 	file = par::outfile + ".bped";
// 	
// 	ofstream BPED;
//	BPED.open( file.c_str(), ios::binary | ios::app );
// 	
// 	printLog("Writing bped file [ " + file + " ]\n");
// 	
// 	// Write options
// 	BPED.write( reinterpret_cast<char*>( &par::snpcount ), sizeof(int) );
// 	BPED.write( reinterpret_cast<char*>( &par::personcount ), sizeof(int) );
// 	
// 	// Write ped logic vectors
// 	int count = 0;
// 	int bcount = 0;
// 	char c[1];
// 	
// 	while( count < par::snpcount )
// 	{
// 		bitset<8> bits;
// 		bits.reset();
// 		
// 		bcount = 0;
// 		
// 		while( bcount < 8  and count < par::snpcount)
// 		{
// 			if (ped.hasAllele1[ count ]) bits.set( bcount );
// 			++bcount;
// 			++count;
// 		}
// 		
// 		c[0] = (char)bits.to_ulong();
// 		BPED.write( c, 1 );
// 	}
// 	
// 	count = 0;
// 	bcount = 0;
// 	
// 	while( count < par::snpcount )
// 	{
// 		bitset<8> bits;
// 		bits.reset();
// 		
// 		bcount = 0;
// 		
// 		while( bcount < 8 and count < par::snpcount )
// 		{
// 			if (ped.hasAllele2[ count ]) bits.set( bcount );
// 			++bcount;
// 			++count;
// 		}
// 		
// 		c[0] = (char)bits.to_ulong();
// 		BPED.write( c, 1 );
// 	}
// 	
// 	// Write ped Person objects
// 	count = 0;
// 	bcount = 0;
// 	
// 	for (int i = 0; i < par::personcount; ++i)
// 	{
// 		// hasGenotype
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (ped.samples[i]->hasGenotype[count]) bits.set( bcount );
// 				++bcount;
// 				++count;
// 			}
// 			
// 			c[0] = (char)bits.to_ulong();
// 			BPED.write( c, 1 );
// 		}
// 		
// 		// a1
// 		count = 0;
// 		bcount = 0;
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (ped.samples[i]->a1[count]) bits.set( bcount );
// 				++bcount;
// 				++count;
// 			}
// 			
// 			c[0] = (char)bits.to_ulong();
// 			BPED.write( c, 1 );
// 		}
// 		
// 		// a2
// 		count = 0;
// 		bcount = 0;
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (ped.samples[i]->a2[count]) bits.set( bcount );
// 				++bcount;
// 				++count;
// 			}
// 			
// 			c[0] = (char)bits.to_ulong();
// 			BPED.write( c, 1 );
// 		}
// 	}
// 	
// 	BPED.close();
// }

//////////////////////////////////////////////////
//
// Function to write data in a SNPduo compatible
// AA/AB/BB coding nomenclature
//
//////////////////////////////////////////////////
void writeForWeb( Ped &ped, Map &map )
{
	string file = par::outfile + ".csv";
	string geno = "";
	string chrTmp = "";
	
	printLog( "Writing data to Custom format file for Web SNPduo to file [ " + file + "  ]\n" );
	
	// Open file
	ofstream WEB;
	WEB.open( file.c_str(), ios::out );
	
	if (!WEB.is_open() or !WEB.good()) error( "Cannot write Web SNPduo output [ " + file + " ]");
	
	// Write headers
	WEB << "Chromosome,Physical.Position,RSID,A/B";
	
	for (unsigned int i = 0; i < ped.numPeople(); ++i)
	{
		WEB << "," << ped.samples[i]->fid << "_" << ped.samples[i]->iid;
	}
	WEB << "\n";
	
	// Print data
	for (unsigned int i = 0; i < map.numMarkers(); ++i)
	{
		if (map.chr[i] < 23) WEB << map.chr[i];
		else if (map.chr[i] == XINT) WEB << "X";
		else if (map.chr[i] == YINT) WEB << "Y";
		else if (map.chr[i] == XYINT) WEB << "XY";
		else if (map.chr[i] == MTINT) WEB << "MT";
		
		WEB << "," << map.position[i] << "," << map.rsid[i] << ",";
		
		if (ped.hasAllele1[i])
		{
			if (ped.hasAllele2[i])
			{
				WEB << ped.allele1[i] << "/" << ped.allele2[i];
			}
			else
			{
				WEB << ped.allele1[i] << "/" << "N";
			}
		}
		else
		{
			if (ped.hasAllele2[i])
			{
				WEB << "N" << "/" << ped.allele2[i];
			}
			else
			{
				WEB << "N/N";
			}
		}
		
		for (unsigned int j = 0; j < ped.numPeople(); ++j)
		{
			geno = "";
			
			if (ped.samples[j]->hasGenotype[i])
			{
				if (ped.samples[j]->a1[i])
				{
					geno = "B";
				}
				else
				{
					geno = "A";
				}
				
				if (ped.samples[j]->a2[i])
				{
					geno += "B";
				}
				else
				{
					geno += "A";
				}
				
				if (geno == "BA") WEB << ",AB";
				else WEB << "," << geno;
				
			}
			else
			{
				WEB << ",NC";
			}
		}
		
		WEB << "\n";
	}
		
	// Close file
	WEB.close();
}
