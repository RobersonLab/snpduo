#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>

#include "duo.h"
#include "snpduo.h"

// Macros for chromosome numbers
const int XINT = 23;
const int YINT = 24;
const int XYINT = 25;
const int MTINT = 26;

// Conversion functions
int intGenoFromChar( const char & );
int intChrFromString( const string & );

// File reading functions
void readPedFile( Ped & );
void readMapFile( Map & );
void readTpedFile( Ped &, Map & );

// void readBpedFile( Ped &, Map & );
// void readBfamFile( Ped & );
// void readBmapFile( Ped &, Map & );

void readPLINKGenome( Duo &, Ped & );

#endif
