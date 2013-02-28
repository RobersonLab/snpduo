// Standard libraries
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>

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
void readPedFile(Ped &ped)
{
	printLog( "Reading ped file [ " + par::pedfile + " ] ..." );
	
	// Resize appropriate vectors
	ped.hasAllele1.resize( par::snpcount );
	ped.hasAllele2.resize( par::snpcount );
	ped.allele1.resize( par::snpcount );
	ped.allele2.resize( par::snpcount );
	
	string buffer;
	stringstream ss;
	int linecount = 0;
	bool stringBool = false;
	
	ifstream myfile(par::pedfile.c_str());
	
	if (!myfile.is_open()) error ("Ped file [ " + par::pedfile + " ] couldn't be opened");
	
	while (1)
	{
		
		
		// Check end of file
		if (myfile.eof()) break;
		
		// Make a new Person
		Person *per = new Person;
		
		// Read FID
		stringBool = getString(myfile, buffer);
		
		if (stringBool) // If characters were read
		{
			if (buffer.substr(0,1) == "#") // If line starts with comment
			{
				delete per;
				
				char comment = ' ';
				while (comment != '\n' and  !myfile.eof())
				{
					myfile.get(comment);
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
			continue;
		}
		
		// Read IID
		stringBool = getString(myfile, buffer);
		if (stringBool) per->iid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error("No Individual ID for line " + ss.str());
		}
		
		// Read PID
		stringBool = getString(myfile, buffer);
		if (stringBool) per->pid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error("No Paternal ID for line " + ss.str());
		}
		
		// Read MID
		stringBool = getString( myfile, buffer );
		if (stringBool) per->mid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error("No Maternal ID for line " + ss.str());
		}
		
		// Read Sex
		stringBool = getString( myfile, buffer );
		if (stringBool)
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
			ss << linecount + 1;
			error("No Sex Code for  line " + ss.str());
		}
		
		// Read Phenotype
		stringBool = getString( myfile, buffer);
		if (stringBool)
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
			ss << linecount + 1;
			error("No Phenotype data for line " + ss.str());
		}
		
		// Read genotypes until end of line
		// Setup important variables first
		bool doa1 = true;
		per->a1.resize( par::snpcount );
		per->a2.resize( par::snpcount );
		per->hasGenotype.resize( par::snpcount );
		int vectorcount = 0;
		char c = ' ';

		while (1)
		{			
			myfile.get(c);
			if (c == ' ' or c == '\t' or c == '\r') continue;
			if (c == '\n' or myfile.eof()) break;
			
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
					per->hasGenotype[ vectorcount ] = false;
					per->a1[ vectorcount ] = false;
				}
				else if (c == ped.allele1[ vectorcount ])
				{
					per->hasGenotype[ vectorcount ] = true;
					per->a1[ vectorcount ] = false;
				}
				else if (c == ped.allele2[ vectorcount ])
				{
					per->hasGenotype[ vectorcount ] = true;
					per->a1[ vectorcount ] = true;
				}
				else
				{
					ss << linecount + 1;
					string linenum = ss.str();
					ss << vectorcount + 1;
					string snpnum = ss.str();
					
					error("\nSNP " + snpnum + " allele 1, line " + linenum + ". More than two SNP alleles detected in population:\n'" + ped.allele1[ vectorcount] + "' '" + ped.allele2[ vectorcount ] + "' '" + c + "'");
				}
				doa1 = false;
			}
			else
			{
				if (c == '0')
				{
					per->a2[ vectorcount ] = false;
				}
				else if (c == ped.allele1[ vectorcount ])
				{
					per->a2[ vectorcount ] = false;
				}
				else if (c == ped.allele2[ vectorcount ])
				{
					per->a2[ vectorcount ] = true;
				}
				else
				{
					ss << linecount + 1;
					string linenum = ss.str();
					ss << vectorcount + 1;
					string snpnum = ss.str();
					
					error("\nSNP " + snpnum + " allele 2, line " + linenum + ". More than two SNP alleles detected in population:\n'" + ped.allele1[ vectorcount] + "' '" + ped.allele2[ vectorcount ] + "' '" + c + "'");
				}
				doa1 = true;
				++vectorcount;
			}
		}
		
		// Increment bin
		++linecount;
		
		// PUSH INTO PED VECTOR
		ped.samples.push_back( per );
	}
	
	myfile.close();
	
	par::personcount = linecount;
	
	printLog( "done!\n" );
}

