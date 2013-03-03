#ifndef _OUTPUT_H_
#define _OUTPUT_H_

// stl
#include <string>

// imports
using std::string;

// custom
#include "ped.h"
#include "locus.h"

// functions
void writeLog( string const&  );
void writeTranspose( Ped&, LocusMap& );
void writeForWeb( Ped&, LocusMap& );

string fileGenotypeString( Ped const& , int, int );

#endif
