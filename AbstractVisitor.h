#ifndef PHARMML_ABSTRACTVISITOR_H_
#define PHARMML_ABSTRACTVISITOR_H_

namespace PharmML
{
    class UniopLog;
    class UniopLog2;
    class UniopLog10;
    class UniopExp;
    class UniopMinus;
    class UniopAbs;
    class UniopSqrt;
    class UniopLogistic;
    class UniopLogit;
    class UniopProbit;
    class UniopNormcdf;
    class UniopFactorial;
    class UniopFactln;
    class UniopGamma;
    class UniopGammaln;
    class UniopSin;
    class UniopSinh;
    class UniopCos;
    class UniopCosh;
    class UniopTan;
    class UniopTanh;
    class UniopCot;
    class UniopCoth;
    class UniopSec;
    class UniopSech;
    class UniopCsc;
    class UniopCsch;
    class UniopArcsin;
    class UniopArcsinh;
    class UniopArccos;
    class UniopArccosh;
    class UniopArctan;
    class UniopArctanh;
    class UniopArccot;
    class UniopArccoth;
    class UniopArcsec;
    class UniopArcsech;
    class UniopArccsc;
    class UniopArccsch;
    class UniopHeaviside;
    class UniopSign;
    class UniopFloor;
    class UniopCeiling;
    
    class LogicUniopIsdefined;
    class LogicUniopNot;
    
    class ScalarInt;
    class ScalarReal;
    
    class BinopPlus;
    class BinopMinus;
    class BinopDivide;
    class BinopTimes;
    class BinopPower;
    class BinopLogx;
    class BinopRoot;
    class BinopMin;
    class BinopMax;
    class BinopRem;
    class BinopAtan2;
    
    class LogicBinopLt;
    class LogicBinopLeq;
    class LogicBinopGt;
    class LogicBinopGeq;
    class LogicBinopEq;
    class LogicBinopNeq;
    class LogicBinopAnd;
    class LogicBinopOr;
    class LogicBinopXor;
    
    class SymbRef;
    class TargetMapping;
    
    class LogicFalse;
    class LogicTrue;
    class Pi;
    class Exponentiale;
    class NullValue;
    
    class Piecewise;
    class Piece;
    
    class FunctionCall;
    class FunctionArgument;
    
    class FunctionDefinition;
    class PopulationParameter;
    class IndividualParameter;
    class RandomVariable;
    class Variable;
    class DerivativeVariable;
    class IndependentVariable;
    class Covariate;
    class ObservationModel;
    class Distribution;

    class AbstractVisitor
    {
        public:
            virtual std::string visit(UniopLog *node) = 0;
            virtual std::string visit(UniopLog2 *node) = 0;
            virtual std::string visit(UniopLog10 *node) = 0;
            virtual std::string visit(UniopExp *node) = 0;
            virtual std::string visit(UniopMinus *node) = 0;
            virtual std::string visit(UniopAbs *node) = 0;
            virtual std::string visit(UniopSqrt *node) = 0;
            virtual std::string visit(UniopLogistic *node) = 0;
            virtual std::string visit(UniopLogit *node) = 0;
            virtual std::string visit(UniopProbit *node) = 0;
            virtual std::string visit(UniopNormcdf *node) = 0;
            virtual std::string visit(UniopFactorial *node) = 0;
            virtual std::string visit(UniopFactln *node) = 0;
            virtual std::string visit(UniopGamma *node) = 0;
            virtual std::string visit(UniopGammaln *node) = 0;
            virtual std::string visit(UniopSin *node) = 0;
            virtual std::string visit(UniopSinh *node) = 0;
            virtual std::string visit(UniopCos *node) = 0;
            virtual std::string visit(UniopCosh *node) = 0;
            virtual std::string visit(UniopTan *node) = 0;
            virtual std::string visit(UniopTanh *node) = 0;
            virtual std::string visit(UniopCot *node) = 0;
            virtual std::string visit(UniopCoth *node) = 0;
            virtual std::string visit(UniopSec *node) = 0;
            virtual std::string visit(UniopSech *node) = 0;
            virtual std::string visit(UniopCsc *node) = 0;
            virtual std::string visit(UniopCsch *node) = 0;
            virtual std::string visit(UniopArcsin *node) = 0;
            virtual std::string visit(UniopArcsinh *node) = 0;
            virtual std::string visit(UniopArccos *node) = 0;
            virtual std::string visit(UniopArccosh *node) = 0;
            virtual std::string visit(UniopArctan *node) = 0;
            virtual std::string visit(UniopArctanh *node) = 0;
            virtual std::string visit(UniopArccot *node) = 0;
            virtual std::string visit(UniopArccoth *node) = 0;
            virtual std::string visit(UniopArcsec *node) = 0;
            virtual std::string visit(UniopArcsech *node) = 0;
            virtual std::string visit(UniopArccsc *node) = 0;
            virtual std::string visit(UniopArccsch *node) = 0;
            virtual std::string visit(UniopHeaviside *node) = 0;
            virtual std::string visit(UniopSign *node) = 0;
            virtual std::string visit(UniopFloor *node) = 0;
            virtual std::string visit(UniopCeiling *node) = 0;
            
