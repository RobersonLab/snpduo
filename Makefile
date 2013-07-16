CXX = g++

# Any custom compiler flags here ( -g, -march, -mtune, -m64, etc)
# For 64-bit compilation add -m64 to CXXFLAGS
CXXFLAGS = 
# --------------------------------------------------------------------
# Do not edit below this line
# --------------------------------------------------------------------

CXXFLAGS += -O3 -I. -Wall
OUTPUT = snpduo

SRC = cargs.cpp duo.cpp helper.cpp input.cpp locus.cpp options.cpp output.cpp ped.cpp snpduo.cpp

HDR = cargs.h duo.h helper.h input.h options.h output.h ped.h

OBJ = $(SRC:.cpp=.o)

all : $(OUTPUT) 

$(OUTPUT) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJ)

$(OBJ) : $(HDR)

.cpp.o : 
	$(CXX) $(CXXFLAGS) -c $*.cpp
.SUFFIXES : .cpp .c .o $(SUFFIXES)

.PHONEY : clean install uninstall

clean:
	rm -f *.o $(OUTPUT)

install:
	install -m 0755 ./$(OUTPUT) /usr/bin

uninstall:
	rm /usr/bin/$(OUTPUT)
