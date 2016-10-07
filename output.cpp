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

// gcc -std=c++14 output.cpp -ooutput -Isrc -L. -lstdc++ -lpharmmlcpp $(xml2-config --cflags --libs)

#include <iostream>
#include <exception>
#include <PharmML/PharmML.h>

using namespace pharmmlcpp;

int main(int argc, char **argv)
{
    const char *filename;
    if (argc < 2) {
        filename = "Executable_Simeoni_2004_oncology_TGI.xml";
    } else {
        filename = argv[1];
    }

    PharmML *model;
    try {
        model = new PharmML(filename);
    } 
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 5;
    }

    model->write("output.xml");

    return 0;
}
