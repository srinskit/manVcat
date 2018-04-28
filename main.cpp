/*
    Made with ❤ by srinSkit.
    Created on 10 March 2018.
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <cstdio>
#include "Loader.h"
#include "Model.h"

int win;
int width = 800, height = 800;
extern Tank *mainTank;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Model::nextFrame();
    Model::renderStatic();
    Model::renderDynamic();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(w / 2 - h / 2, 0, h, h);
}

void initOpenGL() {
    gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void deInitOpenGl() {
    glDisableClientState(GL_VERTEX_ARRAY);
}

void onKeyPressed(unsigned char key, int x1, int y1) {
    switch (key) {
        case 'q':
            glutDestroyWindow(win);
            break;
        case 'w':
            mainTank->move(+1);
            break;
        case 'a':
            mainTank->turn(+1);
            break;
        case 's':
            mainTank->move(-1);
            break;
        case 'd':
            mainTank->turn(-1);
            break;
        case 'j':
            mainTank->rotate(+1);
            break;
        case 'k':
            mainTank->fire();
            break;
        case 'l':
            mainTank->rotate(-1);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    load();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//    glutInitWindowSize(width, height);
    glutInitWindowPosition(2000, 0);
    win = glutCreateWindow("manVcat");
//    glutFullScreen();
    initOpenGL();
    glutKeyboardFunc(onKeyPressed);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    deInitOpenGl();
    unload();
    return 0;
}