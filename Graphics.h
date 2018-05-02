/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_GRAPHICS_H
#define MANVCAT_GRAPHICS_H

#include "Common.h"

void screenToWorld(int &x, int &y);

void screenToGrid(int &x, int &y);

void worldToGrid(int &x, int &y);

void gridToWorld(int &, int &);

int rangeRandom(int low, int high);

void circle(int xc, int yc, int radius, bool fill);

#endif //MANVCAT_GRAPHICS_H
