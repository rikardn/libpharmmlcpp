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

#include <iostream>
#include <exception>
#include "Model.h"
#include "Scalar.h"
#include "Variable.h"
#include "PopulationParameter.h"

using namespace PharmML;

int main(int argc, char **argv)
{
    const char *filename;
    if (argc < 2) {
        filename = "Executable_Simeoni_2004_oncology_TGI.xml";
        // filename = "Executable_Simeoni_2004_oncology_TGI_trialdesign.xml";
    } else {
        filename = argv[1];
    }

    Model *model;
    try {
        model = new Model(filename);
    } 
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 5;
    }

    Administration *admin = model->getTrialDesign()->getInterventions()->getAdministrations()[0];
    admin->update();

    Arm *arm = model->getTrialDesign()->getArms()->getArms()[0];
    arm->update();

    model->write("output.xml");


    return 0;
}
