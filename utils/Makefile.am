ACLOCAL_AMFLAGS = -I m4

transformationsdir = $(datadir)/transformations
dist_transformations_DATA = \
    transformations/0_6to0_8_1.xslt \
    transformations/0_8_1to0_9.xslt

schemadir = $(datadir)/schema
nobase_dist_schema_DATA = \
    schema/0.8.1/pharmml-probonto/probOnto.xsd \
    schema/0.8.1/pharmml-schema/definitions/commonTypes.xsd \
    schema/0.8.1/pharmml-schema/definitions/dataset.xsd \
    schema/0.8.1/pharmml-schema/definitions/maths.xsd \
    schema/0.8.1/pharmml-schema/definitions/modelDefinition.xsd \
    schema/0.8.1/pharmml-schema/definitions/modellingSteps.xsd \
    schema/0.8.1/pharmml-schema/definitions/pharmml.xsd \
    schema/0.8.1/pharmml-schema/definitions/trialDesign.xsd \
    schema/0.8.1/pharmml-schema/definitions/xmlCatalog.xml \
    schema/0.8.1/pharmml-schema/uncertweb/UncertML30.xsd \
    schema/0.9/pharmml-probonto/probOnto.xsd \
    schema/0.9/pharmml-schema/definitions/commonTypes.xsd \
    schema/0.9/pharmml-schema/definitions/dataset.xsd \
    schema/0.9/pharmml-schema/definitions/maths.xsd \
    schema/0.9/pharmml-schema/definitions/modelDefinition.xsd \
    schema/0.9/pharmml-schema/definitions/modellingSteps.xsd \
    schema/0.9/pharmml-schema/definitions/pharmml.xsd \
	schema/0.9/pharmml-schema/definitions/trialDesign.xsd \
    schema/0.9/pharmml-schema/definitions/xmlCatalog.xml \
    schema/0.9/pharmml-schema/uncertweb/UncertML30.xsd

bin_PROGRAMS = pharmmltool

pharmmltool_LDADD = $(LIBXML2_LIBS) $(LIBXSLT_LIBS) -lstdc++fs
pharmmltool_SOURCES = \
	pharmmltool.cpp
