#ifndef PHARMML_RGENERATOR_H_
#define PHARMML_RGENERATOR_H_

#include <string>
#include "AbstractVisitor.h"
#include "symbols.h"
#include "Uniop.h"
#include "Binop.h"
#include "Scalar.h"
#include "Piecewise.h"
#include "Covariate.h"
#include "Variable.h"

namespace PharmML
{
    class RGenerator : public AbstractVisitor
    {
        public:
            virtual std::string visit(SymbRef *node);
            virtual std::string visit(UniopLog *node);
            virtual std::string visit(UniopExp *node);
            virtual std::string visit(UniopMinus *node);
            virtual std::string visit(ScalarInt *node);
            virtual std::string visit(BinopPlus *node);
            virtual std::string visit(BinopMinus *node);
            virtual std::string visit(BinopDivide *node);
            virtual std::string visit(BinopTimes *node);
            virtual std::string visit(LogicBinopLt *node);
            virtual std::string visit(LogicBinopLeq *node);
            virtual std::string visit(Piecewise *node);
            virtual std::string visit(Piece *node);
            virtual std::string visit(Covariate *node);
            virtual std::string visit(Variable *node);
    };
}

#endif
