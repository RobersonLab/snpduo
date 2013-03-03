// stl
#include <map>
#include <string>

// imports
using std::map;
using std::iterator;
using std::string;

// custom headers
#include "helper.h"
#include "ped.h"

// functions
void Ped::addPerson( Person* p )
{
	string uid = makeUniqueId( p->getFid(), p->getIid() ); // build unique id
	++personCount; // increment person count. no need to wait until after, it dies if unsuccessful
	
	pair<map<string, unsigned long int>::iterator, bool> result = index.insert( pair<string,unsigned long int>(uid, Ped::personCount));
	// try to add to assoc array
	
	if (!result.second)
	{
		error("Cannot add person [" + p->getFid() + ", " + p->getIid() + "], they already exist in the frame!");
	}
	
	// actually push onto the array
	samples.push_back( p ); 
}

// ATTN need to update this to be an unsigned long int passed by reference. would remove the static cast 
bool Ped::boolHavePerson( string const& fid, string const& iid, int& i )
{
	map<string, unsigned long int>::iterator hashEnd = index.end();
	map<string, unsigned long int>::iterator itemIter = index.find( makeUniqueId(fid, iid) ); // ATTN replace with search?
	
	if (itemIter != hashEnd)
	{
		i = static_cast<int>(itemIter->second);
		return true;
	}
	else { return false; }
}

unsigned long int Ped::findPerson( string const& famID, string const& indID)
{
	map<string, unsigned long int>::iterator hashEnd = index.end();
	map<string, unsigned long int>::iterator itemIter = index.find( makeUniqueId(famID, indID) ); // ATTN replace with search
	
	if ( itemIter != hashEnd) { return itemIter->second; }
	else { error("Unable to find individual with family ID " + famID + " and individual ID " + indID); }
	
	return 0;
}

string Ped::fileGenotypeString( bool hasCall, bool a1, bool a2, Locus& snp )
{
	string s(" ");
	
	if (hasCall)
	{
		if (a1)
		{
			if (a2)
			{
				// homozygous allele 2
				s += snp.getAllele2();
				s += " ";
				s += snp.getAllele2();
			}
			else
			{
				// heterozygous
				if (snp.getAllele1() < snp.getAllele2())
				{
					s += snp.getAllele1();
					s += " ";
					s += snp.getAllele2();
				}
				else
				{
					s += snp.getAllele2();
					s += " ";
					s += snp.getAllele1();
				}
			}
		}
		else
		{
			if (a2)
			{
				// heterozygous
				if (snp.getAllele1() < snp.getAllele2())
				{
					s += snp.getAllele1();
					s += " ";
					s += snp.getAllele2();
				}
				else
				{
					s += snp.getAllele2();
					s += " ";
					s += snp.getAllele1();
				}
			}
			else
			{
				// homozygous allele 1
				s += snp.getAllele1();
				s += " ";
				s += snp.getAllele1();
			}
		}
		
		return s;
	}
	else { return " 0 0"; }
}
