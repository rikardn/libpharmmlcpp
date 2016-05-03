TARGET = UNIX

LIBS = -lxml2

ifeq ($(TARGET), UNIX)
  CC = g++
  CFLAGS = -std=c++11 -g -Isrc `xml2-config --cflags`
  OUTPUT = main
else
  CC = x86_64-w64-mingw32-g++
  CFLAGS = -std=c++11 -static-libgcc -static-libstdc++ -L./windep/libs -Isrc -I./windep/include
  OUTPUT = main.exe
endif

main: main.cpp libpharmmlc.a
	$(CC) main.cpp -o$(OUTPUT) -lpharmmlc $(CFLAGS) -L. $(LIBS)

pharmml2poped: pharmml2poped.cpp src/generators/PopED/PopEDAstGenerator.cpp src/generators/PopED/PopEDAstGenerator.h src/generators/PopED/PopEDGenerator.cpp src/generators/PopED/PopEDGenerator.h src/generators/PopED/PopEDSymbols.cpp src/generators/PopED/PopEDSymbols.h libpharmmlc.a
	$(CC) pharmml2poped.cpp src/generators/PopED/PopEDAstGenerator.cpp src/generators/PopED/PopEDGenerator.cpp src/generators/PopED/PopEDSymbols.cpp -opharmml2poped -lpharmmlc $(CFLAGS) -L. $(LIBS)

output: output.cpp libpharmmlc.a
	$(CC) output.cpp -ooutput -lpharmmlc $(CFLAGS) -L. $(LIBS)

libpharmmlc.a: PharmMLContext.o Model.o ModellingSteps.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RAstGenerator.o RPharmMLGenerator.o RPharmMLConsolidator.o StructuralModel.o Variable.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o Dependencies.o Dataset.o ExternalDataset.o ColumnMapping.o Interval.o XMLAstVisitor.o StringVisitor.o Symbol.o xml.o
	ar -rcs libpharmmlc.a PharmMLContext.o Model.o ModellingSteps.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RAstGenerator.o RPharmMLGenerator.o RPharmMLConsolidator.o StructuralModel.o Variable.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o Dependencies.o Dataset.o ExternalDataset.o ColumnMapping.o Interval.o XMLAstVisitor.o StringVisitor.o Symbol.o xml.o


Symbol.o: src/symbols/Symbol.cpp src/symbols/Symbol.h
	$(CC) -c src/symbols/Symbol.cpp -oSymbol.o $(CFLAGS)

StringVisitor.o: src/visitors/StringVisitor.cpp src/visitors/StringVisitor.h
	$(CC) -c src/visitors/StringVisitor.cpp -oStringVisitor.o $(CFLAGS)

XMLAstVisitor.o: src/visitors/XMLAstVisitor.cpp src/visitors/XMLAstVisitor.h
	$(CC) -c src/visitors/XMLAstVisitor.cpp -oXMLAstVisitor.o $(CFLAGS)

Interval.o: src/AST/Interval.cpp src/AST/Interval.h
	$(CC) -c src/AST/Interval.cpp -oInterval.o $(CFLAGS)

ColumnMapping.o: src/PharmML/ColumnMapping.cpp src/PharmML/ColumnMapping.h
	$(CC) -c src/PharmML/ColumnMapping.cpp -oColumnMapping.o $(CFLAGS)

Dataset.o: src/PharmML/Dataset.cpp src/PharmML/Dataset.h
	$(CC) -c src/PharmML/Dataset.cpp -oDataset.o $(CFLAGS)

ExternalDataset.o: src/PharmML/ExternalDataset.cpp src/PharmML/ExternalDataset.h
	$(CC) -c src/PharmML/ExternalDataset.cpp -oExternalDataset.o $(CFLAGS)

Dependencies.o: src/PharmML/Dependencies.cpp src/PharmML/Dependencies.h
	$(CC) -c src/PharmML/Dependencies.cpp -oDependencies.o $(CFLAGS)

DerivativeVariable.o: src/symbols/DerivativeVariable.cpp src/symbols/DerivativeVariable.h
	$(CC) -c src/symbols/DerivativeVariable.cpp -oDerivativeVariable.o $(CFLAGS)

