
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CXXFLAGS =  -std=c++17 -O2 -march=native -Wall -Wextra -Wshadow 

EXECUTABLES=sheng



all: $(EXECUTABLES)

sheng: main.cpp sheng.h common_defs.h
	$(CXX) $(CXXFLAGS) -o sheng main.cpp


clean:
	rm -f $(EXECUTABLES)

cleandist:
	rm -f $(EXECUTABLES)
