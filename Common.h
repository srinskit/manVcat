/*
    Made with ❤ by srinSkit.
    Created on 30 April 2018.
*/

#ifndef MANVCAT_COMMON_H
#define MANVCAT_COMMON_H

#include <cmath>

extern int worldWidth;
extern int worldHeight;
extern int cellWidth, cellHeight, nCellsX, nCellsY;
extern int screenWidth, screenHeight;
extern int score;
extern int win;

void limitX(int &);

void limitGx(int &);

void limitY(int &);

void limitGy(int &);

#endif //MANVCAT_COMMON_H
