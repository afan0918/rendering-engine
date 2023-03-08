//
// Created by afan on 2022/12/16.
//

#ifndef LAB4_SHADER_H
#define LAB4_SHADER_H

#include "Vector3d.h"
#include "Light.h"
#include <cmath>

Vector3d blinn_phong_shader(double _kd, double _ks, double gloss,
                            Vector3d view_pos, Vector3d eye_pos, const Vector3d color,
                            Vector3d normal, Vector3d amb_light_intensity,
                            const std::vector<Light> &lights) {
    Vector3d result_color = {0, 0, 0};
    normal = unit_vector(normal);

    for (auto &light: lights) {
        if (!light.is_used) continue;
        Vector3d light_dir = view_pos - light.point;
        Vector3d view_dir = view_pos - eye_pos;
        /// 光到物體的距離
        double r = light_dir.length();
        /// 半程向量
        Vector3d h = unit_vector(light_dir + view_dir);
        /// 漫反射
        Vector3d ld = (_kd * light.intensity * 20.0 / (r * r)) * std::max(0.0, dot(normal, light_dir));
        ld = ld * color;
        /// 高光的衰減程度
        double p = 150.0;
        /// 高光的能量
        Vector3d ls = (_ks * light.intensity * 20.0 / (r * r)) * pow(std::max(0.0, dot(normal, h)), gloss);

        result_color += (ld + ls);
    }

    /// 環境光
    Vector3d la = amb_light_intensity; // la = KaIa
    result_color += la * color;

    return result_color;
}

#endif //LAB4_SHADER_H