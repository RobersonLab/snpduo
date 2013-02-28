// Standard Libraries
#include <iostream>
#include <cmath>
#include <iomanip>

// Namespace
using namespace std;

// Custom Libraries
#include "Duo.h"
#include "options.h"

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

// Functions
double isFirstDegreeClassifier ( double &mean, double &sd )
{
	return ((ISFIRSTSLOPE * mean) + ISFIRSTINTERCEPT);
}

double whichFirstDegreeClassifier ( double &mean, double &sd )
{
	return (WHICHFIRSTSLOPESQUARE * (mean * mean)) + (WHICHFIRSTSLOPE * mean) + WHICHFIRSTINTERCEPT;
}

void Duo::fillIdentities( Ped &ped )
{
	int comparisons, counter;
	counter = 0;
	comparisons = (par::personcount * (par::personcount - 1)) / 2;
	
	fid1.resize( comparisons );
	iid1.resize( comparisons );
	fid2.resize( comparisons );
	iid2.resize( comparisons );
	
	for (int i = 0; i < par::personcount - 1; ++i)
	{
		for (int j = i + 1; j < par::personcount; ++j)
		{
			fid1[ counter ] = ped.samples[i]->fid;
			iid1[ counter ] = ped.samples[i]->iid;
			fid2[ counter ] = ped.samples[j]->fid;
			iid2[ counter ] = ped.samples[j]->iid;
			
			++counter;
		}
	}
}

void Duo::getCounts(Ped &ped)
{
	int comparisons;
	comparisons = (par::personcount * (par::personcount - 1)) / 2;
	
	fid1.resize( comparisons );
	iid1.resize( comparisons );
	fid2.resize( comparisons );
	iid2.resize( comparisons );
	ibs0Count.resize( comparisons );
	ibs1Count.resize( comparisons );
	ibs2Count.resize( comparisons );
	
	int counter,ibs0, ibs1, ibs2;
	ibs0 = ibs1 = ibs2 = counter = 0;
	
	// Go comparison by comparison to calculate counts
	for (int i = 0; i < par::personcount - 1; ++i)
	{
		
		for (int j = i + 1; j < par::personcount; ++j)
		{
			
			fid1[ counter ] = ped.samples[i]->fid;
			iid1[ counter ] = ped.samples[i]->iid;
			fid2[ counter ] = ped.samples[j]->fid;
			iid2[ counter ] = ped.samples[j]->iid;
			
			ibs0 = ibs1 = ibs2 = 0;
			
			for (int k = 0; k < par::snpcount; ++k)
			{
				
				if (ped.samples[i]->hasGenotype[k] and ped.samples[j]->hasGenotype[k])
				{
					if (ped.samples[i]->a1[k] == ped.samples[j]->a1[k])
					{
						if (ped.samples[i]->a2[k] == ped.samples[j]->a2[k])
						{
							++ibs2;
						}
						else
						{
							++ibs1;
						}
					}
					else
					{
						if (ped.samples[i]->a2[k] == ped.samples[j]->a2[k])
						{
							++ibs1;
						}
						else
						{
							++ibs0;
						}
					}
				}
			}
			
			ibs0Count[ counter ] = ibs0;
			ibs1Count[ counter ] = ibs1;
			ibs2Count[ counter ] = ibs2;
			
			++counter;			
		}
	}
}

void Duo::getMeanAndSDFromCounts( )
{
	if (meanIbs.size() != ibs0Count.size()) meanIbs.resize( ibs0Count.size(), 0.0 );
	if (sdIbs.size() != ibs0Count.size()) sdIbs.resize( ibs0Count.size(), 0.0 );
	
	for (unsigned int i = 0; i < ibs0Count.size(); ++i)
	{
		meanIbs[i] = (double) ((ibs2Count[i]* 2) + ibs1Count[i]) / (ibs0Count[i] + ibs1Count[i] + ibs2Count[i]);
		sdIbs[i] = calculateSD( meanIbs[i], ibs0Count[i], ibs1Count[i], ibs2Count[i] );
	}
}

double calculateSD (double &mean, int &ibs0, int &ibs1, int &ibs2)
{
	double tmp0, tmp1, tmp2, sum;
	tmp0 = tmp1 = tmp2 = sum = 0.0;
	
	tmp2 = 2.0 - mean;
	tmp2 *= tmp2;
	tmp2 *= (double) ibs2;
	
	tmp1 = 1.0 - mean;
	tmp1 *= tmp1;
	tmp1 *= (double) ibs1;
	
	tmp0 = 0.0 - mean;
	tmp0 *= tmp0;
	tmp0 *= (double) ibs0;
	sum = (double) ( ibs0 + ibs1 + ibs2 );
	
	return sqrt ((tmp2 + tmp1 + tmp0) / sum);
}

