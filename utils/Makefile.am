ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
    README \
    LICENSE \
    test/testfiles/UseCase1.xml \
    test/testfiles/Executable_Krippendorff_ModelB.xml \
    test/testfiles/glucoseKinetics.xml

transformationsdir = $(pkgdatadir)/transformations
dist_transformations_DATA = \
    transformations/0_6to0_8_1.xslt \
    transformations/0_8_1to0_9.xslt

schemadir = $(pkgdatadir)/schema
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

pharmmltool_LDADD = $(LIBXML2_LIBS) $(LIBXSLT_LIBS)
pharmmltool_SOURCES = \
	pharmmltool.cpp

dist_check_SCRIPTS = test/pharmmltool.py
TESTS = test/pharmmltool.py
TEST_EXTENSIONS = .py
PY_LOG_COMPILER = python3

# For binary win64 release
WIN_RELEASE_PACKAGE = ${PACKAGE_NAME}-${PACKAGE_VERSION}-windows-x64
.PHONY: winrelease
winrelease:
	rm pharmmlutils-0.4-windows-x64.zip
	rm pharmmlutils-0.4-windows-x64 -rf
	mkdir -p ${WIN_RELEASE_PACKAGE}
	cp .libs/pharmmltool.exe ${WIN_RELEASE_PACKAGE}
	x86_64-w64-mingw32-strip ${WIN_RELEASE_PACKAGE}/pharmmltool.exe
	cp ../windep/lib/*.dll ${WIN_RELEASE_PACKAGE}
	cp -r schema ${WIN_RELEASE_PACKAGE}
	cp -r transformations ${WIN_RELEASE_PACKAGE}
	zip -r ${WIN_RELEASE_PACKAGE}.zip ${WIN_RELEASE_PACKAGE}
	rm -rf ${WIN_RELEASE_PACKAGE}
