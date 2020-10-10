SHELL=/bin/bash

BUILDDIR:=build
BINDIR:=bin
SRCDIR:=src

PYTHON?=python3
PYTARGET:=_ffcache$(shell ${PYTHON}-config --extension-suffix)
TARGET:=$(BINDIR)/ffcache
CCOPT+=-std=c++17 -O2 -g -W -Wall -Wno-unused-value -Wno-range-loop-analysis -Wno-deprecated-declarations $(shell $(PYTHON) -m pybind11 --includes)
CXX?=g++

_PYOBJS:=util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o pymain.o
_OBJS:=util.o structs.o ffcacheentry.o ffcacheindex.o httpheader.o ffcache.o main.o

PYOBJS:=$(addprefix $(BUILDDIR)/,$(_PYOBJS))
OBJS:=$(addprefix $(BUILDDIR)/,$(_OBJS))

PYDEPS:=$(PYOBJS:%.o=%.d)
DEPS:=$(OBJS:%.o=%.d)

ifeq ($(shell uname), Darwin)
	LIB:=$(shell echo $$($(PYTHON)-config --prefix)/lib/libpython*.dylib)
endif

all: $(TARGET) $(PYTARGET)
py: $(PYTARGET)
cli: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	-@mkdir -p build
	$(CXX) -fPIC $(CCOPT) -MMD -MP -c -o $@ $<

$(PYTARGET): $(PYOBJS)
	$(CXX) $(CCOPT) -fPIC -shared $^ $(LIB) -o $@

$(TARGET): $(OBJS)
	-@mkdir -p bin
	$(CXX) $(CCOPT) $^ -o $@

clean:
	rm -rfv $(TARGET) $(BINDIR) $(BUILDDIR)

test:
	$(PYTHON) -c 'import ffcache'

-include $(PYDEPS)
-include $(DEPS)