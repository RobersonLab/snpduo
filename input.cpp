// Standard libraries
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>

// Namespace
using namespace std;

// Custom libraries
#include "snpduo.h"
#include "helper.h"
#include "options.h"
#include "input.h"
#include "output.h"

// Macros

// Functions

//////////////////////////////////////////////////
// Ped / Map file handling
//////////////////////////////////////////////////
// void readMapFile (Map &m)
// {
// 	printLog( "Reading map file [ " + par::mapfile + " ]\n" );
// 	
// 	string buffer = " "; // For reading chars into strings
// 	int linecount = 0; // Count for markers
// 	bool stringBool = false; // bool to determine if value read
// 	
// 	ifstream MAP( par::mapfile.c_str(), ios::in );
// 	
// 	if (!MAP.is_open() or !MAP.good()) error ("Map file [ " + par::mapfile + " ] cannot be read");
// 	
// 	while (1)
// 	{
// 		// Check EOF
// 		if ( MAP.eof() ) break;
// 		
// 		// Read chr
// 		stringBool = getString( MAP, buffer );
// 		
// 		if (stringBool)
// 		{
// 			if (buffer.substr(0,1) == "#") // If line starts with comment
// 			{
// 				char comment = ' ';
// 				while (comment != '\n' and !MAP.eof()) MAP.get( comment );
// 				
// 				continue;
// 			}
// 			
// 			else m.chr.push_back( intChrFromString( buffer ) );
// 		}
// 		else continue;
// 		
// 		// Read RSID and pushback
// 		stringBool = getString( MAP, buffer );
// 		if (stringBool) m.rsid.push_back( buffer );
// 		
// 		else error("No RSID for line " + int2String(linecount + 1));
// 		
// 		// Read Genetic Distance if used
// 		if (!par::map3)
// 		{
// 			stringBool = getString( MAP, buffer );
// 			
// 			if(stringBool) m.genetic.push_back( atoi( buffer.c_str() ) );
// 			
// 			else error("No Genetic Distance for line " + int2String( linecount + 1) + ". Specify --map3 if map file doesn't include genetic distance");
// 		}
// 		
// 		else m.genetic.push_back( 0 );
// 		
// 		// Read position
// 		stringBool = getString( MAP, buffer );
// 		if (stringBool) m.position.push_back( atoi( buffer.c_str() ) );
// 		
// 		else error ("No physical position for line " + int2String( linecount + 1));
// 		
// 		// Increment Line
// 		++linecount;	
// 	}
// 	
// 	MAP.close();
// 	
// 	par::snpcount = linecount;
//	
//	printLog( int2String( linecount ) + " markers read\n" );
// }

void readMapFile (Map &m)
{
	printLog( "Reading map file [ " + par::mapfile + " ]\n" );
	
	string buffer = " "; // For reading chars into strings
	int linecount = 0; // Count for markers
	
	ifstream MAP( par::mapfile.c_str(), ios::in );
	
	if (!MAP.is_open() or !MAP.good()) error ("Map file [ " + par::mapfile + " ] cannot be read");
	
	while (1)
	{
		// Check EOF
		if ( MAP.eof() ) break;
		
		// Read Chr
		if ( getString( MAP, buffer ) )
		{
			if (buffer.substr(0,1) == "#") // If line starts with comment
			{
				char comment = ' ';
				while (comment != '\n' and !MAP.eof()) MAP.get( comment );
				
				continue;
			}
			
			else m.chr.push_back( intChrFromString( buffer ) );
		}
		else continue;
		
		// Read RSID and pushback
		if ( getString( MAP, buffer ) ) m.rsid.push_back( buffer );
		
		else error("No RSID for line " + int2String( linecount + 1 ));
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			if( getString( MAP, buffer ) ) m.genetic.push_back( atoi( buffer.c_str() ) );
			
			else error("No Genetic Distance for line " + int2String( linecount + 1 ) + ". Specify --map3 if map file doesn't include genetic distance");
		}
		
		else m.genetic.push_back( 0 );
		
		// Read position
		if ( getString( MAP, buffer ) ) m.position.push_back( atoi( buffer.c_str() ) );
		
		else error ("No physical position for line " + int2String( linecount + 1 ));
		
		++linecount; // Used to count SNPs
	}
	
	MAP.close();
	
	par::snpcount = linecount;
	
	printLog( int2String( linecount ) + " markers read\n\n" );
}

