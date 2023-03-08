//
// Created by afan on 2022/10/24.
//

#ifndef LAB4_VECTOR3D_H
#define LAB4_VECTOR3D_H

#include <cmath>
#include <iostream>
#include "obj/OBJ_Loader.h"

using std::sqrt;
using std::fabs;

class Vector3d {
public:
    double e[3];

    Vector3d() : e{0, 0, 0} {}

    Vector3d(objl::Vector3 &v) : e{v.X, v.Y, v.Z} {}

    Vector3d(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }

    double y() const { return e[1]; }

    double z() const { return e[2]; }

    Vector3d operator-() const { return {-e[0], -e[1], -e[2]}; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    Vector3d &operator+=(const Vector3d &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3d &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3d &operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double dot(Vector3d &v) const {
        return e[0] * v[0] + e[1] * v[1] + e[2] * v[2];
    }

    Vector3d cross(Vector3d &v) const {
        return {e[1] * v.e[2] - e[2] * v.e[1],
                e[2] * v.e[0] - e[0] * v.e[2],
                e[0] * v.e[1] - e[1] * v.e[0]};
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    void copy(Vector3d &v) {
        for (int i = 0; i < 3; i++) {
            e[i] = v[i];
        }
    }

    inline static double random_double(int min = 0, int max = 1) {
        return (max - min) * rand() / (RAND_MAX + max) + min;
    }

    inline static Vector3d random() {
        return Vector3d(random_double(), random_double(), random_double());
    }

    inline static Vector3d random(double min, double max) {
        return Vector3d(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// Vector3d Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector3d &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3d operator+(const Vector3d &u, const Vector3d &v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vector3d operator-(const Vector3d &u, const Vector3d &v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vector3d operator*(const Vector3d &u, const Vector3d &v) {
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline Vector3d operator*(double t, const Vector3d &v) {
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vector3d operator*(const Vector3d &v, double t) {
    return t * v;
}

inline Vector3d operator/(Vector3d v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector3d &u, const Vector3d &v) {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}

inline Vector3d cross(const Vector3d &u, const Vector3d &v) {
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
            u.e[2] * v.e[0] - u.e[0] * v.e[2],
            u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline Vector3d unit_vector(Vector3d v) {
    return v / v.length();
}

#endif //LAB4_VECTOR3D_H
