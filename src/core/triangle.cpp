#include <Eigen/Eigen>
#include "triangle.hpp"

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;

    Eigen::AlignedBox3f box;
    for (int i = 0; i < 3; i++) {
        Eigen::Vector3f _v(vertices[i].position.x(), vertices[i].position.y(), vertices[i].position.z());
        box.extend(_v);
    }

    boxMin = box.min();
    boxMax = box.max();

    //normal
    Eigen::Vector3f ab = vertices[1].position.head<3>() - vertices[0].position.head<3>();
    Eigen::Vector3f bc = vertices[2].position.head<3>() - vertices[0].position.head<3>();

    normal = ab.cross(bc).normalized();
}

bool Triangle::inside(float x, float y) const {
    Eigen::Vector3f point(x, y, 1.0f);
    Eigen::Vector3f _v[3];
    for (int i = 0; i < 3; i++)
        _v[i] = {vertices[i].position.x(), vertices[i].position.y(), 1.0f};

    Eigen::Vector3f ap = point - _v[0];
    Eigen::Vector3f bp = point - _v[1];
    Eigen::Vector3f cp = point - _v[2];
    float sign1 = ap.cross(_v[1] - _v[0]).z();
    float sign2 = bp.cross(_v[2] - _v[1]).z();
    float sign3 = cp.cross(_v[0] - _v[2]).z();
    return (sign1 >= 0 && sign2 >= 0 && sign3 >= 0) || (sign1 <= 0 && sign2 <= 0 && sign3 <= 0);
}

std::tuple<float, float, float> Triangle::computeBarycentric2D(float x, float y) {
    Eigen::Vector2f v[3];
    for (int i = 0; i < 3; i++) {
        v[i] = vertices[i].position.head<2>();
    }
    float w0 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) /
               (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() -
                v[2].x() * v[1].y());
    float w1 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) /
               (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() -
                v[0].x() * v[2].y());
    // float w2 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) /
    //            (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() -
    //             v[1].x() * v[0].y());
    float w2 = 1.0f - w0 - w1;
    return {w0, w1, w2};
}

bool Triangle::crossBack(Eigen::Vector3f ray) const {
    /**
     * return true if view_ray see the back of face
     * return false, if view_ray see the front of face
     */
    float cos_ = ray.dot(normal);
    return cos_ > 0;
}
