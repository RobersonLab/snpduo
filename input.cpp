// Standard libraries
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

// Namespace
using std::ifstream;
using std::ios;
using std::iterator;
using std::string;

// Custom libraries
#include "helper.h"
#include "input.h"
#include "locus.h"
#include "options.h"
#include "output.h"

// Functions
void LocusMap::read()
{
	// new Locus each iteration
	// push onto vector if successful read
	writeLog( "Reading map file " + string2Filename(par::mapFile) + "\n" );
	
	// file operations
	ifstream MAP( par::mapFile.c_str(), ios::in );
	if (!MAP.good()) { error ("Map file " + string2Filename(par::mapFile) + " cannot be read"); }
	
	// variables
	string buffer = " "; // For reading chars into strings
	int lineCount = 0; // Count for markers
	
	// parse file
	while (!MAP.eof())
	{
		Locus newLocus;
		
		// Read Chr
		if ( readString(MAP, buffer) )
		{
			if (buffer.substr(0,1) == "#")
			{
				readCommentLine(MAP);
				continue;
			}
			else { newLocus.setChrom(intChrFromString(buffer)); }
		}
		else { continue; }
		
		// Read RSID and pushback
		if ( readString(MAP, buffer) ) { newLocus.setId( buffer ); }
		else { error("No RSID for line " + convertToString(lineCount + 1)); }
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			if( readString(MAP, buffer) ) { newLocus.setGenetic( string2Int(buffer) ); }
			else { error("No Genetic Distance for line " + convertToString(lineCount + 1) + ". Specify --map3 if map file doesn't include genetic distance"); }
		}
		else { newLocus.setGenetic( 0 ); }
		
		// Read position
		if ( readString(MAP, buffer) ) { newLocus.setPosition( string2Int(buffer) ); }
		else { error ("No physical position for line " + convertToString(lineCount + 1)); }
		
		++lineCount; // Used to count SNPs ATTN SWITCH TO AN AUTOINCREMENT
		
		// push onto array
		addLocus( newLocus );
	}
	
	MAP.close();
	
	par::snpCount = size();
	
	writeLog( convertToString(lineCount) + " markers read\n\n" );
}

