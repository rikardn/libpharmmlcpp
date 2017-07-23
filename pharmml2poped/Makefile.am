bin_PROGRAMS = pharmml2poped

pharmml2poped_LDADD = -lpharmmlcpp -L../.libs $(LIBXML2_LIBS)
pharmml2poped_CPPFLAGS = -I../libpharmmlcpp $(LIBXML2_CFLAGS) -std=c++14
pharmml2poped_SOURCES = \
	pharmml2poped.cpp \
	popedconvert.cpp \
	PopED/PopEDAstGenerator.cpp \
	PopED/PopEDGenerator.cpp \
	PopED/PopEDPastDerivativesSymbols.cpp \
	PopED/PopEDSymbols.cpp \
	PopED/PopEDObjects.cpp \
	R/RSymbolNamer.cpp \
	R/RAstGenerator.cpp \
	R/RSymbols.cpp \
	R/RPharmMLGenerator.cpp \
	R/RPharmMLConsolidator.cpp
