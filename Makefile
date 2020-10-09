SHELL=/bin/bash

PYTHON?=python3
PYTARGET:=_ffcache$(shell ${PYTHON}-config --extension-suffix)
TARGET:=ffcache
CCOPT+=-std=c++17 -O2 -g -W -Wall -Wno-unused-value -Wno-range-loop-analysis -Wno-deprecated-declarations $(shell $(PYTHON) -m pybind11 --includes)
CXX?=g++

ifeq ($(shell uname), Darwin)
	LIB:=$(shell echo $$($(PYTHON)-config --prefix)/lib/libpython*.dylib)
endif

all: $(TARGET) $(PYTARGET)
py: $(PYTARGET)
cli: $(TARGET)

%.o: %.cpp
	$(CXX) -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o pymain.o
	$(CXX) $(CCOPT) -fPIC -shared $^ $(LIB) -o $@

$(TARGET): util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o main.o
	mkdir -p bin
	$(CXX) $(CCOPT) $^ -o bin/$@

clean:
	rm -rfv *.o bin *.so

test:
	$(PYTHON) -c 'import ffcache'
