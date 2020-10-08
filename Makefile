SHELL=/bin/bash

PYTHON:=python3.8
PYTARGET:=_ffcache$(shell ${PYTHON}-config --extension-suffix)
ARCH:=x86-64
TARGET:=ffcache-linux-$(ARCH)
CCOPT:=-std=c++2a -march=$(ARCH) -O2 -g -W -Wall $(shell ${PYTHON} -m pybind11 --includes)
CXX:=g++

all: $(TARGET) $(PYTARGET)
py: $(PYTARGET)
cli: $(TARGET)

%.o: %.cpp
	$(CXX) -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o pymain.o
	$(CXX) $(CCOPT) -fPIC -shared $^ -o $@

$(TARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o main.o
	$(CXX) -static $(CCOPT) $^ -o $@

clean:
	rm -f *.o $(TARGET) *.so

test:
	$(PYTHON) -c 'import ffcache'
