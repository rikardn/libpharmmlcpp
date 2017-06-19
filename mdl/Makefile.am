bin_PROGRAMS = mdl

mdl_LDADD = -lpharmmlcpp -L../.libs  $(LIBXML2_LIBS)
mdl_CPPFLAGS = -I../src $(LIBXML2_CFLAGS) -std=c++14
mdl_SOURCES = \
	mdl.cpp \
	MDLAstGenerator.cpp \
	MDLColumnMappingAstGenerator.cpp \
	MDLGenerator.cpp \
	MDLSymbols.cpp \
	MDLDeclaredVariables.cpp
