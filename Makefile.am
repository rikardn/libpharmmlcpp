if LINUX
AM_CPPFLAGS = -I./src $(LIBXML2_CFLAGS) -std=c++14		# Need proper check of C++14
endif
if WINDOWS
AM_CPPFLAGS = -I./src -std=c++14 -static-libgcc -static-libstdc++ -L./windep/lib -I./windep/include
AM_LDFLAGS = -static-libgcc -static-libstdc++ -L./windep/lib
endif

noinst_LIBRARIES = libpharmmlcpp.a
libpharmmlcpp_a_SOURCES = \
   	src/AST/AstBuilder.cpp \
   	src/AST/AstNode.cpp \
	src/AST/AstTransformation.cpp \
   	src/AST/Binop.cpp \
   	src/AST/Constant.cpp \
   	src/AST/FunctionCall.cpp \
   	src/AST/Interval.cpp \
   	src/AST/Piecewise.cpp \
   	src/AST/Scalar.cpp \
   	src/AST/symbols.cpp \
   	src/AST/Uniop.cpp \
   	src/AST/Vector.cpp \
	src/AST/Sequence.cpp \
	src/AST/MatrixVectorIndex.cpp \
	src/AST/MatrixSelector.cpp \
	src/consolidators/Consolidator.cpp \
   	src/consolidators/Covariates.cpp \
   	src/consolidators/PopulationParameters.cpp \
	src/consolidators/VariabilityModels.cpp \
   	src/generators/TextFormatter.cpp \
	src/helpers/Logger.cpp \
	src/helpers/StringTools.cpp \
	src/helpers/SymbolNamer.cpp \
	src/helpers/Version.cpp \
	src/objects/Object.cpp \
	src/PharmML/Arms.cpp \
	src/PharmML/Block.cpp \
	src/PharmML/ColumnMapping.cpp \
	src/PharmML/Correlation.cpp \
	src/PharmML/CovariateModel.cpp \
	src/PharmML/Covariates.cpp \
	src/PharmML/Dataset.cpp \
	src/PharmML/DesignSpaces.cpp \
	src/PharmML/Distribution.cpp \
	src/PharmML/DistributionParameter.cpp \
	src/PharmML/ExternalDataset.cpp \
	src/PharmML/Interventions.cpp \
	src/PharmML/ModelDefinition.cpp \
	src/PharmML/ModellingSteps.cpp \
	src/PharmML/MultipleDVMapping.cpp \
	src/PharmML/Observations.cpp \
	src/PharmML/ParameterModel.cpp \
	src/PharmML/PharmML.cpp \
	src/PharmML/PharmMLReader.cpp \
	src/PharmML/PharmMLSection.cpp \
	src/PharmML/PharmMLWriter.cpp \
	src/PharmML/PKMacro.cpp \
	src/PharmML/StructuralModel.cpp \
	src/PharmML/TDCovariateModel.cpp \
	src/PharmML/TrialDesign.cpp \
	src/PharmML/VariabilityModel.cpp \
	src/symbols/Category.cpp \
	src/symbols/Covariate.cpp \
	src/symbols/DerivativeVariable.cpp \
	src/symbols/DiscreteVariable.cpp \
	src/symbols/FunctionDefinition.cpp \
	src/symbols/IndependentVariable.cpp \
	src/symbols/IndividualParameter.cpp \
	src/symbols/MacroGathering.cpp \
	src/symbols/ObservationModel.cpp \
	src/symbols/PopulationParameter.cpp \
	src/symbols/RandomVariable.cpp \
	src/symbols/Symbol.cpp \
	src/symbols/SymbolGathering.cpp \
	src/symbols/SymbolSet.cpp \
	src/symbols/VariabilityLevel.cpp \
	src/symbols/Variable.cpp \
	src/visitors/AstAnalyzer.cpp \
	src/visitors/AstParenthesizer.cpp \
	src/visitors/StringVisitor.cpp \
	src/visitors/SymbolNameVisitor.cpp \
	src/visitors/SymbolSortVisitor.cpp \
	src/visitors/SymbRefFinder.cpp \
	src/visitors/XMLAstVisitor.cpp \
	src/xml/Document.cpp \
	src/xml/xml.cpp \
	src/xml/XPathContext.cpp


bin_PROGRAMS = pharmml2poped mdl pharmmltool

pharmml2poped_LDADD = libpharmmlcpp.a $(LIBXML2_LIBS)
pharmml2poped_SOURCES = \
	pharmml2poped.cpp \
	src/popedconvert.cpp \
	src/generators/PopED/PopEDAstGenerator.cpp \
	src/generators/PopED/PopEDGenerator.cpp \
	src/generators/PopED/PopEDPastDerivativesSymbols.cpp \
	src/generators/PopED/PopEDSymbols.cpp \
	src/generators/PopED/PopEDObjects.cpp \
	src/generators/R/RSymbolNamer.cpp \
	src/generators/R/RAstGenerator.cpp \
	src/generators/R/RSymbols.cpp \
	src/generators/R/RPharmMLGenerator.cpp \
	src/generators/R/RPharmMLConsolidator.cpp

mdl_LDADD = libpharmmlcpp.a $(LIBXML2_LIBS)
mdl_SOURCES = \
	mdl.cpp \
	src/generators/MDL/MDLAstGenerator.cpp \
	src/generators/MDL/MDLColumnMappingAstGenerator.cpp \
	src/generators/MDL/MDLGenerator.cpp \
	src/generators/MDL/MDLSymbols.cpp \
	src/generators/MDL/MDLDeclaredVariables.cpp

pharmmltool_LDADD = $(LIBXML2_LIBS)
pharmmltool_SOURCES = \
	utils/pharmmltool.cpp

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
unit_LDADD = libpharmmlcpp.a $(LIBXML2_LIBS)

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
