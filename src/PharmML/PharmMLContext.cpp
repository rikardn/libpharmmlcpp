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

#include <PharmML/PharmMLContext.h>
#include <PharmML/PharmML.h>

namespace pharmmlcpp
{
    std::string PharmMLContext::getNamespaceVersion() {
        xml::Node root = this->doc.getRoot();
        std::string version = root.getAttribute("writtenVersion").getValue();
        int first_dot_index = version.find_first_of(".");
        int last_dot_index = version.find_last_of(".");
        if (first_dot_index != last_dot_index) {
            version = version.substr(0, last_dot_index);
        }
        return version;
    }

    PharmMLContext::PharmMLContext(std::string filename, PharmML *model) {
        this->model = model;
        this->doc.read(filename);
        this->doc.validate();
        this->xpath_context = xmlXPathNewContext(this->doc.doc);    // FIXME!
        std::string version = getNamespaceVersion();
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "x", BAD_CAST xml::buildNamespace("PharmML", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "math", BAD_CAST xml::buildNamespace("Maths", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ct", BAD_CAST xml::buildNamespace("CommonTypes", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "ds", BAD_CAST xml::buildNamespace("Dataset", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "mdef", BAD_CAST xml::buildNamespace("ModelDefinition", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "msteps", BAD_CAST xml::buildNamespace("ModellingSteps", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "design", BAD_CAST xml::buildNamespace("TrialDesign", version).c_str());
        xmlXPathRegisterNs(this->xpath_context, BAD_CAST "po", BAD_CAST "http://www.pharmml.org/probonto/ProbOnto");
    }

    xml::Node PharmMLContext::getSingleElement(xml::Node node, const char *xpath) {
        return node.getSingleElement(this->xpath_context, xpath);
    }

    std::vector<xml::Node> PharmMLContext::getElements(xml::Node node, const char *xpath) {
        return node.getElements(this->xpath_context, xpath);
    }

    PharmMLContext::~PharmMLContext() {
        if (this->xpath_context) {
            xmlXPathFreeContext(xpath_context);
        }
    }
}
