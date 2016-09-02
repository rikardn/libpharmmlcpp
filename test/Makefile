CXX = g++
CXXFLAGS = -std=c++14 -g `xml2-config --cflags`
LIBS = -lxml2

#../libpharmmlc.a: ../libpharmmlc.d.a
#	make -C .. libpharmmlc.a

#~ test: test.cpp Scalar.cpp SymbolSet.cpp SymbolNamer.cpp ModellingSteps.cpp AstParenthesizer.cpp ../libpharmmlc.a
#~ 	$(CXX) $(CXXFLAGS) test.cpp Scalar.cpp SymbolSet.cpp SymbolNamer.cpp ModellingSteps.cpp AstParenthesizer.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest

#test: test.cpp SymbolSet.cpp SymbolNamer.cpp AstParenthesizer.cpp ../libpharmmlc.a
#	$(CXX) $(CXXFLAGS) test.cpp SymbolSet.cpp SymbolNamer.cpp AstParenthesizer.cpp -L.. -I../src -lpharmmlc $(LIBS) -otest


test: test.cpp IndependentVariable.cpp symbols.cpp Scalar.cpp Uniop.cpp Binop.cpp Interval.cpp FunctionCall.cpp SymbolSet.cpp SymbolNamer.cpp
	$(CXX) $(CXXFLAGS) test.cpp symbols.cpp IndependentVariable.cpp Scalar.cpp Uniop.cpp Binop.cpp Interval.cpp FunctionCall.cpp SymbolSet.cpp SymbolNamer.cpp -L.. -I../src -lpharmmlcpp $(LIBS) -otest

.PHONY: check
check: test
	./test

.PHONY: clean
clean:
	rm -f test