void Duo::specifiedRelationships( Ped &ped )
{
	if (fid1.size() == 0) this->fillIdentities( ped );
	
	specifiedRelationship.resize( fid1.size(), 0 );
	
	bool sharemother, sharefather, checkshare, done;
	sharemother = sharefather = checkshare = false;
	
	int i1, i2;
	i1 = i2 = 0;
	
	for (unsigned int i = 0; i < fid1.size(); ++i)
	{
		i1 = ped.findPerson( fid1[i], iid1[i] );
		i2 = ped.findPerson( fid2[i], iid2[i] );
		
		sharemother = sharefather = checkshare = false;
		
		// Different family IDs = unrelated
		if (ped.samples[i1]->fid != ped.samples[i2]->fid) specifiedRelationship[i] = UNREL;
		
		// Same family ID, but no parents = unrelated
		else if (ped.samples[i1]->pid == "0" and ped.samples[i1]->mid == "0" and ped.samples[i2]->pid == "0" and ped.samples[i2]->mid == "0") specifiedRelationship[i] = UNREL;
		
		else
		{
			// Ind 1 has a father
			if (ped.samples[i1]->pid != "0")
			{	
				// Ind1 father ID == Ind2 individual ID = parent child		
				if (ped.samples[i1]->pid == ped.samples[i2]->iid) specifiedRelationship[i] = PARCHLD;
				
				// Ind2 has a father ID
				else if (ped.samples[i2]->pid != "0")
				{
					// ParentID Ind2 == IndID of Ind1 = parent child
					if (ped.samples[i2]->pid == ped.samples[i1]->iid) specifiedRelationship[i] = PARCHLD;
					
					// Have same father
					else if (ped.samples[i1]->pid == ped.samples[i2]->pid) { sharefather = true; checkshare = true; }
					
					// Have different father
					else { sharefather = false; checkshare = true; }
				}
				
				// Ind2 doesn't have father ID
				else { sharefather = false; checkshare = true; }
			}
			
			// Ind 1 doesn't have father listed
			else if (ped.samples[i1]->pid == "0")
			{
				// Ind2 does have a father
				if (ped.samples[i2]->pid != "0")
				{
					// Ind2 paternal == Ind1 ID = parent child
					if (ped.samples[i2]->pid == ped.samples[i1]->iid) specifiedRelationship[i] = PARCHLD;
					
					// Don't share father
					else { sharefather = false; checkshare = true; }
				}
				
				// If Ind2 has no father listed can't share father
				else { sharefather = false; checkshare = true; }
			}
			
			// Ind1 has mother ID
			if (ped.samples[i1]->mid != "0")
			{
				// Ind1 MID == Ind2 IndID = parent child
				if (ped.samples[i1]->mid == ped.samples[i2]->iid) specifiedRelationship[i] = PARCHLD;
				
				// Ind2 has mother ID
				else if (ped.samples[i2]->mid != "0")
				{
					// Ind2 mother ID == Ind2 IndID = parent child
					if (ped.samples[i2]->mid == ped.samples[i1]->iid) specifiedRelationship[i] = PARCHLD;
					
					// Ind1 mother == Ind2 mother 
					else if (ped.samples[i1]->mid == ped.samples[i2]->mid) { sharemother = true; checkshare = true; }
					
					// Ind1 different mother than Ind2
					else { sharemother = false; checkshare = true; }
				}
				
				// Ind1 has mother ID but Ind2 doesn't
				else { sharemother = false; checkshare = true; }
			}
			
			// Ind1 doesn't have mother ID
			else if (ped.samples[i1]->mid == "0")
			{
				// Ind2 has mother ID
				if (ped.samples[i2]->mid != "0")
				{
					// Ind2 mother ID == Ind1 IndID
					if (ped.samples[i2]->mid == ped.samples[i1]->iid) specifiedRelationship[i] = PARCHLD;
					
					// Ind2 mother ID isn't Ind1 IID
					else { sharemother = false; checkshare = true; }
				}
				
				else { sharemother = false; checkshare = true; }
			}
			
			if (checkshare)
			{
				if (sharefather and sharemother) specifiedRelationship[i] = SIBS;
				
				else if (sharefather or sharemother) specifiedRelationship[i] = OTHERREL;
				
				else UNREL;
			}
		}
	}
}

void Duo::calculatedRelationships( )
{
	calculatedRelationship.resize( fid1.size(), 0 );
	
	for (unsigned int i = 0; i < meanIbs.size(); ++i)
	{
		
		// Check identical
		if (meanIbs[i] > IDENT_MEANCUTOFF and sdIbs[i] < IDENT_SDCUTOFF)
		{
			calculatedRelationship[i] = IDENT;
		}
		
		// Check first degree
		else if (sdIbs[i] > isFirstDegreeClassifier( meanIbs[i], sdIbs[i] ))
		{
			if (sdIbs[i] < whichFirstDegreeClassifier( meanIbs[i], sdIbs[i] ))
			{
				calculatedRelationship[i] = PARCHLD;
			}
			
			else calculatedRelationship[i] = SIBS;
		}
		
		// Later add other unrelated
		
		// Unrelated
		else calculatedRelationship[i] = UNREL;
	}
}
