SHELL=/bin/bash

PYTHON:=python3.8
PYTARGET:=ffcache$(shell ${PYTHON}-config --extension-suffix)
TARGET:=ffcache-linux-x86_64
CCOPT:=-std=c++2a -W -Wall $(shell ${PYTHON} -m pybind11 --includes)
CXX:=g++

all: $(TARGET) $(PYTARGET)

%.o: %.cpp
	$(CXX) -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o pymain.o
	$(CXX) $(CCOPT) -O3 -fPIC -shared $^ -lstdc++fs -lz -o $@

$(TARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o main.o
	$(CXX) -static $(CCOPT) $^ -lstdc++fs -lz -o $@

clean:
	rm -f *.o $(TARGET) *.so

test:
	$(PYTHON) -c 'import ffcache'

install:
	cp -i $(TARGET) ~/local/bin/ffcache