void Ped::read(LocusMap& lm)
{
	writeLog( "Reading Ped file " + string2Filename(par::pedFile) + "\n" );
		
	// variables
	string buffer = " ";
	int lineCount = 0;
	
	// file operations
	ifstream PED( par::pedFile.c_str(), ios::in );
	if (not PED.good()) { error( "Ped file " + string2Filename(par::pedFile) + " cannot be read" ); }
	
	while (!PED.eof())
	{
		// Make a new Person
		Person* per = new Person;
		
		// Read FID
		if (readString( PED, buffer )) // If characters were read
		{
			if (buffer.substr(0,1) == "#") // If line starts with comment
			{
				// delete memory and reset to 0
				delete per;
				per = 0;
				
				readCommentLine(PED);
				continue;
			}
			else { per->setFid( buffer ); }
		}
		else { continue; }
		
		// Read IID
		if (readString( PED, buffer )) { per->setIid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error( "No Individual ID for line " + convertToString( lineCount + 1 ) );
		}
		
		// Read PID
		if (readString( PED, buffer )) { per->setPid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error( "No Paternal ID for line " + convertToString( lineCount + 1 ) );
		}
		
		// Read MID
		if (readString( PED, buffer )) { per->setMid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error( "No Maternal ID for line " + convertToString( lineCount + 1 ) );
		}
		
		// Read Sex
		if (readString( PED, buffer ))
		{
			if (buffer == "1") { per->setSex( 1 ); }
			else if (buffer == "2") { per->setSex( 2 ); }
			else { per->setSex( 0 ); }
		}
		else
		{
			delete per;
			per = 0;
			
			error( "No Sex Code for  line " + convertToString( lineCount + 1 ) );
		}
		
		// Read Phenotype
		if (readString( PED, buffer))
		{
			if (buffer == "1") { per->setPheno( 1 ); }
			else if (buffer == "2") { per->setPheno( 2 ); }
			else { per->setPheno( 0 ); }
		}
		else
		{
			delete per;
			per = 0;
			
			error( "No Phenotype data for line " + convertToString( lineCount + 1 ) );
		}
		
		// Read genotypes until end of line
		// Setup important variables first
		bool doa1 = true;
		int vectorcount = 0;
		char c = ' ';
		
		vector<Locus>::iterator locmapIter = lm.begin(); // incremented in the following loop
		
		while (!PED.eof())
		{
			// read one character at a time from file
			PED.get( c );
			
			if (PED.eof()) { break; } // the eof isn't set until a read is attempted
			if (c == ' ' or c == '\t' or c == '\r') { continue; }
			if (c == '\n') { break; }
			if (locmapIter == lm.end()) { error("Not at the end of ped line, but have already reached end of locus map."); }
						
			if (c != '0')
			{
				if (not locmapIter->getHasAllele1())
				{
					locmapIter->setAllele1(c);
				}
				else if (not locmapIter->getHasAllele2() and c != locmapIter->getAllele1())
				{
					locmapIter->setAllele2(c);
				}
			}
			
			// we're on allele 1 here
			if (doa1)
			{
				if (c == '0')
				{
					per->addHasGenotype( false );
					per->addAllele1( false );
				}
				else if (c == locmapIter->getAllele1())
				{
					per->addHasGenotype( true );
					per->addAllele1( false );
					locmapIter->incrAllele1Count();
				}
				else if (c == locmapIter->getAllele2())
				{
					per->addHasGenotype( true );
					per->addAllele1( true );
					locmapIter->incrAllele2Count();
				}
				else
				{
					error("\nSNP " + convertToString(vectorcount + 1) + " allele 1, line " 
					+ convertToString( lineCount + 1 ) + ". More than two SNP alleles detected in population:\n'" 
					+ locmapIter->getAllele1() + "' '" + locmapIter->getAllele2() + "' '" + c + "'");
				}
				
				doa1 = false;
			}
			else // on allele 2 now
			{
				if (c == '0')
				{
					per->addAllele2( false );
				}
				else if (c == locmapIter->getAllele1())
				{
					per->addAllele2( false );
					locmapIter->incrAllele1Count();
				}
				else if (c == locmapIter->getAllele2())
				{
					per->addAllele2( true );
					locmapIter->incrAllele2Count();
				}
				else
				{
					error("\nSNP " + convertToString(vectorcount + 1) + " allele 2, line " 
					+ convertToString( lineCount + 1 ) + ". More than two SNP alleles detected in population:\n'" 
					+ locmapIter->getAllele1() + "' '" + locmapIter->getAllele2() + "' '" + c + "'");
				}
				
				doa1 = true;
				++vectorcount;
				++locmapIter; //increment here because TWO loops mean one allele processed due to format
			}
		}
		
		// Increment bin
		++lineCount;
		
		addPerson( per ); // add to our Pedigree list of people
	}
	
	PED.close();
	
	lm.setMajorAllele();
	
	writeLog( convertToString(lineCount) + " individuals read\n\n" );
}

