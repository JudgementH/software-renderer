#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include <iostream>
#include "obj_loader.hpp"

loader::OBJLoader::OBJLoader(std::string inputfile) : filename(inputfile) {
    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &shapes = reader.GetShapes();
    auto &attrib = reader.GetAttrib();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        Shape shape;

        shape.indices.resize(shapes[s].mesh.indices.size());
        shape.position_list.resize(attrib.vertices.size());
        shape.normal_list.resize(attrib.normals.size());
        shape.texcoord_list.resize(attrib.texcoords.size());

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (fv != 3) {
                std::cout << "目前只支持三角形\n";
                throw std::exception();
            }

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                shape.indices[index_offset + v] = {idx.vertex_index, idx.normal_index, idx.texcoord_index};
                shape.position_list[idx.vertex_index] = {float(attrib.vertices[3 * size_t(idx.vertex_index) + 0]),
                                                         float(attrib.vertices[3 * size_t(idx.vertex_index) + 1]),
                                                         float(attrib.vertices[3 * size_t(idx.vertex_index) + 2])};

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    shape.has_normal = true;
                    shape.normal_list[idx.normal_index] = {float(attrib.normals[3 * size_t(idx.normal_index) + 0]),
                                                           float(attrib.normals[3 * size_t(idx.normal_index) + 1]),
                                                           float(attrib.normals[3 * size_t(idx.normal_index) + 2])};

                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    shape.has_texcoord = true;
                    shape.texcoord_list[idx.texcoord_index] = {
                            float(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]),
                            float(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1])};
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

            }
            index_offset += fv;

            // per-face material
        }

        this->shapes.push_back(shape);
    }
}
