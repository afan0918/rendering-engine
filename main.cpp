#include <iostream>
#include <vector>
#include <cfloat>

#include <GLUT/glut.h>
#include "obj/OBJ_Loader.h"
#include "Vector3d.h"
#include "Triangle.h"
#include "Matrix4d.h"
#include "Light.h"
#include "Shader.h"

using namespace std;

int height = 600, width = 800;
float line_size = 1.0f;

vector<vector<double>> z_buffer;

Matrix4d world_translate_matrix;
Matrix4d world_scale_matrix;
Matrix4d world_rotate_matrix;
Matrix4d view_matrix;
Matrix4d project_matrix;

Vector3d eye_pos;
Vector3d ambient_KI_rgb;
Vector3d background_rgb;

Vector3d from_to;
Vector4d rectangle;
double scale_x, scale_y;
double shift_x, shift_y;

class OBJ {
public:
    string name;
    vector<Triangle *> triangles;
    Vector3d color;
    double Kd;// 漫反射係數
    double Ks;// 高光係數
    double gloss;// 光澤度
};

vector<Light> light(5);
vector<OBJ> objects;

void print_vector3d(Vector3d v) {
    cout << "vector3d\n";
    for (int i = 0; i < 3; i++) {
        cout << v[i] << " ";
    }
    cout << '\n';
}

void print_vector4d(Vector4d v) {
    cout << "vector4d\n";
    for (int i = 0; i < 4; i++) {
        cout << v[i] << " ";
    }
    cout << '\n';
}

void drawDot(int x, int y) {
    glPointSize(line_size);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawDot(int x, int y, Vector3d color) {
//    print_vector3d(color);
    glColor3d(color.x(), color.y(), color.z());
    glPointSize(line_size);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawLine(double x0, double x1, double y0, double y1) {
    double dx = x1 - x0;
    double dy = y1 - y0;

    if (abs(dx) > abs(dy)) {
        if (dx < 0) {    // 乾只依照x軸來做會有斷點，還超明顯= ="
            swap(x0, x1);
            swap(y0, y1);

            dx = x1 - x0;
            dy = y1 - y0;
        }
        for (int i = x0; i <= x1; i++) {
            drawDot(i, y0 + (int) (dy * (i - x0) / dx));
        }
    } else {
        if (dy < 0) {    // 乾只依照x軸來做會有斷點，還超明顯= ="
            swap(x0, x1);
            swap(y0, y1);

            dx = x1 - x0;
            dy = y1 - y0;
        }
        for (int i = y0; i <= y1; i++) {
            drawDot(x0 + (int) (dx * (i - y0) / dy), i);
        }
    }
}

void drawRectangle(int vxl, int vxr, int vyb, int vyt) {
    line_size = 3.0f;
    glColor3f(255, 255, 255);
    drawLine(vxr, vxl, vyb, vyb);
    drawLine(vxr, vxl, vyt, vyt);
    drawLine(vxr, vxr, vyb, vyt);
    drawLine(vxl, vxl, vyb, vyt);
    line_size = 1.0f;
    glColor3f(0, 255, 0);
}

std::vector<std::string> split(const std::string &str, const std::string &pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end - begin));
        }
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;
}

void view(double wxl, double wxr, double wyb, double wyt,
          double vxl, double vxr, double vyb, double vyt) {
    vxl /= 2, vxr /= 2, vyb /= 2, vyt /= 2;
    vxl += 0.5, vxr += 0.5, vyb += 0.5, vyt += 0.5;
    vxl *= width, vxr *= width;
    vyb *= height, vyt *= height;
//    vxl = 0, vxr = width;
//    vyb = 0, vyt = height;
    /// compute the scaling factor
    scale_x = (vxr - vxl) / (wxr - wxl);
    scale_y = (vyt - vyb) / (wyt - wyb);
    /// compute shift
    shift_x = vxl - scale_x * wxl;
    shift_y = vyb - scale_y * wyb;

    cout << scale_x << " " << scale_y << endl;
    cout << shift_x << " " << shift_y << endl;
    cout << vxl << " " << vxr << " " << vyb << " " << vyt << endl;

    rectangle = {vxl, vxr, vyb, vyt};
}

