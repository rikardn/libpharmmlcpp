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

#ifndef PHARMML_MODELDEFINITION_H_
#define PHARMML_MODELDEFINITION_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "PharmMLContext.h"
#include "CovariateModel.h"
#include "ParameterModel.h"
#include "StructuralModel.h"
#include "ObservationModel.h"

namespace PharmML
{
    class ModelDefinition
    {
        PharmML::PharmMLContext *context;
        PharmML::CovariateModel *CovariateModel = nullptr;
        PharmML::ParameterModel *ParameterModel = nullptr;
        PharmML::StructuralModel *StructuralModel = nullptr;
        PharmML::ObservationModel *ObservationModel = nullptr;

        public:
        PharmML::CovariateModel *getCovariateModel();
        PharmML::ParameterModel *getParameterModel();
        PharmML::StructuralModel *getStructuralModel();
        PharmML::ObservationModel *getObservationModel();
        ModelDefinition(PharmML::PharmMLContext *context, xml::Node node);
        void parse(xml::Node node);
    };
}

#endif
