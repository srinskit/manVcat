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
#include <stack>
#include <vector>
#include <algorithm>

#define RADIAN(x) ((x)*M_PI/180.0)

using namespace std;
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

stack<vector<vector<double>> *> matrixStack;
vector<vector<double>> *matrix = nullptr;

void loadGraphicsMod() {
    matrix = new vector<vector<double>>(3, vector<double>(3));
    for (int i = 0; i < matrix->size(); ++i)
        for (int j = 0; j < (*matrix)[i].size(); ++j)
            (*matrix)[i][j] = (i == j ? 1 : 0);
}

void unloadGraphicsMod() {
    delete (matrix);
    while (!matrixStack.empty()) {
        delete (matrixStack.top());
        matrixStack.pop();
    }
}

void myPushMatrix() {
    matrixStack.push(matrix);
    auto tmp = matrix;
    matrix = new vector<vector<double>>(3, vector<double>(3));
    for (int i = 0; i < matrix->size(); ++i)
        for (int j = 0; j < (*matrix)[i].size(); ++j)
            (*matrix)[i][j] = (*tmp)[i][j];
}

void myPopMatrix() {
    if (matrixStack.empty())
        return;
    delete (matrix);
    matrix = (vector<vector<double>> *) matrixStack.top();
    matrixStack.pop();
}

void multiplyToMatrix(const vector<vector<double>> &mat) {
    auto tm = vector<vector<double>>(3, vector<double>(3, 0));
    if (matrix == nullptr)return;
    for (int i = 0; i < tm.size(); ++i)
        for (int j = 0; j < tm[i].size(); ++j)
            for (int k = 0; k < mat.size(); ++k)
                tm[i][j] += (mat[i][k] * ((*matrix)[k][j]));
    for (int i = 0; i < tm.size(); ++i)
        for (int j = 0; j < tm[i].size(); ++j)
            (*matrix)[i][j] = tm[i][j];
}


void myTranslatef(int x, int y, int z) {
    auto tm = vector<vector<double>>(3, vector<double>(3));
    tm[0][2] = x;
    tm[1][2] = y;
    tm[1][1] = tm[0][0] = tm[2][2] = 1;
    multiplyToMatrix(tm);
}


void fillPoly(int n, int v[][2]) {
    struct EdgeBucket {
        int yMax, yMin, sign, dx, dy, rem;
        float x;
    };
    EdgeBucket edgeTable[n];
    int m = 0;
    for (int i = 0; i < n; ++i) {
        int i1 = i;
        int i2 = (i + 1) % n;
        if (v[i1][1] != v[i2][1]) {
            edgeTable[m].yMax = max(v[i1][1], v[i2][1]);
            edgeTable[m].yMin = min(v[i1][1], v[i2][1]);
            edgeTable[m].rem = edgeTable[m].yMax - edgeTable[m].yMin;
            edgeTable[m].x = v[i1][1] < v[i2][1] ? v[i1][0] : v[i2][0];
            if (edgeTable[m].yMax == v[i2][1])
                edgeTable[m].sign = (v[i2][0] - v[i1][0]) < 0 ? -1 : 1;
            else
                edgeTable[m].sign = (v[i2][0] - v[i1][0]) <= 0 ? 1 : -1;
            edgeTable[m].dy = abs(v[i1][1] - v[i2][1]);
            edgeTable[m].dx = abs(v[i1][0] - v[i2][0]);
            m++;
        }
    }
    sort(edgeTable, edgeTable + m, [](EdgeBucket &e1, EdgeBucket &e2) {
        return e1.yMin < e2.yMin;
    });
    int activeList[n];
    for (int y = edgeTable[0].yMin; true; ++y) {
        int j = 0;
        for (int i = 0; i < m; ++i)
            if (edgeTable[i].yMin <= y && edgeTable[i].rem > 0)
                activeList[j++] = i;
        if (j == 0)
            break;
        sort(activeList, activeList + j, [&edgeTable](int &e1, int &e2) {
            return edgeTable[e1].x < edgeTable[e2].x;
        });
        for (int k = 0; k < j; k += 2) {
            int e1 = activeList[k];
            int e2 = activeList[k + 1];
            drawLineBresenham((int) round(edgeTable[e1].x), y, (int) round(edgeTable[e2].x), y);
            edgeTable[e1].x += edgeTable[e1].sign * (float) edgeTable[e1].dx / edgeTable[e1].dy;
            edgeTable[e2].x += edgeTable[e2].sign * (float) edgeTable[e2].dx / edgeTable[e2].dy;
            edgeTable[e1].rem--;
            edgeTable[e2].rem--;
        }
    }
}