/////////////////////////////////////////
// TPED / TFAM file handling functions //
/////////////////////////////////////////
void readTpedFile( Ped& ped, LocusMap& lm )
{
	string buffer = " ";
	unsigned long int lineCount = 0;
	
	//////////////////////////////////
	// process Ped object from tfam //
	//////////////////////////////////
	writeLog( "Reading TFAM file " + string2Filename(par::tfamfile) + "\n" );
	
	ifstream TFAM( par::tfamfile.c_str(), ios::in );
	if (!TFAM.good()) { error ("TFAM file " + string2Filename(par::tfamfile) + " cannot be opened"); }
	
	// parse file
	while (!TFAM.eof())
	{
		Person *per = new Person;
		
		// Read FID
		if (readString( TFAM, buffer ))
		{
			if (buffer.substr(0,1) == "#") // If line is a comment
			{
				delete per;
				per = 0;
				
				readCommentLine(TFAM);
				continue;
			}
			
			else { per->setFid( buffer ); }
		}
		else
		{
			delete per;
			per = 0;
			
			continue;
		}
		
		// Read IID
		if (readString( TFAM, buffer )) { per->setIid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error ("No Individual ID for line " + convertToString(lineCount + 1));
		}
		
		// Read PID
		if (readString( TFAM, buffer )) { per->setPid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error( "No Paternal ID for line " + convertToString(lineCount + 1) );
		}
		
		// Read MID
		if (readString( TFAM, buffer )) { per->setMid( buffer ); }
		else
		{
			delete per;
			per = 0;
			
			error( "No Maternal ID for line " + convertToString(lineCount + 1) );
		}
		
		// Read Sex
		if (readString( TFAM, buffer )) 
		{
			if (buffer == "1") { per->setSex( 1 ); }
			else if (buffer == "2") { per->setSex( 2 ); }
			else { per->setSex( 0 ); }
		}
		else
		{
			delete per;
			per = 0;
			
			error ("No Sex code for line " + convertToString(lineCount + 1));
		}
		
		// Read Phenotype
		if (readString(TFAM, buffer))
		{
			if (buffer == "1") { per->setPheno( 1 ); }
			else if (buffer == "2") { per->setPheno( 2 ); }
			else { per->setPheno( 0 ); }
		}
		else
		{
			delete per;
			per = 0;
			
			error ("No pheno data for line " + convertToString(lineCount + 1));
		}
		
		++lineCount;
		
		ped.addPerson( per ); // add to associative array
	}
	
	writeLog( "Pedigrees for " + convertToString(lineCount) + " individuals read\n\n" );
	
	TFAM.close();
	
	////////////////////////////////
	// Read TPED into Ped and map //
	////////////////////////////////
	writeLog( "Reading TPED from " + string2Filename(par::tpedFile) + "\n");
	
	ifstream TPED(par::tpedFile.c_str(), ios::in);
	if (!TPED.good()) { error ("File " + string2Filename(par::tpedFile) + " couldn't be opened"); }
	
	lineCount = 0; // Reset lineCount
	
	while (!TPED.eof())
	{
		Locus myLocus; // initialize object to hold map coordinates
		// Read chr
		if (readString( TPED, buffer ))
		{
			if (buffer.substr(0,1) == "#")
			{
				readCommentLine(TPED);
				continue;
			}			
			else { myLocus.setChrom( intChrFromString(buffer) ); }
		}
		
		else { continue; }
		
		// Read RSID
		if (readString( TPED, buffer )) { myLocus.setId( buffer ); }
		
		else { error ("No RSID for line " + convertToString(lineCount + 1)); }
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			if (readString( TPED, buffer )) { myLocus.setGenetic( string2Int(buffer) ); }
			
			else { error ("No Genetic Distance for line " + convertToString(lineCount + 1) + ". Specify --map3 if TPED file doesn't include genetic distance."); }
		}
		else { myLocus.setGenetic( 0 ); }
		
		// Read Position
		if (readString( TPED, buffer )){ myLocus.setPosition( string2Int(buffer) ); }
		else { error ("No physical position for line " + convertToString( lineCount + 1 )); }
		
		// Read in genotype
		bool doa1 = true;
		char c = ' ';
		
		int indCount = 0; // Must have an individual counter
		
		while (!TPED.eof())
		{
			TPED.get( c );
			
			if (TPED.eof()) { break; }
			if (c == ' ' or c == '\t' or c == '\r') { continue; }
			if (c == '\n') { break; }
			
			Person* myPerson = ped[indCount];
			
			if (c != '0')
			{
				if (not myLocus.getHasAllele1())
				{
					myLocus.setAllele1(c);
				}
				else if (myLocus.getHasAllele1() and not myLocus.getHasAllele2() and c != myLocus.getAllele1())
				{
					myLocus.setAllele2(c);
				}
			}
			
			if (doa1)
			{
				if (c == '0')
				{
					myPerson->addHasGenotype( false );
					myPerson->addAllele1( false );
				}
				else if (c == myLocus.getAllele1())
				{
					myPerson->addHasGenotype( true );
					myPerson->addAllele1( false );
					myLocus.incrAllele1Count();
				}
				else if (c == myLocus.getAllele2())
				{
					myPerson->addHasGenotype( true );
					myPerson->addAllele1( true );
					myLocus.incrAllele2Count();
				}
				else
				{
					error("\nSNP " + myLocus.getId() + " allele 1. More than two SNP alleles detected in population:\n'" 
					+ myLocus.getAllele1() + "' '" + myLocus.getAllele2() + "' '" + c + "'");
				}
				
				doa1 = false;
			}
			else
			{
				if (c == '0')
				{
					myPerson->addAllele2( false );
				}
				else if (c == myLocus.getAllele1())
				{
					myPerson->addAllele2( false );
					myLocus.incrAllele1Count();
				}
				else if (c == myLocus.getAllele2())
				{
					myPerson->addAllele2( true );
					myLocus.incrAllele2Count();
				}
				else
				{
					error("\nSNP " + myLocus.getId() + " allele 2. More than two SNP alleles detected in population:\n'" 
					+ myLocus.getAllele1() + "' '" + myLocus.getAllele2() + "' '" + c + "'");
				}
				
				doa1 = true;
				++indCount;
			}
		}
		
		// Increment line count
		++lineCount;
		
		// add to LocusMap
		lm.addLocus( myLocus );
	}
	
	lm.setMajorAllele();
	
	par::snpCount = lineCount;
	
	writeLog(convertToString( lineCount ) + " markers read\n\n" );
}

