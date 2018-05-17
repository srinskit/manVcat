/*
    Made with ❤ by srinSkit.
    Created on 10 March 2018.
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include "Loader.h"
#include "Model.h"
#include "Graphics.h"
#include "Common.h"

int win;
extern Tank *mainTank;
int nBullets = 0;
int gridx, gridy, drawGridCount = 0, drawPath = 0;
bool autoMove = false;
stack<Cell *> moveStack, tmpStack;
stack<char> keyStack;
#define SHOWPATHDELAY 50

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Model::renderStatic();
    Model::renderDynamic();
    Model::nextFrame();
    if (drawGridCount > 0) {
        if (drawGridCount == SHOWPATHDELAY) {
            moveStack = mainTank->autoPilot(gridx, gridy);
            if (!moveStack.empty())
                drawPath = SHOWPATHDELAY;
        }
        glColor3ub(255, 255, 255);
        glBegin(GL_LINE_LOOP);
        glVertex2i(gridx * cellWidth, gridy * cellHeight);
        glVertex2i(gridx * cellWidth + cellWidth, gridy * cellHeight);
        glVertex2i(gridx * cellWidth + cellWidth, gridy * cellHeight + cellHeight);
        glVertex2i(gridx * cellWidth, gridy * cellHeight + cellHeight);
        glVertex2i(gridx * cellWidth, gridy * cellHeight);
        glEnd();
        drawGridCount--;
    }
    if (drawPath > 0) {
        glColor3ub(255, 255, 255);
        glBegin(GL_LINE_STRIP);
        tmpStack = moveStack;
        while (!tmpStack.empty()) {
            auto res = tmpStack.top();
            tmpStack.pop();
            glVertex2i(res->gridx * cellWidth + cellWidth / 2, res->gridy * cellHeight + cellHeight / 2);
        }
        glEnd();
        drawPath--;
        if (drawPath <= 0)
            mainTank->autoMoveEnable();
    }
//    if (!moveStack.empty()) {
//        auto node = moveStack.top();
//        auto theta = atan2(node->gridy * cellHeight + cellHeight / 2 - mainTank->y,
//                           node->gridx * cellHeight + cellWidth / 2 - mainTank->x);
//    }
    glutSwapBuffers();

}


void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(w / 2 - h / 2, 0, h, h);
}

void initOpenGL() {
    gluOrtho2D(0, worldWidth, 0, worldHeight);
    glClearColor(0, 0, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void deInitOpenGl() {
    glDisableClientState(GL_VERTEX_ARRAY);
}

void onKeyPressed(unsigned char key, int x1, int y1) {
    switch (key) {
        case 'q':
            printf("Score: %d!", max(0, score));
            glutDestroyWindow(win);
            break;
        case 'w':
        case 'a':
        case 's':
        case 'd':
        case 'j':
        case 'l':
            mainTank->actionQ.push(key);
            break;
        case 'k':
            if (nBullets < 30) {
                mainTank->actionQ.push(key);
                nBullets++;
            }
            break;
        default:
            break;
    }
    while (!moveStack.empty())moveStack.pop();
}

void onMouseClick(int button, int state, int x, int y) {
    if (state != GLUT_DOWN)return;
    gridx = x;
    gridy = y;
    screenToGrid(gridx, gridy);
    drawGridCount = SHOWPATHDELAY;
}

int main(int argc, char **argv) {
    load();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(worldWidth, worldHeight);
    glutInitWindowPosition(2000, 0);
    win = glutCreateWindow("manVcat");
//    glutFullScreen();
    initOpenGL();
    glutKeyboardFunc(onKeyPressed);
    glutMouseFunc(onMouseClick);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    deInitOpenGl();
    unload();
    return 0;
}
