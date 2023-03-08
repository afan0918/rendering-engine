//
// Created by afan on 2022/10/20.
//

#ifndef LAB4_MATRIX4d_H
#define LAB4_MATRIX4d_H

#include "Vector4d.h"
#include "Math.h"

class Matrix4d {
public:
    double e[4][4];

    Matrix4d(Matrix4d const &m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] = m.e[i][j];
            }
        }
    }

    explicit Matrix4d(double initial_value = 0.0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] = initial_value;
            }
        }
    }

    explicit Matrix4d(double _matrix[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] = _matrix[i][j];
            }
        }
    }

    /// 轉單位矩陣
    void identity_matrix() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j)
                    e[i][j] = 1.0;
                else
                    e[i][j] = 0.0;
            }
        }
    }

    void print_matrix(std::string matrix_name = "") {
        std::cout << matrix_name << '\n';
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << e[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    void translate(double x, double y, double z) {
        e[0][3] += x;
        e[1][3] += y;
        e[2][3] += z;
    }

    void scale(double x, double y, double z) {
        e[0][0] *= x;
        e[1][1] *= y;
        e[2][2] *= z;
    }

    void rotate(double theta_x, double theta_y, double theta_z) {
        if (theta_x != 0.0) {
            theta_x = degreeToRadian(theta_x);
            Matrix4d m_rotation_x;
            m_rotation_x.identity_matrix();

            m_rotation_x.e[1][1] = cos(theta_x);
            m_rotation_x.e[1][2] = -sin(theta_x);
            m_rotation_x.e[2][1] = sin(theta_x);
            m_rotation_x.e[2][2] = cos(theta_x);

            multiplication(m_rotation_x);
        }

        if (theta_y != 0.0) {
            theta_y = degreeToRadian(theta_y);
            Matrix4d m_rotation_y;
            m_rotation_y.identity_matrix();

            m_rotation_y.e[0][0] = cos(theta_y);
            m_rotation_y.e[0][2] = sin(theta_y);
            m_rotation_y.e[2][0] = -sin(theta_y);
            m_rotation_y.e[2][2] = cos(theta_y);

            multiplication(m_rotation_y);
        }

        if (theta_z != 0.0) {
            theta_z = degreeToRadian(theta_z);
            Matrix4d m_rotation_z;
            m_rotation_z.identity_matrix();

            m_rotation_z.e[0][0] = cos(theta_z);
            m_rotation_z.e[0][1] = -sin(theta_z);
            m_rotation_z.e[1][0] = sin(theta_z);
            m_rotation_z.e[1][1] = cos(theta_z);

            multiplication(m_rotation_z);
        }
    }

    void copy(const Matrix4d &m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] = m.e[i][j];
            }
        }
    }

    void multiplication(Matrix4d m) {
        double tmp[4][4] = {{0.0}};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    tmp[i][j] += e[i][k] * m[k][j];
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    e[i][j] = tmp[i][j];
                }
            }
        }
    }

    const double *operator[](int i) const { return e[i]; }

    Matrix4d &operator+=(const Matrix4d &v) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] += v[i][j];
            }
        }
        return *this;
    }

    Matrix4d &operator-=(const Matrix4d &v) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] -= v[i][j];
            }
        }
        return *this;
    }

    Matrix4d &operator*=(const double t) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                e[i][j] *= t;
            }
        }
        return *this;
    }

    Matrix4d &operator*(const Matrix4d &m) {
        Matrix4d *result = new Matrix4d(0.0);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result->e[i][j] += e[i][k] * m[k][j];
                }
            }
        }

        return *result;
    }

    Vector4d &operator*(const Vector4d &v) {
        Vector4d *result = new Vector4d();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result->e[i] += e[i][j] * v[j];
            }
        }
        return *result;
    }

    Matrix4d &operator/=(const double t) {
        return *this *= 1 / t;
    }
};

inline Matrix4d multiplication(Matrix4d m1, Matrix4d m2) {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.e[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

inline Vector4d multiplication(Matrix4d m, Vector4d v) {
    Vector4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.e[i] += m[i][j] * v[j];
        }
    }
    return result;
}

#endif //LAB4_MATRIX4d_H
