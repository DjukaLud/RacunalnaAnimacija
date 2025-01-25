#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include <GL/glut.h>
#include <GL/freeglut.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>

const double pi = 3.14159265;
GLuint width = 600, height = 600;
int kut = 0;
std::vector<glm::vec3> g_curve;
int counter = 0;

typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 2.0f };

struct Particle {
    float x, y, z;
    float dx, dy, dz;
    float lifetime;
    float speed;
    float radius;
};

std::vector<Particle> particles;
const int maxParticles = 1000;

void myDisplay();
void myReshape(int width, int height);
void myRenderScene();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void idle();

void drawSource() {
    if(g_curve.empty()) return; 
    glm::vec3 currentPosition = g_curve[counter];

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);

    glPushMatrix();
    glTranslatef(-0.86f + currentPosition.x, currentPosition.y, currentPosition.z);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    int segments = 50;
    float radius = 0.06f;
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * pi * float(i) / float(segments);
        glVertex3f(cos(angle) * radius, sin(angle) * radius, 0.0f);
    }
    glEnd();

    glPopMatrix();

    glBegin(GL_LINES);
    glVertex3f(-0.86f + currentPosition.x, currentPosition.y - 0.055f, currentPosition.z);
    glVertex3f(-0.86f + currentPosition.x, currentPosition.y - 0.2f, currentPosition.z);
    glEnd();
}


void calculateCurve() {
    std::vector<glm::vec3> curve;
    std::vector<glm::vec3> rs;
    std::ifstream file("C:/Faks2/racani/lab2/poligon2.txt");

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        float x, y, z;
        if (!(stream >> x >> y >> z)) {
            continue;
        }
        rs.push_back(glm::vec3(x, y, z));
    }
    file.close();

    glm::mat4 B = glm::mat4(
        -1, 3, -3, 1,
        3, -6, 3, 0,
        -3, 3, 0, 0,
        1, 0, 0, 0
    );

    for (int i = 0; i <= rs.size() - 4; i++) {
        glm::vec4 Rx = glm::vec4(rs[i].x, rs[i + 1].x, rs[i + 2].x, rs[i + 3].x);
        glm::vec4 Ry = glm::vec4(rs[i].y, rs[i + 1].y, rs[i + 2].y, rs[i + 3].y);
        glm::vec4 Rz = glm::vec4(rs[i].z, rs[i + 1].z, rs[i + 2].z, rs[i + 3].z);

        for (double t = 0; t <= 1.0; t += 0.01) {
            glm::vec4 T = glm::vec4(t * t * t, t * t, t, 1);
            glm::vec4 TB = T * B * (1.0f / 6.0f);

            float x = glm::dot(TB, Rx);
            float y = glm::dot(TB, Ry);
            float z = glm::dot(TB, Rz);

            curve.push_back(glm::vec3(x, y, z));
        }
    }
    g_curve = curve;
}


void addParticle() {
    if (particles.size() >= maxParticles || g_curve.empty()) return;

    if (counter >= g_curve.size()) {
        counter = 0;
    }

    Particle p;
    p.x = g_curve[counter].x - 0.83f;
    p.y = g_curve[counter].y;
    p.z = g_curve[counter].z;
    p.speed = 0.3f + (rand() % 1000) / 1000.0f;
    p.radius = 0.01f + (rand() % 50) / 1000.0f;

    float azimuth = ((rand() % 30) - 15) * pi / 180.0f;
    float elevation = ((rand() % 30) - 15) * pi / 180.0f;

    p.dx = cos(elevation) * cos(azimuth) * p.speed;
    p.dy = sin(elevation) * p.speed;
    p.dz = cos(elevation) * sin(azimuth) * p.speed;

    p.lifetime = 3.0f + (rand() % 5);
    particles.push_back(p);
}


void updateParticles(float deltaTime) {
    for (size_t i = 0; i < particles.size(); ) {
        particles[i].x += particles[i].dx * deltaTime;
        particles[i].y += particles[i].dy * deltaTime;
        particles[i].z += particles[i].dz * deltaTime;
        particles[i].dy += 0.05f * deltaTime;
        if (particles[i].dx > 0) {
            particles[i].dx -= 0.05f * deltaTime;
        }

        particles[i].lifetime -= deltaTime;
        if (particles[i].lifetime <= 0) {
            particles.erase(particles.begin() + i);
        }
        else {
            i++;
        }
    }
}

void drawParticles() {
    for (const auto& p : particles) {
        glPushMatrix();
        glTranslatef(p.x, p.y, p.z);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.5f, 0.7f, 1.0f, 0.3f);

        glutSolidSphere(p.radius, 10, 10);
        glPopMatrix();
    }
}

int count_helper = 0;
void idle() {
    static clock_t before = clock();
    clock_t now = clock();
    float deltaTime = float(now - before) / CLOCKS_PER_SEC;
    before = now;

    if (counter >= g_curve.size() - 1) {
        counter = 0;
    }
    //printf("%d\n", counter);
    if (count_helper == 5) {
        counter++;
        count_helper = 0;
    }
    count_helper++;
    addParticle();
    updateParticles(deltaTime);

    glutPostRedisplay();
}

void myDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void myRenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSource();
    drawParticles();
    //glutSwapBuffers();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'a': ociste.x = ociste.x + 0.2f;
        break;
    case 'd': ociste.x = ociste.x - 0.2f;
        break;
    case 'w': ociste.y = ociste.y + 0.2f;
        break;
    case 's': ociste.y = ociste.y - 0.2f;
        break;
    case 'q': ociste.z = ociste.z + 0.2f;
        break;
    case 'e': ociste.z = ociste.z - 0.2f;
        break;
    case 'r': ociste.x = 0.0; ociste.y = 0.0; ociste.z = 0.0;
        break;
    case 27:  exit(0);
        break;
    }

    myReshape(width, height);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    srand(time(nullptr));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Particles");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    printf("Tipka: a/d - pomicanje ocista po x os +/-\n");
    printf("Tipka: w/s - pomicanje ocista po y os +/-\n");
    printf("Tipka: r - pocetno stanje\n");
    printf("esc: izlaz iz programa\n");

    calculateCurve();

    glutIdleFunc(idle);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}