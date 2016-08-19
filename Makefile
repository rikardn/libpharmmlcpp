TARGET = UNIX

LIBS = -lxml2

ifeq ($(TARGET), UNIX)
  CC = g++
  CFLAGS = -MMD -Wall -std=c++14 -g -Isrc `xml2-config --cflags`
  OUTPUT = pharmml2poped
else
  CC = x86_64-w64-mingw32-g++
  CFLAGS = -MMD -Wall -O3 -std=c++14 -static-libgcc -static-libstdc++ -L./windep/lib -Isrc -I./windep/include
  OUTPUT = pharmml2poped.exe
endif

#main: main.cpp libpharmmlc.a
#	$(CC) main.cpp -o$(OUTPUT) -lpharmmlc $(CFLAGS) -L. $(LIBS)

pharmml2poped: pharmml2poped.cpp src/popedconvert.cpp src/generators/PopED/PopEDAstGenerator.cpp src/generators/PopED/PopEDAstGenerator.h src/generators/PopED/PopEDGenerator.cpp src/generators/PopED/PopEDGenerator.h src/generators/PopED/PopEDSymbols.cpp src/generators/PopED/PopEDPastDerivativesSymbols.cpp src/generators/PopED/PopEDPastDerivativesSymbols.h src/generators/PopED/PopEDSymbols.h src/generators/PopED/PopEDObjects.cpp src/generators/PopED/PopEDObjects.h src/generators/TextFormatter.cpp src/generators/TextFormatter.h src/generators/R/RSymbolNamer.cpp src/generators/R/RSymbolNamer.h libpharmmlc.a
	$(CC) pharmml2poped.cpp src/popedconvert.cpp src/generators/PopED/PopEDAstGenerator.cpp src/generators/PopED/PopEDGenerator.cpp src/generators/PopED/PopEDSymbols.cpp src/generators/PopED/PopEDPastDerivativesSymbols.cpp src/generators/PopED/PopEDObjects.cpp src/generators/R/RSymbolNamer.cpp -o$(OUTPUT) -lpharmmlc $(CFLAGS) -L. $(LIBS)

mdl: mdl.cpp src/generators/MDL/MDLAstGenerator.cpp src/generators/MDL/MDLAstGenerator.h src/generators/MDL/MDLSymbols.cpp src/generators/MDL/MDLSymbols.h src/generators/MDL/MDLGenerator.cpp src/generators/MDL/MDLGenerator.h src/generators/TextFormatter.cpp src/generators/TextFormatter.h libpharmmlc.a
	$(CC) mdl.cpp src/generators/MDL/MDLAstGenerator.cpp src/generators/MDL/MDLSymbols.cpp src/generators/MDL/MDLGenerator.cpp -omdl -lpharmmlc $(CFLAGS) -L. $(LIBS)

pharmmltool: utils/pharmmltool.cpp
	$(CC) utils/pharmmltool.cpp -opharmmltool `xml2-config --libs --cflags`

.PHONY: install
install:
	cp pharmmltool /usr/bin
	mkdir -p /usr/share/libpharmmlcpp
	cp -rf pharmml_internalRelease_0_8_1 /usr/share/libpharmmlcpp




output: output.cpp libpharmmlc.a
	$(CC) output.cpp -ooutput -lpharmmlc $(CFLAGS) -L. $(LIBS)

