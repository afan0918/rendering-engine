# rendering-engine
my rendering engine

## 使用lib

### OBJ Loader
https://github.com/Bly7/OBJ-Loader/blob/master/Source/OBJ_Loader.h

只有一個.h檔案，沒有依賴其他lib，我用他來讀取obj檔並寫入自製的三角形結構。

## build

* 複製以下指令即可編譯
```
mkdir build
cd build
cmake ..
make
```

## 功能概要

* 光照模型
```cpp=
Vector3d shader(double _kd, double _ks, double n,
                Vector3d view_pos, Vector3d eye_pos, Vector3d color,
                Vector3d normal, Vector3d amb_light_intensity,
                const std::vector<Light> &lights) {
    normal = unit_vector(normal);

    Vector3d result_color = {0, 0, 0};
    double diffuse = 0, specular = 0;

    for (auto &light: lights) {
        if (!light.is_used) continue;
        Vector3d light_dir = unit_vector(light.point - view_pos);
        double normal_dot_light = abs(dot(normal, light_dir));
        Vector3d reflect = unit_vector(2 * normal_dot_light * (normal - light_dir));
        Vector3d view_dir = unit_vector(eye_pos - view_pos);
        diffuse += _kd * normal_dot_light;
        specular += _ks * pow(abs(dot(reflect, view_dir)), n);
    }

    for (int i = 0; i < 3; i++) {
        result_color[i] += (amb_light_intensity[i] + diffuse) * color[i] + specular;
    }

    return result_color;
}
```
* 根據z-buffer進行計算
```cpp=
            for (int j = x_min; j <= x_max; j++) {
                for (int k = y_min; k <= y_max; k++) {
                    if (isInTriangle(points[0],
                                     points[1],
                                     points[2],
                                     Vector4d(j + 0.5, k + 0.5, 0.0, 0.0))) {
                        auto [alpha, beta, gamma] = computeBarycentric2D(j + 0.5, k + 0.5, points);
//                        cout << alpha << " " << beta << " " << gamma << endl;
                        auto Z = 1.0 / (alpha / points[0].w() + beta / points[1].w() + gamma / points[2].w());
                        auto zp = alpha * points[0].z() / points[0].w() + beta * points[1].z() / points[1].w() +
                                  gamma * points[2].z() / points[2].w();
                        zp *= Z;

//                        cout << zp << endl;
                        if (zp < z_buffer[j][k]) {
                            auto interpolated_color = interpolate(alpha, beta, gamma,
                                                                  obj.triangles[i]->color[0],
                                                                  obj.triangles[i]->color[1],
                                                                  obj.triangles[i]->color[2],
                                                                  alpha + beta + gamma);
                            drawDot(j, k, interpolated_color);
                            z_buffer[j][k] = zp;
                        }
                    }
                }
            }
```
* shader (雖然說是shader，但感覺給的參數怪怪的，所以有部分乾脆自己土炮，但應該是對的)
```cpp=
Vector3d blinn_phong_shader(double _kd, double _ks, double n,
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
        Vector3d ls = (_ks * light.intensity * 20.0 / (r * r)) * pow(std::max(0.0, dot(normal, h)), p);
        ls = ls;

        result_color += (ld + ls);
    }

    /// 環境光
    Vector3d la = amb_light_intensity; // la = KaIa
    result_color += la * color;

    return result_color;
}
```

![image](https://user-images.githubusercontent.com/70462625/223661777-9bbe8195-29f1-4621-bdbb-d4b32d4bab71.png)

![image](https://user-images.githubusercontent.com/70462625/223661804-adfc3837-03d5-4627-8bbe-37dd86d9f961.png)

![image](https://user-images.githubusercontent.com/70462625/223661825-aed674ba-ab0c-404b-8c57-23e57def702b.png)

![image](https://user-images.githubusercontent.com/70462625/223661845-cb310ab7-52c6-498c-aefa-18bbe9f600a6.png)

![image](https://user-images.githubusercontent.com/70462625/223661873-c398c848-ef13-4825-8a59-52678b1e1f2c.png)

![image](https://user-images.githubusercontent.com/70462625/223661910-039f6fa8-2b3b-41f2-8739-b43b57260c79.png)
光源太遠了，光會/(r^2)的速度衰弱，光強度係數(在shader.h中)改成90出來的結果。

![image](https://user-images.githubusercontent.com/70462625/223661942-b45c0c52-a864-4bd9-abdb-8919f7974a41.png)
一龍馬在test.in裡面，20萬面有夠多。

## 使用方式

以上述方式生成的 build 中開啟終端機，找到執行檔。(不要移動它的相對位置)

我自己是採用跟助教一樣的方式，直接透過寫好相對路徑來執行，以下指令就可以跑了。
```
./Lab4 lab4A.in
```

由於使用fgetc(stdin);來進行暫停，所以輸入任意鍵就可以繼續往下跑了，所有運算過程都有對過和列出來。

![](https://i.imgur.com/qh8JJPF.png)

## 我的檔案結構

### class

* Matrix4d : 4 * 4 矩陣
* Vector4d : 4 個 double的陣列
* Vector3d : 3 個 double的陣列
* Triangle : 利用Vector3d和Vector4d來存放三角形和其屬性
* Math : 做一點數學運算
* Main : 進行主要運算和繪製圖形
* Shader : 放 shader 函數
