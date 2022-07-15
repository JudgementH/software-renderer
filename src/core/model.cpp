#include "model.hpp"

#include <omp.h>
#include <utility>
#include "../loader/obj_loader.hpp"

Model::Model(const std::string &filename) {
    std::string postfix = filename.substr(filename.find_last_of('.') + 1);
    if (postfix == "obj") {
        loader::OBJLoader loader(filename);
        shape_num = loader.shapes.size();
        if (shape_num > 1) {
            std::cout << "目前只能处理一个shape的模型\n";
            throw std::exception();
        }

        for (auto &shape: loader.shapes) {
            vertices.resize(shape.vertices.size());
            indices = shape.indices;
#pragma omp parallel for
            for (int i = 0; i < shape.vertices.size(); i++) {
                vertices[i].position = Eigen::Vector4f(shape.vertices[i].position.x,
                                                       shape.vertices[i].position.y,
                                                       shape.vertices[i].position.z,
                                                       1.0f);

                vertices[i].color = Eigen::Vector4f::Ones();

                vertices[i].normal = Eigen::Vector3f::Zero();

                if (shape.has_normal) {
                    vertices[i].normal = Eigen::Vector3f(shape.vertices[i].normal.x,
                                                         shape.vertices[i].normal.y,
                                                         shape.vertices[i].normal.z);
                }

                vertices[i].texcoord = Eigen::Vector2f::Zero();
                if (shape.has_texcoord) {
                    vertices[i].texcoord = Eigen::Vector2f(shape.vertices[i].texcoord.x,
                                                           shape.vertices[i].texcoord.y);
                }

            }
        }
    } else {
        throw std::exception();
    }
}

Model::Model(std::vector<Vertex> vertices, std::vector<int> indices)
        : vertices(std::move(vertices)), indices(std::move(indices)) {

}

void Model::setTexture(Texture tex) {
    texture = tex;
}