// VERSION 1
void readPedFile(Ped &ped)
{
	printLog( "Reading ped file [ " + par::pedfile + " ]\n" );
	
	// Reserve appropriate vectors
	ped.hasAllele1.reserve( par::snpcount );
	ped.hasAllele2.reserve( par::snpcount );
	ped.allele1.reserve( par::snpcount );
	ped.allele2.reserve( par::snpcount );
	
	for (int i = 0; i < par::snpcount; ++i) ped.hasAllele1.push_back( false );
	for (int i = 0; i < par::snpcount; ++i) ped.hasAllele2.push_back( false );
	for (int i = 0; i < par::snpcount; ++i) ped.allele1.push_back( 'N' );
	for (int i = 0; i < par::snpcount; ++i) ped.allele2.push_back( 'N' );
	
	string buffer = " ";
	int linecount = 0;
	bool stringBool = false;
	
	ifstream PED( par::pedfile.c_str(), ios::in );
	
	if (!PED.is_open() or !PED.good()) error( "Ped file [ " + par::pedfile + " ] cannot be read" );
	
	while ( 1 )
	{
		// Check end of file
		if (PED.eof()) break;
		
		// Make a new Person
		Person *per = new Person;
		
		// Read FID
		stringBool = getString( PED, buffer );
		
		if (stringBool) // If characters were read
		{
			if (buffer.substr(0,1) == "#") // If line starts with comment
			{
				delete per;
				
				char comment = ' ';
				while (comment != '\n' and  !PED.eof())
				{
					PED.get( comment );
				}
				continue;
			}
			else per->fid = buffer;
		}
		else continue;
		
		// Read IID
		stringBool = getString( PED, buffer );
		if (stringBool) per->iid = buffer;
		else
		{
			delete per;
			error( "No Individual ID for line " + int2String( linecount + 1 ) );
		}
		
		// Read PID
		stringBool = getString( PED, buffer );
		if (stringBool) per->pid = buffer;
		else
		{
			delete per;
			error( "No Paternal ID for line " + int2String( linecount + 1 ) );
		}
		
		// Read MID
		stringBool = getString( PED, buffer );
		if (stringBool) per->mid = buffer;
		else
		{
			delete per;
			error( "No Maternal ID for line " + int2String( linecount + 1 ) );
		}
		
		// Read Sex
		stringBool = getString( PED, buffer );
		if (stringBool)
		{
			if (buffer == "1") per->sex = 1;
			else if (buffer == "2") per->sex = 2;
			else per->sex = 0;
		}
		else
		{
			delete per;
			error( "No Sex Code for  line " + int2String( linecount + 1 ) );
		}
		
		// Read Phenotype
		stringBool = getString( PED, buffer);
		if (stringBool)
		{
			if (buffer == "1") per->pheno = 1;
			else if (buffer == "2") per->pheno = 2;
			else per->pheno = 0;
		}
		else
		{
			delete per;
			error( "No Phenotype data for line " + int2String( linecount + 1 ) );
		}
		
		// Read genotypes until end of line
		// Setup important variables first
		bool doa1 = true;
		int vectorcount = 0;
		char c = ' ';

		while (1)
		{			
			PED.get( c );
			if (c == ' ' or c == '\t' or c == '\r') continue;
			if (c == '\n' or PED.eof()) break;
			
			if (!ped.hasAllele1[ vectorcount ] and c != '0')
			{
				ped.allele1[ vectorcount ] = c;
				ped.hasAllele1[ vectorcount ] = true;
			}
			else if (ped.hasAllele1[ vectorcount ] and !ped.hasAllele2[ vectorcount ] and c != '0' and c != ped.allele1[ vectorcount ] )
			{
				ped.allele2[ vectorcount ] = c;
				ped.hasAllele2[ vectorcount ] = true;
			}
			
			if (doa1)
			{
				if (c == '0')
				{
					per->hasGenotype.push_back( false );
					per->a1.push_back( false );
				}
				else if (c == ped.allele1[ vectorcount ])
				{
					per->hasGenotype.push_back( true );
					per->a1.push_back( false );
				}
				else if (c == ped.allele2[ vectorcount ])
				{
					per->hasGenotype.push_back( true );
					per->a1.push_back( true );
				}
				else error("\nSNP " + int2String( vectorcount + 1) + " allele 1, line " + int2String( linecount + 1 ) + ". More than two SNP alleles detected in population:\n'" + ped.allele1[ vectorcount] + "' '" + ped.allele2[ vectorcount ] + "' '" + c + "'");
				
				doa1 = false;
			}
			else
			{
				if (c == '0')
				{
					per->a2.push_back( false );
				}
				else if (c == ped.allele1[ vectorcount ])
				{
					per->a2.push_back( false );
				}
				else if (c == ped.allele2[ vectorcount ])
				{
					per->a2.push_back( true );
				}
				else error("\nSNP " + int2String( vectorcount + 1) + " allele 2, line " + int2String( linecount + 1 ) + ". More than two SNP alleles detected in population:\n'" + ped.allele1[ vectorcount] + "' '" + ped.allele2[ vectorcount ] + "' '" + c + "'");
				
				doa1 = true;
				++vectorcount;
			}
		}
		
		// Increment bin
		++linecount;
		
		// PUSH INTO PED VECTOR
		ped.samples.push_back( per );
	}
	
	PED.close();
	
	par::personcount = linecount;
	
	printLog( int2String( linecount ) + " individuals read\n\n" );
}

