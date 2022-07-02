#include "model.hpp"
#include "../loader/obj_loader.hpp"

Model::Model(const std::string filename)
{
    std::string postfix = filename.substr(filename.find_last_of('.') + 1);
    if (postfix == "obj")
    {
        loader::OBJLoader loader(filename);
        shape_num = loader.shapes.size();
        for (auto &shape : loader.shapes)
        {
            for (int i = 0; i < shape.vertices.size(); i = i + 3)
            {
                Vertex v[3];
                for (int j = 0; j < 3; j++)
                {
                    v[j].position = Eigen::Vector4f(shape.vertices[i + j].position.x,
                                                    shape.vertices[i + j].position.y,
                                                    shape.vertices[i + j].position.z,
                                                    1.0f);
                }

                if (shape.has_normal)
                    for (int j = 0; j < 3; j++)
                    {
                        v[j].normal = Eigen::Vector3f(shape.vertices[i + j].normal.x,
                                                      shape.vertices[i + j].normal.y,
                                                      shape.vertices[i + j].normal.z);
                    }

                faces.emplace_back(v[0], v[1], v[2]);
            }
        }
    }
    else
    {
        throw "不支持的文件格式";
    }

    for (auto &face : faces)
    {
        for (auto &v : face.vertices)
        {
            allVertices.emplace_back(v);
        }
    }
}