#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <string>

#include <vector>

GLuint width = 600, height = 600;
int kut = 0;

typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 2.0f };

std::vector<glm::vec3> g_vs;
std::vector<glm::vec3> g_fs;
std::vector<glm::vec3> g_rs;
std::vector<glm::vec3> g_curve;
std::vector<glm::vec3> g_tangents;

glm::vec3 s = glm::vec3(0, 0, 1);
const double pi = 3.14159265;

bool poly = false;
bool tans = true;
bool airplane = false;

void myDisplay();
void myReshape(int width, int height);
void myDrawSquare();
void myRenderScene();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void idle();

std::vector<glm::vec3> object_scaling(std::vector<glm::vec3> object) {
    int obj_num = object.size();

    float min = object[0].x;
    float max = object[0].x;

    float x_min = object[0].x;
    float x_max = object[0].x;
    float y_min = object[0].y;
    float y_max = object[0].y;
    float z_min = object[0].z;
    float z_max = object[0].z;

    for (int i = 0; i < obj_num; i++) {
        if (object[i].x < min) min = object[i].x;
        if (object[i].y < min) min = object[i].y;
        if (object[i].z < min) min = object[i].z;
        if (object[i].x > max) max = object[i].x;
        if (object[i].y > max) max = object[i].y;
        if (object[i].z > max) max = object[i].z;

        if (object[i].x < x_min) x_min = object[i].x;
        if (object[i].x > x_max) x_max = object[i].x;
        if (object[i].y < y_min) y_min = object[i].y;
        if (object[i].y > y_max) y_max = object[i].y;
        if (object[i].z < z_min) z_min = object[i].z;
        if (object[i].z > z_max) z_max = object[i].z;
    }

    float x_middle = (x_max + x_min) / 2;
    float y_middle = (y_max + y_min) / 2;
    float z_middle = (z_max + z_min) / 2;

    glm::vec3 middle(x_middle, y_middle, z_middle);

    for (int i = 0; i < obj_num; i++) { //translacija u ishodiste
        object[i] = object[i] - middle;
    }

    float scale_f = std::abs(max) > std::abs(min) ? std::abs(max) : std::abs(min);
    scale_f = 1.0 / scale_f;

    for (int i = 0; i < object.size(); i++) {
        object[i].x *= scale_f;
        object[i].y *= scale_f;
        object[i].z *= scale_f;
        //printf("%f, %f, %f\n", object[i].x, object[i].y, object[i].z);
    }

    return object;
}

void load_airplane() {
    std::vector<glm::vec3> vs;
    std::vector<glm::vec3> fs;

    std::ifstream file("C:/Faks2/racani/objects/f16.obj");

    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        char c;
        std::istringstream stream(line);
        if (!(stream >> c)) continue;

        if (c == 'v') {
            float x, y, z;
            if (!(stream >> x >> y >> z)) {
                continue;
            }
            vs.push_back(glm::vec3(x, y, z));
        }
        else if (c == 'f') {
            float i1, i2, i3;
            if (!(stream >> i1 >> i2 >> i3)) {
                continue;
            }
            fs.push_back(glm::vec3(i1, i2, i3));
        }
        else if (c == '#') {
            printf("KOMENTAR\n");
        }
    }

    vs = object_scaling(vs);
    for (int i = 0; i < vs.size(); i++) {
        vs[i].x *= 1.0f / 10.0f;
        vs[i].y *= 1.0f / 10.0f;
        vs[i].z *= 1.0f / 10.0f;
        //printf("%f, %f, %f\n", object[i].x, object[i].y, object[i].z);
    }
    g_vs = vs;
    g_fs = fs;
}

int main(int argc, char** argv)
{
    load_airplane();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Primjer animacije");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    printf("Tipka: a/d - pomicanje ocista po x os +/-\n");
    printf("Tipka: w/s - pomicanje ocista po y os +/-\n");
    printf("Tipka: r - pocetno stanje\n");
    printf("esc: izlaz iz programa\n");

    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

void myDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    myRenderScene();
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    width = w; height = h;
    //glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 1, 100);
    //glFrustum(-1, 1, -1, 1, 1, 5);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
}

void draw_tangents() {
    glm::mat4x3 B = glm::mat4x3(
        -1, 2, -1,
        3, -4, 0,
        -3, 2, 1,
        1, 0, 0
    );

    std::vector<glm::vec3> tangents;

    const double epsilon = 1e-7;
    for (int segments = 1; segments <= g_rs.size() - 3; segments++) {
        glm::vec4 Rx = glm::vec4(g_rs[segments - 1].x, g_rs[segments].x, g_rs[segments + 1].x, g_rs[segments + 2].x);
        glm::vec4 Ry = glm::vec4(g_rs[segments - 1].y, g_rs[segments].y, g_rs[segments + 1].y, g_rs[segments + 2].y);
        glm::vec4 Rz = glm::vec4(g_rs[segments - 1].z, g_rs[segments].z, g_rs[segments + 1].z, g_rs[segments + 2].z);

        for (double t = 0; t < 1.0 - epsilon; t = t + 0.01) {
            glm::vec3 T = glm::vec3(t * t, t, 1);
            glm::vec4 TB = T * B * (1.0f / 2.0f);

            float x = glm::dot(TB, Rx);
            float y = glm::dot(TB, Ry);
            float z = glm::dot(TB, Rz);

            //printf("%f %f %f\n", x, y, z);

            glm::vec3 p = glm::vec3(x, y, z);
            p = glm::normalize(p);

            tangents.push_back(p);
        }
    }
    g_tangents = tangents;
    glColor3f(1, 0, 0);
    for (int i = 0; i < g_curve.size(); i = i + 25) {
        glBegin(GL_LINES);
        glVertex3f(g_curve[i].x, g_curve[i].y, g_curve[i].z);
        glVertex3f(g_curve[i].x + tangents[i].x / 10, g_curve[i].y + tangents[i].y / 10, g_curve[i].z + tangents[i].z / 10);
        glEnd();
    }
    glColor3f(0, 0, 0);
}

