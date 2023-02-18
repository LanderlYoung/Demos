CXX=/usr/local/opt/llvm/bin/clang++
CXXFLAGS=-std=c++20

all: Hello.out

Hello.pcm: Hello.cppm
	$(CXX) $(CXXFLAGS) $< --precompile -o $@

Hello.out: Hello.pcm
	$(CXX) $(CXXFLAGS) use.cpp -fprebuilt-module-path=. Hello.pcm -o $@

clean:
	-rm *.pcm *.out
