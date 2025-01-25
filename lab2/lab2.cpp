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
const int maxParticles = 300;

void myDisplay();
void myReshape(int width, int height);
void myRenderScene();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void idle();

void drawSource() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);
    glPushMatrix();
    glTranslatef(-0.83f, 0.0f, 0.0f);
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
    glVertex3f(-0.83f, -0.055f, 0.0f);
    glVertex3f(-0.83f, -0.2f, 0.0f);
    glEnd();
}

void addParticle() {
    if (particles.size() >= maxParticles) return;
    Particle p;
    p.x = -0.8f;
    p.y = 0.0f;
    p.z = 0.0f;
    p.speed = 0.3f + (rand() % 1000) / 1000.0f;
    p.radius = 0.01f + (rand() % 50) / 1000.0f;
    float azimuth = ((rand() % 30) - 15) * pi / 180.0f;
    float elevation = ((rand() % 30) - 15) * pi / 180.0f;

    p.dx = cos(elevation) * cos(azimuth) * p.speed;
    p.dy = sin(elevation) * p.speed;
    p.dz = cos(elevation) * sin(azimuth) * p.speed;

    p.lifetime = 3.0f + (rand() % 5);
    //p.lifetime = 3.0f + (rand() % 5) + ((rand() % 10) * 0.1f);
    //p.lifetime = (rand() % 5) + ((rand() % 10) * 0.1f);

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

void idle() {
    static clock_t before = clock();
    clock_t now = clock();
    float deltaTime = float(now - before) / CLOCKS_PER_SEC;
    before = now;

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

    glutIdleFunc(idle);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