            virtual std::string visit(LogicUniopIsdefined *node) = 0;
            virtual std::string visit(LogicUniopNot *node) = 0;
            
            virtual std::string visit(ScalarInt *node) = 0;
            virtual std::string visit(ScalarReal *node) = 0;
            
            virtual std::string visit(BinopPlus *node) = 0;
            virtual std::string visit(BinopMinus *node) = 0;
            virtual std::string visit(BinopDivide *node) = 0;
            virtual std::string visit(BinopTimes *node) = 0;  
            virtual std::string visit(BinopPower *node) = 0;
            virtual std::string visit(BinopLogx *node) = 0;
            virtual std::string visit(BinopRoot *node) = 0;
            virtual std::string visit(BinopMin *node) = 0;
            virtual std::string visit(BinopMax *node) = 0;
            virtual std::string visit(BinopRem *node) = 0;
            virtual std::string visit(BinopAtan2 *node) = 0;
            
            virtual std::string visit(LogicBinopLt *node) = 0;
            virtual std::string visit(LogicBinopLeq *node) = 0;
            virtual std::string visit(LogicBinopGt *node) = 0;
            virtual std::string visit(LogicBinopGeq *node) = 0;
            virtual std::string visit(LogicBinopEq *node) = 0;
            virtual std::string visit(LogicBinopNeq *node) = 0;
            virtual std::string visit(LogicBinopAnd *node) = 0;
            virtual std::string visit(LogicBinopOr *node) = 0;
            virtual std::string visit(LogicBinopXor *node) = 0;
            
            virtual std::string visit(SymbRef *node) = 0;
            virtual std::string visit(TargetMapping *node) = 0;
            
            virtual std::string visit(LogicFalse *node) = 0;
            virtual std::string visit(LogicTrue *node) = 0;
            virtual std::string visit(Pi *node) = 0;
            virtual std::string visit(Exponentiale *node) = 0;
            virtual std::string visit(NullValue *node) = 0;
            
            virtual std::string visit(Piecewise *node) = 0;
            virtual std::string visit(Piece *node) = 0;
            
            virtual std::string visit(FunctionCall *node) = 0;
            virtual std::string visit(FunctionArgument *node) = 0;
            
            virtual std::string visit(FunctionDefinition *node) = 0;
            virtual std::string visit(PopulationParameter *node) = 0;
            virtual std::string visit(IndividualParameter *node) = 0;
            virtual std::string visit(RandomVariable *node) = 0;
            virtual std::string visit(Covariate *node) = 0;
            virtual std::string visit(IndependentVariable *node) = 0;
            virtual std::string visit(Variable *node) = 0;
            virtual std::string visit(DerivativeVariable *node) = 0;
            virtual std::string visit(ObservationModel *node) = 0;
            virtual std::string visit(Distribution *node) = 0;
    };
}

#endif