int c_t = 0;
void airplane_animation() {

    glm::vec3 e = glm::vec3(g_tangents[c_t].x, g_tangents[c_t].y, g_tangents[c_t].z);
    glm::vec3 os = glm::cross(s, e);

    double kut = acos(glm::dot(s, e));
    kut = kut * (180 / pi);

    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef(g_curve[c_t].x, g_curve[c_t].y, g_curve[c_t].z);
    glRotatef(kut, os.x, os.y, os.z);
    glColor3f(0, 0, 0);
    for (int i = 0; i < g_fs.size(); i++) {
        int i1 = static_cast<int>(g_fs[i].x) - 1;
        int i2 = static_cast<int>(g_fs[i].y) - 1;
        int i3 = static_cast<int>(g_fs[i].z) - 1;

        glBegin(GL_TRIANGLES);
        glVertex3f(g_vs[i1].x, g_vs[i1].y, g_vs[i1].z);
        glVertex3f(g_vs[i2].x, g_vs[i2].y, g_vs[i2].z);
        glVertex3f(g_vs[i3].x, g_vs[i3].y, g_vs[i3].z);
        glEnd();
    }
    c_t++;
    if (c_t == 100 * (g_rs.size() - 3)) c_t = 0;
    glFlush();
}

void myDrawSquare()
{
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    std::vector<glm::vec3> rs;
    std::ifstream file("C:/Faks2/racani/lab1/poligon.txt");

    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        float x, y, z;
        if (!(stream >> x >> y >> z)) {
            continue;
        }
        rs.push_back(glm::vec3(x, y, z));
    }

    g_rs = rs;

    glm::mat4 B = glm::mat4(
        -1, 3, -3, 1,
        3, -6, 0, 4,
        -3, 3, 3, 1,
        1, 0, 0, 0
    );

    std::vector<glm::vec3> curve;
    const double epsilon = 1e-7;
    for (int segments = 1; segments <= rs.size() - 3; segments++) {
        glm::vec4 Rx = glm::vec4(rs[segments - 1].x, rs[segments].x, rs[segments + 1].x, rs[segments + 2].x);
        glm::vec4 Ry = glm::vec4(rs[segments - 1].y, rs[segments].y, rs[segments + 1].y, rs[segments + 2].y);
        glm::vec4 Rz = glm::vec4(rs[segments - 1].z, rs[segments].z, rs[segments + 1].z, rs[segments + 2].z);

        for (double t = 0; t < 1.0 - epsilon; t = t + 0.01) {
            glm::vec4 T = glm::vec4(t * t * t, t * t, t, 1);
            glm::vec4 TB = T * B * (1.0f / 6.0f);

            float x = glm::dot(TB, Rx);
            float y = glm::dot(TB, Ry);
            float z = glm::dot(TB, Rz);

            //printf("%f %f %f\n", x, y, z);

            glm::vec3 p = glm::vec3(x, y, z);

            curve.push_back(p);
        }
    }

    curve = object_scaling(curve);
    g_curve = curve;
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < curve.size() - 1; i++) {
        glBegin(GL_LINES);
        glVertex3f(g_curve[i].x, g_curve[i].y, g_curve[i].z);
        glVertex3f(g_curve[i + 1].x, g_curve[i + 1].y, g_curve[i + 1].z);
        glEnd();
    }

    if (poly) {
        std::vector<glm::vec3> poly = object_scaling(rs);
        glColor3f(0.0f, 1.0f, 0.0f);
        for (int i = 0; i < poly.size() - 1; i++) {
            glBegin(GL_LINES);
            glVertex3f(poly[i].x, poly[i].y, poly[i].z);
            glVertex3f(poly[i + 1].x, poly[i + 1].y, poly[i + 1].z);
            glEnd();
        }
    }

    if (tans) draw_tangents();
    if (airplane) airplane_animation();
}

void myRenderScene()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    myDrawSquare();
    airplane_animation();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'p':
        if (poly) poly = false;
        else poly = true;
        break;
    case 't':
        if (tans) tans = false;
        else tans = true;
        break;
    case 'a': ociste.x = ociste.x + 0.2f;
        break;
    case 'd': ociste.x = ociste.x - 0.2f;
        break;
    case 'w': ociste.y = ociste.y + 0.2f;
        break;
    case 's': ociste.y = ociste.y - 0.2f;
        break;
    case 'r': ociste.x = 0.0; ociste.y = 0.0;
        break;
    case 27:  exit(0);
        break;
    }

    myReshape(width, height);
    glutPostRedisplay();
}

int now = 0;
int past = 0;
void idle() {
    now = glutGet(GLUT_ELAPSED_TIME);
    if ((now - past) > 10) {
        myDisplay();
        past = now;
    }
}