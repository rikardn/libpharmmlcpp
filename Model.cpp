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
    }

    Model::Model(const char *filename) {
        this->context = new PharmMLContext(filename);
        this->parse(this->context->getRoot());
    }

    Model::~Model() {
        delete context;
    }

    PharmML::ModelDefinition *Model::getModelDefinition() {
        return this->ModelDefinition;
    }
}
