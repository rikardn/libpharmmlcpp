#include <iostream>
#include "Model.h"
#include "PharmMLContext.h"
#include "xml.h"

namespace PharmML
{
    void Model::parse(xml::Node node) {
        xml::Node mdef_node = this->context->getSingleElement(node, "/x:PharmML/mdef:ModelDefinition");
        if (mdef_node.exists()) {
            this->ModelDefinition = new PharmML::ModelDefinition(this->context, mdef_node);
        }

        std::vector<xml::Node> function_nodes = this->context->getElements(node, "/x:PharmML/ct:FunctionDefinition");
        for (xml::Node n : function_nodes) {
            this->FunctionDefinitions.push_back(new PharmML::FunctionDefinition(this->context, n));
        }
    }

    Model::Model(const char *filename) {
        this->context = new PharmMLContext(filename);
        this->parse(this->context->getRoot());
    }

    Model::~Model() {
        delete context;
    }

    std::vector<PharmML::FunctionDefinition *> Model::getFunctionDefinitions() {
        return this->FunctionDefinitions;
    }

    PharmML::ModelDefinition *Model::getModelDefinition() {
        return this->ModelDefinition;
    }
}
