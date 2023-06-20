# list of test drivers (with main()) for development
TESTSOURCES = $(wildcard test*.cpp)
# names of test executables
TESTS       = $(TESTSOURCES:%.cpp=%)

# list of sources used in project
SOURCES 	= $(wildcard *.cpp)
SOURCES     := $(filter-out $(TESTSOURCES), $(SOURCES))
# list of objects used in project
OBJECTS		= $(SOURCES:%.cpp=%.o)

SO_PATH = $(LD_LIBRARY_PATH)

LIB = Channel.so

#Default Flags
CXXFLAGS = -std=c++14 -Wconversion -Wall -Werror -Wextra -pedantic -pthread

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG so that "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

# highest target; sews together all objects into executable
all: $(LIB)

final: clean $(LIB)
	ln -f $(LIB) $(SO_PATH)

$(LIB): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(LIB) -shared

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(TESTS) $(PARTIAL_SUBMITFILE) $(FULL_SUBMITFILE)
