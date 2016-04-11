#include "RGenerator.h"

namespace PharmML
{
    std::string RGenerator::visit(SymbRef *node) {
        return node->toString();
    }

    std::string RGenerator::visit(UniopLog *node) {
        return("log(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopLog2 *node) {
        return("log2(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopLog10 *node) {
        return("log10(" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(UniopExp *node) {
        return("exp(" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(UniopMinus *node) {
        return("(-" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopAbs *node) {
        return("abs(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopSqrt *node) {
        return("sqrt(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopLogistic *node) {
        return("(1/(1 + exp(-" + node->acceptChild(this) + ")))");
    }
    
    std::string RGenerator::visit(UniopLogit *node) {
        return("log((" + node->acceptChild(this) + ")/(1 - " + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopProbit *node) {
        return("qnorm(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopNormcdf *node) {
        return("pnorm(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopFactorial *node) {
        return("factorial(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopFactln *node) {
        return("lfactorial(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopGamma *node) {
        return("gamma(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopGammaln *node) {
        return("lgamma(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopSin *node) {
        return("sin(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopSinh *node) {
        return("sinh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopCos *node) {
        return("cos(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopCosh *node) {
        return("cosh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopTan *node) {
        return("tan(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopTanh *node) {
        return("tanh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopCot *node) {
        return("(1/tan(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopCoth *node) {
        return("(1/tanh(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopSec *node) {
        return("(1/cos(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopSech *node) {
        return("(1/cosh(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopCsc *node) {
        return("(1/sin(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopCsch *node) {
        return("(1/sinh(" + node->acceptChild(this) + "))");
    }
    
    std::string RGenerator::visit(UniopArcsin *node) {
        return("asin(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArcsinh *node) {
        return("asinh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccos *node) {
        return("acos(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccosh *node) {
        return("acosh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArctan *node) {
        return("atan(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArctanh *node) {
        return("atanh(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccot *node) {
        return("atan(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccoth *node) {
        return("atanh(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArcsec *node) {
        return("acos(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArcsech *node) {
        return("acosh(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccsc *node) {
        return("asin(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopArccsch *node) {
        return("asinh(1/" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopHeaviside *node) {
        return("((sign(" + node->acceptChild(this) + ") + 1) / 2)");
    }
    
    std::string RGenerator::visit(UniopSign *node) {
        return("sign(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopFloor *node) {
        return("floor(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(UniopCeiling *node) {
        return("ceiling(" + node->acceptChild(this) + ")");
    }

    std::string RGenerator::visit(ScalarInt *node) {
        return("(" + node->toString() + ")");
    }

    std::string RGenerator::visit(ScalarReal *node) {
        return("(" + node->toString() + ")");
    }

    std::string RGenerator::visit(BinopPlus *node) {
        return("(" + node->acceptLeft(this) + " + " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopMinus *node) {
        return("(" + node->acceptLeft(this) + " - " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopDivide *node) {
        return("(" + node->acceptLeft(this) + " / " + node->acceptRight(this) + ")");
    }

    std::string RGenerator::visit(BinopTimes *node) {
        return("(" + node->acceptLeft(this) + " * " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopPower *node) {
        return("(" + node->acceptLeft(this) + " ^ " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopLogx *node) {
        return("log(" + node->acceptLeft(this) + ", base = " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopRoot *node) {
        return("(" + node->acceptLeft(this) + " ^ (1/" + node->acceptRight(this) + "))");
    }
    
    std::string RGenerator::visit(BinopMin *node) {
        return("min(" + node->acceptLeft(this) + ", " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopMax *node) {
        return("max(" + node->acceptLeft(this) + ", " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopRem *node) {
        return("(" + node->acceptLeft(this) + " %% " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(BinopAtan2 *node) {
        return("atan2(" + node->acceptLeft(this) + ", " + node->acceptRight(this) + ")");
    }
    
    std::string RGenerator::visit(LogicUniopIsdefined *node) {
        return("!is.null(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(LogicUniopNot *node) {
        return("!(" + node->acceptChild(this) + ")");
    }
    
    std::string RGenerator::visit(LogicBinopLt *node) {
        return(node->infix(this, " < "));
    }

    std::string RGenerator::visit(LogicBinopLeq *node) {
        return(node->infix(this, " <= "));
    }
    
    std::string RGenerator::visit(LogicBinopGt *node) {
        return(node->infix(this, " > "));
    }
    
    std::string RGenerator::visit(LogicBinopGeq *node) {
        return(node->infix(this, " >= "));
    }
    
    std::string RGenerator::visit(LogicBinopEq *node) {
        return(node->infix(this, " == "));
    }
    
    std::string RGenerator::visit(LogicBinopNeq *node) {
        return(node->infix(this, " != "));
    }
    
    std::string RGenerator::visit(LogicBinopAnd *node) {
        return(node->infix(this, " && "));
    }
    
    std::string RGenerator::visit(LogicBinopOr *node) {
        return(node->infix(this, " || "));
    }
    
    std::string RGenerator::visit(LogicBinopXor *node) {
        return("((" + node->acceptLeft(this) + " || " + node->acceptRight(this) + ")" +
            " && !(" + node->acceptLeft(this) + " && " + node->acceptRight(this) + "))");
    }
    
    std::string RGenerator::visit(Piecewise *node) {
        std::vector<Piece *> pieces = node->getPieces();
        Piece *otherwise = nullptr;
        std::string s = "ifelse(";
        for (Piece *p : pieces) {
			if (!p->isOtherwise()) {
				s += p->accept(this) + ", (";
			} else {
				otherwise = p; // Only one otherwise per Piece
			}
        }
        if (otherwise == nullptr) {
            // And the missing otherwise said, Let it be 'NULL'. And all was good.
            NullValue *null = new NullValue();
            s += null->accept(this) + ")";
        } else {
            s += otherwise->getExpression()->accept(this);
        }
        return(s + std::string(pieces.size(), ')'));
    }

    std::string RGenerator::visit(Piece *node) {
        return(node->getCondition()->accept(this) + ", " + node->getExpression()->accept(this));
    }
    
    std::string RGenerator::visit(LogicNullopFalse *node) {
        return("(FALSE)");
    }
    
    std::string RGenerator::visit(LogicNullopTrue *node) {
        return("(TRUE)");
    }
    
    std::string RGenerator::visit(NullValue *node) {
        return("NULL");
    }

    std::string RGenerator::visit(FunctionCall *node) {
        bool first = true;
        std::string argument_list;
        for (FunctionArgument *arg : node->getFunctionArguments()) {
            if (first) {
                first = false;
            } else {
                argument_list += ", ";
            }
            argument_list += arg->accept(this);
        }
        return node->getFunctionName()->accept(this) + "(" + argument_list + ")";
    }

    std::string RGenerator::visit(FunctionArgument *node) {
        return node->getSymbId() + "=" + node->getArgument()->accept(this);
    }

    std::string RGenerator::visit(FunctionDefinition *node) {
        std::string head = node->getSymbId() + " <- function(";
        std::vector<std::string> args = node->getArguments();
        for (int i = 0; i < args.size(); i++) {
            head += args[i];
            if (i != args.size() - 1) {
                head += ", ";
            }
        }
        head += ") {\n";
        std::string code = "  " + node->getAssignment()->accept(this) + "\n}";
        return(head + code);
    }

    std::string RGenerator::visit(Covariate *node) {
        return(node->getTransformedName() + " <- " + node->getAssignment()->accept(this));
    }

    std::string RGenerator::visit(PopulationParameter *node) {
        return "\"" + node->getSymbId() + "\"";
    }

    std::string RGenerator::visit(IndividualParameter *node) {
        std::string pop = node->getPopulationValue()->accept(this);
        if (node->getTransformation() != "") {
            pop = node->getTransformation() + "(" + pop + ")";
        }
        std::string cov;
        if (node->getFixedEffect()) {
            cov = " + " + node->getFixedEffect()->accept(this) + " * " + node->getCovariate()->accept(this);
        }
        return(node->getTransformation() + node->getSymbId() + " = " + pop + cov + " + " + node->getRandomEffects()->accept(this));
    }

    std::string RGenerator::visit(RandomVariable *node) {
        std::string var_ref = "variability_reference=\"" + node->getVariabilityReference()->accept(this) + "\"";
        std::string dist = node->getDistribution()->accept(this);
        return node->getSymbId() + " = list(" + var_ref + ", " + dist + ")";
    }

    std::string RGenerator::visit(IndependentVariable *node) {
        return "IndependentVariable = \"" + node->getSymbId() + "\"";
    }

    std::string RGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            return(node->getSymbId() + " <- " + node->getAssignment()->accept(this));
        } else {
            return std::string();
        }
    }

    std::string RGenerator::visit(ObservationModel *node) {
        std::string error = "W = " + node->getErrorModel()->accept(this);
        std::string y = node->getSymbId() + " = " + node->getOutput()->accept(this) + " + W * " + node->getResidualError()->accept(this);
        return error + "\n" + y;
    }

    std::string RGenerator::visit(Distribution *node) {
        std::string result = "distribution=\"" + node->getName() + "\"";
        for (DistributionParameter *p : node->getDistributionParameters()) {
            result += ", " + p->getName() + "=" + p->getAssignment()->accept(this);
        }
        return result;
    }
}