//////////////////////////////////////////////////
// TPED / TFAM file handling functions
//////////////////////////////////////////////////
void readTpedFile( Ped &ped, Map &map )
{
	string buffer = " ";
	int linecount = 0;
	
	// Read TFAM into ped
	printLog( "Reading TFAM file [ " + par::tfamfile + " ]\n" );
	
	ifstream TFAM( par::tfamfile.c_str(), ios::in );
	
	if (!TFAM.is_open() or !TFAM.good()) error ("TFAM file [ " + par::tfamfile + " ] cannot be opened");
	
	while (1)
	{
		// Check eof
		if ( TFAM.eof() ) break;
		
		Person *per = new Person;
		
		// Read FID
		if (getString( TFAM, buffer ))
		{
			if (buffer.substr(0,1) == "#") // If line is a comment
			{
				delete per;
				
				char comment = ' ';
				while (comment != '\n' and !TFAM.eof())
				{
					TFAM.get( comment );
				}
				continue;
			}
			
			else
			{
				per->fid = buffer;
			}
		}
		
		else
		{
			delete per;
			continue;
		}
		
		// Read IID
		if (getString( TFAM, buffer )) per->iid = buffer;
		
		else
		{
			delete per;
			error ("No Individual ID for line " + int2String( linecount + 1 ));
		}
		
		// Read PID
		if (getString( TFAM, buffer )) per->pid = buffer;
		
		else
		{
			delete per;
			error( "No Paternal ID for line " + int2String( linecount + 1 ) );
		}
		
		// Read MID
		if (getString( TFAM, buffer )) per->mid = buffer;
		
		else
		{
			delete per;
			error( "No Maternal ID for line " + int2String( linecount + 1 ) );
		}
		
		// Read Sex
		if (getString( TFAM, buffer )) 
		{
			if (buffer == "1")
			{
				per->sex = 1;
			}
			else if (buffer == "2")
			{
				per->sex = 2;
			}
			else
			{
				per->sex = 0;
			}
		}
		else
		{
			delete per;
			error ("No Sex code for line " + int2String( linecount + 1 ));
		}
		
		// Read Phenotype
		if (getString( TFAM, buffer))
		{
			if (buffer == "1")
			{
				per->pheno = 1;
			}
			else if (buffer == "2")
			{
				per->pheno = 2;
			}
			else
			{
				per->pheno = 0;
			}
		}
		else
		{
			delete per;
			error ("No pheno data for line " + int2String( linecount + 1 ));
		}
		
		++linecount;
		
		// PUSH THE POINTER INTO THE PED OBJECT
		ped.samples.push_back( per );
	}
	
	par::personcount = linecount;
	
	printLog( "Pedigrees for " + int2String( linecount ) + " individuals read\n\n" );
	
	TFAM.close();
	
	// Read TPED into ped and map
	printLog( "Reading TPED from [ " + par::tpedfile + " ]\n");
	
	ifstream TPED(par::tpedfile.c_str(), ios::in);
	
	if (!TPED.is_open() or !TPED.good()) error (par::tpedfile + " couldn't be opened");
	
	linecount = 0; // Reset linecount
	
	while (1)
	{
		if (TPED.eof()) break;
		
		// Read chr
		if (getString( TPED, buffer ))
		{
			if (buffer.substr(0,1) == "#") // if a comment
			{
				char comment = ' ';
				while (comment != '\n' and !TPED.eof())
				{
					TPED.get( comment );
				}
				continue;
			}
			
			else map.chr.push_back( intChrFromString( buffer ) );
		}
		
		else continue;
		
		// Read RSID
		if (getString( TPED, buffer )) map.rsid.push_back( buffer );
		
		else error ("No RSID for line " + int2String( linecount + 1));
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			if (getString( TPED, buffer )) map.genetic.push_back( atoi( buffer.c_str() ) );
			
			else error ("No Genetic Distance for line " + int2String( linecount + 1 ) + ". Specify --map3 if TPED file doesn't include genetic distance.");
		}
		
		else map.genetic.push_back( 0 );
		
		// Read Position
		if (getString( TPED, buffer ))
		{
			map.position.push_back( atoi( buffer.c_str() ) );
		}
		
		else error ("No physical position for line " + int2String( linecount + 1 ));
		
		// Read in genotype
		bool doa1 = true;
		bool gotallele1 = false;
		bool gotallele2 = false;
		char c = ' ';
		
		int indCount = 0; // Must have an individual counter
		
		while (1)
		{
			TPED.get( c );
			
			if (c == ' ' or c == '\t' or c == '\r') continue;
			if (c == '\n' or TPED.eof()) break;
			
			if (!gotallele1 and c != '0')
			{
				ped.allele1.push_back( c );
				ped.hasAllele1.push_back( true );
				gotallele1 = true;
			}
			else if (gotallele1 and !gotallele2 and c != '0' and c != ped.allele1.back() )
			{
				ped.allele2.push_back( c );
				ped.hasAllele2.push_back( true );
				gotallele2 = true;
			}
			
			if (doa1)
			{
				if (c == '0')
				{
					ped.samples[ indCount ]->hasGenotype.push_back( false );
					ped.samples[ indCount ]->a1.push_back( false );
				}
				else if (c == ped.allele1[ linecount ])
				{
					ped.samples[ indCount ]->hasGenotype.push_back( true );
					ped.samples[ indCount ]->a1.push_back( false );
				}
				else if (c == ped.allele2[ linecount ])
				{
					ped.samples[ indCount ]->hasGenotype.push_back( true );
					ped.samples[ indCount ]->a1.push_back( true );
				}
				else
				{
					error("\nSNP " + map.rsid[ linecount ] + " allele 1. More than two SNP alleles detected in population:\n'" + ped.allele1[ linecount ] + "' '" + ped.allele2[ linecount ] + "' '" + c + "'");
				}
				
				doa1 = false;
			}
			else
			{
				if (c == '0')
				{
					ped.samples[ indCount ]->a2.push_back( false );
				}
				else if (c == ped.allele1[ linecount ])
				{
					ped.samples[ indCount ]->a2.push_back( false );
				}
				else if (c == ped.allele2[ linecount ])
				{
					ped.samples[ indCount ]->a2.push_back( true );
				}
				else
				{
					error("\nSNP " + map.rsid[ linecount ] + " allele 2. More than two SNP alleles detected in population:\n'" + ped.allele1[ linecount ] + "' '" + ped.allele2[ linecount ] + "' '" + c + "'");
				}
				
				doa1 = true;
				++indCount;
			}
		}
		
		if (!gotallele1)
		{
			ped.hasAllele1.push_back( false );
			ped.allele1.push_back( 'N' );
		}
		
		if (!gotallele2)
		{
			ped.hasAllele1.push_back( false );
			ped.allele2.push_back( 'N' );
		}
		
		// Increment line count
		++linecount;
	}
	
	par::snpcount = linecount;
	
	printLog(int2String( linecount ) + " markers read\n\n" );
}

