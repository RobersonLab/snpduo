# Replace g++ with your compiler, or a direct link if necessary
CXX = g++

# Any custom compiler flags here , such as -Wall, march, mtune, -m32 or -m64, etc.
CXXFLAGS = 

# --------------------------------------------------------------------
# Do not edit below this line
# --------------------------------------------------------------------

CXXFLAGS += -O3 -I.
OUTPUT = snpduo

SRC = duo.cpp helper.cpp input.cpp options.cpp output.cpp parse.cpp snpduo.cpp

HDR = duo.h helper.h input.h options.h output.h snpduo.h

OBJ = $(SRC:.cpp=.o)

all : $(OUTPUT) 

$(OUTPUT) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJ)

$(OBJ) : $(HDR)

.cpp.o : 
	$(CXX) $(CXXFLAGS) -c $*.cpp
.SUFFIXES : .cpp .c .o $(SUFFIXES)

FORCE:

clean:
	rm -f *.o *~
