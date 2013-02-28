#ifndef _SNPDUO_H_
#define _SNPDUO_H_

#include <string>
#include <vector>

#include "helper.h"

// Classes
class Map
{
	public:
	// Class members
	vector<int> chr;
	vector<string> rsid;
	vector<int> genetic;
	vector<int> position;
	
	// Functions
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
	
	// Functions
	Person(): fid(" "), iid( fid ), pid( fid ), mid( fid ), sex( 0 ), pheno( 0 )
	{
		a1.resize(0);
		a2.resize(0);
	}
};

class Ped
{
	public:
	// Class members
	vector<Person*> samples;
	
	vector<bool> hasAllele1;
	vector<bool> hasAllele2;
	vector<char> allele1;
	vector<char> allele2;
	
	// Functions
	int getIndIndex( string, string );
	void print();
	int findPerson( string &, string & );
	string fileGenotypeString( int, int );
	
	// Constructor resizing vector to 0 for starting point
	Ped() { samples.resize( 0 ); }
	
	// Destructor -- release memory from each pointer to avoid leaks
	~Ped() { for (unsigned int i = 0; i < samples.size(); ++i) delete samples[i]; }
};

#endif
