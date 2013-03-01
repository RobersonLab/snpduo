#ifndef _SNPDUO_H_
#define _SNPDUO_H_

#include <string>
#include <vector>

#include "helper.h"
#include "options.h"

// Classes
class Map
{
	public:
	// Class members
	vector<int> chr;
	vector<int> genetic;
	vector<int> position;
	vector<string> rsid;
	
	// Count function
	unsigned int numMarkers() { return chr.size(); }
	
	// Constructor
	Map(): chr( 0 ), rsid( 0 ), genetic( 0 ), position( 0 )
	{
		if (par::snpcount != 0)
		{
			chr.reserve( par::snpcount );
			rsid.reserve( par::snpcount );
			genetic.reserve( par::snpcount );
			position.reserve( par::snpcount );
		}
	}
	
	// Write function
	void print();
};

class Person
{
	public:
	// Class Members
	string fid;
	string iid;
	string pid;
	string mid;
	
	// Consider moving Sex / Phenotype to Ped and making bool for simply input and for memory saving space.
	// Each int minimum 2 bytes ( 16 bits ). double vector<bool> = 2 bits for each value ( 8X less )
	int sex;
	int pheno;
	
	// SNP data
	vector<bool> a1;
	vector<bool> a2;
	vector<bool> hasGenotype;
	
	// Constructor
	Person(): fid(" "), iid( fid ), pid( fid ), mid( fid ), sex( 0 ), pheno( 0 ), a1( 0 ), a2( 0 ), hasGenotype( 0 )
	{
		if ( par::snpcount != 0 )
		{
			a1.reserve( par::snpcount );
			a2.reserve( par::snpcount );
			hasGenotype.reserve( par::snpcount );
		}
	}
};

class Ped
{
	public:
	// Class members
	vector<Person*> samples;
	
	vector<char> allele1;
	vector<char> allele2;
	vector<bool> hasAllele1;
	vector<bool> hasAllele2;
		
	// Functions
	unsigned int numPeople() { return samples.size(); }
	
	// Search functions
	int getIndIndex( string, string );
	bool boolHavePerson( string &, string & , int & );
	int findPerson( string &, string & );
	
	// Writing functions
	void print();
	
	// Miscellaneous	
	string fileGenotypeString( int, int );
	
	// Constructor resizing vector to 0 for starting point
	Ped(): hasAllele1( 0 ), hasAllele2( 0 ), allele1( 0 ), allele2( 0 ), samples( 0 )
	{
		if (par::snpcount != 0)
		{
			hasAllele1.reserve( par::snpcount );
			hasAllele2.reserve( par::snpcount );
			allele1.reserve( par::snpcount );
			allele2.reserve( par::snpcount );
		}
		
		if (par::personcount != 0)
		{
			samples.reserve( par::personcount );
		}
	}
	
	// Destructor -- release memory from each pointer to avoid leaks
	~Ped() { for (unsigned int i = 0; i < samples.size(); ++i) delete samples[i]; }
};

#endif
