#include "RGenerator.h"

namespace PharmML
{
    // Helper function to reduce redundant code
    // TODO: Overload with similar function accepting vector of nodes and performing element->accept(this) instead (how?)
    std::string RGenerator::formatVector(std::vector<std::string> vector, std::string prefix, std::string quote) {
        std::string s = prefix + "(";
        
        bool first = true;
        for (std::string element : vector) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            s += quote + element + quote;
        }
        return(s + ")");
    }
    
    std::string RGenerator::visit(SymbRef *node) {
        return node->toString();
    }
    
    std::string RGenerator::visit(SteadyStateParameter *node) {
        return node->getSymbRef()->accept(this) + " = " + node->getAssignment()->accept(this);
    }
    
    std::string RGenerator::visit(ColumnRef *node) {
        return node->toString();
    }

    std::string RGenerator::visit(TargetMapping *node) {
        std::string type = "type=\"" + node->getType() + "\"";
        std::string blkIdRef = "blkIdRef=\"" + node->getBlkIdRef() + "\"";
        std::string ref = "ref=\"" + node->getRef() + "\"";
        return "list(" + type + ", " + blkIdRef + ", " + ref + ")";
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
    
    std::string RGenerator::visit(Vector *node) {
        std::vector<AstNode *> elements = node->getElements();
        std::string s = "c(";
        bool first = true;
        for (AstNode *element : elements) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            s += element->accept(this);
        }
        return(s += ")");
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
    
    std::string RGenerator::visit(LogicFalse *node) {
        return("(FALSE)");
    }
    
    std::string RGenerator::visit(LogicTrue *node) {
        return("(TRUE)");
    }
    
    std::string RGenerator::visit(Pi *node) {
        return("(pi)");
    }
    
    std::string RGenerator::visit(Exponentiale *node) {
        return("exp(1)");
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
        std::string result;

        if (node->isStructured()) {
            std::string pop = node->getPopulationValue()->accept(this);
            if (node->getTransformation() != "") {
                pop = node->getTransformation() + "(" + pop + ")";
            }
            std::string cov;
            if (node->getFixedEffect()) {
                cov = " + " + node->getFixedEffect()->accept(this) + " * " + node->getCovariate()->accept(this);
            }
            
            result = node->getTransformation() + node->getSymbId() + " = " + pop + cov + " + " + node->getRandomEffects()->accept(this);
        } else {
            result = node->getSymbId() + " = " + node->getAssignment()->accept(this);
        }
        return result;
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
    
    std::string RGenerator::visit(DerivativeVariable *node) {
        std::string expr;
        if (node->getAssignment()) {
            expr = node->getSymbId() + " <- " + node->getAssignment()->accept(this);
        } else {
            expr = std::string();
        }
        std::string init_val = "x0=" + node->getInitialValue()->accept(this);
        std::string init_t = "t0=" + node->getInitialTime()->accept(this);
        return "deriv(" + expr + ", iv=" + node->getIndependentVariable()->accept(this) + ", " + init_val + ", " + init_t + ")";
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

    std::string RGenerator::visit(ColumnMapping *node) {
        return node->getColumnIdRef() + " = " + node->getAssignment()->accept(this);
    }
    
    // Class Interventions and all its content
    std::string RGenerator::visit(Administration *node) {
        std::string s = node->getOid() + " = list(";
        
        s += "type = \"" + node->getType() + "\"";
        s += ", target = " + node->getTarget()->accept(this);
        if (node->getTimes()) {
            s += ", times = " + node->getTimes()->accept(this);
        }
        if (node->getSteady()) {
            s += ", steady = " + node->getSteady()->accept(this);
        }
        if (node->getDuration()) {
            s += ", duration = " + node->getDuration()->accept(this);
        }
        if (node->getRate()) {
            s += ", rate = " + node->getRate()->accept(this);
        }

        return(s + ")");
    }
    
    std::string RGenerator::visit(Interventions *node) {
        std::string s;
        
        // <Administration>'s
        std::vector<Administration *> administrations = node->getAdministrations();
        if (!administrations.empty()) {
            s += "# Administration\n";
            std::vector<std::string> adm_oids;
            for (Administration *adm : administrations) {
                s += adm->accept(this) + "\n";
                adm_oids.push_back(adm->getOid());
            }
            s += "administration_oids = " + formatVector(adm_oids, "c") + "\n";
        }

        return(s);
    }
    
    // Class Observations and all its content
    std::string RGenerator::visit(Observation *node) {
        std::string s = node->getOid() + " = list(";
        
        s += "times = " + node->getTimes()->accept(this);
        if (node->getOidRef() != "") {
            s += ", oidRef = \"" + node->getOidRef() + "\"";
        }
        if (node->getNumber()) {
            s += ", number = " + node->getNumber()->accept(this);
        }
        if (!node->getContinuousVariables().empty()) {
            s += ", cont_vars = c(";
            bool first = true;
            for (SymbRef *symbol : node->getContinuousVariables()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += "'" + symbol->accept(this) + "'";
            }
            s += ")";
        }
        if (!node->getDiscreteVariables().empty()) {
            s += ", disc_vars = c(";
            bool first = true;
            for (SymbRef *symbol : node->getDiscreteVariables()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += "'" + symbol->accept(this) + "'";
            }
            s += ")";
        }
        
        return (s + ")");
    }
    
    std::string RGenerator::visit(ObservationCombination *node) {
        std::string s = node->getOid() + " = list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getRelative()) {
            s += ", relative = " + node->getRelative()->accept(this);
        }
        
        return (s + ")");
    }
    
    std::string RGenerator::visit(Observations *node) {
        std::string s;
        
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                s += var->accept(this) + "\n";
            }
        }
        
        std::vector<Observation *> observations = node->getObservations();
        if (!observations.empty()) {
            s += "# Observation\n";
            std::vector<std::string> obs_oids;
            for (Observation *observation : observations) {
                s += observation->accept(this) + "\n";
                obs_oids.push_back(observation->getOid());
            }
            s += "observation_oids = " + formatVector(obs_oids, "c") + "\n";
        }
        
        std::vector<ObservationCombination *> combinations = node->getObservationCombinations();
        if (!combinations.empty()) {
            s += "# Observation combinations\n";
            std::vector<std::string> comb_oids;
            for (ObservationCombination *comb : combinations) {
                s += comb->accept(this) + "\n";
                comb_oids.push_back(comb->getOid());
            }
            s += "combination_oids = c(";
            for (std::string oid : comb_oids) {
                s += "'" + oid + "'";
            }
        }
        
        return(s + ")" + "\n");
    }
    
    // Class Arms and all its contents
    std::string RGenerator::visit(InterventionSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            s += ", start = " + node->getStart()->accept(this);
        }
        
        return(s + ")");
    }
    
    std::string RGenerator::visit(ObservationSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            s += ", start = " + node->getStart()->accept(this);
        }
        
        return(s + ")");
    }
    
    std::string RGenerator::visit(OccasionSequence *node) {
        return("[WIP]"); // Not implemented
    }
    
    std::string RGenerator::visit(Arm *node) {
        std::string s = node->getOid() + " = ";
        std::vector<std::string> list;
        
        if (node->getOidRef() != "") {
            list.push_back("oidRef = '" + node->getOidRef() + "'");
        }
        if (node->getArmSize()) {
            list.push_back("size = " + node->getArmSize()->accept(this));
        }
        if (node->getNumSamples()) {
            list.push_back("samples = " + node->getNumSamples()->accept(this));
        }
        if (node->getNumTimes()) {
            list.push_back("times = " + node->getNumTimes()->accept(this));
        }
        if (node->getSameTimes()) {
            list.push_back("same_times = " + node->getSameTimes()->accept(this));
        }
        if (!node->getInterventionSequences().empty()) {
            std::string s = "intervention_seq = c(";
            bool first = true;
            for (InterventionSequence *seq : node->getInterventionSequences()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += seq->accept(this);
            }
            list.push_back(s + ")");
        }
        if (!node->getObservationSequences().empty()) {
            std::string s = "observation_seq = c(";
            bool first = true;
            for (ObservationSequence *seq : node->getObservationSequences()) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                }
                s += seq->accept(this);
            }
            list.push_back(s + ")");
        }
        // TODO: Implement output of node->getOccasionSequences
        
        s += formatVector(list, "list", "");
        return(s + ")");
    }
    
    std::string RGenerator::visit(Arms *node) {
        std::string s;
        
        // Top-level settings that may or may not exist
        std::vector<std::string> top;
        // <ArmSize>
        if (node->getArmSize()) {
            top.push_back("arm_size = " + node->getArmSize()->accept(this));
        }
        // <CostFunction>
        if (node->getCostFunction()) {
            top.push_back("cost_function = " + node->getCostFunction()->accept(this));
        }
        // <NumberArms>
        if (node->getNumArms()) {
            top.push_back("num_arms = " + node->getNumArms()->accept(this));
        }
        // <NumberSamples>
        if (node->getNumSamples()) {
            top.push_back("num_samples = " + node->getNumSamples()->accept(this));
        }
        // <NumberTimes>
        if (node->getNumTimes()) {
            top.push_back("num_times = " + node->getNumTimes()->accept(this));
        }
        // <SameTimes>
        if (node->getSameTimes()) {
            top.push_back("same_times = " + node->getSameTimes()->accept(this));
        }
        // <TotalCost>
        if (node->getTotalCost()) {
            top.push_back("total_cost = " + node->getTotalCost()->accept(this));
        }
        // <TotalSize>
        if (node->getTotalSize()) {
            top.push_back("total_size = " + node->getTotalSize()->accept(this));
        }
        if (!top.empty()) {
            s += "arms = " + formatVector(top, "list") + "\n";
        }
        
        // <DesignParameter>'s
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                s += var->accept(this) + "\n";
            }
        }
        
        // <Arm>'s
        std::vector<Arm *> arms = node->getArms();
        if (!arms.empty()) {
            s += "# Arm\n";
            std::vector<std::string> arm_oids;
            for (Arm *arm : arms) {
                s += arm->accept(this) + "\n";
                arm_oids.push_back(arm->getOid());
            }
            s += "arm_oids = c(";
            for (std::string oid : arm_oids) {
                s += "'" + oid + "'";
            }
        }
        
        return(s + ")" + "\n");
    }
}
