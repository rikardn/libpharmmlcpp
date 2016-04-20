TARGET = UNIX

LIBS = -lxml2

ifeq ($(TARGET), UNIX)
  CC = g++
  CFLAGS = -std=c++11 -g `xml2-config --cflags`
  OUTPUT = main
else
  CC = x86_64-w64-mingw32-g++
  CFLAGS = -std=c++11 -static-libgcc -static-libstdc++ -L./windep/libs -I./windep/include
  OUTPUT = main.exe
endif

main: main.o libpharmmlc.a
	$(CC) main.o -o$(OUTPUT) -lpharmmlc $(CFLAGS) -L. $(LIBS)

libpharmmlc.a: PharmMLContext.o Model.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RGenerator.o StructuralModel.o Variable.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o Dependencies.o ExternalDataset.o ColumnMapping.o Interval.o xml.o
	ar -rcs libpharmmlc.a PharmMLContext.o Model.o ModelDefinition.o CovariateModel.o Covariate.o AstNodeFactory.o Constant.o Uniop.o Binop.o symbols.o Scalar.o RGenerator.o StructuralModel.o Variable.o Vector.o Piecewise.o FunctionDefinition.o ParameterModel.o Interventions.o Observations.o Arms.o DesignSpaces.o TrialDesign.o PopulationParameter.o IndividualParameter.o ObservationModel.o FunctionCall.o IndependentVariable.o RandomVariable.o Distribution.o DistributionParameter.o DerivativeVariable.o Dependencies.o ExternalDataset.o ColumnMapping.o Interval.o xml.o

main.o: main.cpp
	$(CC) -c main.cpp -omain.o $(CFLAGS)



Interval.o: Interval.cpp Interval.h
	$(CC) -c Interval.cpp -oInterval.o $(CFLAGS)

ColumnMapping.o: ColumnMapping.cpp ColumnMapping.h
	$(CC) -c ColumnMapping.cpp -oColumnMapping.o $(CFLAGS)

ExternalDataset.o: ExternalDataset.cpp ExternalDataset.h
	$(CC) -c ExternalDataset.cpp -oExternalDataset.o $(CFLAGS)

Dependencies.o: Dependencies.cpp Dependencies.h
	$(CC) -c Dependencies.cpp -oDependencies.o $(CFLAGS)

DerivativeVariable.o: DerivativeVariable.cpp DerivativeVariable.h
	$(CC) -c DerivativeVariable.cpp -oDerivativeVariable.o $(CFLAGS)

DistributionParameter.o: DistributionParameter.cpp DistributionParameter.h
	$(CC) -c DistributionParameter.cpp -oDistributionParameter.o $(CFLAGS)

Distribution.o: Distribution.cpp Distribution.h
	$(CC) -c Distribution.cpp -oDistribution.o $(CFLAGS)

RandomVariable.o: RandomVariable.cpp RandomVariable.h
	$(CC) -c RandomVariable.cpp -oRandomVariable.o $(CFLAGS)

IndependentVariable.o: IndependentVariable.cpp IndependentVariable.h
	$(CC) -c IndependentVariable.cpp -oIndependentVariable.o $(CFLAGS)

FunctionCall.o: FunctionCall.cpp FunctionCall.h
	$(CC) -c FunctionCall.cpp -oFunctionCall.o $(CFLAGS)

ObservationModel.o: ObservationModel.cpp ObservationModel.h
	$(CC) -c ObservationModel.cpp -oObservationModel.o $(CFLAGS)

PopulationParameter.o: PopulationParameter.cpp PopulationParameter.h
	$(CC) -c PopulationParameter.cpp -oPopulationParameter.o $(CFLAGS)

IndividualParameter.o: IndividualParameter.cpp IndividualParameter.h
	$(CC) -c IndividualParameter.cpp -oIndividualParameter.o $(CFLAGS)

ParameterModel.o: ParameterModel.cpp ParameterModel.h
	$(CC) -c ParameterModel.cpp -oParameterModel.o $(CFLAGS)

Interventions.o: Interventions.cpp Interventions.h
	$(CC) -c Interventions.cpp -oInterventions.o $(CFLAGS)

Observations.o: Observations.cpp Observations.h
	$(CC) -c Observations.cpp -oObservations.o $(CFLAGS)

Arms.o: Arms.cpp Arms.h
	$(CC) -c Arms.cpp -oArms.o $(CFLAGS)

DesignSpaces.o: DesignSpaces.cpp DesignSpaces.h
	$(CC) -c DesignSpaces.cpp -oDesignSpaces.o $(CFLAGS)

TrialDesign.o: TrialDesign.cpp TrialDesign.h
	$(CC) -c TrialDesign.cpp -oTrialDesign.o $(CFLAGS)

FunctionDefinition.o: FunctionDefinition.cpp FunctionDefinition.h
	$(CC) -c FunctionDefinition.cpp -oFunctionDefinition.o $(CFLAGS)

Vector.o: Vector.cpp Vector.h
	$(CC) -c Vector.cpp -oVector.o $(CFLAGS)

Piecewise.o: Piecewise.cpp Piecewise.h
	$(CC) -c Piecewise.cpp -oPiecewise.o $(CFLAGS)

StructuralModel.o: StructuralModel.cpp StructuralModel.h
	$(CC) -c StructuralModel.cpp -oStructuralModel.o $(CFLAGS)

Variable.o: Variable.cpp Variable.h
	$(CC) -c Variable.cpp -oVariable.o $(CFLAGS)

RGenerator.o: RGenerator.cpp RGenerator.h
	$(CC) -c RGenerator.cpp -oRGenerator.o $(CFLAGS)

symbols.o: symbols.cpp symbols.h
	$(CC) -c symbols.cpp -osymbols.o $(CFLAGS)

Scalar.o: Scalar.cpp Scalar.h
	$(CC) -c Scalar.cpp -oScalar.o $(CFLAGS)

Binop.o: Binop.cpp Binop.h
	$(CC) -c Binop.cpp -oBinop.o $(CFLAGS)

Uniop.o: Uniop.cpp Uniop.h
	$(CC) -c Uniop.cpp -oUniop.o $(CFLAGS)

Constant.o: Constant.cpp Constant.h
	$(CC) -c Constant.cpp -oConstant.o $(CFLAGS)

AstNodeFactory.o: AstNodeFactory.cpp AstNodeFactory.h
	$(CC) -c AstNodeFactory.cpp -oAstNodeFactory.o $(CFLAGS)

Covariate.o: Covariate.cpp Covariate.h
	$(CC) -c Covariate.cpp -oCovariate.o $(CFLAGS)

CovariateModel.o: CovariateModel.cpp CovariateModel.h
	$(CC) -c CovariateModel.cpp -oCovariateModel.o $(CFLAGS)

ModelDefinition.o: ModelDefinition.cpp ModelDefinition.h
	$(CC) -c ModelDefinition.cpp -oModelDefinition.o $(CFLAGS)

PharmMLContext.o: PharmMLContext.cpp PharmMLContext.h
	$(CC) -c PharmMLContext.cpp -oPharmMLContext.o $(CFLAGS)

Model.o: Model.cpp Model.h
	$(CC) -c Model.cpp -oModel.o $(CFLAGS)

xml.o: xml.cpp xml.h
	$(CC) -c xml.cpp -oxml.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f libpharmmlc.a
	rm -f main
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