void drawLineBresenham(int x11, int y11, int x21, int y21) {
    int x1 = x11, x2 = x21, y1 = y11, y2 = y21;
    bool draw_xy = true;
    if (y1 != y2) {
        float m = (float) (y2 - y1) / (x2 - x1);
        if (y2 > y1) {
//        oct 1
            if (0 <= m && m <= 1) {
            }
//        oct 2 & 3
            else if (m > 1 || m < -1) {
                swap(x1, y1);
                swap(x2, y2);
                draw_xy = false;
            }
//        oct 4
            else if (m > -1) {
                swap(x1, x2);
                swap(y1, y2);
            }
        } else {
//        oct 5
            if (0 <= m && m <= 1) {
                swap(x1, x2);
                swap(y1, y2);
            }
//        oct 6 & 7
            else if (m > 1 || m < -1) {
                swap(x1, x2);
                swap(y1, y2);
                swap(x1, y1);
                swap(x2, y2);
                draw_xy = false;
            }
//        oct 8
            else if (m > -1) {
            }
        }
    } else {
        x1 = min(x11, x21);
        x2 = max(x11, x21);
    }
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int p = 2 * dy - dx;
    int x = x1, y = y1;
    int dec = 2 * dx, inc = 2 * dy, i = 0;
    auto *vertices = new GLint[2 * (abs(dx) + 2)];
    bool inc_y = y < y2;
    if (y1 != y2) {
        while (x <= x2) {
            vertices[i] = draw_xy ? x : y;
            vertices[i + 1] = draw_xy ? y : x;
            i += 2;
            ++x;
            if (p >= 0) {
                y = y + (inc_y ? 1 : -1);
                p -= dec;
            }
            p += inc;
        }
    } else
        while (x <= x2) {
            vertices[i] = x;
            vertices[i + 1] = y;
            ++x;
            i += 2;
        }
    glVertexPointer(2, GL_INT, 2 * sizeof(GLint), vertices);
    glDrawArrays(GL_POINTS, 0, i / 2);
    delete (vertices);
}

GLsizei stride;
void *ptr;

void myVertexPointer(int size, GLenum type, GLsizei stride, void *ptr) {
    ::stride = stride;
    ::ptr = ptr;
}

#define MAX_COUNT 100
int vvv[MAX_COUNT][2];

void myDrawArrays(GLenum mode, GLint first, GLsizei count) {
    auto tmp = vector<vector<int>>(3, vector<int>(count, 1));
    auto tm = vector<vector<double >>(3, vector<double>(count, 0));
    for (int vi = 0; vi < count; ++vi) {
        auto arr = (GLint *) ((char *) ptr + vi * stride);
        tmp[0][vi] = arr[0];
        tmp[1][vi] = arr[1];
    }
    for (int i = 0; i < tm.size(); ++i)
        for (int j = 0; j < tm[i].size(); ++j)
            for (int k = 0; k < matrix->size(); ++k)
                tm[i][j] += (((*matrix)[i][k]) * tmp[k][j]);
    for (int i = 0; i < count; ++i) {
        vvv[i][0] = static_cast<int>(tm[0][i]);
        vvv[i][1] = static_cast<int>(tm[1][i]);
    }
    switch (mode) {
        case GL_POLYGON:
            fillPoly(count, vvv);
            break;
        case GL_LINES:
            for (int i = 0; i < count; i += 2)
                drawLineBresenham(vvv[i][0], vvv[i][1], vvv[i + 1][0], vvv[i + 1][1]);
            break;
        default:
            break;
    }
}


void myRotated(int angle, int x, int y, int z) {
    auto tm = vector<vector<double>>(3, vector<double>(3, 0));
    tm[0][0] = tm[1][1] = cos(RADIAN(angle));
    tm[0][1] = -sin(RADIAN(angle));
    tm[1][0] = -tm[0][1];
    tm[2][2] = 1;
    multiplyToMatrix(tm);
}