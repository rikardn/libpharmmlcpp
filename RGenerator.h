/* libpharmmlcpp - Library to handle PharmML
 * Copyright (C) 2016 Rikard Nordgren and Gunnar Yngman
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * his library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHARMML_RGENERATOR_H_
#define PHARMML_RGENERATOR_H_

#include <string>
#include "AbstractVisitor.h"
#include "symbols.h"
#include "Uniop.h"
#include "Binop.h"
#include "Scalar.h"
#include "Constant.h"
#include "Vector.h"
#include "Piecewise.h"
#include "FunctionDefinition.h"
#include "PopulationParameter.h"
#include "IndividualParameter.h"
#include "Covariate.h"
#include "Variable.h"
#include "DerivativeVariable.h"
#include "FunctionCall.h"
#include "Interval.h"
#include "ObservationModel.h"
#include "IndependentVariable.h"
#include "RandomVariable.h"
#include "Distribution.h"
#include "ColumnMapping.h"
#include "Interventions.h"
#include "Observations.h"
#include "Arms.h"
#include "DesignSpaces.h"

namespace PharmML
{
    class RGenerator : public AbstractVisitor
    {
        private:
            std::string value;
            void setValue(std::string str);
            std::string acceptLeft(Binop *binop);
            std::string acceptRight(Binop *binop);
            std::string infix(Binop *binop, std::string op); 
            std::string acceptChild(Uniop *uniop);
            std::string accept(AstNode *node);
            // Helper function to reduce redundant code
            std::string formatVector(std::vector<std::string> vector, std::string prefix, std::string quote = "'");
            std::string getLogicLiteral(bool value);

        public:
            std::string getValue();

            virtual void visit(SymbRef *node);
            virtual void visit(SteadyStateParameter *node);
            virtual void visit(ColumnRef *node);
            virtual void visit(TargetMapping *node);
            
            virtual void visit(UniopLog *node);
            virtual void visit(UniopLog2 *node);
            virtual void visit(UniopLog10 *node);
            virtual void visit(UniopExp *node);
            virtual void visit(UniopMinus *node);
            virtual void visit(UniopAbs *node);
            virtual void visit(UniopSqrt *node);
            virtual void visit(UniopLogistic *node);
            virtual void visit(UniopLogit *node);
            virtual void visit(UniopProbit *node);
            virtual void visit(UniopNormcdf *node);
            virtual void visit(UniopFactorial *node);
            virtual void visit(UniopFactln *node);
            virtual void visit(UniopGamma *node);
            virtual void visit(UniopGammaln *node);
            virtual void visit(UniopSin *node);
            virtual void visit(UniopSinh *node);
            virtual void visit(UniopCos *node);
            virtual void visit(UniopCosh *node);
            virtual void visit(UniopTan *node);
            virtual void visit(UniopTanh *node);
            virtual void visit(UniopCot *node);
            virtual void visit(UniopCoth *node);
            virtual void visit(UniopSec *node);
            virtual void visit(UniopSech *node);
            virtual void visit(UniopCsc *node);
            virtual void visit(UniopCsch *node);
            virtual void visit(UniopArcsin *node);
            virtual void visit(UniopArcsinh *node);
            virtual void visit(UniopArccos *node);
            virtual void visit(UniopArccosh *node);
            virtual void visit(UniopArctan *node);
            virtual void visit(UniopArctanh *node);
            virtual void visit(UniopArccot *node);
            virtual void visit(UniopArccoth *node);
            virtual void visit(UniopArcsec *node);
            virtual void visit(UniopArcsech *node);
            virtual void visit(UniopArccsc *node);
            virtual void visit(UniopArccsch *node);
            virtual void visit(UniopHeaviside *node);
            virtual void visit(UniopSign *node);
            virtual void visit(UniopFloor *node);
            virtual void visit(UniopCeiling *node);
            
            virtual void visit(ScalarInt *node);
            virtual void visit(ScalarReal *node);
            
            virtual void visit(BinopPlus *node);
            virtual void visit(BinopMinus *node);
            virtual void visit(BinopDivide *node);
            virtual void visit(BinopTimes *node);
            virtual void visit(BinopPower *node);
            virtual void visit(BinopLogx *node);
            virtual void visit(BinopRoot *node);
            virtual void visit(BinopMin *node);
            virtual void visit(BinopMax *node);
            virtual void visit(BinopRem *node);
            virtual void visit(BinopAtan2 *node);
            
            virtual void visit(LogicFalse *node);
            virtual void visit(LogicTrue *node);
            virtual void visit(Pi *node);
            virtual void visit(Exponentiale *node);
            virtual void visit(NullValue *node);
            
            virtual void visit(LogicUniopIsdefined *node);
            virtual void visit(LogicUniopNot *node);
            
            virtual void visit(LogicBinopLt *node);
            virtual void visit(LogicBinopLeq *node);
            virtual void visit(LogicBinopGt *node);
            virtual void visit(LogicBinopGeq *node);
            virtual void visit(LogicBinopEq *node);
            virtual void visit(LogicBinopNeq *node);
            virtual void visit(LogicBinopAnd *node);
            virtual void visit(LogicBinopOr *node);
            virtual void visit(LogicBinopXor *node);
            
            virtual void visit(Vector *node);
            
            virtual void visit(Piecewise *node);
            virtual void visit(Piece *node);
            
            virtual void visit(FunctionCall *node);
            virtual void visit(FunctionArgument *node);
            virtual void visit(Interval *node);
            
            virtual void visit(FunctionDefinition *node);
            virtual void visit(PopulationParameter *node);
            virtual void visit(IndividualParameter *node);
            virtual void visit(RandomVariable *node);
            virtual void visit(Covariate *node);
            virtual void visit(IndependentVariable *node);
            virtual void visit(Variable *node);
            virtual void visit(DerivativeVariable *node);
            virtual void visit(ObservationModel *node);
            virtual void visit(Distribution *node);
            virtual void visit(ColumnMapping *node);
            
            virtual void visit(Interventions *node);
            virtual void visit(Administration *node);
            
            virtual void visit(Observations *node);
            virtual void visit(Observation *node);
            virtual void visit(IndividualObservations *node);
            virtual void visit(ObservationCombination *node);
            
            virtual void visit(Arms *node);
            virtual void visit(Arm *node);
            virtual void visit(InterventionSequence *node);
            virtual void visit(ObservationSequence *node);
            virtual void visit(OccasionSequence *node);
            
            virtual void visit(DesignSpaces *node);
            virtual void visit(DesignSpace *node);
    };
}

#endif