//////////////////////////////////////////////////
// Binary file handling (development phase)
//////////////////////////////////////////////////

// void readBpedFile( Ped &ped, Map &map )
// {
// 	int tmpInt;
// 	char c[1];
// 	int count, bcount;
// 	count = bcount = 0;
// 	
// 	string file = par::outfile + ".bped";
// 	
// 	printLog("Reading bped file [ " + file + " ]\n");
//
// 	ifstream BPED.open( file.c_str(), ios::in | ios::binary );
// 	
// 	BPED.read( reinterpret_cast<char*>( &tmpInt ), sizeof(int) );
// 	par::snpcount = tmpInt;
// 	
// 	BPED.read( reinterpret_cast<char*>( &tmpInt ), sizeof(int) );
// 	par::personcount = tmpInt;
// 	
// 	// Resize all the proper variables
// 	map.chr.resize( par::snpcount );
// 	map.rsid.resize( par::snpcount );
// 	map.genetic.resize( par::snpcount );
// 	map.position.resize( par::snpcount );
// 	
// 	ped.samples.resize( par::personcount );
// 	ped.hasAllele1.resize( par::snpcount );
// 	ped.hasAllele2.resize( par::snpcount );
// 	ped.allele1.resize( par::snpcount );
// 	ped.allele2.resize( par::snpcount );
// 	
// 	// Read in hasAllele1
// 	count = 0;
// 	
// 	while (count < par::snpcount)
// 	{
// 		bitset<8> bits;
// 		bits.reset();
// 		
// 		BPED.read( c, 1 );
// 		bits = c[0];
// 		
// 		bcount = 0;
// 		
// 		while (bcount < 8 and count < par::snpcount)
// 		{
// 			if (bits[bcount]) ped.hasAllele1[count] = true;
// 			else ped.hasAllele1[count] = false;
// 			
// 			++bcount;
// 			++count;
// 		}
// 	}
// 	
// 	// Read in hasAllele2
// 	count = 0;
// 	
// 	while (count < par::snpcount)
// 	{
// 		bitset<8> bits;
// 		bits.reset();
// 		
// 		BPED.read( c, 1 );
// 		bits = c[0];
// 		
// 		bcount = 0;
// 		
// 		while (bcount < 8 and count < par::snpcount)
// 		{
// 			if (bits[bcount]) ped.hasAllele2[count] = true;
// 			else ped.hasAllele2[count] = false;
// 			
// 			++bcount;
// 			++count;
// 		}
// 	}
// 	
// 	// Read in Person objects
// 	for (int i = 0; i < par::personcount; ++i)
// 	{
// 		Person *per = new Person;
// 		
// 		// hasGenotype
// 		count = 0;
// 		
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			BPED.read( c, 1 );
// 			
// 			bits = c[0];
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (bits[bcount]) per->hasGenotype[count] = true;
// 				else per->hasGenotype[count] = false;
// 				
// 				++count;
// 				++bcount;
// 			}
// 		}
// 		
// 		// a1
// 		count = 0;
// 		
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			BPED.read( c, 1 );
// 			
// 			bits = c[0];
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (bits[bcount]) per->a1[count] = true;
// 				else per->a1[count] = false;
// 				
// 				++count;
// 				++bcount;
// 			}
// 		}
// 		
// 		// a2
// 		count = 0;
// 		
// 		while (count < par::snpcount)
// 		{
// 			bitset<8> bits;
// 			bits.reset();
// 			
// 			BPED.read( c, 1 );
// 			
// 			bits = c[0];
// 			
// 			bcount = 0;
// 			
// 			while (bcount < 8 and count < par::snpcount)
// 			{
// 				if (bits[bcount]) per->a2[count] = true;
// 				else per->a2[count] = false;
// 				
// 				++count;
// 				++bcount;
// 			}
// 		}
// 		
// 		// Add per to sample list
// 		ped.samples[i] = per;		
// 	}
// 	
// 	BPED.close();
// 	printLog(" done!\n");
// }

