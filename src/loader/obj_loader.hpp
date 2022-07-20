#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "loader.hpp"

namespace loader
{
    class OBJLoader
    {
    public:
        std::string filename;
        int num_face_vertices = 3; //仅支持三角形
        std::vector<Shape> shapes;
        OBJLoader(std::string inputfile);
    };

}