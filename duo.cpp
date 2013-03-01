// Standard Libraries
#include <iostream>
#include <cmath>
#include <iomanip>

// Namespace
using namespace std;

// Custom Libraries
#include "duo.h"
#include "options.h"
#include "output.h"

// Macros
const double ISFIRSTSLOPE = -4.1632;
const double ISFIRSTINTERCEPT = 7.2408;

const double WHICHFIRSTSLOPESQUARE = -1.5378;
const double WHICHFIRSTSLOPE = 4.739;
const double WHICHFIRSTINTERCEPT = -3.1408;

const double IDENT_MEANCUTOFF = 1.80;
const double IDENT_SDCUTOFF = 0.35;

const int UNREL = 0;
const int IDENT = 1;
const int PARCHLD = 2;
const int SIBS = 3;
const int OTHERREL = 4;

//////////////////////////////////////////////////
//
// Classifier Functions
//
//////////////////////////////////////////////////
inline double isFirstDegreeClassifier ( const double &mean, const double &sd )
{
	return ((ISFIRSTSLOPE * mean) + ISFIRSTINTERCEPT);
}

inline double whichFirstDegreeClassifier ( const double &mean, const double &sd )
{
	return (WHICHFIRSTSLOPESQUARE * (mean * mean)) + (WHICHFIRSTSLOPE * mean) + WHICHFIRSTINTERCEPT;
}

//////////////////////////////////////////////////
//
// Standard functions
//
//////////////////////////////////////////////////
void Duo::fillIdentities( const Ped &ped )
{
	int comparisons = (par::personcount * (par::personcount - 1)) / 2;
	
	ind1Index.reserve( comparisons );
	ind2Index.reserve( comparisons );
	
	for (int i = 0; i < par::personcount - 1; ++i)
	{
		for (int j = i + 1; j < par::personcount; ++j)
		{
			ind1Index.push_back( ped.samples[ i ] );
			ind2Index.push_back( ped.samples[ j ] );
		}
	}
}

void Duo::getCounts( const Ped &ped )
{
	printLog("Getting IBS counts\n");
	
	int comparisons = (par::personcount * (par::personcount - 1)) / 2;
	
	ind1Index.reserve( comparisons );
	ind2Index.reserve( comparisons );
	ibs0Count.reserve( comparisons );
	ibs1Count.reserve( comparisons );
	ibs2Count.reserve( comparisons );
	
	// Go comparison by comparison to calculate counts
	for (int i = 0; i < par::personcount - 1; ++i)
	{		
		for (int j = i + 1; j < par::personcount; ++j)
		{			
			ind1Index.push_back( ped.samples[ i ] );
			ind2Index.push_back( ped.samples[ j ] );
			
			IBSCount tmp = getIBS( ped.samples[ i ], ped.samples[ j ] );
			
			ibs0Count.push_back( tmp.ibs0 );
			ibs1Count.push_back( tmp.ibs1 );
			ibs2Count.push_back( tmp.ibs2 );
		}
	}
}

// IBSCount getIBS( Person *i1, Person *i2 )
// {
// 	IBSCount tmp;
// 	
// 	vector<bool>::iterator i1a1 = i1->a1.begin();
// 	vector<bool>::iterator i1a2 = i1->a2.begin();
// 	vector<bool>::iterator i2a1 = i2->a1.begin();
// 	vector<bool>::iterator i2a2 = i2->a2.begin();
// 	vector<bool>::iterator i1NC = i1->hasGenotype.begin();
// 	vector<bool>::iterator i2NC = i2->hasGenotype.begin();
// 	
// 	while ( i1a1 != i1->a1.end() )
// 	{
// 		bool NC1 = *i1NC;
// 		
// 		if (NC1)
// 		{
// 			bool NC2 = *i2NC;
// 			
// 			if (NC2)
// 			{
// 				bool p1a1 = *i1a1;
// 				bool p1a2 = *i1a2;
// 				bool p2a1 = *i2a1;
// 				bool p2a2 = *i2a2;
// 				
// 				if (p1a1 == p2a1)
// 				{
// 					if (p1a2 == p2a2) tmp.ibs2++;
// 					else tmp.ibs1++;
// 				}
// 				else
// 				{
// 					if (p1a2 == p2a2) tmp.ibs1++;
// 					else tmp.ibs0++;
// 				}
// 				
// 				++i1a1;
// 				++i1a2;
// 				++i2a1;
// 				++i2a2;
// 				++i1NC;
// 				++i2NC;
// 			}
// 			else
// 			{
// 				++i1a1;
// 				++i1a2;
// 				++i2a1;
// 				++i2a2;
// 				++i1NC;
// 				++i2NC;
// 			}
// 		}
// 		
// 		else
// 		{
// 			++i1a1;
// 			++i1a2;
// 			++i2a1;
// 			++i2a2;
// 			++i1NC;
// 			++i2NC;
// 		}
// 	}
// 	
// 	return tmp;
// }

