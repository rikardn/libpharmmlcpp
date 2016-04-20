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

#include "RGenerator.h"

namespace PharmML
{
    // private
    void RGenerator::setValue(std::string str) {
        this->value = str;
    }

    std::string RGenerator::acceptLeft(Binop *binop) {
        binop->getLeft()->accept(this);
        return this->getValue();
    }

    std::string RGenerator::acceptRight(Binop *binop) {
        binop->getRight()->accept(this);
        return this->getValue();
    }

    std::string RGenerator::infix(Binop *binop, std::string op) {
        std::string result;
        binop->getLeft()->accept(this);
        result = "(" + this->getValue() + op;
        binop->getRight()->accept(this);
        result += this->getValue() + ")";
        return result;
    }

    std::string RGenerator::acceptChild(Uniop *uniop) {
        uniop->getChild()->accept(this);
        return this->getValue();
    }
   
    std::string RGenerator::accept(AstNode *node) {
        node->accept(this);
        return this->getValue();
    }

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

    std::string RGenerator::getLogicLiteral(bool value) {
        if (value) {
            return "TRUE";
        } else {
            return "FALSE";
        }
    }

    // public
    std::string RGenerator::getValue() {
        return this->value;
    }
    
    void RGenerator::visit(SymbRef *node) {
        this->setValue(node->toString());
    }
    
    void RGenerator::visit(SteadyStateParameter *node) {
        node->getSymbRef()->accept(this);
        std::string symbref = this->getValue();
        node->getAssignment()->accept(this);
        std::string assignment = this->getValue();
        this->setValue(symbref + " = " + assignment);
    }
    
    void RGenerator::visit(ColumnRef *node) {
        this->setValue(node->toString());
    }

    void RGenerator::visit(TargetMapping *node) {
        std::string type = "type=\"" + node->getType() + "\"";
        std::string blkIdRef = "blkIdRef=\"" + node->getBlkIdRef() + "\"";
        std::string ref = "ref=\"" + node->getRef() + "\"";
        this->setValue("list(" + type + ", " + blkIdRef + ", " + ref + ")");
    }

