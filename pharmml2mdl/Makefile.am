ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = pharmml2mdl

pharmml2mdl_LDADD = $(PHARMMLCPP_LDADD) $(LIBXML2_LIBS)
pharmml2mdl_CPPFLAGS = $(PHARMMLCPP_INCLUDE) $(LIBXML2_CFLAGS) -std=c++14
pharmml2mdl_SOURCES = \
	pharmml2mdl.cpp \
	MDLAstGenerator.cpp \
	MDLColumnMappingAstGenerator.cpp \
	MDLGenerator.cpp \
	MDLSymbols.cpp \
	MDLDeclaredVariables.cpp
