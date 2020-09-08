PYTARGET:=ffcache$(shell python3-config --extension-suffix)
TARGET:=ffcache-linux-x86_64
CCOPT:=-std=c++11 -W -Wall $(shell python3 -m pybind11 --includes)

all: $(TARGET) $(PYTARGET)

%.o: %.cpp
	g++ -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o ffcache.o pymain.o
	g++ $(CCOPT) -O3 -fPIC -shared $^ -lstdc++fs -o $@

$(TARGET): util.o ffcache.o main.o
	g++ -static $(CCOPT) $^ -lstdc++fs -o $@

clean:
	rm -f *.o $(TARGET) *.so

test:
	python3 -c 'import ffcache'