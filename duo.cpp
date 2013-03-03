// Standard Libraries
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

// Namespace
using std::iterator;
using std::sqrt;
using std::string;

// Custom Libraries
#include "duo.h"
#include "options.h"
#include "output.h"
#include "ped.h" // for Ped::personCount

////////////////////////
// Standard functions //
////////////////////////
void DuoMap::fillIdentities( Ped& p )
{
	unsigned long int comparisons = (p.getPersonCount() * (p.getPersonCount() - 1)) / 2; // all possible pairwise combinations without replacement
	
	reserve(comparisons);
	
	for (unsigned long int i = 0; i < p.getPersonCount() - 1; ++i)
	{
		for (unsigned long int j = i + 1; j < p.getPersonCount(); ++j)
		{
			Duo tmpDuo( p[i], p[j] );
			push_back( tmpDuo );
		}
	}
}

void DuoMap::getCounts( Ped& ped )
{
	writeLog("Getting IBS counts\n");
	
	// Go comparison by comparison to calculate counts
	for (unsigned long int i = 0; i < ped.getPersonCount() - 1; ++i)
	{		
		for (unsigned long int j = i + 1; j < ped.getPersonCount(); ++j)
		{
			unsigned long int ibs0Count = 0;
			unsigned long int ibs1Count = 0;
			unsigned long int ibs2Count = 0;
			unsigned long int ibs2StarCount = 0;
			
			vector<bool>::iterator i1a1 = ped[i]->beginIterA1();
			vector<bool>::iterator i1a2 = ped[i]->beginIterA2();
			vector<bool>::iterator i2a1 = ped[j]->beginIterA1();
			vector<bool>::iterator i2a2 = ped[j]->beginIterA2();
			vector<bool>::iterator i1NC = ped[i]->beginIterHasGenotype();
			vector<bool>::iterator i2NC = ped[j]->beginIterHasGenotype();
			
			while (i1a1 != ped[i]->endIterA1())
			{
				if (*i1NC)
				{
					if (*i2NC)
					{
						if (*i1a1 == *i2a1)
						{
							if (*i1a2 == *i2a2)
							{
								if (*i1a1 != *i1a2)
								{
									++ibs2StarCount;
								}
								
								++ibs2Count;
							}
							else
							{
								++ibs1Count;
							}
						}
						else
						{
							if (*i1a2 == *i2a2)
							{
								++ibs1Count;
							}
							else
							{
								if (*i1a1 == *i1a2)
								{
									++ibs0Count;
								}
								else
								{
									++ibs2Count;
									++ibs2StarCount;
								}
							}
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
			
			Duo newDuo( ped[i], ped[j], ibs0Count, ibs1Count, ibs2Count, ibs2StarCount );
			push_back(newDuo);
		}
	}
}

// void DuoMap::getCounts( Ped& ped )
// {
	// writeLog("Getting IBS counts\n");
	
	// // Go comparison by comparison to calculate counts
	// for (unsigned long int i = 0; i < ped.getPersonCount() - 1; ++i)
	// {		
		// for (unsigned long int j = i + 1; j < ped.getPersonCount(); ++j)
		// {
			// IbsCount tmp = getIBS( ped[i], ped[j] );
			
			// Duo tmpDuo( ped[i], ped[j], tmp.getIbs0(), tmp.getIbs1(), tmp.getIbs2(), tmp.getIbs2Star() );
			
			// push_back( tmpDuo ); // add to the vector
		// }
	// }
// }

IbsCount getIBS( Person* i1, Person* i2 )
{
	///////////////////////////
	// CONSIDER CHANGING TO A FUNCTION WITHIN THE DUO CLASS
	// AND THE IBSCOUNT OBJECT COULD THEN BE PASSED DIRECTLY INTO A VECTOR, OR PASSED DIRECTLY INTO THE APPROPRIATE VECTOR FOR STORAGE
	///////////////////////////
	IbsCount tmp;
	
	vector<bool>::iterator i1a1 = i1->beginIterA1();
	vector<bool>::iterator i1a2 = i1->beginIterA2();
	vector<bool>::iterator i2a1 = i2->beginIterA1();
	vector<bool>::iterator i2a2 = i2->beginIterA2();
	vector<bool>::iterator i1NC = i1->beginIterHasGenotype();
	vector<bool>::iterator i2NC = i2->beginIterHasGenotype();
	
	while (i1a1 != i1->endIterA1())
	{
		if (*i1NC)
		{
			if (*i2NC)
			{
				if (*i1a1 == *i2a1)
				{
					if (*i1a2 == *i2a2)
					{
						if (*i1a1 != *i1a2) { tmp.addIbs2Star(); }
						
						tmp.addIbs2();
					}
					else tmp.addIbs1();
				}
				else
				{
					if (*i1a2 == *i2a2) tmp.addIbs1();
					else
					{
						if (*i1a1 == *i1a2) tmp.addIbs0();
						
						else
						{
							tmp.addIbs2();
							tmp.addIbs2Star();
						}
					}
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

void DuoMap::getMeanAndSDFromCounts( )
{
	writeLog("Calculating Mean and Standard Deviation of IBS\n");
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while ( currIter != endIter )
	{
		int ibs0 = (*currIter).getIbs0Count();
		int ibs1 = (*currIter).getIbs1Count();
		int ibs2 = (*currIter).getIbs2Count();
		int ibs2star = (*currIter).getIbs2StarCount();
		
		double mean = static_cast<double>((ibs2 * 2) + ibs1) / static_cast<double>(ibs0 + ibs1 + ibs2);
		
		// set all the values in the current duo object
		(*currIter).setMeanIbs( mean );
		(*currIter).setSdIbs( calculateSD( mean, ibs0, ibs1, ibs2 ) );
		(*currIter).setIbs2StarPercent( static_cast<double>(ibs2star) / static_cast<double>(ibs2 + ibs1 + ibs0) );
		(*currIter).setInformativePercent( static_cast<double>((ibs2star + ibs0)) / static_cast<double>(ibs2 + ibs0 + ibs1) );
		(*currIter).setIbs2StarPercentOfInformative( static_cast<double>(ibs2star) / static_cast<double>(ibs2star + ibs0) );
		
		++currIter;
	}
}

// ATTN POSSIBLE OPTIMIZATION
double calculateSD (double mean, int ibs0, int ibs1, int ibs2)
{
	double tmp0 = ( 0.0 - mean ) * ( 0.0 - mean ) * static_cast<double>(ibs0);
	double tmp1 = ( 1.0 - mean ) * ( 1.0 - mean ) * static_cast<double>(ibs1);
	double tmp2 = ( 2.0 - mean ) * ( 2.0 - mean ) * static_cast<double>(ibs2);
	
	return sqrt( (tmp2 + tmp1 + tmp0) / static_cast<double>(ibs0 + ibs1 + ibs2) );
}

void DuoMap::specifiedRelationships( Ped& ped )
{
	writeLog("Getting specified relationships\n");
	
	if (count() == 0) { fillIdentities( ped ); }
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while (currIter != endIter)
	{
		// Different family IDs = unrelated
		if ((*currIter).getInd1()->getFid() != (*currIter).getInd2()->getFid()) { (*currIter).setSpecifiedRelationship( UNREL ); }
		
		// Same family ID, but no parents = unrelated
		else if ((*currIter).getInd1()->getPid() == "0" and (*currIter).getInd1()->getMid() == "0" and (*currIter).getInd2()->getPid() == "0" and (*currIter).getInd2()->getMid() == "0")
		{ (*currIter).setSpecifiedRelationship( UNREL ); }
		
		else
		{
			bool sharemother = false;
			bool sharefather = false;
			bool checkshare = false;
		
			// ATTN POSSIBLE LOCAL VAR FOR OPTIMIZATION
			// Ind 1 has a father
			if ((*currIter).getInd1()->getPid() != "0")
			{	
				// Ind1 father ID == Ind2 individual ID = parent child		
				if ((*currIter).getInd1()->getPid() == (*currIter).getInd2()->getIid())
				{
					(*currIter).setSpecifiedRelationship( PARCHLD );
					continue;
				}
				
				// Ind2 has a father ID
				else if ((*currIter).getInd2()->getPid() != "0")
				{
					// ParentID Ind2 == IndID of Ind1 = parent child
					if ((*currIter).getInd2()->getPid() == (*currIter).getInd1()->getIid())
					{
						(*currIter).setSpecifiedRelationship( PARCHLD );
						continue;
					}
					
					// Have same father
					else if ((*currIter).getInd1()->getPid() == (*currIter).getInd2()->getPid())
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
			else
			{
				// Ind2 does have a father
				if ((*currIter).getInd2()->getPid() != "0")
				{
					// Ind2 paternal == Ind1 ID = parent child
					if ((*currIter).getInd2()->getPid() == (*currIter).getInd1()->getIid())
					{
						(*currIter).setSpecifiedRelationship( PARCHLD );
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
			if ((*currIter).getInd1()->getMid() != "0")
			{
				// Ind1 MID == Ind2 IndID = parent child
				if ((*currIter).getInd1()->getMid() == (*currIter).getInd2()->getIid())
				{
					(*currIter).setSpecifiedRelationship( PARCHLD );
					continue;
				}
				
				// Ind2 has mother ID
				else if ((*currIter).getInd2()->getMid() != "0")
				{
					// Ind2 mother ID == Ind2 IndID = parent child
					if ((*currIter).getInd2()->getMid() == (*currIter).getInd1()->getIid())
					{
						(*currIter).setSpecifiedRelationship( PARCHLD );
						continue;
					}
					
					// Ind1 mother == Ind2 mother 
					else if ((*currIter).getInd1()->getMid() == (*currIter).getInd2()->getMid())
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
			else if ((*currIter).getInd1()->getMid() == "0")
			{
				// Ind2 has mother ID
				if ((*currIter).getInd2()->getMid() != "0")
				{
					// Ind2 mother ID == Ind1 IndID
					if ((*currIter).getInd2()->getMid() == (*currIter).getInd1()->getIid())
					{
						(*currIter).setSpecifiedRelationship( PARCHLD );
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
				if (sharefather and sharemother) (*currIter).setSpecifiedRelationship( SIBS );
								
				else if (sharefather or sharemother) (*currIter).setSpecifiedRelationship( OTHERREL );
				
				else (*currIter).setSpecifiedRelationship( UNREL );
			}
		}
		
		++currIter;
	}
}

void DuoMap::oldCalculatedRelationships( )
{
	writeLog("Getting calculated relationships (old method)\n");
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end(); 
	
	while (currIter != endIter)
	{
		if ((*currIter).getMeanIbs() > IDENT_MEANCUTOFF and (*currIter).getSdIbs() < IDENT_SDCUTOFF)
		{
			(*currIter).setCalculatedRelationship( IDENT );
		}
		
		else if ((*currIter).getSdIbs() > isFirstDegreeClassifier( (*currIter).getMeanIbs(), (*currIter).getSdIbs() ))
		{
			if ((*currIter).getSdIbs() < whichFirstDegreeClassifier( (*currIter).getMeanIbs(), (*currIter).getSdIbs() ))
			{
				(*currIter).setCalculatedRelationship( PARCHLD );
			}
			
			else
			{
				(*currIter).setCalculatedRelationship( SIBS );
			}
		}
		
		// Methods for other relationships can go here
		
		else
		{
			(*currIter).setCalculatedRelationship( UNREL );
		}
		
		++currIter;
	}
}

void DuoMap::calculatedRelationships( )
{
	writeLog("Getting calculated relationships\n");
	
	vector<Duo>::iterator currIter = begin();
	vector<Duo>::iterator endIter = end();
	
	while (currIter != endIter)
	{
		if ((*currIter).getMeanIbs() > NEWIDENT_MEANCUTOFF and (*currIter).getSdIbs() < NEWIDENT_SDCUTOFF)
		{
			(*currIter).setCalculatedRelationship( IDENT ); // identical samples
		}
		else if ((*currIter).getInformativePercent()  >= NEWPARCHLD_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( PARCHLD ); // parchild
		}
		else if ((*currIter).getInformativePercent() >= NEWSIB_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( SIBS ); // sibling
		}
		else if ((*currIter).getInformativePercent() >= NEWSECOND_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( SECOND_DEGREE ); // second
		}
		else if ((*currIter).getInformativePercent() >= NEWSECONDORTHIRD_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( SECOND_OR_THIRD_DEGREE ); // second or third
		}
		else if ((*currIter).getInformativePercent() >= NEWTHIRD_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( THIRD_DEGREE ); // third
		}
		else if ((*currIter).getInformativePercent() >= NEWCRYPTIC_CUTOFF)
		{
			(*currIter).setCalculatedRelationship( CRYPTIC ); // cryptic
		}
		else
		{
			(*currIter).setCalculatedRelationship( UNREL );
		}
		
		++currIter;
	}
}
