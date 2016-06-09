CC = g++
CFLAGS = -std=c++11 -g `xml2-config --cflags`
LIBS = -lxml2

test: test.cpp Scalar.cpp SymbolSet.cpp SymbolNamer.cpp ModellingSteps.cpp ../libpharmmlc.a
	$(CC) $(CFLAGS) test.cpp Scalar.cpp SymbolSet.cpp SymbolNamer.cpp ModellingSteps.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest

.PHONY: check
check: test
#~ 	g++ -std=c++11 $(CFLAGS) SymbolSet.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest
#~ 	g++ -std=c++11 $(CFLAGS) SymbolNamer.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest 
	./test

.PHONY: clean
clean:
	rm -f test