DistributionParameter.o: src/PharmML/DistributionParameter.cpp src/PharmML/DistributionParameter.h
	$(CC) -c src/PharmML/DistributionParameter.cpp -oDistributionParameter.o $(CFLAGS)

Distribution.o: src/PharmML/Distribution.cpp src/PharmML/Distribution.h
	$(CC) -c src/PharmML/Distribution.cpp -oDistribution.o $(CFLAGS)

RandomVariable.o: src/symbols/RandomVariable.cpp src/symbols/RandomVariable.h
	$(CC) -c src/symbols/RandomVariable.cpp -oRandomVariable.o $(CFLAGS)

IndependentVariable.o: src/symbols/IndependentVariable.cpp src/symbols/IndependentVariable.h
	$(CC) -c src/symbols/IndependentVariable.cpp -oIndependentVariable.o $(CFLAGS)

FunctionCall.o: src/AST/FunctionCall.cpp src/AST/FunctionCall.h
	$(CC) -c src/AST/FunctionCall.cpp -oFunctionCall.o $(CFLAGS)

ObservationModel.o: src/PharmML/ObservationModel.cpp src/PharmML/ObservationModel.h
	$(CC) -c src/PharmML/ObservationModel.cpp -oObservationModel.o $(CFLAGS)

PopulationParameter.o: src/symbols/PopulationParameter.cpp src/symbols/PopulationParameter.h
	$(CC) -c src/symbols/PopulationParameter.cpp -oPopulationParameter.o $(CFLAGS)

IndividualParameter.o: src/symbols/IndividualParameter.cpp src/symbols/IndividualParameter.h
	$(CC) -c src/symbols/IndividualParameter.cpp -oIndividualParameter.o $(CFLAGS)

ParameterModel.o: src/PharmML/ParameterModel.cpp src/PharmML/ParameterModel.h
	$(CC) -c src/PharmML/ParameterModel.cpp -oParameterModel.o $(CFLAGS)

Interventions.o: src/PharmML/Interventions.cpp src/PharmML/Interventions.h
	$(CC) -c src/PharmML/Interventions.cpp -oInterventions.o $(CFLAGS)

Observations.o: src/PharmML/Observations.cpp src/PharmML/Observations.h
	$(CC) -c src/PharmML/Observations.cpp -oObservations.o $(CFLAGS)

Arms.o: src/PharmML/Arms.cpp src/PharmML/Arms.h
	$(CC) -c src/PharmML/Arms.cpp -oArms.o $(CFLAGS)

DesignSpaces.o: src/PharmML/DesignSpaces.cpp src/PharmML/DesignSpaces.h
	$(CC) -c src/PharmML/DesignSpaces.cpp -oDesignSpaces.o $(CFLAGS)

TrialDesign.o: src/PharmML/TrialDesign.cpp src/PharmML/TrialDesign.h
	$(CC) -c src/PharmML/TrialDesign.cpp -oTrialDesign.o $(CFLAGS)

FunctionDefinition.o: src/PharmML/FunctionDefinition.cpp src/PharmML/FunctionDefinition.h
	$(CC) -c src/PharmML/FunctionDefinition.cpp -oFunctionDefinition.o $(CFLAGS)

Vector.o: src/AST/Vector.cpp src/AST/Vector.h
	$(CC) -c src/AST/Vector.cpp -oVector.o $(CFLAGS)

Piecewise.o: src/AST/Piecewise.cpp src/AST/Piecewise.h
	$(CC) -c src/AST/Piecewise.cpp -oPiecewise.o $(CFLAGS)

StructuralModel.o: src/PharmML/StructuralModel.cpp src/PharmML/StructuralModel.h
	$(CC) -c src/PharmML/StructuralModel.cpp -oStructuralModel.o $(CFLAGS)

Variable.o: src/symbols/Variable.cpp src/symbols/Variable.h
	$(CC) -c src/symbols/Variable.cpp -oVariable.o $(CFLAGS)

RPharmMLGenerator.o: src/generators/R/RPharmMLGenerator.cpp src/generators/R/RPharmMLGenerator.h
	$(CC) -c src/generators/R/RPharmMLGenerator.cpp -oRPharmMLGenerator.o $(CFLAGS)

