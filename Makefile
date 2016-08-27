CXX=g++
CFLAGS=--std=c++11 -pthread
INCLUDE=-Iinclude
LIBS= 	-lopenal \
		-logg -lvorbis -lvorbisfile

DIR=bin objs

.SUFFIXES:
.SUFFIXES: .o .cpp

SRC=src/jukebox.cpp src/main.cpp src/source.cpp $(wildcard src/sources/source.*.cpp)
OBJ=$(addsuffix .o, $(addprefix objs/, $(notdir $(SRC))))

all: $(DIR) $(OBJ)
	$(CXX) $(CFLAGS) $(LIBS) $(INCLUDE) -o bin/main $(OBJ) -lm 

clean:
	@rm -rf objs/*

sure: clean all

# Helper rules
$(DIR): 
	@mkdir -p $(DIR)
	
objs/%.cpp.o: src/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

objs/source.%.cpp.o: src/sources/source.%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@