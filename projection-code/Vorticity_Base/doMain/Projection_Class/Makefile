#Projection makefile

#Project name
PROJ_NAME=doProjection.exe

#Compiler and linker
CXX=g++


#Flags
#CXXFLAGS  = -O3 -Wall -fPIC -std=c++17 # Uncomment if running
CXXFLAGS  = -g -Wall -fPIC -std=c++17 # Uncomment if debugging
CXXFLAGS +=`root-config --cflags --glibs`

all: objFolder $(PROJ_NAME)

$(PROJ_NAME): ./objects/Proj_main.o ./objects/parameters.o ./objects/projection.o
#	$(CXX) -o $@ $^
	$(CXX) $(CXXFLAGS) -o $@ $^

./objects/%.o: ./src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

#./objects/parameters.o: ./src/parameters.cxx 
#	$(CXX) $(CXXFLAGS) -c $< -o $@

#./objects/projection.o: ./src/projection.cxx
#	$(CXX) $(CXXFLAGS) -c $< -o $@

#./objects/Proj_main.o: ./src/Proj_main.cxx
#	$(CXX) $(CXXFLAGS) -c $< -o $@ 

objFolder:
	@ mkdir -p objects

clean:
	@rm -rf ./objects/*.o *~ $(PROJ_NAME)
	@rmdir objects
