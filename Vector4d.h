//
// Created by afan on 2022/10/24.
//

#ifndef LAB4_VECTOR4D_H
#define LAB4_VECTOR4D_H

#include <cmath>
#include <iostream>
#include "Vector3d.h"

using std::sqrt;
using std::fabs;

class Vector4d {
public:
    double e[4];
    // Type aliases for Vector4d
    using point4 = Vector4d;   // 4D point
    using color = Vector4d;    // RGB color

    Vector4d() : e{0, 0, 0, 0} {}

    Vector4d(Vector3d v, double e3) : e{v.x(), v.y(), v.z(), e3} {}

    Vector4d(double e0, double e1, double e2, double e3) : e{e0, e1, e2, e3} {}

    double x() const { return e[0]; }

    double y() const { return e[1]; }

    double z() const { return e[2]; }

    double w() const { return e[3]; }

    Vector3d to_vector3d(){
        return {e[0], e[1], e[2]};
    }

    Vector4d operator-() const { return {-e[0], -e[1], -e[2], -e[3]}; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    Vector4d &operator+=(const Vector4d &v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        e[3] += v[3];
        return *this;
    }

    Vector4d &operator-=(const Vector4d &v) {
        e[0] -= v[0];
        e[1] -= v[1];
        e[2] -= v[2];
        e[3] -= v[3];
        return *this;
    }

    Vector4d &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        e[3] *= t;
        return *this;
    }

    Vector4d &operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3];
    }

    void copy(const Vector4d &v){
        for(int i = 0; i <4; i++) {
            e[i] = v[i];
        }
    }
};

// Vector4d Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector4d &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2] << ' ' << v.e[3];
}

inline Vector4d operator+(const Vector4d &u, const Vector4d &v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2], u.e[3] + v.e[3]};
}

inline Vector4d operator-(const Vector4d &u, const Vector4d &v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2], u.e[3] - v.e[3]};
}

inline Vector4d operator*(const Vector4d &u, const Vector4d &v) {
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2], u.e[3] * v.e[3]};
}

inline Vector4d operator*(double t, const Vector4d &v) {
    return {t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]};
}

inline Vector4d operator*(const Vector4d &v, double t) {
    return t * v;
}

inline Vector4d operator/(Vector4d v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector4d &u, const Vector4d &v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2]
           + u.e[3] * v.e[3];
}

inline Vector4d unit_vector(Vector4d v) {
    return v / v.length();
}

#endif //LAB4_VECTOR4D_H