void readMapFile (Map &m)
{
	printLog( "Reading map file [ " + par::mapfile + " ] ..." );
	
	string buffer; // For reading chars into strings
	stringstream ss; // For string streaming values to error
	int linecount = 0; // Count for markers
	bool stringBool = false; // bool to determine if value read
	
	ifstream myfile(par::mapfile.c_str());
	
	if (!myfile.is_open()) error ("Map file [ " + par::mapfile + " ] couldn't be opened");
	
	while (1)
	{
		// Check EOF
		if (myfile.eof()) break;
		
		// Read chr
		stringBool = getString(myfile, buffer);
		
		if (stringBool)
		{
			if (buffer.substr(0,1) == "#") // If line starts with comment
			{
				char comment = ' ';
				while (comment != '\n' or !myfile.eof())
				{
					myfile.get(comment);
				}
				continue;
			}
			else
			{
				m.chr.push_back( intChrFromString( buffer ) );
			}
		}
		else continue;
		
		// Read RSID and pushback
		stringBool = getString(myfile, buffer);
		if (stringBool) m.rsid.push_back( buffer );
		else
		{
			ss << linecount + 1;
			error("No RSID for line " + ss.str());
		}
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			stringBool = getString(myfile, buffer);
			
			if(stringBool) m.genetic.push_back( atoi( buffer.c_str() ) );
			else
			{
				ss << linecount + 1;
				error("No Genetic Distance for line " + ss.str() + ". Specify --map3 if mapfile doesn't include genetic distance");
			}
		}
		else m.genetic.push_back( 0 );
		
		// Read position
		stringBool = getString(myfile, buffer);
		if (stringBool)
		{
			int tmp = atoi( buffer.c_str() );
			
			if (tmp < 0)
			{
				m.genetic.pop_back();
				m.rsid.pop_back();
				m.chr.pop_back();
				
				char skip = ' ';
				while (skip != '\n' or !myfile.eof())
				{
					myfile.get(skip);
				}
				continue;
			}
			else
			{
				m.position.push_back( tmp );
			}
		}
		else
		{
			ss << linecount + 1;
			error ("No physical position for line " + ss.str());
		}
		
		// Increment Line
		++linecount;	
	}
	
	myfile.close();
	
	par::snpcount = linecount;
	
	printLog( "done!\n" );
}

//////////////////////////////////////////////////
// TPED / TFAM file handling functions
//////////////////////////////////////////////////

