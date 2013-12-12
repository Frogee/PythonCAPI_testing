CXXFLAGS := -I/usr/include/python2.7
LDFLAGS := -L/usr/lib
LDLIBS := -lfftw3 -lm -lpython2.7

EXECUTABLE := testEmbed

all: $(EXECUTABLE)

testEmbed: testEmbed.o
	g++ testEmbed.o -o $(EXECUTABLE) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

testEmbed.o: testEmbed.cpp
	g++ -c testEmbed.cpp $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf *o $(EXECUTABLE)
