#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "obj_loader.hpp"

loader::OBJLoader::OBJLoader(const std::string inputfile) : filename(inputfile)
{
    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &shapes = reader.GetShapes();
    auto &attrib = reader.GetAttrib();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        Shape shape;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (num_face_vertices != fv)
            {
                throw "目前仅支持三角形";
            }

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                Vertex vertex;
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vertex.position.x = float(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
                vertex.position.y = float(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
                vertex.position.z = float(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    shape.has_normal = true;
                    vertex.normal.x = float(attrib.normals[3 * size_t(idx.normal_index) + 0]);
                    vertex.normal.y = float(attrib.normals[3 * size_t(idx.normal_index) + 1]);
                    vertex.normal.z = float(attrib.normals[3 * size_t(idx.normal_index) + 2]);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    shape.has_texcoord = true;
                    vertex.texcoord.x = float(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
                    vertex.texcoord.y = float(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

                shape.vertices.emplace_back(vertex);
            }
            index_offset += fv;

            // per-face material
        }

        this->shapes.push_back(shape);
    }
}