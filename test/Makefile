CFLAGS = -std=c++11 -g `xml2-config --cflags`
LIBS = -lxml2

.PHONY: check

check:
	g++ -std=c++11 $(CFLAGS) Dependencies.cpp -L.. -lpharmmlc $(LIBS) -otest
	./test
