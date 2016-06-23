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

#include <xml/XPathContext.h>

namespace xml
{
    XPathContext::XPathContext(Document &doc) {
        this->xpath_context = xmlXPathNewContext(doc.doc);
        if (!this->xpath_context) {
            throw std::bad_alloc();
        }
        std::string version = doc.getNamespaceVersion();
        int fail;
        fail = xmlXPathRegisterNs(this->xpath_context, BAD_CAST "x", BAD_CAST xml::buildNamespace("PharmML", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "math", BAD_CAST xml::buildNamespace("Maths", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ct", BAD_CAST xml::buildNamespace("CommonTypes", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ds", BAD_CAST xml::buildNamespace("Dataset", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "mdef", BAD_CAST xml::buildNamespace("ModelDefinition", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "msteps", BAD_CAST xml::buildNamespace("ModellingSteps", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "design", BAD_CAST xml::buildNamespace("TrialDesign", version).c_str());
        fail |= xmlXPathRegisterNs(this->xpath_context, BAD_CAST "po", BAD_CAST "http://www.pharmml.org/probonto/ProbOnto");

        if (fail) {
            throw std::bad_alloc();
        }
    }

    XPathContext::~XPathContext() {
        xmlXPathFreeContext(this->xpath_context);
    }
}