libpharmmlc.a: PharmMLContext.o PharmML.o ModellingSteps.o ModelDefinition.o VariabilityModel.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RAstGenerator.o RPharmMLGenerator.o RPharmMLConsolidator.o StructuralModel.o Variable.o PKMacro.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o Correlation.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o VariabilityLevel.o Dataset.o ExternalDataset.o ColumnMapping.o Interval.o XMLAstVisitor.o StringVisitor.o Symbol.o SymbolSet.o SymbRefFinder.o AstAnalyzer.o xml.o TextFormatter.o Consolidator.o CPopulationParameters.o CCovariates.o CVariabilityModels.o RSymbols.o Logger.o PharmMLSection.o Object.o AstTransformation.o SymbolNameVisitor.o AstBuilder.o SymbolSortVisitor.o AstParenthesizer.o StringTools.o SymbolNamer.o Block.o SymbolGathering.o MacroGathering.o Document.o XPathContext.o PharmMLReader.o Version.o PharmMLWriter.o
	ar -rcs libpharmmlc.a PharmMLContext.o PharmML.o ModellingSteps.o ModelDefinition.o VariabilityModel.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RAstGenerator.o RPharmMLGenerator.o RPharmMLConsolidator.o StructuralModel.o Variable.o PKMacro.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o Correlation.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o VariabilityLevel.o Dataset.o ExternalDataset.o ColumnMapping.o Interval.o XMLAstVisitor.o StringVisitor.o Symbol.o SymbolSet.o SymbRefFinder.o AstAnalyzer.o xml.o TextFormatter.o Consolidator.o CPopulationParameters.o CCovariates.o CVariabilityModels.o RSymbols.o Logger.o PharmMLSection.o Object.o AstTransformation.o SymbolNameVisitor.o AstBuilder.o SymbolSortVisitor.o AstParenthesizer.o StringTools.o SymbolNamer.o Block.o SymbolGathering.o MacroGathering.o Document.o XPathContext.o PharmMLReader.o Version.o PharmMLWriter.o
	ar -rcs libpharmmlc.d.a PharmMLContext.d PharmML.d ModellingSteps.d ModelDefinition.d VariabilityModel.d CovariateModel.d Covariate.d AstNodeFactory.d Constant.d Uniop.d Binop.d symbols.d Scalar.d RAstGenerator.d RPharmMLGenerator.d RPharmMLConsolidator.d StructuralModel.d Variable.d PKMacro.d Vector.d Piecewise.d FunctionDefinition.d ParameterModel.d Interventions.d Observations.d Arms.d DesignSpaces.d TrialDesign.d PopulationParameter.d IndividualParameter.d ObservationModel.d FunctionCall.d IndependentVariable.d Correlation.d RandomVariable.d Distribution.d DistributionParameter.d DerivativeVariable.d VariabilityLevel.d Dataset.d ExternalDataset.d ColumnMapping.d Interval.d XMLAstVisitor.d StringVisitor.d Symbol.d SymbolSet.d SymbRefFinder.d AstAnalyzer.d xml.d TextFormatter.d Consolidator.d CPopulationParameters.d CCovariates.d CVariabilityModels.d RSymbols.d Logger.d PharmMLSection.d Object.d AstTransformation.d SymbolNameVisitor.d AstBuilder.d SymbolSortVisitor.d AstParenthesizer.o StringTools.d SymbolNamer.d Block.d SymbolGathering.d MacroGathering.d Document.d XPathContext.d PharmMLReader.d Version.d PharmMLWriter.d


Version.o: src/helpers/Version.cpp src/helpers/Version.h
	$(CC) -c src/helpers/Version.cpp -oVersion.o $(CFLAGS)

PharmMLWriter.o: src/PharmML/PharmMLWriter.cpp src/PharmML/PharmMLWriter.h
	$(CC) -c src/PharmML/PharmMLWriter.cpp -oPharmMLWriter.o $(CFLAGS)

PharmMLReader.o: src/PharmML/PharmMLReader.cpp src/PharmML/PharmMLReader.h
	$(CC) -c src/PharmML/PharmMLReader.cpp -oPharmMLReader.o $(CFLAGS)

Document.o: src/xml/Document.cpp src/xml/Document.h
	$(CC) -c src/xml/Document.cpp -oDocument.o $(CFLAGS)

XPathContext.o: src/xml/XPathContext.cpp src/xml/XPathContext.h
	$(CC) -c src/xml/XPathContext.cpp -oXPathContext.o $(CFLAGS)

SymbolGathering.o: src/symbols/SymbolGathering.cpp src/symbols/SymbolGathering.h
	$(CC) -c src/symbols/SymbolGathering.cpp -oSymbolGathering.o $(CFLAGS)

MacroGathering.o: src/symbols/MacroGathering.cpp src/symbols/MacroGathering.h
	$(CC) -c src/symbols/MacroGathering.cpp -oMacroGathering.o $(CFLAGS)

Block.o: src/PharmML/Block.cpp src/PharmML/Block.h
	$(CC) -c src/PharmML/Block.cpp -oBlock.o $(CFLAGS)

SymbolSortVisitor.o: src/visitors/SymbolSortVisitor.cpp src/visitors/SymbolSortVisitor.h
	$(CC) -c src/visitors/SymbolSortVisitor.cpp -oSymbolSortVisitor.o $(CFLAGS)

AstBuilder.o: src/AST/AstBuilder.cpp src/AST/AstBuilder.h
	$(CC) -c src/AST/AstBuilder.cpp -oAstBuilder.o $(CFLAGS)

SymbolNameVisitor.o: src/visitors/SymbolNameVisitor.cpp src/visitors/SymbolNameVisitor.h
	$(CC) -c src/visitors/SymbolNameVisitor.cpp -oSymbolNameVisitor.o $(CFLAGS)

AstTransformation.o: src/AST/AstTransformation.cpp src/AST/AstTransformation.h
	$(CC) -c src/AST/AstTransformation.cpp -oAstTransformation.o $(CFLAGS)

