/*
    Made with ❤ by srinSkit.
    Created on 02 May 2018.
*/

#ifndef MANVCAT_AUTOPILOT_H
#define MANVCAT_AUTOPILOT_H

#include <stack>
#include <vector>

using namespace std;

class Cell {
public:
    int gridx, gridy;
    int direction;
    int f, g;
    Cell *prev;

    Cell(int gx, int gy);


    int hue(int gridx, int gridy, int gridu, int gridv);

    int hue(int gridu, int gridv);


};

stack<Cell *> autoPilotG(int gx, int gy, int gridx, int gridy);

#endif //MANVCAT_AUTOPILOT_H