IBSCount getIBS( Person *i1, Person *i2 )
{
	IBSCount tmp;
	
	vector<bool>::iterator i1a1 = i1->a1.begin();
	vector<bool>::iterator i1a2 = i1->a2.begin();
	vector<bool>::iterator i2a1 = i2->a1.begin();
	vector<bool>::iterator i2a2 = i2->a2.begin();
	vector<bool>::iterator i1NC = i1->hasGenotype.begin();
	vector<bool>::iterator i2NC = i2->hasGenotype.begin();
	
	while ( i1a1 != i1->a1.end() )
	{
		bool NC1 = *i1NC;
		
		if (NC1)
		{
			bool NC2 = *i2NC;
			
			if (NC2)
			{
				bool p1a1 = *i1a1;
				bool p1a2 = *i1a2;
				bool p2a1 = *i2a1;
				bool p2a2 = *i2a2;
				
				if (p1a1 == p2a1)
				{
					if (p1a2 == p2a2) tmp.ibs2++;
					else tmp.ibs1++;
				}
				else
				{
					if (p1a2 == p2a2) tmp.ibs1++;
					else tmp.ibs0++;
				}
			}
		}
		
		++i1a1;
		++i1a2;
		++i2a1;
		++i2a2;
		++i1NC;
		++i2NC;
	}
	
	return tmp;
}

void Duo::getMeanAndSDFromCounts( )
{
	printLog("Calculating Mean and Standard Deviation of IBS\n");
	
	if (numMean() != numCounts()) meanIbs.reserve( numCounts() );
	if (numSD() != numCounts()) sdIbs.reserve( numCounts() );
	
	vector<int>::iterator ibs0iter = ibs0Count.begin();
	vector<int>::iterator ibs1iter = ibs1Count.begin();
	vector<int>::iterator ibs2iter = ibs2Count.begin();
	
	while ( ibs0iter < ibs0Count.end() )
	{
		int ibs0 = *ibs0iter;
		int ibs1 = *ibs1iter;
		int ibs2 = *ibs2iter;
		
		double mean = (double) ((ibs2 * 2.0) + ibs1) / (ibs0 + ibs1 + ibs2);
		
		meanIbs.push_back( mean );
		sdIbs.push_back( calculateSD( mean, ibs0, ibs1, ibs2 ) );
		
		++ibs0iter;
		++ibs1iter;
		++ibs2iter;
	}
}

double calculateSD (const double &mean, const int &ibs0, const int &ibs1, const int &ibs2)
{
	double tmp0 = ( 0.0 - mean ) * ( 0.0 - mean ) * ibs0;
	double tmp1 = ( 1.0 - mean ) * ( 1.0 - mean ) * ibs1;
	double tmp2 = ( 2.0 - mean ) * ( 2.0 - mean ) * ibs2;
	double sum  = (double) ( ibs0 + ibs1 + ibs2 );
	
	return sqrt ((tmp2 + tmp1 + tmp0) / sum);
}

