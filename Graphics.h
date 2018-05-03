/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_GRAPHICS_H
#define MANVCAT_GRAPHICS_H

#include <GL/gl.h>
#include "Common.h"

void screenToWorld(int &x, int &y);

void screenToGrid(int &x, int &y);

void worldToGrid(int &x, int &y);

void gridToWorld(int &, int &);

int rangeRandom(int low, int high);

void circle(int xc, int yc, int radius, bool fill);

void loadGraphicsMod();

void unloadGraphicsMod();

void myPushMatrix();

void myPopMatrix();

void myTranslatef(int x, int y, int z);

void myRotated(int angle, int x, int y, int z);

void myVertexPointer(int size, GLenum type, int stride, void *ptr);

void myDrawArrays(GLenum mode, GLint first, GLsizei count);

void drawLineBresenham(int x1, int y1, int x2, int y2);

#endif //MANVCAT_GRAPHICS_H