// void readBfamFile( Ped &ped )
// {
// 	string file = par::outfile + ".bfam";
// 	string buf = " ";
// 	
// 	printLog("Reading bfam file [ " + file + " ]\n");
// 	
// 	ifstream BFAM.open( file.c_str(), ios::in );
// 	
// 	for (int i = 0; i < par::personcount; ++i)
// 	{
// 		getline( BFAM, buf, ' ' );
// 		ped.samples[i]->fid = buf;
// 		
// 		getline( BFAM, buf, ' ' );
// 		ped.samples[i]->iid = buf;
// 		
// 		getline( BFAM, buf, ' ' );
// 		ped.samples[i]->pid = buf;
// 		
// 		getline( BFAM, buf, ' ' );
// 		ped.samples[i]->mid = buf;
// 		
// 		getline( BFAM, buf, ' ' );
// 		ped.samples[i]->sex = atoi( buf.c_str() );
// 		
// 		getline( BFAM, buf, '\n');
// 		ped.samples[i]->pheno = atoi( buf.c_str() );
// 	}
// 		
// 	BFAM.close();
// }

// void readBmapFile( Ped &ped, Map &map )
// {	
// 	string file = par::outfile + ".bmap";
// 	string buf = " ";
// 	
// 	printLog("Reading bmap file from [ " + file + " ]\n");
// 	
// 	ifstream BMAP.open( file.c_str(), ios::in );
// 	
// 	char c = ' ';
// 	for (int i = 0; i < par::snpcount; ++i)
// 	{
// 		getline( BMAP, buf, ' ' );
// 		map.chr[i] = atoi( buf.c_str() );
// 		
// 		getline( BMAP, buf, ' ');
// 		map.rsid[i] = buf;
// 		
// 		getline( BMAP, buf, ' ');
// 		map.genetic[i] = atoi( buf.c_str() );
// 		
// 		getline( BMAP, buf, ' ');
// 		map.position[i] = atoi( buf.c_str() );
// 		
// 		BMAP.get(c); // Get space
// 		BMAP.get(c); // Get Allele1
// 		ped.allele1[i] = c;
// 		
// 		BMAP.get(c); // Get space
// 		BMAP.get(c); // Get Allele2
// 		ped.allele2[i] = c;
// 		
// 		while (c != '\n') BMAP.get(c);
// 	}
// 	
// 	BMAP.close();
// }

