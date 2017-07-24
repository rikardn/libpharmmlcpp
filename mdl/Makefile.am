ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = mdl

mdl_LDADD = $(PHARMMLCPP_LDADD) $(LIBXML2_LIBS)
mdl_CPPFLAGS = $(PHARMMLCPP_INCLUDE) $(LIBXML2_CFLAGS) -std=c++14
mdl_SOURCES = \
	mdl.cpp \
	MDLAstGenerator.cpp \
	MDLColumnMappingAstGenerator.cpp \
	MDLGenerator.cpp \
	MDLSymbols.cpp \
	MDLDeclaredVariables.cpp
