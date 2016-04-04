CFLAGS = -std=c++11 -g `xml2-config --cflags`
LIBS = -lxml2

main: main.o PharmMLContext.o Model.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Uniop.o Binop.o symbols.o Scalar.o RGenerator.o StructuralModel.o Variable.o Piecewise.o xml.o
	g++ main.o -omain PharmMLContext.o Model.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Uniop.o Binop.o symbols.o Scalar.o RGenerator.o StructuralModel.o Variable.o Piecewise.o xml.o $(CFLAGS) $(LIBS)

main.o: main.cpp
	g++ -c main.cpp -omain.o $(CFLAGS)

Piecewise.o: Piecewise.cpp Piecewise.h
	g++ -c Piecewise.cpp -oPiecewise.o $(CFLAGS)

StructuralModel.o: StructuralModel.cpp StructuralModel.h
	g++ -c StructuralModel.cpp -oStructuralModel.o $(CFLAGS)

Variable.o: Variable.cpp Variable.h
	g++ -c Variable.cpp -oVariable.o $(CFLAGS)

RGenerator.o: RGenerator.cpp RGenerator.h
	g++ -c RGenerator.cpp -oRGenerator.o $(CFLAGS)

symbols.o: symbols.cpp symbols.h
	g++ -c symbols.cpp -osymbols.o $(CFLAGS)

Scalar.o: Scalar.cpp Scalar.h
	g++ -c Scalar.cpp -oScalar.o $(CFLAGS)

Binop.o: Binop.cpp Binop.h
	g++ -c Binop.cpp -oBinop.o $(CFLAGS)

Uniop.o: Uniop.cpp Uniop.h
	g++ -c Uniop.cpp -oUniop.o $(CFLAGS)

AstNodeFactory.o: AstNodeFactory.cpp AstNodeFactory.h
	g++ -c AstNodeFactory.cpp -oAstNodeFactory.o $(CFLAGS)

Covariate.o: Covariate.cpp Covariate.h
	g++ -c Covariate.cpp -oCovariate.o $(CFLAGS)

CovariateModel.o: CovariateModel.cpp
	g++ -c CovariateModel.cpp -oCovariateModel.o $(CFLAGS)

ModelDefinition.o: ModelDefinition.cpp
	g++ -c ModelDefinition.cpp -oModelDefinition.o $(CFLAGS)

PharmMLContext.o: PharmMLContext.cpp
	g++ -c PharmMLContext.cpp -oPharmMLContext.o $(CFLAGS)

Model.o: Model.cpp
	g++ -c Model.cpp -oModel.o $(CFLAGS)

xml.o: xml.cpp
	g++ -c xml.cpp -oxml.o $(CFLAGS)
