#ifndef _DUO_H_
#define _DUO_H_

// Standard libraries
#include <string>
#include <vector>

// Custom libraries
#include "helper.h"
#include "snpduo.h"

// Macro -- needed in multiple files
const string RELATIONSHIP[] = {"Unrelated","Identical","ParentChild","Sibling","OtherRelated"};

// Class
class Duo
{
	public:
	// Variables
	vector<Person*> ind1Index;
	vector<Person*> ind2Index;
	vector<int> specifiedRelationship; // Determined from ped file
	vector<int> calculatedRelationship; // From Mean / SD
	vector<int> ibs0Count; // A count of IBS0 calls
	vector<int> ibs1Count; // A count of IBS1 calls
	vector<int> ibs2Count; // A count of IBS2 calls
	vector<int> ibs2starCount; // A count of IBS2* calls (AB -> AB)
	vector<double> meanIbs; // Mean IBS score for analyzed snps
	vector<double> sdIbs; // SD IBS score for analyzed snps
	
	vector<double> informativepercent; // Percent informative = (IBS2* + IBS0*) / (IBS2 + IBS1 + IBS0)
	vector<double> ibs2starpercent; // Percent IBS* = IBS2* / (IBS2* + IBS1 + IBS0*)
	
	// Size functions
	unsigned int size() { return ind1Index.size(); }
	unsigned int numCounts() { return ibs0Count.size(); }
	unsigned int numSpecified() { return specifiedRelationship.size(); }
	unsigned int numCalculated() { return calculatedRelationship.size(); }
	unsigned int numMean() { return meanIbs.size(); }
	unsigned int numSD() { return sdIbs.size(); }
	unsigned int numibs2star() { return ibs2starCount.size(); }
	unsigned int numibs2starpercent() { return ibs2starpercent.size(); }
	unsigned int numinformativepercent() { return informativepercent.size(); }
	
	// Analysis functions
	void fillIdentities( const Ped & );
	void getCounts( const Ped & );
	void getMeanAndSDFromCounts();
	
	void specifiedRelationships( const Ped & );
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
	
	// Constructor
	Duo(): ind1Index( 0 ), ind2Index( 0 ), specifiedRelationship( 0 ), calculatedRelationship( 0 ), ibs0Count( 0 ), ibs1Count( 0 ), ibs2Count ( 0 ), ibs2starCount( 0 ), informativepercent( 0 ), ibs2starpercent( 0 ) {}
};

class IBSCount
{
	public:
	int ibs0;
	int ibs1;
	int ibs2;
	int ibs2star;
	
	IBSCount(): ibs0( 0 ), ibs1( 0 ), ibs2( 0 ), ibs2star( 0 )  {}
};

IBSCount getIBS( Person *, Person * );

double isFirstDegreeClassifier (const double &, const double &);
double whichFirstDegreeClassifier (const double &, const double &);

double calculateSD (const double &, const int &, const int &, const int &);

#endif
