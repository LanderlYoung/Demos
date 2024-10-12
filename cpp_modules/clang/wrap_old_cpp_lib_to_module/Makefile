CXX=/usr/local/opt/llvm/bin/clang++
CXXFLAGS=-std=c++20

all: use_module.out

%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -c -o $@

%.pcm: %.cppm
	$(CXX) $(CXXFLAGS) $< --precompile -o $@

old_lib.pcm: old_lib.cppm

old_lib.o: old_lib.cc

use_module.out: old_lib.pcm old_lib.o
	$(CXX) $(CXXFLAGS) use_module.cc old_lib.o -fmodule-file=old_lib.pcm -o $@

clean:
	-rm *.pcm *.out *.o
