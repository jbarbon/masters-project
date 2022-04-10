#Vorticity surface makefile

#Project name
PROJ_NAME=doVortSurf.exe

#Compiler and linker
CXX=g++

#Flags
CXXFLAGS  = -O3 -Wall -fPIC -std=c++17 # Uncomment if running
#CXXFLAGS  = -g -Wall -fPIC -std=c++17 # Uncomment if debugging
CXXFLAGS +=`root-config --cflags --glibs`

all: $(PROJ_NAME)

$(PROJ_NAME): VortSurf.o
	$(CXX) $(CXXFLAGS) -o $@ $^

VortSurf.o: VortSurf.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o *~ $(PROJ_NAME)

