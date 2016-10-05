AM_CPPFLAGS = -I./src $(LIBXML2_CFLAGS) -std=c++14		# Need proper check of C++14

noinst_LIBRARIES = libpharmmlcpp.a
libpharmmlcpp_a_SOURCES = \
   	src/AST/AstBuilder.cpp \
   	src/AST/AstNode.cpp \
	src/AST/AstTransformation.cpp \
   	src/AST/Binop.cpp src/AST/Constant.cpp \
   	src/AST/FunctionCall.cpp src/AST/Interval.cpp \
   	src/AST/Piecewise.cpp \
   	src/AST/Scalar.cpp \
   	src/AST/symbols.cpp \
   	src/AST/Uniop.cpp \
   	src/AST/Vector.cpp \
	src/AST/Sequence.cpp \
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
	src/PharmML/PharmMLContext.cpp \
	src/PharmML/PharmML.cpp \
	src/PharmML/PharmMLReader.cpp \
	src/PharmML/PharmMLSection.cpp \
	src/PharmML/PharmMLWriter.cpp \
	src/PharmML/PKMacro.cpp \
	src/PharmML/StructuralModel.cpp \
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


bin_PROGRAMS = pharmml2poped mdl

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
	src/generators/MDL/MDLSymbols.cpp

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
	test/FunctionCall.cpp \
	test/SymbolSet.cpp \
	test/SymbolNamer.cpp
unit_LDADD = libpharmmlcpp.a $(LIBXML2_LIBS)
