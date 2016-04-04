#ifndef PHARMML_SCALAR_H_
#define PHARMML_SCALAR_H_

#include "AstNode.h"
#include "xml.h"

namespace PharmML
{
    class Scalar : public AstNode
    {

    };

    class ScalarInt : public Scalar
    {
        private:
            int value;

        public:
            virtual std::string accept(AbstractVisitor *visitor);
            ScalarInt(int value);
            ScalarInt(std::string s);
            ScalarInt(xml::Node xml_node);
            std::string toString();
    };

}

#endif
