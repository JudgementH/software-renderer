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

    struct Index{
        int position_index;
        int normal_index;
        int texcoord_index;
    };

    struct Shape
    {
        std::vector<Position> position_list;
        std::vector<Normal> normal_list;
        std::vector<Texcoord> texcoord_list;
        std::vector<loader::Index> indices;
        bool has_normal = false;
        bool has_texcoord = false;
    };

    class Loader
    {
    };
}