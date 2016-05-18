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

#ifndef PHARMML_OBJECT_H_
#define PHARMML_OBJECT_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <xml/xml.h>
#include <AST/AstNode.h>
#include <visitors/SymbolVisitor.h>
#include <symbols/SymbolSet.h>
#include <AST/symbols.h>

namespace PharmML
{
    class Object
    {
        public:
            std::string getOid();
            //virtual void gatherSymbRefs(std::unordered_map<std::string, Symbol *> &symbolMap) = 0;
            //TODO: Visitor for virtual void accept(ObjectVisitor *visitor) = 0;

        protected:
            std::string oid;
            void parse(xml::Node node);
    };

    class ObjectRef
    {
        public:
            ObjectRef(xml::Node node);
            std::string getOidRef();
            Object *getObject();
            void setObject(Object *object);

        protected:
            std::string oidRef;
            Object *object;
    };
}

#endif