RPharmMLConsolidator.o: src/generators/R/RPharmMLConsolidator.cpp src/generators/R/RPharmMLConsolidator.h
	$(CC) -c src/generators/R/RPharmMLConsolidator.cpp -oRPharmMLConsolidator.o $(CFLAGS)

RAstGenerator.o: src/generators/R/RAstGenerator.cpp src/generators/R/RAstGenerator.h
	$(CC) -c src/generators/R/RAstGenerator.cpp -oRAstGenerator.o $(CFLAGS)

symbols.o: src/AST/symbols.cpp src/AST/symbols.h
	$(CC) -c src/AST/symbols.cpp -osymbols.o $(CFLAGS)

Scalar.o: src/AST/Scalar.cpp src/AST/Scalar.h
	$(CC) -c src/AST/Scalar.cpp -oScalar.o $(CFLAGS)

Binop.o: src/AST/Binop.cpp src/AST/Binop.h
	$(CC) -c src/AST/Binop.cpp -oBinop.o $(CFLAGS)

Uniop.o: src/AST/Uniop.cpp src/AST/Uniop.h
	$(CC) -c src/AST/Uniop.cpp -oUniop.o $(CFLAGS)

Constant.o: src/AST/Constant.cpp src/AST/Constant.h
	$(CC) -c src/AST/Constant.cpp -oConstant.o $(CFLAGS)

AstNodeFactory.o: src/AST/AstNodeFactory.cpp src/AST/AstNodeFactory.h
	$(CC) -c src/AST/AstNodeFactory.cpp -oAstNodeFactory.o $(CFLAGS)

Covariate.o: src/PharmML/Covariate.cpp src/PharmML/Covariate.h
	$(CC) -c src/PharmML/Covariate.cpp -oCovariate.o $(CFLAGS)

CovariateModel.o: src/PharmML/CovariateModel.cpp src/PharmML/CovariateModel.h
	$(CC) -c src/PharmML/CovariateModel.cpp -oCovariateModel.o $(CFLAGS)

ModelDefinition.o: src/PharmML/ModelDefinition.cpp src/PharmML/ModelDefinition.h
	$(CC) -c src/PharmML/ModelDefinition.cpp -oModelDefinition.o $(CFLAGS)

ModellingSteps.o: src/PharmML/ModellingSteps.cpp src/PharmML/ModellingSteps.h
	$(CC) -c src/PharmML/ModellingSteps.cpp -oModellingSteps.o $(CFLAGS)

PharmMLContext.o: src/PharmML/PharmMLContext.cpp src/PharmML/PharmMLContext.h
	$(CC) -c src/PharmML/PharmMLContext.cpp -oPharmMLContext.o $(CFLAGS)

Model.o: src/PharmML/Model.cpp src/PharmML/Model.h
	$(CC) -c src/PharmML/Model.cpp -oModel.o $(CFLAGS)

xml.o: src/xml/xml.cpp src/xml/xml.h
	$(CC) -c src/xml/xml.cpp -oxml.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f pharmml2poped
	rm -f libpharmmlc.a
	rm -f main
	rm -f output
	rm -f main.exe



.PHONY: release
release:
	mkdir -p release
	cp main.exe release
	cp windep/lib/*.dll release
	cp -r pharmml_internalRelease_0_8_1 release
	cp Executable_Simeoni_2004_oncology_TGI_trialdesign.xml release	


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
	cd windep/libxml2-2.9.3; ./configure --host=x86_64-w64-mingw32 --without-python --without-docbook --without-ftp --without-http --without-schematron --with-lzma=no --with-zlib=/home/rikard/moose/pharmmlc/windep --with-iconv=/home/rikard/moose/pharmml/windep; make 
#	--without-html --without-legacy --without-regexps --without-sax1 --without-schemas --without-valid --without-xpath 
	cp -r windep/libxml2-2.9.3/include/libxml windep/include
	cp windep/libxml2-2.9.3/.libs/libxml2-2.dll windep/lib
	cp windep/libxml2-2.9.3/.libs/libxml2.dll.a windep/lib
