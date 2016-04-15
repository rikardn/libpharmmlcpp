#include "Distribution.h"
#include <iostream>

namespace PharmML
{
    Distribution::Distribution(PharmMLContext *context, xml::Node node) {
        this->context = context;
        this->parse(node);
    }

    void Distribution::parse(xml::Node node) {
        if (node.getName() == "ProbOnto") {
            this->name = node.getAttribute("name").getValue();
            std::vector<xml::Node> params = this->context->getElements(node, ".//po:Parameter");
            for (xml::Node n : params) {
                this->parameters.push_back(new PharmML::DistributionParameter(this->context, n));
            }
        } else {
            // UncertML. Support only normal distribution and make lots of assumptions and hope that UncertML will go away.
            this->name = "Normal2";
            auto mean_param = new PharmML::DistributionParameter(this->context);
            mean_param->setAssignment(new ScalarReal(node.getChild().getChild().getChild().getText()));
            mean_param->setName("mean");
            this->parameters.push_back(mean_param);
            auto stdev_param = new PharmML::DistributionParameter(this->context);
            stdev_param->setAssignment(new SymbRef(node.getChild().getLastChild().getChild().getAttribute("varId").getValue()));
            stdev_param->setName("var");
            this->parameters.push_back(stdev_param);
        }
    }
        
    std::string Distribution::getName() {
        return this->name;
    }
    
    std::vector<PharmML::DistributionParameter *> Distribution::getDistributionParameters() {
        return this->parameters;
    }

    void Distribution::accept(AbstractVisitor *visitor) {
        visitor->visit(this);
    }
}