Object.o: src/objects/Object.cpp src/objects/Object.h
	$(CC) -c src/objects/Object.cpp -oObject.o $(CFLAGS)

PharmMLSection.o: src/PharmML/PharmMLSection.cpp src/PharmML/PharmMLSection.h
	$(CC) -c src/PharmML/PharmMLSection.cpp -oPharmMLSection.o $(CFLAGS)

Logger.o: src/helpers/Logger.cpp src/helpers/Logger.h
	$(CC) -c src/helpers/Logger.cpp -oLogger.o $(CFLAGS)

CPopulationParameters.o: src/consolidators/PopulationParameters.cpp src/consolidators/PopulationParameters.h
	$(CC) -c src/consolidators/PopulationParameters.cpp -oCPopulationParameters.o $(CFLAGS)

CCovariates.o: src/consolidators/Covariates.cpp src/consolidators/Covariates.h
	$(CC) -c src/consolidators/Covariates.cpp -oCCovariates.o $(CFLAGS)

CVariabilityModels.o: src/consolidators/VariabilityModels.cpp src/consolidators/VariabilityModels.h
	$(CC) -c src/consolidators/VariabilityModels.cpp -oCVariabilityModels.o $(CFLAGS)

RSymbols.o: src/generators/R/RSymbols.cpp src/generators/R/RSymbols.h
	$(CC) -c src/generators/R/RSymbols.cpp -oRSymbols.o $(CFLAGS)

Consolidator.o: src/consolidators/Consolidator.cpp src/consolidators/Consolidator.h
	$(CC) -c src/consolidators/Consolidator.cpp -oConsolidator.o $(CFLAGS)

SymbRefFinder.o: src/visitors/SymbRefFinder.cpp src/visitors/SymbRefFinder.h
	$(CC) -c src/visitors/SymbRefFinder.cpp -oSymbRefFinder.o $(CFLAGS)

AstAnalyzer.o: src/visitors/AstAnalyzer.cpp src/visitors/AstAnalyzer.h
	$(CC) -c src/visitors/AstAnalyzer.cpp -oAstAnalyzer.o $(CFLAGS)

SymbolSet.o: src/symbols/SymbolSet.cpp src/symbols/SymbolSet.h
	$(CC) -c src/symbols/SymbolSet.cpp -oSymbolSet.o $(CFLAGS)

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

Correlation.o: src/PharmML/Correlation.cpp src/PharmML/Correlation.h
	$(CC) -c src/PharmML/Correlation.cpp -oCorrelation.o $(CFLAGS)

VariabilityLevel.o: src/symbols/VariabilityLevel.cpp src/symbols/VariabilityLevel.h
	$(CC) -c src/symbols/VariabilityLevel.cpp -oVariabilityLevel.o $(CFLAGS)

FunctionCall.o: src/AST/FunctionCall.cpp src/AST/FunctionCall.h
	$(CC) -c src/AST/FunctionCall.cpp -oFunctionCall.o $(CFLAGS)

ObservationModel.o: src/symbols/ObservationModel.cpp src/symbols/ObservationModel.h
	$(CC) -c src/symbols/ObservationModel.cpp -oObservationModel.o $(CFLAGS)

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

FunctionDefinition.o: src/symbols/FunctionDefinition.cpp src/symbols/FunctionDefinition.h
	$(CC) -c src/symbols/FunctionDefinition.cpp -oFunctionDefinition.o $(CFLAGS)

Vector.o: src/AST/Vector.cpp src/AST/Vector.h
	$(CC) -c src/AST/Vector.cpp -oVector.o $(CFLAGS)

Piecewise.o: src/AST/Piecewise.cpp src/AST/Piecewise.h
	$(CC) -c src/AST/Piecewise.cpp -oPiecewise.o $(CFLAGS)

StructuralModel.o: src/PharmML/StructuralModel.cpp src/PharmML/StructuralModel.h
	$(CC) -c src/PharmML/StructuralModel.cpp -oStructuralModel.o $(CFLAGS)

Variable.o: src/symbols/Variable.cpp src/symbols/Variable.h
	$(CC) -c src/symbols/Variable.cpp -oVariable.o $(CFLAGS)

PKMacro.o: src/PharmML/PKMacro.cpp src/PharmML/PKMacro.h
	$(CC) -c src/PharmML/PKMacro.cpp -oPKMacro.o $(CFLAGS)

RPharmMLGenerator.o: src/generators/R/RPharmMLGenerator.cpp src/generators/R/RPharmMLGenerator.h
	$(CC) -c src/generators/R/RPharmMLGenerator.cpp -oRPharmMLGenerator.o $(CFLAGS)

