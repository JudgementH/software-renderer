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
            std::cout<<"aaaaa "<<shape.indices.size()<<std::endl;
            position_list.resize(shape.position_list.size());
            normal_list.resize(shape.normal_list.size());
            texcoord_list.resize(shape.texcoord_list.size());
            indices.resize(shape.indices.size());

            std::cout<<"bbbbb "<<indices.size()<<std::endl;

            if (shape.indices.size() % 3 != 0) {
                std::cout << "index个数必须是3的倍数\n";
                throw std::exception();
            }

#pragma omp parallel for
            for (int i = 0; i < shape.indices.size(); i++) {
                indices[i] = {shape.indices[i].position_index,
                              shape.indices[i].texcoord_index,
                              shape.indices[i].normal_index};

                int position_index = indices[i].position_index;
                int normal_index = indices[i].normal_index;
                int texcoord_index = indices[i].texcoord_index;

                position_list[i] = Eigen::Vector4f(shape.position_list[position_index].x,
                                                   shape.position_list[position_index].y,
                                                   shape.position_list[position_index].z,
                                                   1.0f);

                normal_list[i] = Eigen::Vector3f::Zero();
                if (shape.has_normal) {
                    normal_list[i] = Eigen::Vector3f(shape.normal_list[normal_index].x,
                                                     shape.normal_list[normal_index].y,
                                                     shape.normal_list[normal_index].z);
                }

                texcoord_list[i] = Eigen::Vector2f::Zero();
                if (shape.has_texcoord) {
                    texcoord_list[i] = Eigen::Vector2f(shape.texcoord_list[texcoord_index].x,
                                                       shape.texcoord_list[texcoord_index].y);
                }

            }
        }
    } else {
        throw std::exception();
    }

    face_num = int(indices.size()) / 3;
}


void Model::setTexture(Texture tex) {
    texture = tex;
}

Triangle Model::getFace(int i) {
    if (i < 0 || i >= face_num) {
        std::cout << "Triangle out of range" << std::endl;
        throw std::exception();
    }
    int index = 3 * i;
    Vertex v[3];
    for (int i = 0; i < 3; i++) {
        v[i].position = position_list[indices[index + i].position_index];
        v[i].normal = normal_list[indices[index + i].normal_index];
        v[i].texcoord = texcoord_list[indices[index + i].texcoord_index];
    }
    Triangle triangle(v[0], v[1], v[2]);
    return triangle;
}

