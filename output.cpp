// Standard Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <bitset>

// Namespace
using namespace std;

// Custom headers
#include "options.h"
#include "Duo.h"
#include "snpduo.h"

// RELATIONSHIP const string comes from Duo.h
// Used to maintain flexibility in adding relationship classes

// LOG declared in snpduo.cpp
extern ofstream LOG;

// FUNCTIONS
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
	COUNTS.open( file.c_str() );
	
	printLog( "Writing pair-wise IBS counts to [ " + file + " ] ...");
	
	COUNTS << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2" << "\n";
	
	for (unsigned int i = 0; i < ibs0Count.size(); ++i)
	{
		COUNTS << fid1[i] << "," << iid1[i] << "," << fid2[i] << "," << iid2[i] << ",";
		COUNTS << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << "\n";
	}
	
	printLog("done! \n");
	
	COUNTS.close();
}

void Duo::printMeanSD()
{
//	cout.setf(ios::fixed);
	
	string file = par::outfile + ".summary";
	
	ofstream MEANSD;
	MEANSD.open( file.c_str() );
	
	printLog("Writing pair-wise Mean and SD summary information to [ " + file + " ] ...");
	
	MEANSD.setf(ios::fixed);
	
	MEANSD << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,Mean_IBS,SD_IBS" << "\n";
	
	for (unsigned int i = 0; i < meanIbs.size(); ++i)
	{
		MEANSD << fid1[i] << "," << iid1[i] << "," << fid2[i] << "," << iid2[i] << ",";
		MEANSD << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << ",";
		MEANSD << setprecision(7) << meanIbs[i] << "," << setprecision(8) << sdIbs[i] << "\n"; 
	}
	
	printLog("done! \n");
	
	MEANSD.close();
}

void Duo::printRelationships()
{
	string file = par::outfile + ".specified";
	
	ofstream RELATE;
	RELATE.open( file.c_str());
	
	printLog("Writing pair-wise relationships specified from input pedigrees to [ " + file + " ] ...");
	
	if (specifiedRelationship.size() == 0) error("No specified relationships found!");
	
	RELATE << "FID1,IID1,FID2,IID2,SpecifiedRelationship" << "\n";
	
	for (unsigned int i = 0; i < specifiedRelationship.size(); ++i)
	{
		RELATE << fid1[i] << "," << iid1[i] << ",";
		RELATE << fid2[i] << "," << iid2[i] << ",";
		RELATE << RELATIONSHIP[ specifiedRelationship[i] ] << "\n";
	}
	
	RELATE.close();
	
	printLog("done! \n");
}

void Duo::printSpecifiedAndCalculated()
{
	string file = par::outfile + ".theoretical";
	
	ofstream THEORY;
	THEORY.open( file.c_str() );
	
	printLog("Writing theoretical relationships to [ " + file + " ] ...");	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,Mean_IBS,SD_IBS,SpecifiedRelationship,CalculatedRelationship" << "\n";
	
	for (unsigned int i = 0; i < calculatedRelationship.size(); ++i)
	{
		THEORY << fid1[i] << "," << iid1[i] << ",";
		THEORY << fid2[i] << "," << iid2[i] << ",";
		THEORY << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << ",";
		THEORY << meanIbs[i] << "," << sdIbs[i] << ",";
		THEORY << RELATIONSHIP[ specifiedRelationship[i] ] << ",";
		THEORY << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
	}
	
	printLog("done! \n");
	
	THEORY.close();
}

void Duo::printCalculatedOnly()
{
	string file = par::outfile + ".calculated";
	
	ofstream THEORY;
	THEORY.open( file.c_str() );
	
	printLog("Writing theoretical relationships to [ " + file + " ] ...");	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,Mean_IBS,SD_IBS,CalculatedRelationship" << "\n";
	
	for (unsigned int i = 0; i < calculatedRelationship.size(); ++i)
	{
		THEORY << fid1[i] << "," << iid1[i] << ",";
		THEORY << fid2[i] << "," << iid2[i] << ",";
		THEORY << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << ",";
		THEORY << meanIbs[i] << "," << sdIbs[i] << ",";
		THEORY << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
	}
	
	printLog("done! \n");
	
	THEORY.close();
}

