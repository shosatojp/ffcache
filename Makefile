PYTARGET:=ffcache$(shell python3-config --extension-suffix)
TARGET:=ffcache
CCOPT:=-std=c++11 -W -Wall

all: $(TARGET) $(PYTARGET)

%.o: %.cpp
	g++ -fPIC $(CCOPT) -c -o $@ $^

$(PYTARGET): util.o ffcache.o pymain.o
	g++ $(CCOPT) -O3 -fPIC -shared `python3 -m pybind11 --includes` $^ -lstdc++fs -o $@

$(TARGET): util.o ffcache.o main.o
	g++ $(CCOPT) $^ -lstdc++fs -o $@

clean:
	rm -f *.o $(TARGET) $(PYTARGET)
