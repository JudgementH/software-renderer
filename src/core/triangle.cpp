#include <Eigen/Eigen>
#include "triangle.hpp"

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;

    Eigen::AlignedBox3f box;
    for (int i = 0; i < 3; i++)
    {
        Eigen::Vector3f _v(vertices[i].position.x(), vertices[i].position.y(), vertices[i].position.z());
        box.extend(_v);
    }

    boxMin = box.min();
    boxMax = box.max();
}

bool Triangle::inside(float x, float y) const
{
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