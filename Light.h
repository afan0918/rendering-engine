//
// Created by afan on 2022/12/16.
//

#ifndef LAB4_LIGHT_H
#define LAB4_LIGHT_H

#include "Vector3d.h"

class Light {
public:
    bool is_used = false;// 目前不發光
    Vector3d point;// 座標點
    Vector3d intensity = {1.0, 1.0, 1.0};// 光強度
};

#endif //LAB4_LIGHT_H
