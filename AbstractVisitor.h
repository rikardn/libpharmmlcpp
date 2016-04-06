#ifndef PHARMML_ABSTRACTVISITOR_H_
#define PHARMML_ABSTRACTVISITOR_H_

namespace PharmML
{
    class UniopLog;
    class UniopExp;
    class UniopMinus;
    class UniopAbs;
    class ScalarInt;
    class BinopPlus;
    class BinopMinus;
    class BinopDivide;
    class BinopTimes;
    class SymbRef;
    class LogicBinopLt;
    class LogicBinopLeq;
    class Piecewise;
    class Piece;
    class FunctionDefinition;
    class IndividualParameter;
    class Variable;
    class Covariate;
    class FunctionCall;
    class FunctionArgument;

    class AbstractVisitor
    {
        public:
            virtual std::string visit(UniopLog *node) = 0;
            virtual std::string visit(UniopExp *node) = 0;
            virtual std::string visit(UniopMinus *node) = 0;
            virtual std::string visit(UniopAbs *node) = 0;
            virtual std::string visit(ScalarInt *node) = 0;
            virtual std::string visit(BinopPlus *node) = 0;
            virtual std::string visit(BinopMinus *node) = 0;
            virtual std::string visit(BinopDivide *node) = 0;
            virtual std::string visit(BinopTimes *node) = 0;
            virtual std::string visit(SymbRef *node) = 0;
            virtual std::string visit(LogicBinopLt *node) = 0;
            virtual std::string visit(LogicBinopLeq *node) = 0;
            virtual std::string visit(Piecewise *node) = 0;
            virtual std::string visit(Piece *node) = 0;
            virtual std::string visit(FunctionDefinition *node) = 0;
            virtual std::string visit(IndividualParameter *node) = 0;
            virtual std::string visit(Covariate *node) = 0;
            virtual std::string visit(Variable *node) = 0;
            virtual std::string visit(FunctionCall *node) = 0;
            virtual std::string visit(FunctionArgument *node) = 0;
    };
}

#endif