void Duo::specifiedRelationships( const Ped &ped )
{
	printLog("Getting specified relationships\n");
	
	if (size() == 0) fillIdentities( ped );
	
	specifiedRelationship.reserve( size() );
	
	for (unsigned int i = 0; i < size(); ++i)
	{
		// Different family IDs = unrelated
		if (ind1Index[i]->fid != ind2Index[i]->fid) specifiedRelationship.push_back( UNREL );
		
		// Same family ID, but no parents = unrelated
		else if (ind1Index[i]->pid == "0" and ind1Index[i]->mid == "0" and ind2Index[i]->pid == "0" and ind2Index[i]->mid == "0")
		{ specifiedRelationship.push_back( UNREL ); }
		
		else
		{
			bool sharemother = false;
			bool sharefather = false;
			bool checkshare = false;
		
			// Ind 1 has a father
			if (ind1Index[i]->pid != "0")
			{	
				// Ind1 father ID == Ind2 individual ID = parent child		
				if (ind1Index[i]->pid == ind2Index[i]->iid)
				{
					specifiedRelationship.push_back( PARCHLD );
					continue;
				}
				
				// Ind2 has a father ID
				else if (ind2Index[i]->pid != "0")
				{
					// ParentID Ind2 == IndID of Ind1 = parent child
					if (ind2Index[i]->pid == ind1Index[i]->iid)
					{
						specifiedRelationship.push_back( PARCHLD );
						continue;
					}
					
					// Have same father
					else if (ind1Index[i]->pid == ind2Index[i]->pid)
					{
						sharefather = true;
						checkshare = true;
					}
					
					// Have different father
					else
					{
						sharefather = false;
						checkshare = true;
					}
				}
				
				// Ind2 doesn't have father ID
				else
				{
					sharefather = false;
					checkshare = true;
				}
			}
			
			// Ind 1 doesn't have father listed
			else if (ind1Index[i]->pid == "0")
			{
				// Ind2 does have a father
				if (ind2Index[i]->pid != "0")
				{
					// Ind2 paternal == Ind1 ID = parent child
					if (ind2Index[i]->pid == ind1Index[i]->iid)
					{
						specifiedRelationship.push_back( PARCHLD );
						continue;
					}
					
					// Don't share father
					else
					{
						sharefather = false;
						checkshare = true;
					}
				}
				
				// If Ind2 has no father listed can't share father
				else
				{
					sharefather = false;
					checkshare = true;
				}
			}
			
			// Ind1 has mother ID
			if (ind1Index[i]->mid != "0")
			{
				// Ind1 MID == Ind2 IndID = parent child
				if (ind1Index[i]->mid == ind2Index[i]->iid)
				{
					specifiedRelationship.push_back( PARCHLD );
					continue;
				}
				
				// Ind2 has mother ID
				else if (ind2Index[i]->mid != "0")
				{
					// Ind2 mother ID == Ind2 IndID = parent child
					if (ind2Index[i]->mid == ind1Index[i]->iid)
					{
						specifiedRelationship.push_back( PARCHLD );
						continue;
					}
					
					// Ind1 mother == Ind2 mother 
					else if (ind1Index[i]->mid == ind2Index[i]->mid)
					{
						sharemother = true;
						checkshare = true;
					}
					
					// Ind1 different mother than Ind2
					else
					{
						sharemother = false;
						checkshare = true;
					}
				}
				
				// Ind1 has mother ID but Ind2 doesn't
				else
				{
					sharemother = false;
					checkshare = true;
				}
			}
			
			// Ind1 doesn't have mother ID
			else if (ind1Index[i]->mid == "0")
			{
				// Ind2 has mother ID
				if (ind2Index[i]->mid != "0")
				{
					// Ind2 mother ID == Ind1 IndID
					if (ind2Index[i]->mid == ind1Index[i]->iid)
					{
						specifiedRelationship.push_back( PARCHLD );
						continue;
					}
					
					// Ind2 mother ID isn't Ind1 IID
					else
					{
						sharemother = false;
						checkshare = true;
					}
				}
				
				else
				{
					sharemother = false;
					checkshare = true;
				}
			}
			
			if (checkshare)
			{
				if (sharefather and sharemother) specifiedRelationship.push_back( SIBS );
								
				else if (sharefather or sharemother) specifiedRelationship.push_back( OTHERREL );
				
				else specifiedRelationship.push_back( UNREL );
			}
		}
	}
}

void Duo::calculatedRelationships( )
{
	printLog("Getting calculated relationships\n");
	
	if (numCalculated() != size()) calculatedRelationship.reserve( size() );
	
	vector<double>::iterator meanIter = meanIbs.begin();
	vector<double>::iterator sdIter = sdIbs.begin();
	
	while (meanIter < meanIbs.end())
	{
		double mean = *meanIter;
		double sd = *sdIter;
		
		if (mean > IDENT_MEANCUTOFF and sd < IDENT_SDCUTOFF) calculatedRelationship.push_back( IDENT );
		
		else if (sd > isFirstDegreeClassifier( mean, sd ))
		{
			if (sd < whichFirstDegreeClassifier( mean, sd )) calculatedRelationship.push_back( PARCHLD ); 
			
			else calculatedRelationship.push_back( SIBS );
		}
		
		// Methods for other relationships can go here
		
		else calculatedRelationship.push_back( UNREL );
		
		++meanIter;
		++sdIter;
	}
}
