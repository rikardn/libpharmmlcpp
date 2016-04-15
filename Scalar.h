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
            ScalarInt(int value);
            ScalarInt(std::string s);
            ScalarInt(xml::Node xml_node);
            std::string toString();
            virtual void accept(AbstractVisitor *visitor);
    };

    class ScalarReal : public Scalar
    {
        private:
            double value;

        public:
            ScalarReal(double value);
            ScalarReal(std::string s);
            ScalarReal(xml::Node xml_node);
            std::string toString();
            virtual void accept(AbstractVisitor *visitor);
    };
}

#endif
