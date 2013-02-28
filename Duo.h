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
	vector<string> fid1; // Family ID 1
	vector<string> iid1; // Individual ID 1
	vector<string> fid2; // Family ID 2
	vector<string> iid2; // Individual ID 2
	vector<int> specifiedRelationship; // Determined from ped file
	vector<int> calculatedRelationship; // From Mean / SD
	vector<int> ibs0Count; // A count of IBS0 calls
	vector<int> ibs1Count; // A count of IBS1 calls
	vector<int> ibs2Count; // A count of IBS2 calls
	vector<double> meanIbs; // Mean IBS score for analyzed snps
	vector<double> sdIbs; // SD IBS score for analyzed snps
	
	// Functions
	void fillIdentities( Ped & );
	void getCounts( Ped & );
	void getMeanAndSDFromCounts();
	
	void specifiedRelationships( Ped & );
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

double isFirstDegreeClassifier (double &, double &);
double whichFirstDegreeClassifier (double &, double &);

double calculateSD (double &, int &, int &, int &);

#endif
