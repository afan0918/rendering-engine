//
// Created by afan on 2022/11/13.
//

#ifndef LAB4_MATH_H
#define LAB4_MATH_H

static double degreeToRadian(double degree) {
    return degree * 3.1415926 / 180;
}

static std::tuple<double, double, double> computeBarycentric2D(double x, double y, std::array<Vector4d, 3> v) {
    double c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) /
                (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() -
                 v[2].x() * v[1].y());
    double c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) /
                (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() -
                 v[0].x() * v[2].y());
    double c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) /
                (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() -
                 v[1].x() * v[0].y());
    return {c1, c2, c3};
}

Vector3d interpolate(double alpha, double beta, double gamma,
                     const Vector3d &vert1, const Vector3d &vert2, const Vector3d &vert3,
                     double weight) {
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

#endif //LAB4_MATH_H
