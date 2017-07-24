ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libpharmmlcpp.la
libpharmmlcpp_la_SOURCES = \
   	AST/AstBuilder.cpp \
   	AST/AstNode.cpp \
	AST/AstTransformation.cpp \
   	AST/Binop.cpp \
   	AST/Constant.cpp \
   	AST/FunctionCall.cpp \
   	AST/Interval.cpp \
   	AST/Piecewise.cpp \
   	AST/Scalar.cpp \
   	AST/symbols.cpp \
   	AST/Uniop.cpp \
   	AST/Vector.cpp \
	AST/Sequence.cpp \
	AST/MatrixVectorIndex.cpp \
	AST/MatrixSelector.cpp \
	consolidators/Consolidator.cpp \
   	consolidators/Covariates.cpp \
   	consolidators/PopulationParameters.cpp \
	consolidators/VariabilityModels.cpp \
   	generators/TextFormatter.cpp \
	helpers/Logger.cpp \
	helpers/StringTools.cpp \
	helpers/SymbolNamer.cpp \
	helpers/Version.cpp \
	objects/Object.cpp \
	PharmML/Arms.cpp \
	PharmML/Block.cpp \
	PharmML/ColumnMapping.cpp \
	PharmML/Correlation.cpp \
	PharmML/CovariateModel.cpp \
	PharmML/Covariates.cpp \
	PharmML/Dataset.cpp \
	PharmML/DesignSpaces.cpp \
	PharmML/Distribution.cpp \
	PharmML/DistributionParameter.cpp \
	PharmML/ExternalDataset.cpp \
	PharmML/Interventions.cpp \
	PharmML/ModelDefinition.cpp \
	PharmML/ModellingSteps.cpp \
	PharmML/MultipleDVMapping.cpp \
	PharmML/Observations.cpp \
	PharmML/ParameterModel.cpp \
	PharmML/PharmML.cpp \
	PharmML/PharmMLReader.cpp \
	PharmML/PharmMLSection.cpp \
	PharmML/PharmMLWriter.cpp \
	PharmML/PKMacro.cpp \
	PharmML/StructuralModel.cpp \
	PharmML/TDCovariateModel.cpp \
	PharmML/TrialDesign.cpp \
	PharmML/VariabilityModel.cpp \
	symbols/Category.cpp \
	symbols/Covariate.cpp \
	symbols/DerivativeVariable.cpp \
	symbols/DiscreteVariable.cpp \
	symbols/FunctionDefinition.cpp \
	symbols/IndependentVariable.cpp \
	symbols/IndividualParameter.cpp \
	symbols/MacroGathering.cpp \
	symbols/ObservationModel.cpp \
	symbols/PopulationParameter.cpp \
	symbols/RandomVariable.cpp \
	symbols/Symbol.cpp \
	symbols/SymbolGathering.cpp \
	symbols/SymbolSet.cpp \
	symbols/VariabilityLevel.cpp \
	symbols/Variable.cpp \
	visitors/AstAnalyzer.cpp \
	visitors/AstParenthesizer.cpp \
	visitors/StringVisitor.cpp \
	visitors/SymbolNameVisitor.cpp \
	visitors/SymbolSortVisitor.cpp \
	visitors/SymbRefFinder.cpp \
	visitors/XMLAstVisitor.cpp \
	xml/Document.cpp \
	xml/xml.cpp \
	xml/XPathContext.cpp


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
