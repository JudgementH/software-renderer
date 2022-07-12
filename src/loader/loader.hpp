#pragma once
#include <vector>

namespace loader
{
    struct Position
    {
        float x;
        float y;
        float z;
    };

    struct Normal
    {
        float x;
        float y;
        float z;
    };

    struct Texcoord
    {
        float x;
        float y;
    };

    struct Vertex
    {
        Position position;
        Normal normal;
        Texcoord texcoord;
    };

    struct Shape
    {
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        bool has_normal = false;
        bool has_texcoord = false;
    };

    class Loader
    {
    };
}