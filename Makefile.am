ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libpharmmlcpp.la
libpharmmlcpp_la_SOURCES = \
   	libpharmmlcpp/AST/AstBuilder.cpp \
   	libpharmmlcpp/AST/AstNode.cpp \
	libpharmmlcpp/AST/AstTransformation.cpp \
   	libpharmmlcpp/AST/Binop.cpp \
   	libpharmmlcpp/AST/Constant.cpp \
   	libpharmmlcpp/AST/FunctionCall.cpp \
   	libpharmmlcpp/AST/Interval.cpp \
   	libpharmmlcpp/AST/Piecewise.cpp \
   	libpharmmlcpp/AST/Scalar.cpp \
   	libpharmmlcpp/AST/symbols.cpp \
   	libpharmmlcpp/AST/Uniop.cpp \
   	libpharmmlcpp/AST/Vector.cpp \
	libpharmmlcpp/AST/Sequence.cpp \
	libpharmmlcpp/AST/MatrixVectorIndex.cpp \
	libpharmmlcpp/AST/MatrixSelector.cpp \
	libpharmmlcpp/consolidators/Consolidator.cpp \
   	libpharmmlcpp/consolidators/Covariates.cpp \
   	libpharmmlcpp/consolidators/PopulationParameters.cpp \
	libpharmmlcpp/consolidators/VariabilityModels.cpp \
   	libpharmmlcpp/generators/TextFormatter.cpp \
	libpharmmlcpp/helpers/Logger.cpp \
	libpharmmlcpp/helpers/StringTools.cpp \
	libpharmmlcpp/helpers/SymbolNamer.cpp \
	libpharmmlcpp/helpers/Version.cpp \
	libpharmmlcpp/objects/Object.cpp \
	libpharmmlcpp/PharmML/Arms.cpp \
	libpharmmlcpp/PharmML/Block.cpp \
	libpharmmlcpp/PharmML/ColumnMapping.cpp \
	libpharmmlcpp/PharmML/Correlation.cpp \
	libpharmmlcpp/PharmML/CovariateModel.cpp \
	libpharmmlcpp/PharmML/Covariates.cpp \
	libpharmmlcpp/PharmML/Dataset.cpp \
	libpharmmlcpp/PharmML/DesignSpaces.cpp \
	libpharmmlcpp/PharmML/Distribution.cpp \
	libpharmmlcpp/PharmML/DistributionParameter.cpp \
	libpharmmlcpp/PharmML/ExternalDataset.cpp \
	libpharmmlcpp/PharmML/Interventions.cpp \
	libpharmmlcpp/PharmML/ModelDefinition.cpp \
	libpharmmlcpp/PharmML/ModellingSteps.cpp \
	libpharmmlcpp/PharmML/MultipleDVMapping.cpp \
	libpharmmlcpp/PharmML/Observations.cpp \
	libpharmmlcpp/PharmML/ParameterModel.cpp \
	libpharmmlcpp/PharmML/PharmML.cpp \
	libpharmmlcpp/PharmML/PharmMLReader.cpp \
	libpharmmlcpp/PharmML/PharmMLSection.cpp \
	libpharmmlcpp/PharmML/PharmMLWriter.cpp \
	libpharmmlcpp/PharmML/PKMacro.cpp \
	libpharmmlcpp/PharmML/StructuralModel.cpp \
	libpharmmlcpp/PharmML/TDCovariateModel.cpp \
	libpharmmlcpp/PharmML/TrialDesign.cpp \
	libpharmmlcpp/PharmML/VariabilityModel.cpp \
	libpharmmlcpp/symbols/Category.cpp \
	libpharmmlcpp/symbols/Covariate.cpp \
	libpharmmlcpp/symbols/DerivativeVariable.cpp \
	libpharmmlcpp/symbols/DiscreteVariable.cpp \
	libpharmmlcpp/symbols/FunctionDefinition.cpp \
	libpharmmlcpp/symbols/IndependentVariable.cpp \
	libpharmmlcpp/symbols/IndividualParameter.cpp \
	libpharmmlcpp/symbols/MacroGathering.cpp \
	libpharmmlcpp/symbols/ObservationModel.cpp \
	libpharmmlcpp/symbols/PopulationParameter.cpp \
	libpharmmlcpp/symbols/RandomVariable.cpp \
	libpharmmlcpp/symbols/Symbol.cpp \
	libpharmmlcpp/symbols/SymbolGathering.cpp \
	libpharmmlcpp/symbols/SymbolSet.cpp \
	libpharmmlcpp/symbols/VariabilityLevel.cpp \
	libpharmmlcpp/symbols/Variable.cpp \
	libpharmmlcpp/visitors/AstAnalyzer.cpp \
	libpharmmlcpp/visitors/AstParenthesizer.cpp \
	libpharmmlcpp/visitors/StringVisitor.cpp \
	libpharmmlcpp/visitors/SymbolNameVisitor.cpp \
	libpharmmlcpp/visitors/SymbolSortVisitor.cpp \
	libpharmmlcpp/visitors/SymbRefFinder.cpp \
	libpharmmlcpp/visitors/XMLAstVisitor.cpp \
	libpharmmlcpp/xml/Document.cpp \
	libpharmmlcpp/xml/xml.cpp \
	libpharmmlcpp/xml/XPathContext.cpp


