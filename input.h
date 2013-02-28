#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>
#include "Duo.h"

int intGenoFromChar(const char &);
int intChrFromString(const string &);

void readPedFile(Ped &);
void readMapFile(Map &);
void readTpedFile(Ped &, Map &);
// void readBpedFile( Ped &, Map & );
// void readBfamFile( Ped & );
// void readBmapFile( Ped &, Map & );
void readPLINKGenome( Duo & );

#endif