    void RGenerator::visit(UniopLog *node) {
        this->setValue("log(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopLog2 *node) {
        this->setValue("log2(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopLog10 *node) {
        this->setValue("log10(" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(UniopExp *node) {
        this->setValue("exp(" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(UniopMinus *node) {
        this->setValue("(-" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(UniopAbs *node) {
        this->setValue("abs(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopSqrt *node) {
        this->setValue("sqrt(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopLogistic *node) {
        this->setValue("(1/(1 + exp(-" + this->acceptChild(node) + ")))");
    }
    
    void RGenerator::visit(UniopLogit *node) {
        this->setValue("log((" + this->acceptChild(node) + ")/(1 - " + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopProbit *node) {
        this->setValue("qnorm(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopNormcdf *node) {
        this->setValue("pnorm(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopFactorial *node) {
        this->setValue("factorial(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopFactln *node) {
        this->setValue("lfactorial(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopGamma *node) {
        this->setValue("gamma(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopGammaln *node) {
        this->setValue("lgamma(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopSin *node) {
        this->setValue("sin(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopSinh *node) {
        this->setValue("sinh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopCos *node) {
        this->setValue("cos(" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(UniopCosh *node) {
        this->setValue("cosh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopTan *node) {
        this->setValue("tan(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopTanh *node) {
        this->setValue("tanh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopCot *node) {
        this->setValue("(1/tan(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopCoth *node) {
        this->setValue("(1/tanh(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopSec *node) {
        this->setValue("(1/cos(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopSech *node) {
        this->setValue("(1/cosh(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopCsc *node) {
        this->setValue("(1/sin(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopCsch *node) {
        this->setValue("(1/sinh(" + this->acceptChild(node) + "))");
    }
    
    void RGenerator::visit(UniopArcsin *node) {
        this->setValue("asin(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArcsinh *node) {
        this->setValue("asinh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccos *node) {
        this->setValue("acos(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccosh *node) {
        this->setValue("acosh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArctan *node) {
        this->setValue("atan(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArctanh *node) {
        this->setValue("atanh(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccot *node) {
        this->setValue("atan(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccoth *node) {
        this->setValue("atanh(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArcsec *node) {
        this->setValue("acos(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArcsech *node) {
        this->setValue("acosh(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccsc *node) {
        this->setValue("asin(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopArccsch *node) {
        this->setValue("asinh(1/" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopHeaviside *node) {
        this->setValue("((sign(" + this->acceptChild(node) + ") + 1) / 2)");
    }
    
    void RGenerator::visit(UniopSign *node) {
        this->setValue("sign(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopFloor *node) {
        this->setValue("floor(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(UniopCeiling *node) {
        this->setValue("ceiling(" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(ScalarInt *node) {
        this->setValue("(" + node->toString() + ")");
    }

    void RGenerator::visit(ScalarReal *node) {
        this->setValue("(" + node->toString() + ")");
    }

    void RGenerator::visit(BinopPlus *node) {
        this->setValue("(" + this->acceptLeft(node) + " + " + this->acceptRight(node) + ")");
    }

    void RGenerator::visit(BinopMinus *node) {
        this->setValue("(" + this->acceptLeft(node) + " - " + this->acceptRight(node) + ")");
    }

    void RGenerator::visit(BinopDivide *node) {
        this->setValue("(" + this->acceptLeft(node) + " / " + this->acceptRight(node) + ")");
    }

    void RGenerator::visit(BinopTimes *node) {
        this->setValue("(" + this->acceptLeft(node) + " * " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopPower *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopLogx *node) {
        this->setValue("log(" + this->acceptLeft(node) + ", base = " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopRoot *node) {
        this->setValue("(" + this->acceptLeft(node) + " ^ (1/" + this->acceptRight(node) + "))");
    }
    
    void RGenerator::visit(BinopMin *node) {
        this->setValue("min(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopMax *node) {
        this->setValue("max(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopRem *node) {
        this->setValue("(" + this->acceptLeft(node) + " %% " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(BinopAtan2 *node) {
        this->setValue("atan2(" + this->acceptLeft(node) + ", " + this->acceptRight(node) + ")");
    }
    
    void RGenerator::visit(LogicUniopIsdefined *node) {
        this->setValue("!is.null(" + this->acceptChild(node) + ")");
    }
    
    void RGenerator::visit(LogicUniopNot *node) {
        this->setValue("!(" + this->acceptChild(node) + ")");
    }

    void RGenerator::visit(LogicBinopLt *node) {
        this->setValue(this->infix(node, " < "));
    }

    void RGenerator::visit(LogicBinopLeq *node) {
        this->setValue(this->infix(node, " <= "));
    }
    
    void RGenerator::visit(LogicBinopGt *node) {
        this->setValue(this->infix(node, " > "));
    }
    
    void RGenerator::visit(LogicBinopGeq *node) {
        this->setValue(this->infix(node, " >= "));
    }
    
    void RGenerator::visit(LogicBinopEq *node) {
        this->setValue(this->infix(node, " == "));
    }
    
    void RGenerator::visit(LogicBinopNeq *node) {
        this->setValue(this->infix(node, " != "));
    }
    
    void RGenerator::visit(LogicBinopAnd *node) {
        this->setValue(this->infix(node, " && "));
    }
    
    void RGenerator::visit(LogicBinopOr *node) {
        this->setValue(this->infix(node, " || "));
    }
    
    void RGenerator::visit(LogicBinopXor *node) {
        this->setValue("((" + this->acceptLeft(node) + " || " + this->acceptRight(node) + ")" +
            " && !(" + this->acceptLeft(node) + " && " + this->acceptRight(node) + "))");
    }
    
    void RGenerator::visit(Vector *node) {
        std::vector<AstNode *> elements = node->getElements();
        std::string s = "c(";
        bool first = true;
        for (AstNode *element : elements) {
            if (first) {
                first = false;
            } else {
                s += ", ";
            }
            element->accept(this);
            s += this->getValue();
        }
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(Piecewise *node) {
        std::vector<Piece *> pieces = node->getPieces();
        Piece *otherwise = nullptr;
        std::string s = "ifelse(";
        for (Piece *p : pieces) {
            if (!p->isOtherwise()) {
                p->accept(this);
                s += this->getValue() + ", (";
            } else {
                otherwise = p; // Only one otherwise per Piece
            }
        }
        if (otherwise == nullptr) {
            // And the missing otherwise said, Let it be 'NULL'. And all was good.
            NullValue *null = new NullValue();
            null->accept(this);
            s += this->getValue() + ")";
        } else {
            otherwise->getExpression()->accept(this);
            s += this->getValue();
        }
        this->setValue(s + std::string(pieces.size(), ')'));
    }

    void RGenerator::visit(Piece *node) {
        node->getCondition()->accept(this);
        std::string cond = this->getValue();
        node->getExpression()->accept(this);
        std::string expr = this->getValue();
        this->setValue(cond + ", " + expr);
    }
    
    void RGenerator::visit(LogicFalse *node) {
        this->setValue("(FALSE)");
    }
    
    void RGenerator::visit(LogicTrue *node) {
        this->setValue("(TRUE)");
    }
    
    void RGenerator::visit(Pi *node) {
        this->setValue("(pi)");
    }
    
    void RGenerator::visit(Exponentiale *node) {
        this->setValue("exp(1)");
    }
    
    void RGenerator::visit(NullValue *node) {
        this->setValue("NULL");
    }

    void RGenerator::visit(FunctionCall *node) {
        bool first = true;
        std::string argument_list;
        for (FunctionArgument *arg : node->getFunctionArguments()) {
            if (first) {
                first = false;
            } else {
                argument_list += ", ";
            }
            arg->accept(this);
            argument_list += this->getValue();
        }
        node->getFunctionName()->accept(this);
        this->setValue(this->getValue() + "(" + argument_list + ")");
    }

    void RGenerator::visit(FunctionArgument *node) {
        node->getArgument()->accept(this);
        this->setValue(node->getSymbId() + "=" + this->getValue());
    }

    void RGenerator::visit(FunctionDefinition *node) {
        std::string head = node->getSymbId() + " <- function(";
        std::vector<std::string> args = node->getArguments();
        for (int i = 0; i < args.size(); i++) {
            head += args[i];
            if (i != args.size() - 1) {
                head += ", ";
            }
        }
        head += ") {\n";
        node->getAssignment()->accept(this);
        std::string code = "  " + this->getValue() + "\n}";
        this->setValue(head + code);
    }

    void RGenerator::visit(Interval *node) {
        std::string result = "list(left=" + this->accept(node->getLeftEndpoint());
        result += ", right=" + this->accept(node->getRightEndpoint());
        result += ", openleft=" + this->getLogicLiteral(node->isLeftEndpointOpen());
        result += ", openright=" + this->getLogicLiteral(node->isRightEndpointOpen());
        result += ")";
        this->setValue(result);
    }

    void RGenerator::visit(Covariate *node) {
        node->getAssignment()->accept(this);
        this->setValue(node->getTransformedName() + " <- " + this->getValue());
    }

    void RGenerator::visit(PopulationParameter *node) {
        this->setValue("\"" + node->getSymbId() + "\"");
    }

    void RGenerator::visit(IndividualParameter *node) {
        std::string result;

        if (node->isStructured()) {
            node->getPopulationValue()->accept(this);
            std::string pop = this->getValue();
            if (node->getTransformation() != "") {
                pop = node->getTransformation() + "(" + pop + ")";
            }
            std::string cov;
            if (node->getFixedEffect()) {
                node->getFixedEffect()->accept(this);
                std::string fe = this->getValue();
                node->getCovariate()->accept(this);
                std::string cov = this->getValue();
                cov = " + " + fe + " * " + cov;
            }
            node->getRandomEffects()->accept(this); 
            result = node->getTransformation() + node->getSymbId() + " <- " + pop + cov + " + " + this->getValue();
        } else {
            node->getAssignment()->accept(this);
            result = node->getSymbId() + " <- " + this->getValue();
        }
        this->setValue(result);
    }

    void RGenerator::visit(RandomVariable *node) {
        node->getVariabilityReference()->accept(this);
        std::string var_ref = "variability_reference=\"" + this->getValue() + "\"";
        node->getDistribution()->accept(this);
        std::string dist = this->getValue();
        this->setValue(node->getSymbId() + " <- list(" + var_ref + ", " + dist + ")");
    }

    void RGenerator::visit(IndependentVariable *node) {
        this->setValue("IndependentVariable <- \"" + node->getSymbId() + "\"");
    }

    void RGenerator::visit(Variable *node) {
        if (node->getAssignment()) {
            node->getAssignment()->accept(this);
            this->setValue(node->getSymbId() + " <- " + this->getValue());
        } else {
            this->setValue(std::string());
        }
    }
    
    void RGenerator::visit(DerivativeVariable *node) {
        std::string expr;
        if (node->getAssignment()) {
            node->getAssignment()->accept(this);
            expr = node->getSymbId() + " <- " + this->getValue();
        } else {
            expr = std::string();
        }
        node->getInitialValue()->accept(this);
        std::string init_val = "x0=" + this->getValue();
        node->getInitialTime()->accept(this);
        std::string init_t = "t0=" + this->getValue();
        node->getIndependentVariable()->accept(this);
        this->setValue("deriv(" + expr + ", iv=" + this->getValue() + ", " + init_val + ", " + init_t + ")");
    }

    void RGenerator::visit(ObservationModel *node) {
        node->getErrorModel()->accept(this);
        std::string error = "W <- " + this->getValue();
        node->getOutput()->accept(this);
        std::string output = this->getValue();
        node->getResidualError()->accept(this);
        std::string res = this->getValue();
        std::string y = node->getSymbId() + " <- " + output + " + W * " + res;
        this->setValue(error + "\n" + y);
    }

    void RGenerator::visit(Distribution *node) {
        std::string result = "distribution=\"" + node->getName() + "\"";
        for (DistributionParameter *p : node->getDistributionParameters()) {
            p->getAssignment()->accept(this);
            result += ", " + p->getName() + "=" + this->getValue();
        }
        this->setValue(result);
    }

    void RGenerator::visit(ColumnMapping *node) {
        node->getAssignment()->accept(this);
        this->setValue(node->getColumnIdRef() + " <- " + this->getValue());
    }
    
    // Class Interventions and all its content
    void RGenerator::visit(Administration *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "type = \"" + node->getType() + "\"";
        node->getTarget()->accept(this);
        s += ", target = " + this->getValue();
        if (node->getTimes()) {
            node->getTimes()->accept(this);
            s += ", times = " + this->getValue();
        }
        if (node->getSteady()) {
            node->getSteady()->accept(this);
            s += ", steady = " + this->getValue();
        }
        if (node->getDuration()) {
            node->getDuration()->accept(this);
            s += ", duration = " + this->getValue();
        }
        if (node->getRate()) {
            node->getRate()->accept(this);
            s += ", rate = " + this->getValue();
        }

        this->setValue(s + ")");
    }
    
    void RGenerator::visit(Interventions *node) {
        std::string s;
        
        // <Administration>'s
        std::vector<Administration *> administrations = node->getAdministrations();
        if (!administrations.empty()) {
            s += "# Administration\n";
            std::vector<std::string> adm_oids;
            for (Administration *adm : administrations) {
                adm->accept(this);
                s += this->getValue() + "\n";
                adm_oids.push_back(adm->getOid());
            }
            s += "administration_oids <- " + formatVector(adm_oids, "c") + "\n";
        }

        this->setValue(s);
    }
    
    // Class Observations and all its content
    void RGenerator::visit(Observation *node) {
        std::string s = node->getOid() + " <- list(";
        
        node->getTimes()->accept(this);
        s += "times = " + this->getValue();
        if (node->getOidRef() != "") {
            s += ", oidRef = \"" + node->getOidRef() + "\"";
        }
        if (node->getNumber()) {
            node->getNumber()->accept(this);
            s += ", number = " + this->getValue();
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
                symbol->accept(this);
                s += "'" + this->getValue() + "'";
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
                symbol->accept(this);
                s += "'" + this->getValue() + "'";
            }
            s += ")";
        }
        
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(ObservationCombination *node) {
        std::string s = node->getOid() + " <- list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getRelative()) {
            node->getRelative()->accept(this);
            s += ", relative = " + this->getValue();
        }
        
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(Observations *node) {
        std::string s;
        
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }
        
        std::vector<Observation *> observations = node->getObservations();
        if (!observations.empty()) {
            s += "# Observation\n";
            std::vector<std::string> obs_oids;
            for (Observation *observation : observations) {
                observation->accept(this);
                s += this->getValue() + "\n";
                obs_oids.push_back(observation->getOid());
            }
            s += "observation_oids <- " + formatVector(obs_oids, "c") + "\n";
        }
        
        std::vector<ObservationCombination *> combinations = node->getObservationCombinations();
        if (!combinations.empty()) {
            s += "# Observation combinations\n";
            std::vector<std::string> comb_oids;
            for (ObservationCombination *comb : combinations) {
                comb->accept(this);
                s += this->getValue() + "\n";
                comb_oids.push_back(comb->getOid());
            }
            s += "combination_oids <- c(";
            for (std::string oid : comb_oids) {
                s += "'" + oid + "'";
            }
        }
        
        this->setValue(s + ")" + "\n");
    }
    
    // Class Arms and all its contents
    void RGenerator::visit(InterventionSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            node->getStart()->accept(this);
            s += ", start = " + this->getValue();
        }
        
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(ObservationSequence *node) {
        std::string s = "list(";
        
        s += "refs = " + formatVector(node->getOidRefs(), "c");
        if (node->getStart()) {
            node->getStart()->accept(this);
            s += ", start = " + this->getValue();
        }
        
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(OccasionSequence *node) {
        this->setValue("[WIP]"); // Not implemented
    }
    
    void RGenerator::visit(Arm *node) {
        std::string s = node->getOid() + " <- ";
        std::vector<std::string> list;
        
        if (node->getOidRef() != "") {
            list.push_back("oidRef = '" + node->getOidRef() + "'");
        }
        if (node->getArmSize()) {
            node->getArmSize()->accept(this);
            list.push_back("size = " + this->getValue());
        }
        if (node->getNumSamples()) {
            node->getNumSamples()->accept(this);
            list.push_back("samples = " + this->getValue());
        }
        if (node->getNumTimes()) {
            node->getNumTimes()->accept(this);
            list.push_back("times = " + this->getValue());
        }
        if (node->getSameTimes()) {
            node->getSameTimes()->accept(this);
            list.push_back("same_times = " + this->getValue());
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
                seq->accept(this);
                s += this->getValue();
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
                seq->accept(this);
                s += this->getValue();
            }
            list.push_back(s);
        }
        // TODO: Implement output of node->getOccasionSequences
        
        s += formatVector(list, "list", "");
        this->setValue(s + ")");
    }
    
    void RGenerator::visit(Arms *node) {
        std::string s;
        
        // Top-level settings that may or may not exist
        std::vector<std::string> top;
        // <ArmSize>
        if (node->getArmSize()) {
            node->getArmSize()->accept(this);
            top.push_back("arm_size = " + this->getValue());
        }
        // <CostFunction>
        if (node->getCostFunction()) {
            node->getCostFunction()->accept(this);
            top.push_back("cost_function = " + this->getValue());
        }
        // <NumberArms>
        if (node->getNumArms()) {
            node->getNumArms()->accept(this);
            top.push_back("num_arms = " + this->getValue());
        }
        // <NumberSamples>
        if (node->getNumSamples()) {
            node->getNumSamples()->accept(this);
            top.push_back("num_samples = " + this->getValue());
        }
        // <NumberTimes>
        if (node->getNumTimes()) {
            node->getNumTimes()->accept(this);
            top.push_back("num_times = " + this->getValue());
        }
        // <SameTimes>
        if (node->getSameTimes()) {
            node->getSameTimes()->accept(this);
            top.push_back("same_times = " + this->getValue());
        }
        // <TotalCost>
        if (node->getTotalCost()) {
            node->getTotalCost()->accept(this);
            top.push_back("total_cost = " + this->getValue());
        }
        // <TotalSize>
        if (node->getTotalSize()) {
            node->getTotalSize()->accept(this);
            top.push_back("total_size = " + this->getValue());
        }
        if (!top.empty()) {
            s += "arms = " + formatVector(top, "list") + "\n";
        }
        
        // <DesignParameter>'s
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }
        
        // <Arm>'s
        std::vector<Arm *> arms = node->getArms();
        if (!arms.empty()) {
            s += "# Arm\n";
            std::vector<std::string> arm_oids;
            for (Arm *arm : arms) {
                arm->accept(this);
                s += this->getValue() + "\n";
                arm_oids.push_back(arm->getOid());
            }
            s += "arm_oids <- c(";
            bool first = true;
            for (std::string oid : arm_oids) {
                if (first) {
                    first = false;
                } else {
                    s += ", ";
                } 
                s += "'" + oid + "'";
            }
        }
        
        this->setValue(s + ")" + "\n");
    }
    
    // Class DesignSpaces and all its content
    void RGenerator::visit(DesignSpace *node) {
        std::string s = node->getOid() + " <- ";
        std::vector<std::string> list;
        
        list.push_back("intervention_refs = " + formatVector(node->getInterventionRefs(), "c"));
        list.push_back("observation_refs = " + formatVector(node->getObservationRefs(), "c"));
        list.push_back("arm_refs = " + formatVector(node->getArmRefs(), "c"));
        AstNode *dosing_times = node->getDosingTimes();
        if (dosing_times) {
            list.push_back("dosing_times=" + this->accept(dosing_times));
        }
        
        s += formatVector(list, "list", "");
        this->setValue(s);
    }
    
    void RGenerator::visit(DesignSpaces *node) {
        std::string s;
        
        std::vector<Variable *> variables = node->getDesignParameters();
        if (!variables.empty()) {
            s += "# Design parameters\n";
            for (Variable *var : variables) {
                var->accept(this);
                s += this->getValue() + "\n";
            }
        }
        
        std::vector<DesignSpace *> designSpaces = node->getDesignSpaces();
        if (!designSpaces.empty()) {
            s += "# Design spaces\n";
            s += "design_spaces <- vector(mode=\"list\", length=" + std::to_string(designSpaces.size()) + ")\n";
            int i = 1;
            for (DesignSpace *ds : designSpaces) {
                ds->accept(this);
                s += "design_spaces[[" + std::to_string(i) + "]]" + this->getValue() + "\n";
                i++;
            }
        }
        
        this->setValue(s + "\n");
    }
}
