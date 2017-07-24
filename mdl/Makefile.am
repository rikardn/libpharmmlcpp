ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = mdl

mdl_LDADD = -lpharmmlcpp -L../libpharmmlcpp/.libs  $(LIBXML2_LIBS)
mdl_CPPFLAGS = -I../libpharmmlcpp $(LIBXML2_CFLAGS) -std=c++14
mdl_SOURCES = \
	mdl.cpp \
	MDLAstGenerator.cpp \
	MDLColumnMappingAstGenerator.cpp \
	MDLGenerator.cpp \
	MDLSymbols.cpp \
	MDLDeclaredVariables.cpp