////////////////////////////////////////
// Read method for PLINK .genome file //
////////////////////////////////////////
void readPlinkGenome( DuoMap& duo, Ped& ped )
{
	int EMPTY_INDEX = -1;
	
	bool stringBool = false;
	char c = ' ';
	
	////////////////////
	// String buffers //
	////////////////////
	bool fid1Set, iid1Set, fid2Set, iid2Set;
	fid1Set = iid1Set = fid2Set = iid2Set = false;
	string buffer, fid1Buffer, iid1Buffer, fid2Buffer, iid2Buffer;
	buffer = fid1Buffer = iid1Buffer = fid2Buffer = iid2Buffer = " ";
	
	/////////////////
	// IBS buffers //
	/////////////////
	bool ibs0Set, ibs1Set, ibs2Set;
	ibs0Set = ibs1Set = ibs2Set = false;
	int ibs0Buffer, ibs1Buffer, ibs2Buffer;
	ibs0Buffer = ibs1Buffer = ibs2Buffer = 0;
	
	////////////////////////
	// Index placeholders //
	////////////////////////
	int fid1, iid1, fid2, iid2, ibs0, ibs1, ibs2, index, pedIndex, lineCount;
	fid1 = iid1 = fid2 = iid2 = ibs0 = ibs1 = ibs2 = EMPTY_INDEX;
	index = pedIndex = lineCount = 0;
	
	writeLog("Reading PLINK genome file " + string2Filename(par::genomeFile) + "\n");
	
	// Open specified file
	ifstream GENOME( par::genomeFile.c_str(), ios::in );
	if (not GENOME.good()) { error( "PLINK genome file " + string2Filename(par::genomeFile) + " cannot be read"); }
	
	// On first line figure out column positions (FID1, IID1, FID2, IID2, IBS0, IBS1,IBS2)
	while (!GENOME.eof())
	{
		stringBool = readWhiteSpacePaddedString( GENOME, buffer, c );
		
		if (stringBool)
		{
			if (buffer == "FID1")
			{
				fid1 = index;
			}
			else if (buffer == "IID1")
			{
				iid1 = index;
			}
			else if (buffer == "FID2")
			{
				fid2 = index;
			}
			else if (buffer == "IID2")
			{
				iid2 = index;
			}
			else if (buffer == "IBS0")
			{
				ibs0 = index;
			}
			else if (buffer == "IBS1")
			{
				ibs1 = index;
			}
			else if (buffer == "IBS2")
			{
				ibs2 = index;
			}
		}
		else { break; }
		
		++index;
	}
		
	// Check for all required columns
	// -1 corresponds to the initializer value used above to for these variables
	if (fid1 == EMPTY_INDEX) { error("FID1 not found in genome file"); }
	else if (iid1 == EMPTY_INDEX) { error("IID1 not found in genome file"); }
	else if (fid2 == EMPTY_INDEX) { error("FID2 not found in genome file"); }
	else if (iid2 == EMPTY_INDEX) { error("IID2 not found in genome file"); }
	else if (ibs0 == EMPTY_INDEX) { error("IBS0 not found in genome file"); }
	else if (ibs1 == EMPTY_INDEX) { error("IBS1 not found in genome file"); }
	else if (ibs2 == EMPTY_INDEX) { error("IBS2 not found in genome file"); }
	
	// Loop through file reading in data to proper columns
	// NOTE need to specify method for writing this data to file with no expected
	index = 0;
	while (!GENOME.eof())
	{
		if (readWhiteSpacePaddedString( GENOME, buffer, c ))
		{
			if (index == fid1)
			{
				fid1Buffer = buffer;
				fid1Set = true;
			}
			else if (index == iid1)
			{
				iid1Buffer = buffer;
				iid1Set = true;
			}
			else if (index == fid2)
			{
				fid2Buffer = buffer;
				fid2Set = true;
			}
			else if (index == iid2)
			{
				iid2Buffer = buffer;
				iid2Set = true;
			}
			else if (index == ibs0)
			{
				ibs0Buffer = string2Int(buffer);
				ibs0Set = true;
			}
			else if (index == ibs1)
			{
				ibs1Buffer = string2Int(buffer);
				ibs1Set = true;
			}
			else if (index == ibs2)
			{
				ibs2Buffer = string2Int(buffer);
				ibs2Set = true;
			}
			
		}
		
		else
		{
			index = 0;
			fid1Buffer = iid1Buffer = fid2Buffer = iid2Buffer = " ";
			ibs0Set = ibs1Set = ibs2Set = fid1Set = iid1Set = fid2Set = iid2Set = false;
			ibs0Buffer = ibs1Buffer = ibs2Buffer = 0;
			++lineCount;
			continue;
		}
		
		if (ibs2Set and ibs1Set and ibs0Set and iid2Set and fid2Set and iid1Set and fid1Set)
		{
			// if all of these are true then all values are ready for storage
			
			Duo myDuoObject;
			
			if ( not ped.boolHavePerson( fid1Buffer, iid1Buffer, pedIndex ) )
			{
				myDuoObject.setInd1( ped[pedIndex] );
			}
			else
			{
				Person *per = new Person;
				
				per->setFid( fid1Buffer );
				per->setIid( iid1Buffer );
				
				ped.addPerson( per );
				
				myDuoObject.setInd1( per );
			}
			
			if ( not ped.boolHavePerson( fid2Buffer, iid2Buffer, pedIndex ) )
			{
				myDuoObject.setInd2( ped[pedIndex] );
			}
			else
			{
				Person *per = new Person;
				
				per->setFid( fid2Buffer );
				per->setIid( iid2Buffer );
				
				ped.addPerson( per );
				
				myDuoObject.setInd2( per );
			}
			
			myDuoObject.setIbs0Count( ibs0Buffer );
			myDuoObject.setIbs1Count( ibs1Buffer );
			myDuoObject.setIbs2Count( ibs2Buffer );
			
			duo.push_back( myDuoObject );
		}
		
		++index;
		
		if (c == '\n')
		{
			index = 0;
			++lineCount;
		}
	}
	
	// Close specified file
	GENOME.close();
	
	writeLog( convertToString(lineCount) + " PLINK pairwise comparisons read\n\n" );
}

////////////////////////////////////////////////////
// Conversion functions. Takes char or string and //
// converts to an integer for efficiency          //
////////////////////////////////////////////////////
// ATTN possible inline???
int intChrFromString( string const& s )
{
	if (s == "X") return XINT;
	else if (s == "Y") return YINT;
	else if (s == "XY") return XYINT;
	else if (s == "MT" or s == "M" or s == "Mito" or s == "MITO") return MTINT;
	else return string2Int(s);
}
