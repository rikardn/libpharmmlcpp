#ifndef PHARMML_RGENERATOR_H_
#define PHARMML_RGENERATOR_H_

#include <string>
#include "AbstractVisitor.h"
#include "symbols.h"
#include "Uniop.h"
#include "Binop.h"
#include "Scalar.h"
#include "Piecewise.h"
#include "FunctionDefinition.h"
#include "IndividualParameter.h"
#include "Covariate.h"
#include "Variable.h"
#include "FunctionCall.h"

namespace PharmML
{
    class RGenerator : public AbstractVisitor
    {
        public:
            virtual std::string visit(SymbRef *node);
            virtual std::string visit(UniopLog *node);
            virtual std::string visit(UniopLog2 *node);
            virtual std::string visit(UniopLog10 *node);
            virtual std::string visit(UniopExp *node);
            virtual std::string visit(UniopMinus *node);
            virtual std::string visit(UniopAbs *node);
            virtual std::string visit(UniopSqrt *node);
            virtual std::string visit(UniopLogistic *node);
            virtual std::string visit(UniopLogit *node);
            virtual std::string visit(UniopProbit *node);
            virtual std::string visit(UniopNormcdf *node);
            virtual std::string visit(UniopFactorial *node);
            virtual std::string visit(UniopFactln *node);
            virtual std::string visit(UniopGamma *node);
            virtual std::string visit(UniopGammaln *node);
            virtual std::string visit(UniopSin *node);
            virtual std::string visit(UniopSinh *node);
            virtual std::string visit(UniopCos *node);
            virtual std::string visit(UniopCosh *node);
            virtual std::string visit(UniopTan *node);
            virtual std::string visit(UniopTanh *node);
            virtual std::string visit(UniopCot *node);
            virtual std::string visit(UniopCoth *node);
            virtual std::string visit(UniopSec *node);
            virtual std::string visit(UniopSech *node);
            virtual std::string visit(UniopCsc *node);
            virtual std::string visit(UniopCsch *node);
            virtual std::string visit(UniopArcsin *node);
            virtual std::string visit(UniopArcsinh *node);
            virtual std::string visit(UniopArccos *node);
            virtual std::string visit(UniopArccosh *node);
            virtual std::string visit(UniopArctan *node);
            virtual std::string visit(UniopArctanh *node);
            virtual std::string visit(UniopArccot *node);
            virtual std::string visit(UniopArccoth *node);
            virtual std::string visit(UniopArcsec *node);
            virtual std::string visit(UniopArcsech *node);
            virtual std::string visit(UniopArccsc *node);
            virtual std::string visit(UniopArccsch *node);
            virtual std::string visit(UniopHeaviside *node);
            virtual std::string visit(UniopSign *node);
            virtual std::string visit(UniopFloor *node);
            virtual std::string visit(UniopCeiling *node);
            virtual std::string visit(ScalarInt *node);
            virtual std::string visit(BinopPlus *node);
            virtual std::string visit(BinopMinus *node);
            virtual std::string visit(BinopDivide *node);
            virtual std::string visit(BinopTimes *node);
            virtual std::string visit(LogicBinopLt *node);
            virtual std::string visit(LogicBinopLeq *node);
            virtual std::string visit(Piecewise *node);
            virtual std::string visit(Piece *node);
            virtual std::string visit(FunctionDefinition *node);
            virtual std::string visit(IndividualParameter *node);
            virtual std::string visit(Covariate *node);
            virtual std::string visit(Variable *node);
            virtual std::string visit(FunctionCall *node);
            virtual std::string visit(FunctionArgument *node);
    };
}

#endif
