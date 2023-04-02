TARGET_EXEC := findcomp

CXX := g++
CXXFLAGS := -std=c++20

SRC := $(shell find . -name "*.cpp")
OBJ := $(shell find . -name "*.o")

MAIN_SRC := driver.cpp PGMImageProcessor.cpp PGMMetadata.cpp ConnectedComponent.cpp
MAIN_OBJ := $(patsubst %.cpp, %.o, $(MAIN_SRC))

TST_SRC := test_processor.cpp PGMImageProcessor.cpp PGMMetadata.cpp ConnectedComponent.cpp
TST_OBJ := $(patsubst %.cpp, %.o, $(TST_SRC))

all: $(TARGET_EXEC) tests

$(TARGET_EXEC): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET_EXEC) $(MAIN_OBJ) $(LDLIBS)

tests: $(TST_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test $(TST_OBJ) $(LDLIBS)

depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(OBJ)
	rm -f *~ .depend

include .depend