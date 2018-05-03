/*
    Made with ❤ by srinSkit.
    Created on 30 April 2018.
*/

#include <climits>
#include "Common.h"

int worldHeight = 800;
int worldWidth = 800;
int nCellsX = 10, nCellsY = 10;
int cellWidth = worldWidth / nCellsX;
int cellHeight = worldHeight / nCellsX;
int score = 0;
int screenWidth, screenHeight;

void limitX(int &x) {
    if (x < 0) x = 0;
    if (x >= worldWidth) x = worldWidth - 1;
}

void limitGx(int &x) {
    if (x < 0) x = 0;
    if (x >= nCellsX) x = nCellsX;
}

void limitY(int &x) {
    if (x < 0) x = 0;
    if (x >= worldHeight) x = worldHeight - 1;
}


void limitGy(int &x) {
    if (x < 0) x = 0;
    if (x >= nCellsY) x = nCellsY - 1;
}
