/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/


#include "Graphics.h"
#include <GL/gl.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "Common.h"

#define SAFETY_PAD 10

int rangeRandom(int low, int high) {
    return rand() % (high - low + 1) + low;
}

void screenToWorld(int &x, int &y) {
    y = static_cast<int>((float) worldHeight * (screenHeight - y) / screenHeight);
    x = static_cast<int>((float) worldWidth * (x - (float) (screenWidth - screenHeight) / 2) / screenHeight);
    limitX(x);
}

void screenToGrid(int &x, int &y) {
    screenToWorld(x, y);
    x = x / cellWidth;
    y = y / cellHeight;
}

void worldToGrid(int &x, int &y) {
    x = x / cellWidth;
    y = y / cellHeight;
}

void circle(int xc, int yc, int radius, bool fill) {
    int x = 0, y = radius, p = 5 / 4 - radius;
    int inc = 1 + 2 * x, dec = 2 * y;
    int count = 0;
    auto buff = new GLint[((int) (radius * M_SQRT2) + SAFETY_PAD) * 4];
    while (x <= y) {
        buff[count++] = x;
        buff[count++] = y;
        buff[count++] = x;
        buff[count++] = -y;
        if (p >= 0) {
            --y;
            dec -= 2;
            p -= dec;
        }
        ++x;
        inc += 2;
        p += inc;
    }
    count /= 4;
    glPushMatrix();
    glTranslatef(xc, yc, 0);
    auto fig = fill ? GL_LINES : GL_POINTS;
    for (int i = 0; i < 4; ++i) {
        glRotated(i * 90, 0, 0, 1);
        glVertexPointer(2, GL_INT, 2 * sizeof(GLint), buff);
        glDrawArrays(fig, 0, 2 * count);
    }
    glPopMatrix();
}

void gridToWorld(int &x, int &y) {
    x = x * cellWidth + cellWidth / 2;
    y = y * cellHeight + cellHeight / 2;
}
