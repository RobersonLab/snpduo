#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <string>

#include "snpduo.h"

#include "Duo.h"

void printLog( string );
// void writeBinary( Ped &, Map & );
void writeTranspose( Ped &, Map & );
void writeForWeb( Ped &, Map & );

string fileGenotypeString( Ped &, int, int );

#endif