void readTpedFile( Ped &ped, Map &map )
{
	string buffer;
	stringstream ss;
	int linecount = 0;
	bool stringBool = false;
	
	// Read TFAM into ped
	printLog( "Reading tfam file [ " + par::tfamfile + " ] ..." );
	
	ifstream tfam(par::tfamfile.c_str());
	if (!tfam.is_open()) error (par::tfamfile + " couldn't be opened");
	
	while (1)
	{
		// Check eof
		if (tfam.eof()) break;
		
		Person *per = new Person;
		
		// Read FID
		stringBool = getString( tfam, buffer );
		
		if (stringBool)
		{
			if (buffer.substr(0,1) == "#") // If line is a comment
			{
				delete per;
				
				char comment = ' ';
				while (comment != '\n' and !tfam.eof())
				{
					tfam.get(comment);
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
		stringBool = getString( tfam, buffer );
		if (stringBool) per->iid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error ("No Individual ID for line " + ss.str());
		}
		
		// Read PID
		stringBool = getString( tfam, buffer );
		if (stringBool) per->pid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error("No Paternal ID for line " + ss.str());
		}
		
		// Read MID
		stringBool = getString( tfam, buffer );
		if (stringBool) per->mid = buffer;
		else
		{
			delete per;
			ss << linecount + 1;
			error("No Maternal ID for line " + ss.str());
		}
		
		// Read Sex
		stringBool = getString( tfam, buffer );
		if (stringBool) 
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
			ss << linecount + 1;
			error ("No Sex code for line " + ss.str());
		}
		
		// Read Phenotype
		stringBool = getString( tfam, buffer);
		if (stringBool)
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
			ss << linecount + 1;
			error ("No pheno data for line " + ss.str());
		}
		
		++linecount;
		
		// PUSH THE POINTER INTO THE PED OBJECT
		ped.samples.push_back( per );
	}
	
	par::personcount = linecount;
	
	tfam.close();
	printLog( "done!\n" );
	
	// Read TPED into ped and map
	printLog( "Reading tped from [ " + par::tpedfile + " ] ...");
	
	ifstream tped(par::tpedfile.c_str());
	if (!tped.is_open()) error (par::tpedfile + " couldn't be opened");
	
	linecount = 0; // Reset linecount
	
	while (1)
	{
		if (tped.eof()) break;
		
		// Read chr
		stringBool = getString( tped, buffer );
		
		if (stringBool)
		{
			if (buffer.substr(0,1) == "#") // if a comment
			{
				char comment = ' ';
				while (comment != '\n' or !tped.eof())
				{
					tped.get(comment);
				}
				continue;
			}
			else
			{
				map.chr.push_back( intChrFromString( buffer ) );
			}
		}
		else continue;
		
		// Read RSID
		stringBool = getString( tped, buffer );
		if (stringBool) map.rsid.push_back( buffer );
		else
		{
			ss << linecount + 1;
			error ("No RSID for line " + ss.str());
		}
		
		// Read Genetic Distance if used
		if (!par::map3)
		{
			stringBool = getString( tped, buffer );
			
			if (stringBool) map.genetic.push_back( atoi( buffer.c_str() ) );
			else
			{
				ss << linecount + 1;
				error ("No Genetic Distance for line " + ss.str() + ". Specify --map3 if tped file doesn't include genetic distance.");
			}
		}
		else map.genetic.push_back( 0 );
		
		// Read Position
		stringBool = getString( tped, buffer );
		if (stringBool)
		{
			int tmp = atoi( buffer.c_str() );
			
			if (tmp < 0)
			{
				if (!par::map3) map.genetic.pop_back();
				map.rsid.pop_back();
				map.chr.pop_back();
				
				char skip = ' ';
				while (skip != '\n' or !tped.eof())
				{
					tped.get(skip);
				}
				continue;
			}
			else
			{
				map.position.push_back( tmp );
			}
		}
		else
		{
			ss << linecount + 1;
			error ("No physical position for line " + ss.str());
		}
		
		// Read in genotype
		bool doa1 = true;
		ped.hasAllele1.push_back( false );
		ped.hasAllele2.push_back( false );
		ped.allele1.push_back( 'N' );
		ped.allele2.push_back( 'N' );
		char c = ' ';
		
		int indCount = 0; // Must have an individual counter
		
		while (1)
		{
			tped.get(c);
			
			if (c == ' ' or c == '\t' or c == '\r') continue;
			if (c == '\n' or tped.eof()) break;
			
			if (!ped.hasAllele1[ linecount ] and c != '0')
			{
				ped.allele1[ linecount ] = c;
				ped.hasAllele1[ linecount ] = true;
			}
			else if (ped.hasAllele1[ linecount ] and !ped.hasAllele2[ linecount ] and c != '0' and c != ped.allele1[ linecount ] )
			{
				ped.allele2[ linecount ] = c;
				ped.hasAllele2[ linecount ] = true;
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
		
		// Increment line count
		++linecount;
	}
	
	tped.close();
	printLog( "done!\n" );
	
	par::snpcount = linecount;
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
// 	printLog("Reading bped file [ " + file + " ] ...");
// 	ifstream BPED;
// 	BPED.open( file.c_str(), ios::binary );
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
// 	printLog("done!\n");
// }

// void readBfamFile( Ped &ped )
// {
// 	string file = par::outfile + ".bfam";
// 	string buf = " ";
// 	
// 	printLog("Reading bfam file [ " + file + " ] ...");
// 	
// 	ifstream BFAM;
// 	BFAM.open( file.c_str() );
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
// 	
// 	printLog("done!\n");
// }

// void readBmapFile( Ped &ped, Map &map )
// {	
// 	string file = par::outfile + ".bmap";
// 	string buf = " ";
// 	
// 	printLog("Reading bmap file from [ " + file + " ] ...");
// 	
// 	ifstream BMAP;
// 	BMAP.open( file.c_str() );
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
// 	printLog("done!\n");
// }

//////////////////////////////////////////////////
// Read method for PLINK .genome file
//////////////////////////////////////////////////
void readPLINKGenome( Duo &duo )
{
	// Set up indexes
	bool stringBool = false;
	string buffer = " ";
	char c = ' ';
	
	int fid1,iid1,fid2,iid2,ibs0,ibs1,ibs2, index;
	fid1 = iid1 = fid2 = iid2 = ibs0 = ibs1 = ibs2 = -1;
	index = 0;
	
	printLog("Reading PLINK genome file [ " + par::genomefile + " ] ...");
	// Open specified file
	ifstream GENOME( par::genomefile.c_str() );
	if(!GENOME.is_open()) error("Unable to open genome file [  " + par::genomefile + " ]");
	
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
				duo.fid1.push_back( buffer );
			}
			else if (index == iid1)
			{
				duo.iid1.push_back( buffer );
			}
			else if (index == fid2)
			{
				duo.fid2.push_back( buffer );
			}
			else if (index == iid2)
			{
				duo.iid2.push_back( buffer );
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
			continue;
		}
		
		++index;
		
		if (c == '\n') index = 0;
	}
	
	// Close specified file
	GENOME.close();
	
	printLog("done!\n");
}

//////////////////////////////////////////////////
// Conversion functions. Takes char or string and
// converts to an integer for efficiency
//////////////////////////////////////////////////

int intGenoFromChar(const char &c)
{
	if (c == '1' or c == 'A' or c == 'a') return 1;
	else if (c == '2' or c == 'C' or c == 'c') return 2;
	else if (c == '3' or c == 'G' or c == 'g') return 3;
	else if (c == '4' or c == 'T' or c == 't') return 4;
	else return 0;
}

int intChrFromString(const string &s)
{
	if (s == "X") return 23;
	else if (s == "Y") return 24;
	else if (s == "XY") return 25;
	else if (s == "MT" or s == "M" or s == "Mito" or s == "MITO") return 26;
	else return atoi( s.c_str() );
}
