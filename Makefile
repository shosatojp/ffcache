SHELL=/bin/bash

PYTHON:=python3.8
PYTARGET:=_ffcache$(shell ${PYTHON}-config --extension-suffix)
TARGET:=ffcache
CCOPT:=-std=c++2a -O2 -g -W -Wall $(shell ${PYTHON} -m pybind11 --includes)
CXX:=g++

all: $(TARGET) $(PYTARGET)
py: $(PYTARGET)
cli: $(TARGET)

%.o: %.cpp
	$(CXX) -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o pymain.o
	$(CXX) $(CCOPT) -fPIC -shared $^ -o $@

$(TARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o main.o
	mkdir -p bin
	$(CXX) -static $(CCOPT) $^ -o bin/$@

clean:
	rm -rfv *.o bin *.so

test:
	$(PYTHON) -c 'import ffcache'