//////////////////////////////////////////////////
// Read method for PLINK .genome file
//////////////////////////////////////////////////
void readPLINKGenome( Duo &duo, Ped &ped )
{
	// Set up indexes
	bool stringBool = false;
	string buffer, fid1Buffer, iid1Buffer, fid2Buffer, iid2Buffer;
	buffer = fid1Buffer = iid1Buffer = fid2Buffer = iid2Buffer = " ";
	char c = ' ';
	
	int fid1,iid1,fid2,iid2,ibs0,ibs1,ibs2, index, pedIndex, linecount;
	fid1 = iid1 = fid2 = iid2 = ibs0 = ibs1 = ibs2 = -1;
	index = pedIndex = linecount = 0;
	
	printLog("Reading PLINK genome file [ " + par::genomefile + " ]\n");
	// Open specified file
	ifstream GENOME( par::genomefile.c_str(), ios::in );
	
	if (!GENOME.is_open() or !GENOME.good()) error( "PLINK genome file [ " + par::genomefile + " ] cannot be read");
	
	// On first line figure out column positions (FID1, IID1, FID2, IID2, IBS0, IBS1,IBS2)
	while (1)
	{
		if (GENOME.eof()) error("Empty genome file specified");
		
		stringBool = getWhiteSpacePaddedString( GENOME, buffer, c );
		
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
		else break;
		
		++index;
	}
		
	// Check for all required columns
	// -1 corresponds to the initializer value used above to for these variables
	if (fid1 == -1) error("FID1 not found in genome file");
	else if (iid1 == -1) error("IID1 not found in genome file");
	else if (fid2 == -1) error("FID2 not found in genome file");
	else if (iid2 == -1) error("IID2 not found in genome file");
	else if (ibs0 == -1) error("IBS0 not found in genome file");
	else if (ibs1 == -1) error("IBS1 not found in genome file");
	else if (ibs2 == -1) error("IBS2 not found in genome file");
	
	// Loop through file reading in data to proper columns NOTE need to specify method for writing this data to file with no expected
	index = 0;
	while (1)
	{
		if (GENOME.eof()) break;
		
		stringBool = getWhiteSpacePaddedString( GENOME, buffer, c );
		
		if (stringBool)
		{
			if (index == fid1)
			{
				fid1Buffer = buffer;
			}
			else if (index == iid1)
			{
				iid1Buffer = buffer;
			}
			else if (index == fid2)
			{
				fid2Buffer = buffer;
			}
			else if (index == iid2)
			{
				iid2Buffer = buffer;
			}
			else if (index == ibs0)
			{
				duo.ibs0Count.push_back( atoi( buffer.c_str() ) );
			}
			else if (index == ibs1)
			{
				duo.ibs1Count.push_back( atoi( buffer.c_str() ) );
			}
			else if (index == ibs2)
			{
				duo.ibs2Count.push_back( atoi( buffer.c_str() ) );
			}
			
		}
		
		else
		{
			index = 0;
			fid1Buffer = iid1Buffer = fid2Buffer = iid2Buffer = " ";
			++linecount;
			continue;
		}
		
		if (fid1Buffer != " " and iid1Buffer != " ")
		{
			if (ped.boolHavePerson( fid1Buffer, iid1Buffer, pedIndex ))
			{
				duo.ind1Index.push_back( ped.samples[ pedIndex ] );
			}
			
			else
			{
				Person *per = new Person;
				
				per->fid = fid1Buffer;
				per->iid = iid1Buffer;
				
				ped.samples.push_back( per );
				duo.ind1Index.push_back( per );
			}
			
			fid1Buffer = iid1Buffer = " ";
		}
		
		if (fid2Buffer != " " and iid2Buffer != " ")
		{
			if (ped.boolHavePerson( fid2Buffer, iid2Buffer, pedIndex ))
			{
				duo.ind2Index.push_back( ped.samples[ pedIndex ] );
			}
			else
			{
				Person *per = new Person;
				
				per->fid = fid2Buffer;
				per->iid = iid2Buffer;
				
				ped.samples.push_back( per );
				duo.ind2Index.push_back( per );
			}
			
			fid2Buffer = iid2Buffer = " ";
		}
		
		++index;
		
		if (c == '\n')
		{
			index = 0;
			++linecount;
		}
	}
	
	// Close specified file
	GENOME.close();
	
	printLog( int2String( linecount ) + " PLINK pairwise comparisons read\n\n" );
}

//////////////////////////////////////////////////
// Conversion functions. Takes char or string and
// converts to an integer for efficiency
//////////////////////////////////////////////////
int intChrFromString(const string &s)
{
	if (s == "X") return XINT;
	else if (s == "Y") return YINT;
	else if (s == "XY") return XYINT;
	else if (s == "MT" or s == "M" or s == "Mito" or s == "MITO") return MTINT;
	else return atoi( s.c_str() );
}
