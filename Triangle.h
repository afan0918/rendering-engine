//
// Created by afan on 2022/11/11.
//

#ifndef LAB4_TRIANGLE_H
#define LAB4_TRIANGLE_H

#include <utility>
#include <vector>
#include <cstdlib>
#include <utility>
#include <array>
#include <algorithm>
#include "Vector3d.h"
#include "Vector4d.h"

class Triangle {
public:
    /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
    Vector4d vertex[3] = {{0.0, 0.0, 0.0, 1.0},
                          {0.0, 0.0, 0.0, 1.0},
                          {0.0, 0.0, 0.0, 1.0}};
    /*Per vertex values*/
    Vector3d color[3]= {{0.0, 0.0, 0.0},
                        {0.0, 0.0, 0.0},
                        {0.0, 0.0, 0.0}}; //color at each vertex;
    // Vector2d tex_coords[3]; //texture u,v
    Vector3d normal; //normal vector for each vertex

    Triangle()= default;

    explicit Triangle(const std::array<Vector4d, 3> &vertexs) {
        vertex[0] = vertexs[0];
        vertex[1] = vertexs[1];
        vertex[2] = vertexs[2];
    }

//    void setTexCoord(int ind, Vector2d uv) {
//        tex_coords[ind] = uv;
//    }

    std::array<Vector4d, 3> toVector4() const {
        std::array<Vector4d, 3> res;
        std::transform(std::begin(vertex), std::end(vertex), res.begin(),
                       [](auto &vec) { return Vector4d(vec.x(), vec.y(), vec.z(), 1.0); });
        return res;
    }
};

double product(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3) {
    return (p2.x() - p1.x()) * (p3.y() - p1.y()) -
           (p2.y() - p1.y()) * (p3.x() - p1.x());
}

bool isInTriangle(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3, const Vector3d &o) {
    if (product(p1, p2, p3) < -0.001)
        return isInTriangle(p1, p3, p2, o);
    if (product(p1, p2, o) > -0.001 &&
        product(p2, p3, o) > -0.001 &&
        product(p3, p1, o) > -0.001)
        return true;
    return false;
}

double product(const Vector4d &p1, const Vector4d &p2, const Vector4d &p3) {
    return (p2.x() - p1.x()) * (p3.y() - p1.y()) -
           (p2.y() - p1.y()) * (p3.x() - p1.x());
}

bool isInTriangle(const Vector4d &p1, const Vector4d &p2, const Vector4d &p3, const Vector4d &o) {
    if (product(p1, p2, p3) < -0.001)
        return isInTriangle(p1, p3, p2, o);
    if (product(p1, p2, o) > -0.001 &&
        product(p2, p3, o) > -0.001 &&
        product(p3, p1, o) > -0.001)
        return true;
    return false;
}

#endif //LAB4_TRIANGLE_H
