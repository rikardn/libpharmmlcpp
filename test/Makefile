CFLAGS = -std=c++11 -g `xml2-config --cflags`
LIBS = -lxml2

.PHONY: check

check:
	g++ -std=c++11 $(CFLAGS) SymbolSet.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest
	g++ -std=c++11 $(CFLAGS) SymbolNamer.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest
	./test
