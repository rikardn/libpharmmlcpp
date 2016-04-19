#ifndef PHARMML_INTERVAL_H_
#define PHARMML_INTERVAL_H_

#include "PharmMLContext.h"
#include "AstNode.h"

namespace PharmML
{
    class Interval : public AstNode
    {
        AstNode *LeftEndpoint = nullptr;
        AstNode *RightEndpoint = nullptr;
        bool openLeftEndpoint = false;
        bool openRightEndpoint = false;

        public:
        AstNode *getLeftEndpoint();
        AstNode *getRightEndpoint();
        void setLeftEndpoint(AstNode *node);
        void setRightEndpoint(AstNode *node);
        bool isLeftEndpointOpen();
        bool isRightEndpointOpen();
        void setLeftEndpointOpen(bool open);
        void setRightEndpointOpen(bool open);
        void accept(AbstractVisitor *visitor);
    };
}

#endif
