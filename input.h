#ifndef _INPUT_H_
#define _INPUT_H_

// stl
#include <string>

// imports
using std::string;

// custom
#include "duo.h"
#include "locus.h"
#include "ped.h"

// Conversion functions
int intGenoFromChar( char  );
int intChrFromString( string const&  );

// File reading functions
void readTpedFile( Ped&, LocusMap& );
void readPlinkGenome( DuoMap&, Ped& );

#endif