SUBDIRS = . mdl pharmml2poped utils

TESTS = unit
check_PROGRAMS = unit
unit_SOURCES = \
	test/unit.cpp \
	test/IndependentVariable.cpp \
	test/symbols.cpp \
	test/Scalar.cpp \
	test/Uniop.cpp \
	test/Binop.cpp \
	test/Interval.cpp \
	test/Sequence.cpp \
	test/FunctionCall.cpp \
	test/SymbolSet.cpp \
	test/SymbolNamer.cpp
unit_LDADD = -lpharmmlcpp $(LIBXML2_LIBS)

# For binary win64 release
WIN_RELEASE_PACKAGE = ${PACKAGE_NAME}-${PACKAGE_VERSION}-windows-x64
.PHONY: winrelease
winrelease:
	mkdir -p ${WIN_RELEASE_PACKAGE}
	x86_64-w64-mingw32-strip mdl.exe
	cp mdl.exe ${WIN_RELEASE_PACKAGE}
	cp windep/lib/*.dll ${WIN_RELEASE_PACKAGE}
	cp -r pharmml_internalRelease_0_8_1 ${WIN_RELEASE_PACKAGE}
	zip -r ${WIN_RELEASE_PACKAGE}.zip ${WIN_RELEASE_PACKAGE}
	rm -rf ${WIN_RELEASE_PACKAGE}

LINUX_RELEASE_PACKAGE = ${PACKAGE_NAME}-${PACKAGE_VERSION}-linux-x64
.PHONY: linuxrelease
linuxrelease:
	mkdir -p ${LINUX_RELEASE_PACKAGE}
	strip mdl
	cp mdl ${LINUX_RELEASE_PACKAGE}
	cp -r pharmml_internalRelease_0_8_1 ${LINUX_RELEASE_PACKAGE}
	tar -zcvf ${LINUX_RELEASE_PACKAGE}.tar.gz ${LINUX_RELEASE_PACKAGE}
	rm -rf ${LINUX_RELEASE_PACKAGE}

#Fetch and compile dependencies for windows
.PHONY: windep
windep:
	rm -rf windep
	mkdir -p windep
	mkdir -p windep/include
	mkdir -p windep/lib
	wget -P windep http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz
	cd windep; tar xvfz libiconv-1.14.tar.gz
	cd windep/libiconv-1.14/;./configure --host=x86_64-w64-mingw32;make
	cp windep/libiconv-1.14/include/iconv.h windep/include
	cp windep/libiconv-1.14/lib/.libs/libiconv-2.dll windep/lib
	cp windep/libiconv-1.14/lib/.libs/libiconv.dll.a windep/lib
	cp windep/libiconv-1.14/lib/libcharset.dll.a windep/lib
	cp windep/libiconv-1.14/libcharset/lib/.libs/libcharset-1.dll windep/lib
	wget -P windep http://zlib.net/zlib-1.2.8.tar.gz
	cd windep; tar xvfz zlib-1.2.8.tar.gz
	cd windep/zlib-1.2.8/win32; sed -i 's/PREFIX =/PREFIX = x86_64-w64-mingw32-/' Makefile.gcc
	cd windep/zlib-1.2.8; make -f win32/Makefile.gcc
	cp windep/zlib-1.2.8/zconf.h windep/include
	cp windep/zlib-1.2.8/zlib.h windep/include
	cp windep/zlib-1.2.8/zlib1.dll windep/lib
	cp windep/zlib-1.2.8/libz.dll.a windep/lib
	wget -P windep ftp://xmlsoft.org/libxml2/libxml2-2.9.3.tar.gz
	cd windep; tar xvfz libxml2-2.9.3.tar.gz
	cd windep/libxml2-2.9.3; ./configure --host=x86_64-w64-mingw32 --without-python --without-docbook --without-ftp --without-http --without-schematron --with-lzma=no --with-zlib=$(PWD)/windep --with-iconv=$(PWD)/windep; make 
#	--without-html --without-legacy --without-regexps --without-sax1 --without-schemas --without-valid --without-xpath 
	cp -r windep/libxml2-2.9.3/include/libxml windep/include
	cp windep/libxml2-2.9.3/.libs/libxml2-2.dll windep/lib
	cp windep/libxml4-2.9.3/.libs/libxml2.dll.a windep/lib