void Duo::printConflicted()
{
	string file = par::outfile + ".conflicted";
	
	ofstream CONFLICTED;
	CONFLICTED.open( file.c_str() );
	
	printLog("Writing relationships that conflict with specified pedigrees to [ " + file + " ] ...");
	
	CONFLICTED << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,Mean_IBS,SD_IBS,SpecifiedRelationship,CalculatedRelationship" << "\n"; 
	
	for (unsigned int i = 0; i < fid1.size(); ++i)
	{
		if ( specifiedRelationship[i] != calculatedRelationship[i] )
		{
			CONFLICTED << fid1[i] << "," << iid1[i] << ",";
			CONFLICTED << fid2[i] << "," << iid2[i] << ",";
			CONFLICTED << ibs0Count[i] << "," << ibs1Count[i] << "," << ibs2Count[i] << ",";
			CONFLICTED << meanIbs[i] << "," << sdIbs[i] << ",";
			CONFLICTED << RELATIONSHIP[ specifiedRelationship[i] ] << ",";
			CONFLICTED << RELATIONSHIP[ calculatedRelationship[i] ] << "\n";
		}
	}
	
	printLog("done! \n");
	
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
	PED.open( file.c_str() );
	
	printLog("Writing ped file [ " + file + " ] ...");
	
	for (unsigned int i = 0; i < par::personcount; ++i)
	{
		PED << samples[i]->fid << " " << samples[i]->iid << " ";
		PED << samples[i]->pid << " " << samples[i]->mid << " ";
		PED << samples[i]->sex << " " << samples[i]->pheno;
		
		// REQUIRES REWRITE FOR NEW BOOLEAN EXPRESSION
		for (unsigned int j = 0; j < par::snpcount; ++j)
		{
			PED << " " << samples[i]->a1[j];
			PED << " " << samples[i]->a2[j];
		}
		
		PED << "\n";
	}
	
	PED.close();
	
	printLog("done!");
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
	MAP.open( file.c_str() );
	
	printLog("Writing map file [ " + file + " ] ...");
	
	for (int i = 0; i < par::snpcount; ++i)
	{
		MAP << rsid[i] << " " << chr[i] << " ";
		MAP << genetic[i] << " ";
		MAP << position[i] << "\n";
	}
	
	MAP.close();
	
	printLog("done!");
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
	TFAM.open( file.c_str() );
	
	printLog("Writing tfam file [ " + file + " ] ...");
	
	for (int i = 0; i < par::personcount; ++i)
	{
		TFAM << ped.samples[i]->fid << " " << ped.samples[i]->iid << " ";
		TFAM << ped.samples[i]->pid << " " << ped.samples[i]->mid << " ";
		TFAM << ped.samples[i]->sex << " " << ped.samples[i]->pheno << "\n";
	}
	
	TFAM.close();
	
	printLog("done!\n");
	
	// Write the TPED file
	file = par::outfile + ".tped";
	
	ofstream TPED;
	TPED.open( file.c_str() );
	
	printLog("Writing tped file [ " + file + " ] ...");
	
	for (int i = 0; i < par::snpcount; ++i)
	{
		TPED << map.chr[i] << " ";
		TPED << map.rsid[i] << " ";
		TPED << map.genetic[i] << " ";
		TPED << map.position[i];
		
		for (int j = 0; j < par::personcount; ++j)
		{
			TPED << ped.fileGenotypeString( j, i );
		}
		
		TPED << "\n";
	}
	
	printLog("done!\n");
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
// 	BFAM.open( file.c_str() );
// 	
// 	printLog("Writing bfam file [ " + file + " ] ...");
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
// 	printLog("done!\n");
// 	
// 	// Write the BMAP file
// 	file = par::outfile + ".bmap";
// 	
// 	ofstream BMAP;
// 	BMAP.open( file.c_str() );
// 	
// 	printLog("Writing bmap file [ " + file + " ] ...");
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
// 	printLog("done!\n");
// 	
// 	// Write BPED file
// 	file = par::outfile + ".bped";
// 	
// 	ofstream BPED;
// 	BPED.open( file.c_str(), ios::binary | ios::app );
// 	
// 	printLog("Writing bped file [ " + file + " ] ...");
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
// 	
// 	printLog("done!\n");
// }

//////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////
void writeForWeb( Ped &ped, Map &map )
{
	string file = par::outfile + ".csv";
	string geno = "";
	string chrTmp = "";
	
	printLog("Writing data to Custom format file for Web SNPduo to file [ " + file + "  ] ...");
	
	// Open file
	ofstream WEB;
	WEB.open( file.c_str() );
	if (!WEB.is_open()) error( "Cannot open Web SNPduo output [ " + file + " ]");
	
	// Write headers
	WEB << "Chromosome,Physical.Position,RSID,A/B";
	
	for (int i = 0; i < par::personcount; ++i)
	{
		WEB << "," << ped.samples[i]->fid << "_" << ped.samples[i]->iid;
	}
	WEB << "\n";
	
	// Print data
	for (int i = 0; i < par::snpcount; ++i)
	{
		if (map.chr[i] < 23) WEB << map.chr[i];
		else if (map.chr[i] == 23) WEB << "X";
		else if (map.chr[i] == 24) WEB << "Y";
		else if (map.chr[i] == 25) WEB << "XY";
		else if (map.chr[i] == 26) WEB << "MT";
		
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
		
		for (int j = 0; j < par::personcount; ++j)
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
	
	printLog("done!\n");
}