RPharmMLConsolidator.o: src/generators/R/RPharmMLConsolidator.cpp src/generators/R/RPharmMLConsolidator.h
	$(CC) -c src/generators/R/RPharmMLConsolidator.cpp -oRPharmMLConsolidator.o $(CFLAGS)

RAstGenerator.o: src/generators/R/RAstGenerator.cpp src/generators/R/RAstGenerator.h
	$(CC) -c src/generators/R/RAstGenerator.cpp -oRAstGenerator.o $(CFLAGS)

MDLGenerator.o: src/generators/MDL/MDLGenerator.cpp src/generators/MDL/MDLGenerator.h
	$(CC) -c src/generators/MDL/MDLGenerator.cpp -oMDLGenerator.o $(CFLAGS)

MDLAstGenerator.o: src/generators/MDL/MDLAstGenerator.cpp src/generators/MDL/MDLAstGenerator.h
	$(CC) -c src/generators/MDL/MDLAstGenerator.cpp -oMDLAstGenerator.o $(CFLAGS)

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

Covariate.o: src/symbols/Covariate.cpp src/symbols/Covariate.h
	$(CC) -c src/symbols/Covariate.cpp -oCovariate.o $(CFLAGS)

VariabilityModel.o: src/PharmML/VariabilityModel.cpp src/PharmML/VariabilityModel.h
	$(CC) -c src/PharmML/VariabilityModel.cpp -oVariabilityModel.o $(CFLAGS)

CovariateModel.o: src/PharmML/CovariateModel.cpp src/PharmML/CovariateModel.h
	$(CC) -c src/PharmML/CovariateModel.cpp -oCovariateModel.o $(CFLAGS)

ModelDefinition.o: src/PharmML/ModelDefinition.cpp src/PharmML/ModelDefinition.h
	$(CC) -c src/PharmML/ModelDefinition.cpp -oModelDefinition.o $(CFLAGS)

ModellingSteps.o: src/PharmML/ModellingSteps.cpp src/PharmML/ModellingSteps.h
	$(CC) -c src/PharmML/ModellingSteps.cpp -oModellingSteps.o $(CFLAGS)

PharmMLContext.o: src/PharmML/PharmMLContext.cpp src/PharmML/PharmMLContext.h
	$(CC) -c src/PharmML/PharmMLContext.cpp -oPharmMLContext.o $(CFLAGS)

PharmML.o: src/PharmML/PharmML.cpp src/PharmML/PharmML.h
	$(CC) -c src/PharmML/PharmML.cpp -oPharmML.o $(CFLAGS)

xml.o: src/xml/xml.cpp src/xml/xml.h
	$(CC) -c src/xml/xml.cpp -oxml.o $(CFLAGS)

AstParenthesizer.o: src/visitors/AstParenthesizer.cpp src/visitors/AstParenthesizer.h
	$(CC) -c src/visitors/AstParenthesizer.cpp -oAstParenthesizer.o $(CFLAGS)

TextFormatter.o: src/generators/TextFormatter.cpp src/generators/TextFormatter.h
	$(CC) -c src/generators/TextFormatter.cpp -oTextFormatter.o $(CFLAGS)

StringTools.o: src/helpers/StringTools.cpp src/helpers/StringTools.h
	$(CC) -c src/helpers/StringTools.cpp -oStringTools.o $(CFLAGS)

SymbolNamer.o: src/helpers/SymbolNamer.cpp src/helpers/SymbolNamer.h
	$(CC) -c src/helpers/SymbolNamer.cpp -oSymbolNamer.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f pharmml2poped
	rm -f pharmml2poped.exe
	rm -f mdl
	rm -f libpharmmlc.a
	rm -f libpharmmlc.d.a
	rm -f main
	rm -f output
	rm -f main.exe



.PHONY: release
release:
	mkdir -p release
	cp pharmml2poped.exe release
	cp windep/lib/*.dll release
	cp -r pharmml_internalRelease_0_8_1 release


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
	cd windep/libxml2-2.9.3; ./configure --host=x86_64-w64-mingw32 --without-python --without-docbook --without-ftp --without-http --without-schematron --with-lzma=no --with-zlib=.. --with-iconv=..; make 
#	--without-html --without-legacy --without-regexps --without-sax1 --without-schemas --without-valid --without-xpath 
	cp -r windep/libxml2-2.9.3/include/libxml windep/include
	cp windep/libxml2-2.9.3/.libs/libxml2-2.dll windep/lib
	cp windep/libxml2-2.9.3/.libs/libxml2.dll.a windep/lib
