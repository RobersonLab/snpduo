#ifndef _LOCUS_H_
#define _LOCUS_H_

// stl
#include <string>
#include <vector>

// imports
using std::iterator;
using std::string;
using std::vector;

// custom headers
#include "options.h"
#include "helper.h"

// constants

// classes
class Locus
{
	private:
		int chrom;
		int position;
		int genetic;
		char allele1;
		char allele2;
		bool hasAllele1;
		bool hasAllele2;
		unsigned long int allele1Count;
		unsigned long int allele2Count;
		bool allele1Major;
		string id;
	
	public:
		// constructor
		Locus(): chrom(0), position(0), genetic(0), allele1('N'), allele2('N'), hasAllele1(false), hasAllele2(false), allele1Count(0), allele2Count(0), allele1Major(false), id("NULL_ID") {}
		Locus(int myChrom, int myPosition, int myGenetic, string myId):chrom(myChrom), position(myPosition), genetic(myGenetic), id(myId) {}
		
		// assigners
		void setChrom(int i) { chrom = i; }
		void setPosition(int i) {position = i; }
		void setGenetic(int i) {genetic = i; }
		void setAllele1( char c )
		{
			allele1 = c;
			hasAllele1 = true;
		}
		void setAllele2( char c )
		{
			allele2 = c;
			hasAllele2 = true;
		}
		void setAllele1Count( unsigned long int i ) { allele1Count = i; }
		void setAllele2Count( unsigned long int i ) { allele2Count = i; }
		void setAllele1Major( bool b ) { allele1Major = b; }
		void setId(string const& s) { id = s; }
		
		void incrAllele1Count() { ++allele1Count; }
		void incrAllele2Count() { ++allele2Count; }
		
		// accessors
		int getChrom() { return chrom; }
		int getPosition() { return position; }
		int getGenetic() { return genetic; }
		string getId() { return id; }
		char getAllele1() { return allele1; }
		char getAllele2() { return allele2; }
		bool getHasAllele1() { return hasAllele1; }
		bool getHasAllele2() { return hasAllele2; }
		unsigned long int getAllele1Count() { return allele1Count; }
		unsigned long int getAllele2Count() { return allele2Count; }
		bool getAllele1Major() { return allele1Major; }
		
		// overloaded operators
		bool operator< (Locus const& rhs) const
		{
			return chrom < rhs.chrom or (chrom == rhs.chrom and position < rhs.position);
		}
		
		bool operator> (Locus const& rhs) const
		{
			return chrom > rhs.chrom or (chrom == rhs.chrom and position > rhs.position);
		}
};

class LocusMap
{
	private:
		vector<Locus> markerLoci;
		unsigned int markerCount;
	
	public:
		// counts
		unsigned int numMarkers() { return markerLoci.size(); }
		
		// constructor
		LocusMap():markerLoci(0), markerCount(0) { if (par::snpCount) { markerLoci.reserve( par::snpCount ); } }
		
		// assigners
		void addLocus( Locus const& l ) { markerLoci.push_back( l ); ++markerCount; }
		void reserve( int i ) { markerLoci.reserve( i ); }
		void reserve( unsigned int i ) { markerLoci.reserve( i ); }
		
		// functions
		unsigned int size() { return markerCount; }
		void read();
		void write();
		
		// iterators
		vector<Locus>::iterator begin() { return markerLoci.begin(); }
		vector<Locus>::iterator end() { return markerLoci.end(); }
		
		// set major
		void setMajorAllele()
		{
			for (vector<Locus>::iterator mapIter = begin(); mapIter != end(); ++mapIter)
			{
				if (mapIter->getAllele1() > mapIter->getAllele2())
				{
					mapIter->setAllele1Major( true );
				}
				else
				{
					mapIter->setAllele1Major( false );
				}
			}
		}
		
		// overloaded operator
		Locus& operator[] (unsigned int i)
		{
			if (i >= size()) { error ("Attempting at to access a LocusMap object bin out of range"); }
			return markerLoci[i];
		}
};

// functions

#endif
