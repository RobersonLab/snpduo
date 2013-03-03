#ifndef _PED_H_
#define _PED_H_

// stl
#include <map>
#include <string>
#include <vector>

// imports
using std::iterator;
using std::map;
using std::pair;
using std::string;
using std::vector;

// custom
#include "helper.h"
#include "locus.h"
#include "options.h"

// classes
class Person
{
	private:
		int sex;
		int pheno; // PHENOTYPE SHOULD BE CHANGED TO ACCOMODATE DOUBLES!!!
				   // is that really important since we don't use them? Perhaps for recoding purposes
		float hetRate; // ATTN we do not set a het rate. should this be implemented or dropped???
		string fid;
		string iid;
		string pid;
		string mid;
		
		// SNP data
		vector<bool> a1;
		vector<bool> a2;
		vector<bool> hasGenotype;
	
	public:
		// assigners
		void setSex( int i ) { sex = i; }
		void setPheno( int i ) { pheno = i; }
		void setFid( string const& s ) { fid = s; }
		void setIid( string const& s ) { iid = s; }
		void setPid( string const& s ) { pid = s; }
		void setMid( string const& s ) { mid = s; }
		void setHetRate( float f ) { hetRate = f; }
		void addAllele1( bool b ) { a1.push_back( b ); }
		void addAllele2( bool b ) { a2.push_back( b ); }
		void addHasGenotype( bool b ) { hasGenotype.push_back( b ); }
		
		// accessors
		int getSex() { return sex; }
		int getPheno() { return pheno; }
		string getFid() { return fid; }
		string getIid() { return iid; }
		string getPid() { return pid; }
		string getMid() { return mid; }
		float getHetRate() { return hetRate; }
		vector<bool> getA1() { return a1; }
		vector<bool> getA2() { return a2; }
		vector<bool> getHasGenotype() { return hasGenotype; }
		// some error checking may need to be done here to avoid out of bounds errors
		bool getA1( unsigned int i )
		{
			return a1[i];
		}
		bool getA2( unsigned int i )
		{
			return a2[i];
		}
		bool getHasGenotype( unsigned int i )
		{
			return hasGenotype[i];
		}
		
		// iterators
		vector<bool>::iterator beginIterA1() { return a1.begin(); }
		vector<bool>::iterator beginIterA2() { return a2.begin(); }
		vector<bool>::iterator beginIterHasGenotype() { return hasGenotype.begin(); }
		vector<bool>::iterator endIterA1() { return a1.end(); }
		vector<bool>::iterator endIterA2() { return a2.end(); }
		vector<bool>::iterator endIterHasGenotype() { return hasGenotype.end(); }
		
		// why are we flipping here????
		void flipAllele1( int i )
		{
			// error handling should be added to deal with out-of-bounds writing to prevent crashes and improve reported
			a1[i] = !(a1[i]);
		}
		
		void flipAllele2( int i )
		{
			a2[i] = !(a2[i]);
		}
		
		void flipHasGenotype( int i )
		{
			hasGenotype[i] = !(hasGenotype[i]);
		}
		
		void setAllele1True( int i )
		{
			a1[i] = true;
		}
		
		void setAllele2True( int i )
		{
			a2[i] = true;
		}
		
		void setHasGenotypeTrue( int i )
		{
			hasGenotype[i] = true;
		}
		
		// Constructor
		Person(): sex( 0 ), pheno( 0 ), fid(" "), iid( " " ), pid( " " ), mid( " " )
		{
			if (par::snpCount != 0)
			{
				a1.reserve( par::snpCount );
				a2.reserve( par::snpCount );
				hasGenotype.reserve( par::snpCount );
			}
		}
		
		Person( string const& myFid ): sex(0), pheno(0), fid(myFid), iid(" "), pid(" "), mid(" ")
		{
			if (par::snpCount != 0)
			{
				a1.reserve( par::snpCount );
				a2.reserve( par::snpCount );
				hasGenotype.reserve( par::snpCount );
			}
		}
		
		Person( int mySex, int myPheno, string const& myFid, string const& myIid, string const& myPid, string const& myMid, vector<bool> myA1, vector<bool> myA2, vector<bool> myHasGenotype ): sex(mySex), pheno(myPheno), fid(myFid), iid(myIid), pid(myPid), mid(myMid), a1(myA1), a2(myA2), hasGenotype(myHasGenotype) {}
};

class Ped
{
	private:
		vector<Person*> samples;
		map<string, unsigned long int> index;
		unsigned long int personCount;
	
	public:
		// does person exist in index?
		bool boolHavePerson( string const&, string const&, int& );
		
		// operators
		Person* operator[] (unsigned int i)
		{
			if (i >= samples.size()) { error("Attempting to access a ped bin out of range!"); }
			return samples[i];
		}
		
		// assigners
		void addPerson( Person* );
		
		// iterators
		vector<Person*>::iterator begin() { return samples.begin(); }
		vector<Person*>::iterator end() { return samples.end(); }
		vector<Person*>::const_iterator begin() const { return samples.begin(); }
		vector<Person*>::const_iterator end() const { return samples.end(); }
		
		// functions
		unsigned long int getPersonCount() { return personCount; }
		
		// Search functions
		unsigned long int findPerson( string const& , string const&  );
		
		// Writing functions
		void write(LocusMap&);
		
		// reading function
		void read(LocusMap&);
		
		// Miscellaneous	
		string fileGenotypeString( bool, bool, bool, Locus& );
		
		// Constructor reserves vector sizes if known
		Ped(): personCount(0) {}
		
		// destructor frees the used memory
		~Ped()
		{
			for (unsigned int i = 0; i < getPersonCount(); ++i)
			{
				delete samples[i];
				samples[i] = 0;
			}
		}
		
		// assignment
		Ped& operator=( Ped const& rhs )
		{
			if (this != &rhs)
			{
				// delete the memory currently held in this vector
				// reset to null
				vector<Person*>::iterator currIter = begin();
				vector<Person*>::iterator endIter = end();
				
				while (currIter != endIter)
				{
					delete (*currIter);
					*currIter = 0;
					
					++currIter;
				}
				
				// build a new vector using a deep copy
				vector<Person*>::const_iterator rhsCurrIter = rhs.begin();
				vector<Person*>::const_iterator rhsEndIter = rhs.end();
				
				while (rhsCurrIter != rhsEndIter)
				{
					Person* newPerson = new Person((*rhsCurrIter)->getSex(), 
												   (*rhsCurrIter)->getPheno(),
												   (*rhsCurrIter)->getFid(),
												   (*rhsCurrIter)->getIid(),
												   (*rhsCurrIter)->getPid(),
												   (*rhsCurrIter)->getMid(),
												   (*rhsCurrIter)->getA1(),
												   (*rhsCurrIter)->getA2(),
												   (*rhsCurrIter)->getHasGenotype() );
					addPerson( newPerson );
					
					++rhsCurrIter;
				}
			}
				
			return *this;
		}
		
		// copy constructor
		Ped(Ped const& rhs)
		{
			vector<Person*>::const_iterator currIter = rhs.begin();
			vector<Person*>::const_iterator endIter = rhs.end();
			
			while (currIter != endIter)
			{
				Person* newPerson = new Person((*currIter)->getSex(), 
				                               (*currIter)->getPheno(),
											   (*currIter)->getFid(),
											   (*currIter)->getIid(),
											   (*currIter)->getPid(),
											   (*currIter)->getMid(),
											   (*currIter)->getA1(),
											   (*currIter)->getA2(),
											   (*currIter)->getHasGenotype() );
				addPerson( newPerson );
				
				++currIter;
			}
		}
};

#endif
