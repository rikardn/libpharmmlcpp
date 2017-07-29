ACLOCAL_AMFLAGS = -I m4

transformationsdir = $(datadir)/transformations
dist_transformations_DATA = \
    transformations/0_6to0_8_1.xslt \
    transformations/0_8_1to0_9.xslt

bin_PROGRAMS = pharmmltool

pharmmltool_LDADD = $(LIBXML2_LIBS) $(LIBXSLT_LIBS) -lstdc++fs
pharmmltool_SOURCES = \
	pharmmltool.cpp
