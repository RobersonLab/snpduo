// Standard Libraries
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// imports
using std::cout;
using std::ios;
using std::iterator;
using std::ofstream;
using std::setprecision;
using std::string;

// Custom headers
#include "duo.h"
#include "options.h"
#include "output.h"

// RELATIONSHIP const string comes from helper.h
// Used to maintain flexibility in adding relationship classes

// LOG declared in snpduo.cpp
extern ofstream LOG;

int const PRECISE = 7; // used to specify precision for output

//////////////////
// Log function //
//////////////////
void writeLog( string const& s )
{
	LOG << s;
	LOG.flush();
	
	if (par::verbose)
	{
		cout << s;
		cout.flush();
	}
}

////////////////////////////////
// Duo Class output functions //
////////////////////////////////
void DuoMap::printCounts()
{
	string file = par::out + ".count";
	
	ofstream COUNTS( file.c_str(), ios::out );
	if (not COUNTS.good()) { error("Could not create count file " + string2Filename(file)); }
	
	writeLog( "Writing pair-wise IBS counts to " + string2Filename(file) + "\n");
	
	COUNTS << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*\n";
	
	for (vector<Duo>::iterator currIter = begin(); currIter != end(); ++currIter)
	{		
		COUNTS 
		<< (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << "," 
		<< (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2()->getIid() << "," 
		<< (*currIter).getIbs0Count() << "," << (*currIter).getIbs1Count() << "," << (*currIter).getIbs2Count() 
		<< "," << (*currIter).getIbs2StarCount() << "\n";
	}
	
	COUNTS.close();
}

void DuoMap::printMeanSD()
{
	string file = par::out + ".summary";
	
	ofstream MEANSD( file.c_str(), ios::out );
	if (not MEANSD.good()) { error( "Could not create Mean / SD file " + string2Filename(file) + "\n" ); }
	
	writeLog( "Writing pair-wise Mean and SD summary information to " + string2Filename(file) + "\n" );
	
	MEANSD.setf( ios::fixed );
	
	MEANSD << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,IBS2*%_Of_Informative" << "\n";
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while (currIter != endIter)
	{
		MEANSD
		 << (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << ","
		 << (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2()->getIid() << ","
		 << (*currIter).getIbs0Count() << "," << (*currIter).getIbs1Count() << "," << (*currIter).getIbs2Count()
		 << "," << (*currIter).getIbs2StarCount() << "," << setprecision( PRECISE ) << (*currIter).getMeanIbs()
		 << "," << setprecision( PRECISE ) << (*currIter).getSdIbs() << "," << setprecision( PRECISE )
		 << (*currIter).getIbs2StarPercent() << "," << setprecision( PRECISE ) << (*currIter).getInformativePercent()
		 << "," << setprecision( PRECISE ) << (*currIter).getIbs2StarPercentOfInformative() << "\n";
		 
		 ++currIter;
	}
	
	MEANSD.close();
}

void DuoMap::printRelationships()
{
	string file = par::out + ".specified";
	
	ofstream RELATE( file.c_str(), ios::out );
	if (not RELATE.good()) { error( "Couldn't create file " + string2Filename(file) + "\n" ); }
	
	writeLog("Writing pair-wise relationships specified from input Pedigrees to " + string2Filename(file) + "\n");
	
	if (count() == 0) { error( "Asked to print relationships, but no specified relationships found!" ); }
	
	RELATE << "FID1,IID1,FID2,IID2,SpecifiedRelationship" << "\n";
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while (currIter != endIter)
	{
		RELATE 
		 << (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << ","
		 << (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2() << ","
		 << RELATIONSHIP[ (*currIter).getSpecifiedRelationship() ] << "\n";
		 
		 ++currIter;
	}
	
	RELATE.close();
}

void DuoMap::printSpecifiedAndCalculated()
{
	string file = par::out + ".theoretical";
	
	ofstream THEORY( file.c_str(), ios::out );
	if (not THEORY.good()) { error( "Could not write to file " + string2Filename(file) + "\n" ); }
	
	THEORY.setf( ios::fixed );
	
	writeLog("Writing theoretical relationships to " + string2Filename(file) + "\n");	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,IBS2*_Percent_Of_Informative,SpecifiedRelationship,CalculatedRelationship" << "\n";
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while( currIter != endIter )
	{
		THEORY
		 << (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << ","
		 << (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2()->getIid() << ","
		 << (*currIter).getIbs0Count() << "," << (*currIter).getIbs1Count() << "," << (*currIter).getIbs2Count() << "," 
		 << (*currIter).getIbs2StarCount() << "," << setprecision( PRECISE ) << (*currIter).getMeanIbs() << "," 
		 << setprecision( PRECISE ) << (*currIter).getSdIbs() << "," << setprecision( PRECISE ) 
		 << (*currIter).getIbs2StarPercent() << "," << setprecision( PRECISE ) << (*currIter).getInformativePercent() 
		 << "," << setprecision( PRECISE ) << (*currIter).getIbs2StarPercentOfInformative() << "," 
		 << RELATIONSHIP[ (*currIter).getSpecifiedRelationship() ] << "," << RELATIONSHIP[ (*currIter).getCalculatedRelationship() ] 
		 << "\n";
		 
		 ++currIter;
	}
	
	THEORY.close();
}

void DuoMap::printCalculatedOnly()
{
	string file = par::out + ".calculated";
	
	ofstream THEORY( file.c_str(), ios::out );
	if (not THEORY.good()) { error( "Could not create file " + string2Filename(file) + "\n" ); }
	
	THEORY.setf( ios::fixed );
	
	writeLog( "Writing theoretical relationships to " + string2Filename(file) + "\n" );	
	
	THEORY << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,IBS2*%_Of_Informative,CalculatedRelationship" << "\n";
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while( currIter != endIter )
	{
		THEORY
		 << (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << ","
		 << (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2()->getIid() << ","
		 << (*currIter).getIbs0Count() << "," << (*currIter).getIbs1Count() << "," << (*currIter).getIbs2Count()
		 << "," << (*currIter).getIbs2StarCount() << "," << setprecision( PRECISE ) << (*currIter).getMeanIbs()
		 << "," << setprecision( PRECISE ) << (*currIter).getSdIbs() << "," << setprecision( PRECISE )
		 << (*currIter).getIbs2StarPercent() << "," << (*currIter).getInformativePercent() << "," << setprecision( PRECISE )
		 << (*currIter).getIbs2StarPercentOfInformative() << "," << RELATIONSHIP[ (*currIter).getCalculatedRelationship() ] << "\n";
		 
		 ++currIter;
	}
	
	THEORY.close();
}

void DuoMap::printConflicted()
{
	string file = par::out + ".conflicted";
	
	ofstream CONFLICTED( file.c_str(), ios::out );
	if (not CONFLICTED.good()) { error( "Could not create file " + string2Filename(file) + "\n" ); }
	
	CONFLICTED.setf( ios::fixed );
	
	writeLog( "Writing relationships that conflict with specified Pedigrees to " + string2Filename(file) + "\n" );
	
	CONFLICTED << "FID1,IID1,FID2,IID2,IBS0,IBS1,IBS2,IBS2*,Mean_IBS,SD_IBS,IBS2*%,Informative%,IBS2*%_Of_Informative,SpecifiedRelationship,CalculatedRelationship" << "\n"; 
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while (currIter != endIter)
	{
		if ((*currIter).getSpecifiedRelationship() != (*currIter).getCalculatedRelationship())
		{
			CONFLICTED
			 << (*currIter).getInd1()->getFid() << "," << (*currIter).getInd1()->getIid() << ","
			 << (*currIter).getInd2()->getFid() << "," << (*currIter).getInd2()->getIid() << ","
			 << (*currIter).getIbs0Count() << "," << (*currIter).getIbs1Count() << "," << (*currIter).getIbs2Count()
			 << "," << (*currIter).getIbs2StarCount() << "," << setprecision( PRECISE ) << (*currIter).getMeanIbs()
			 << "," << setprecision( PRECISE ) << (*currIter).getSdIbs() << "," << setprecision( PRECISE )
			 << (*currIter).getIbs2StarPercent() << "," << (*currIter).getInformativePercent() << "," << setprecision( PRECISE )
			 << (*currIter).getIbs2StarPercentOfInformative() << "," << RELATIONSHIP[ (*currIter).getCalculatedRelationship() ] << "\n";
		}
		
		++currIter;
	}
	
	CONFLICTED.close();
}

////////////////////////////////
// Ped Class output functions //
////////////////////////////////
void Ped::write(LocusMap& lm)
{
	string file = par::out + ".ped";
	
	ofstream PED( file.c_str(), ios::out );
	if (not PED.good()) { error("Could not write to file " + string2Filename(file) + "\n"); }
	
	writeLog("Writing ped file " + string2Filename(file) + "\n");
	
	for (vector<Person*>::iterator perIter = begin(); perIter != end(); ++perIter)
	{
		PED
		 << (*perIter)->getFid() << " " << (*perIter)->getIid() << " "
		 << (*perIter)->getPid() << " " << (*perIter)->getMid() << " "
		 << (*perIter)->getSex() << " " << (*perIter)->getPheno();
		
		// iterate through SNPs AND locus
		vector<bool>::iterator a1Iter = (*perIter)->beginIterA1();
		vector<bool>::iterator a2Iter = (*perIter)->beginIterA2();
		vector<bool>::iterator called = (*perIter)->beginIterHasGenotype();
		vector<Locus>::iterator mapIter = lm.begin();
		
		while (a1Iter != (*perIter)->endIterA1() and a2Iter != (*perIter)->endIterA2() and called != (*perIter)->endIterHasGenotype() and mapIter != lm.end())
		{
			// passes bool values for has a genotype call, and what alleles 1 and 2 are.
			PED << fileGenotypeString( *called, *a1Iter, *a2Iter, *mapIter );
			++a1Iter;
			++a2Iter;
			++called;
			++mapIter;
		}
		
		PED << "\n";
	}
	
	PED.close();
}

/////////////////////////////////////
// LocusMap Class output functions //
/////////////////////////////////////
void LocusMap::write()
{
	string file = par::out + ".map";
	
	ofstream MAP( file.c_str(), ios::out );
	if (not MAP.good()) { error( "Couldn't write map file " + string2Filename(file) + "\n" ); }
	
	writeLog("Writing map file " + string2Filename(file) + "\n");
	
	for (vector<Locus>::iterator currIter = begin(); currIter != end(); ++currIter)
	{
		MAP
		 << currIter->getChrom() << " "
		 << currIter->getId() << " "
		 << currIter->getGenetic() << " "
		 << currIter->getPosition() << "\n";
	}
	
	MAP.close();
}

///////////////////////////////
// Transposed format writing //
///////////////////////////////
void writeTranspose( Ped& ped, LocusMap& lm )
{
	// Write the tfam file
	string file = par::out + ".tfam";
	
	ofstream TFAM( file.c_str(), ios::out );
	if (not TFAM.good()) { error( "Couldn't write TFAM file " + string2Filename(file) + "\n" ); }
	
	writeLog( "Writing tfam file " + string2Filename(file) + "\n" );
	
	for (vector<Person*>::iterator perIter = ped.begin(); perIter != ped.end(); ++perIter)
	{
		TFAM
		 << (*perIter)->getFid() << " " << (*perIter)->getIid() << " "
		 << (*perIter)->getPid() << " " << (*perIter)->getMid() << " "
		 << (*perIter)->getSex() << " " << (*perIter)->getPheno() << "\n";
	}
	
	TFAM.close();
	
	// Write the TPED file
	file = par::out + ".tped";
	
	ofstream TPED( file.c_str(), ios::out );
	if (not TPED.good()) { error( "Couldn't write TPED file " + string2Filename(file) + "\n" ); }
	
	writeLog( "Writing tped file " + string2Filename(file) + "\n" );
	
	unsigned int perCount = 0;
	for (vector<Locus>::iterator mapIter = lm.begin(); mapIter != lm.end(); ++mapIter)
	{
		TPED
		 << mapIter->getChrom() << " "
		 << mapIter->getId() << " "
		 << mapIter->getGenetic() << " "
		 << mapIter->getPosition();
		
		for (vector<Person*>::iterator perIter = ped.begin(); perIter != ped.end(); ++perIter)
		{
			TPED << ped.fileGenotypeString( (*perIter)->getHasGenotype(perCount), (*perIter)->getA1(perCount), (*perIter)->getA2(perCount), *mapIter );
			++perIter;
		}
		
		TPED << "\n";
		
		++perCount;
	}
}

///////////////////////////////////////////////////
// Function to write data in a SNPDuo compatible //
// AA/AB/BB coding nomenclature                  //
///////////////////////////////////////////////////
void writeForWeb( Ped& ped, LocusMap& lm )
{
	string file = par::out + ".csv";
	string geno = "";
	string chrTmp = "";
	
	writeLog( "Writing data to Custom format file for Web SNPDuo to file " + string2Filename(file) + "\n" );
	
	// Open file
	ofstream WEB( file.c_str(), ios::out );
	if (not WEB.good()) { error( "Cannot write Web SNPDuo output " + string2Filename(file) + "!"); }
	
	// Write headers
	WEB << "Chromosome,Physical.Position,RSID,A/B";
	
	for (vector<Person*>::iterator perIter = ped.begin(); perIter != ped.end(); ++perIter)
	{
		WEB << "," << (*perIter)->getFid() << "_" << (*perIter)->getIid();
	}
	WEB << "\n";
	
	unsigned int counter = 0;
	
	for (vector<Locus>::iterator mapIter = lm.begin(); mapIter != lm.end(); ++mapIter)
	{
		WEB << CHROM_STRINGS[ mapIter->getChrom() ] << "," << mapIter->getPosition() << "," << mapIter->getId() << ",";
		
		if (mapIter->getHasAllele1())
		{
			if (mapIter->getHasAllele2())
			{
				if (mapIter->getAllele1Major())
				{
					WEB << mapIter->getAllele1() << "/" << mapIter->getAllele2();
				}
				else
				{
					WEB << mapIter->getAllele2() << "/" << mapIter->getAllele1();
				}
			}
			else
			{
				WEB << mapIter->getAllele1() << "/N";
			}
		}
		else
		{
			// if there is no allele 1 there should be no allele 2 (allele 1 is populated first)
			WEB << "N/N";
		}
		
		/////////////////////////////////
		// Process each persons allele //
		/////////////////////////////////
		// ATTN may be a good place to cache the allele1 major / allele 2 major
		// check performance at some point to see if it impacts speed.
		for (vector<Person*>::iterator perIter = ped.begin(); perIter != ped.end(); ++perIter)
		{
			if ( mapIter->getHasAllele1() )
			{
				if ( (*perIter)->getHasGenotype(counter) )
				{
					if ( mapIter->getAllele1Major() )
					{
						// allele 1 is major allele in current population
						// allele 1 == A
						// allele 2 == B
						if ( (*perIter)->getA1(counter))
						{
							// has one copy of allele 2
							if ( (*perIter)->getA2(counter) )
							{
								// homozygous allele 2
								WEB << ",BB";
							}
							else
							{
								// heterozygous
								WEB << ",AB";
							}
						}
						else
						{
							// does not have a first copy of allele 2
							if ( (*perIter)->getA2(counter) )
							{
								// heterozygous
								WEB << ",AB";
							}
							else
							{
								// homozygous allele 1
								WEB << ",AA";
							}
						}
					}
					else
					{
						// allele 2 is major allele in current population
						// allele 1 == B
						// allele 2 == A
						if ( (*perIter)->getA1(counter))
						{
							// has one copy of allele 2
							if ( (*perIter)->getA2(counter) )
							{
								// homozygous allele 2
								WEB << ",AA";
							}
							else
							{
								// heterozygous
								WEB << ",AB";
							}
						}
						else
						{
							// does not have a first copy of allele 2
							if ( (*perIter)->getA2(counter) )
							{
								// heterozygous
								WEB << ",AB";
							}
							else
							{
								// homozygous allele 1
								WEB << ",BB";
							}
						}
					
					}
				}
				else
				{
					WEB << ",NC";
				}
			}
			else
			{
				// no genotype means this is a no call
				// if there are no alleles for this marker then it should be caught in the previous if statement
				WEB << ",NC";
			}
		}
		
		WEB << "\n";
		
		++counter;
	}
		
	// Close file
	WEB.close();
}
