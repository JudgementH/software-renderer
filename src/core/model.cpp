#include "model.hpp"

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
            for (int i = 0; i < shape.indices.size(); i++) {

                int idx = shape.indices[i];
                indices.emplace_back(idx);


                Eigen::Vector4f position = Eigen::Vector4f(shape.vertices[idx].position.x,
                                                           shape.vertices[idx].position.y,
                                                           shape.vertices[idx].position.z,
                                                           1.0f);
                Eigen::Vector4f color = Eigen::Vector4f::Ones();
                Eigen::Vector3f normal = Eigen::Vector3f::Zero();
                if (shape.has_normal) {
                    normal = Eigen::Vector3f(shape.vertices[idx].normal.x,
                                             shape.vertices[idx].normal.y,
                                             shape.vertices[idx].normal.z);
                }

                Vertex v(position, color, normal);

                vertices[idx] = v;
            }
        }
    } else {
        throw std::exception();
    }
}

Model::Model(std::vector<Vertex> vertices, std::vector<int> indices)
        : vertices(std::move(vertices)), indices(std::move(indices)) {

}