void clearScreen() {
    // clear the entire window to the background color
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    // flush the queue to actually paint the dots on the opengl window
    glFlush();
}

void draw() {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            drawDot(i, j, background_rgb);
        }
    }

    for (auto obj: objects) {
        for (int i = 0; i < obj.triangles.size(); i++) {
            for (int j = 0; j < 3; j++) {
                auto color = blinn_phong_shader(obj.Kd, obj.Ks, obj.gloss,
                                                obj.triangles[i]->vertex[j].to_vector3d(), eye_pos, obj.color,
                                                obj.triangles[i]->normal,
                                                ambient_KI_rgb, light);
                obj.triangles[i]->color[j] = color;
            }
        }
    }

    for (int i = 0; i < z_buffer.size(); i++) {
        for (int j = 0; j < z_buffer[i].size(); j++) {
            z_buffer[i][j] = DBL_MAX;
        }
    }

    for (auto obj: objects) {
        for (int i = 0; i < obj.triangles.size(); i++) {
            array<Vector4d, 3> points = obj.triangles[i]->toVector4();

            for (int j = 0; j < 3; j++) {
                points[j] = multiplication(view_matrix, points[j]);
                points[j] = multiplication(project_matrix, points[j]);
                points[j][0] = points[j][0] * scale_x / points[j][3] + shift_x;
                points[j][1] = points[j][1] * scale_y / points[j][3] + shift_y;
            }

            // bounding box
            double min_x = std::min(std::max(0.0, points[0][0]), std::min(points[1][0], points[2][0]));
            double max_x = std::max(std::min((double) width, points[0][0]), std::max(points[1][0], points[2][0]));
            double min_y = std::min(std::max(0.0, points[0][1]), std::min(points[1][1], points[2][1]));
            double max_y = std::max(std::min((double) height, points[0][1]), std::max(points[1][1], points[2][1]));

            int x_min = std::floor(max(min_x, rectangle[0]));
            int x_max = std::ceil(min(max_x, rectangle[1]));
            int y_min = std::floor(max(min_y, rectangle[2]));
            int y_max = std::ceil(min(max_y, rectangle[3]));

            for (int j = x_min; j <= x_max; j++) {
                for (int k = y_min; k <= y_max; k++) {
                    if (isInTriangle(points[0],
                                     points[1],
                                     points[2],
                                     Vector4d(j + 0.5, k + 0.5, 0.0, 0.0))) {
                        auto [alpha, beta, gamma] = computeBarycentric2D(j + 0.5, k + 0.5, points);

                        auto Z = 1.0 / (alpha / points[0].w() + beta / points[1].w() + gamma / points[2].w());
                        auto zp = alpha * points[0].z() / points[0].w() + beta * points[1].z() / points[1].w() +
                                  gamma * points[2].z() / points[2].w();
                        zp *= Z;

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
        }
    }

    drawRectangle(rectangle[0], rectangle[1],
                  rectangle[2], rectangle[3]);
}

// Display function
void display() {
//     clear the entire window to the background color
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(background_rgb.x(), background_rgb.y(), background_rgb.z(), 0);

    // draw the contents!!! Iterate your object's data structure!
    draw();

    // flush the queue to actually paint the dots on the opengl window
    glFlush();
}

// Main
int main(int argc, char *argv[]) {
    // initialize OpenGL utility toolkit (glut)
    glutInit(&argc, argv);

    string path = argv[1];
    ifstream inFile;
    inFile.open("../Data/" + path);
    string head;
    string line;
    inFile >> width >> height;/// 讀入寬高
    z_buffer.resize(height, vector<double>(width));

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);    // set display mode
    glutInitWindowSize(width, height);             // set window size
    glutInitWindowPosition(100, 50);                     // set window position on screen
    glutCreateWindow("LAB4 afan");                         // set window title
    glutDisplayFunc(display);                               // display function
    // misc setup
    gluOrtho2D(0, width, 0, height);
//    glutSwapBuffers();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    world_translate_matrix.identity_matrix();
    world_scale_matrix.identity_matrix();
    world_rotate_matrix.identity_matrix();
    view_matrix.identity_matrix();
    project_matrix.identity_matrix();

    while (inFile >> head) {
        getline(inFile, line);
        vector<string> strs = split(line, " ");

        if (head == "#") {
            cout << "this is #\n";
        } else if (head == "reset") {
            cout << "reset\n";
            world_translate_matrix.identity_matrix();
            world_scale_matrix.identity_matrix();
            world_rotate_matrix.identity_matrix();
        } else if (head == "translate") {
            cout << "translate " << strs[0] << " " << strs[1] << " " << strs[2] << '\n';
            world_translate_matrix.translate(stod(strs[0]), stod(strs[1]), stod(strs[2]));
        } else if (head == "scale") {
            cout << "scale " << strs[0] << " " << strs[1] << " " << strs[2] << '\n';
            world_scale_matrix.scale(stod(strs[0]), stod(strs[1]), stod(strs[2]));
        } else if (head == "rotate") {
            cout << "rotate " << strs[0] << " " << strs[1] << " " << strs[2] << '\n';
            world_rotate_matrix.rotate(stod(strs[0]), stod(strs[1]), stod(strs[2]));
        } else if (head == "clearData") {
            cout << "clearData\n";
            for (int i = 0; i < objects.size(); i++) {
                objects[i].triangles.clear();
            }
            objects.clear();
        } else if (head == "clearScreen") {
            // clear the screen and flush
            cout << "clearScreen\n";
            clearScreen();
        } else if (head == "object") {
            cout << "object\n";
            cout << strs[0] << ' ' << strs[1] << ' ' << strs[2] << ' ' << strs[3] << ' '
                 << strs[4] << ' ' << strs[5] << ' ' << strs[6] << '\n';

            OBJ object;
            object.name = strs[0];
            object.color = {stod(strs[1]), stod(strs[2]), stod(strs[3])};
            object.Kd = stod(strs[4]);
            object.Ks = stod(strs[5]);
            object.gloss = stod(strs[6]);

            // Initialize Loader
            objl::Loader Loader;
            // Load .obj File
            cout << "../Mesh/" + strs[0] << endl;
            bool loadout = Loader.LoadFile("../Mesh/" + strs[0]);
            if (loadout) {
                cout << "Loading " << strs[0] << " successfully.\n";
                for (auto mesh: Loader.LoadedMeshes) {
                    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
                        Triangle *t = new Triangle();
                        for (int j = 0; j < 3; j++) {
                            auto vertex = Vector4d(mesh.Vertices[i + j].Position.X,
                                                   mesh.Vertices[i + j].Position.Y,
                                                   mesh.Vertices[i + j].Position.Z, 1.0);
                            t->vertex[j] = multiplication(world_scale_matrix, vertex);
                            t->vertex[j] = multiplication(world_rotate_matrix, t->vertex[j]);
                            t->vertex[j] = multiplication(world_translate_matrix, t->vertex[j]);
                        }
                        t->normal = cross(t->vertex[2].to_vector3d() - t->vertex[0].to_vector3d(),
                                          t->vertex[1].to_vector3d() - t->vertex[0].to_vector3d());
                        object.triangles.push_back(t);
                    }
                }
                objects.push_back(object);
            } else {    // If not output an error
                // Output Error
                cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
            }
        } else if (head == "observer") {
            /// 攝影機的 x y z 位置 (ep)
            Vector3d from = {stod(strs[0]),
                             stod(strs[1]),
                             stod(strs[2])};
            /// 攝影機看的點 (COI)
            Vector3d to = {stod(strs[3]),
                           stod(strs[4]),
                           stod(strs[5])};
            double tilt = stod(strs[6]);// 以攝影機視線為軸的逆時針旋轉角度
            double _near = stod(strs[7]);// 為near平面到攝影機距離
            double _far = stod(strs[8]);// 為far平面到攝影機距離
            double _fovh = stod(strs[9]);// 為視野闊度(degree)

            eye_pos = from;
            from_to = from - to;

            Matrix4d eye_translate;
            eye_translate.identity_matrix();
            eye_translate.translate(-from.x(), -from.y(), -from.z());

            Matrix4d eye_mirror;
            eye_mirror.identity_matrix();
            eye_mirror.e[0][0] = -1;

            Matrix4d eye_tilt;
            eye_tilt.identity_matrix();
            eye_tilt.rotate(0, 0, tilt);

            Vector3d eye_view = to - from;
            Vector3d tmp_up = {0, 1, 0};

            Vector3d v1 = unit_vector(cross(tmp_up, eye_view));
            Vector3d v3 = unit_vector(Vector3d(eye_view));
            Vector3d v2 = unit_vector(cross(v3, v1));

            Matrix4d eye_grm;
            eye_grm.e[0][0] = v1.x();
            eye_grm.e[0][1] = v1.y();
            eye_grm.e[0][2] = v1.z();
            eye_grm.e[1][0] = v2.x();
            eye_grm.e[1][1] = v2.y();
            eye_grm.e[1][2] = v2.z();
            eye_grm.e[2][0] = v3.x();
            eye_grm.e[2][1] = v3.y();
            eye_grm.e[2][2] = v3.z();
            eye_grm.e[3][3] = 1.0;

            view_matrix.copy(eye_tilt);
            view_matrix.multiplication(eye_mirror);
            view_matrix.multiplication(eye_grm);
            view_matrix.multiplication(eye_translate);

            eye_translate.print_matrix("eye translate");
            eye_mirror.print_matrix("eye mirror");
            eye_grm.print_matrix("eye grm");
            eye_tilt.print_matrix("eye tilt");
            view_matrix.print_matrix("eye Matrix");

            _fovh = degreeToRadian(_fovh);
            project_matrix.identity_matrix();
            project_matrix.e[3][2] = tan(_fovh);
            project_matrix.e[2][2] = (_far / (_far - _near)) * tan(_fovh);
            project_matrix.e[2][3] = ((_near * _far) / (_near - _far)) * tan(_fovh);
            project_matrix.print_matrix("project matrix");
        } else if (head == "viewport") {
            cout << "view\n";
            cout << strs[0] << ' ' << strs[1] << ' ' << strs[2] << ' ' << strs[3] << '\n';

            view(stod(strs[0]), stod(strs[1]),
                 stod(strs[2]), stod(strs[3]),
                 stod(strs[0]), stod(strs[1]),
                 stod(strs[2]), stod(strs[3]));
        } else if (head == "ambient") { /// 設定環境光係數
            cout << "ambient\n";
            cout << strs[0] << ' ' << strs[1] << ' ' << strs[2] << '\n';

            ambient_KI_rgb = {stod(strs[0]), stod(strs[1]), stod(strs[2])};
        } else if (head == "background") { /// 設定背景顏色
            cout << "background\n";
            cout << strs[0] << ' ' << strs[1] << ' ' << strs[2] << '\n';

            background_rgb = {stod(strs[0]), stod(strs[1]), stod(strs[2])};

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(background_rgb.x(), background_rgb.y(), background_rgb.z(), 0);
        } else if (head == "light") {
            cout << "light\n";
            cout << strs[0] << ' ' << strs[1] << ' ' << strs[2] << ' ' << strs[3] << ' '
                 << strs[4] << ' ' << strs[5] << ' ' << strs[6] << '\n';

            int light_index = stoi(strs[0]);
            if (light_index < light.size()) {
                light[light_index].is_used = true;
                light[light_index].intensity = {stod(strs[1]), stod(strs[2]), stod(strs[3])};
                light[light_index].point = {stod(strs[4]), stod(strs[5]), stod(strs[6])};
            } else {
                cout << "ERROR :　light_index >= light_max_num\n";
            }
        } else if (head == "display") {
            clearScreen();
            display();
            fgetc(stdin);
        } else if (head == "end") {
            cout << "end\n";
        } else {
            cout << "ERROR: Invalid input\n";
        }
    }

    glutMainLoop();
}



