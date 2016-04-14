#ifndef PHARMML_VECTOR_H_
#define PHARMML_VECTOR_H_

#include "AstNode.h"
#include "Scalar.h"
#include "xml.h"

namespace PharmML
{
    class VectorCell
    {
        private:
        int index;
        AstNode *content;
        
        public:
        VectorCell(int index, AstNode *content);
        int getIndex();
        AstNode *getContent();
    };
    
    class Vector : public AstNode
    {
        private:
        int length; // To infer length when implicitly built
        ScalarReal *defaultContent; // To infer value of implicit elements
        std::vector<AstNode *> elements;
        
        public:
        Vector(std::string length, std::string defaultValue);
        void addElement(AstNode *element);
        void populateCell(VectorCell *cell);
        std::vector<AstNode *> getElements();
        virtual std::string accept(AbstractVisitor *visitor);
    };
}

#endif
