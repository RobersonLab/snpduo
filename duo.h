#ifndef _DUO_H_
#define _DUO_H_

// stl
#include <string>
#include <vector>

// imports
using std::iterator;
using std::string;
using std::vector;

// Custom libraries
#include "helper.h"
#include "ped.h"

///////////////
// constants //
///////////////
int const UNREL = 0;
int const IDENT = 1;
int const PARCHLD = 2;
int const SIBS = 3;
int const OTHERREL = 4;
int const SECOND_DEGREE = 5;
int const SECOND_OR_THIRD_DEGREE = 6;
int const THIRD_DEGREE = 7;
int const CRYPTIC = 8;

// old method
double const ISFIRSTSLOPE = -4.1632;
double const ISFIRSTINTERCEPT = 7.2408;

double const WHICHFIRSTSLOPESQUARE = -1.5378;
double const WHICHFIRSTSLOPE = 4.739;
double const WHICHFIRSTINTERCEPT = -3.1408;

double const IDENT_MEANCUTOFF = 1.80;
double const IDENT_SDCUTOFF = 0.35;

// new method
double const NEWPARCHLD_CUTOFF=0.98;
double const NEWSIB_CUTOFF=0.89;
double const NEWSECOND_CUTOFF=0.82;
double const NEWSECONDORTHIRD_CUTOFF=0.79;
double const NEWTHIRD_CUTOFF=0.72;
double const NEWCRYPTIC_CUTOFF=0.69;
double const NEWIDENT_MEANCUTOFF=1.825;
double const NEWIDENT_SDCUTOFF=0.35;

// Class
class Duo
{
	private:
		// Members
		Person* ind1;
		Person* ind2;
		int specifiedRelationship; // Determined from Ped file
		int calculatedRelationship; // From Mean / SD
		unsigned long int ibs0Count; // A count of IBS0 calls
		unsigned long int ibs1Count; // A count of IBS1 calls
		unsigned long int ibs2Count; // A count of IBS2 calls
		unsigned long int ibs2StarCount; // A count of IBS2* calls (AB -> AB)
		double meanIbs; // Mean IBS score for analyzed snps
		double sdIbs; // SD IBS score for analyzed snps
		
		double informativePercent; // Percent informative = (IBS2* + IBS0) / (IBS2 + IBS1 + IBS0)
		double ibs2StarPercent; // Percent IBS2* = IBS2* / (IBS2 + IBS1 + IBS0)
		double ibs2StarPercentOfInformative; // Percent of informative = IBS2* / (IBS2* + IBS0)
		
	public:
		// Constructor
		Duo(): ind1(0), ind2(0), specifiedRelationship(0), calculatedRelationship(0), ibs0Count(0), ibs1Count(0), ibs2Count(0), 
		ibs2StarCount(0), meanIbs(0.0), sdIbs(0.0), informativePercent(0.0), ibs2StarPercent(0), ibs2StarPercentOfInformative(0.0) {}
		
		Duo(Person* p1, Person* p2): ind1(p1), ind2(p2), specifiedRelationship(0), calculatedRelationship(0), ibs0Count(0), ibs1Count(0),
		ibs2Count(0), ibs2StarCount(0), meanIbs(0.0), sdIbs(0.0), informativePercent(0.0), ibs2StarPercent(0.0), ibs2StarPercentOfInformative(0.0) {}
		
		Duo(Person* p1, Person* p2, unsigned long int ibs0, unsigned long int ibs1, unsigned long int ibs2, unsigned long int ibs2star): ind1(p1),
		ind2(p2), specifiedRelationship(0), calculatedRelationship(0), ibs0Count(ibs0), ibs1Count(ibs1), ibs2Count(ibs2),
		ibs2StarCount(ibs2star), meanIbs(0.0), sdIbs(0.0), informativePercent(0.0), ibs2StarPercent(0.0),
		ibs2StarPercentOfInformative(0.0) {}
		
		// assignment
		void setInd1( Person* p ) { ind1 = p; }
		void setInd2( Person* p ) { ind2 = p; }
		void setSpecifiedRelationship( int i ) { specifiedRelationship = i; }
		void setCalculatedRelationship( int i ) { calculatedRelationship = i; }
		void setIbs0Count( unsigned long int i ) { ibs0Count = i; }
		void setIbs1Count( unsigned long int i ) { ibs1Count = i; }
		void setIbs2Count( unsigned long int i ) { ibs2Count = i; }
		void setIbs2StarCount( unsigned long int i ) { ibs2StarCount = i; }
		void setMeanIbs( double d ) { meanIbs = d; }
		void setSdIbs( double d ) { sdIbs = d; }
		void setInformativePercent( double d ) { informativePercent = d; }
		void setIbs2StarPercent( double d ) { ibs2StarPercent = d; }
		void setIbs2StarPercentOfInformative( double d ) { ibs2StarPercentOfInformative = d; }
		
		// accessor
		Person* getInd1() { return ind1; }
		Person* getInd2() { return ind2; }
		int getSpecifiedRelationship() { return specifiedRelationship; }
		int getCalculatedRelationship() { return calculatedRelationship; }
		unsigned long int getIbs0Count() { return ibs0Count; }
		unsigned long int getIbs1Count() { return ibs1Count; }
		unsigned long int getIbs2Count() { return ibs2Count; }
		unsigned long int getIbs2StarCount() { return ibs2StarCount; }
		double getMeanIbs() { return meanIbs; }
		double getSdIbs() { return sdIbs; }
		double getInformativePercent() { return informativePercent; }
		double getIbs2StarPercent() { return ibs2StarPercent; }
		double getIbs2StarPercentOfInformative() { return ibs2StarPercentOfInformative; }
};

class DuoMap
{
	private:
		// Members
		vector<Duo> data;
		unsigned long int dataCount;
	
	public:
		// accessor
		Duo& operator[] (unsigned long int i) { return data[i]; }
		
		// assigner
		void push_back( Duo& d )
		{
			data.push_back(d);
			++dataCount;
		}
		
		// functions
		unsigned long int count() { return dataCount; }
		
		// iterators
		vector<Duo>::iterator begin() { return data.begin(); }
		vector<Duo>::iterator end() { return data.end(); }
		vector<Duo>::const_iterator begin() const { return data.begin(); }
		vector<Duo>::const_iterator end() const { return data.end(); }
		
		// reserve
		void reserve( unsigned long int i ) { data.reserve(i); }
		
		// Analysis functions
		void fillIdentities( Ped&  );
		void getCounts( Ped&  );
		void getMeanAndSDFromCounts();
		void specifiedRelationships( Ped&  );
		void oldCalculatedRelationships();
		void calculatedRelationships();
		
		// File writing functions -- in output.cpp
		void printCounts();
		void printMeanSD();
		void printSinglePoint();
		void printAberrant();
		void printRelationships();
		void printSpecifiedAndCalculated();
		void printCalculatedOnly();
		void printConflicted();
};

class IbsCount
{
	private:
		unsigned long int ibs0;
		unsigned long int ibs1;
		unsigned long int ibs2;
		unsigned long int ibs2star;
	
	public:
		// increment
		void addIbs0() { ++ibs0; }
		void addIbs1() { ++ibs1; }
		void addIbs2() { ++ ibs2; }
		void addIbs2Star() { ++ibs2star; }
		
		// accessors
		unsigned long int getIbs0() { return ibs0; }
		unsigned long int getIbs1() { return ibs1; }
		unsigned long int getIbs2() { return ibs2; }
		unsigned long int getIbs2Star() { return ibs2star; }
		
		// constructor
		IbsCount(): ibs0(0), ibs1(0), ibs2(0), ibs2star(0)  {}
};

IbsCount getIBS( Person*, Person* );

double calculateSD (double, int, int, int);

/////////////
// inlines //
/////////////

// ATTN may be good candidates for optimizatin.
// The addition / multiplication may do better with an operator= format to avoid temp objects
inline double isFirstDegreeClassifier ( double mean, double sd )
{
	return ((ISFIRSTSLOPE * mean) + ISFIRSTINTERCEPT);
}

inline double whichFirstDegreeClassifier ( double mean, double sd )
{
	return (WHICHFIRSTSLOPESQUARE * (mean * mean)) + (WHICHFIRSTSLOPE * mean) + WHICHFIRSTINTERCEPT;
}

#endif
