/*
    Made with ❤ by srinSkit.
    Created on 02 May 2018.
*/

#include <climits>
#include <cmath>
#include "AutoPilot.h"
#include "Model.h"
#include "Common.h"

using namespace std;

Cell::Cell(int gx, int gy) {
    gridx = gx;
    gridy = gy;
    g = f = INT_MAX;
    prev = nullptr;
}


int Cell::hue(int gridx, int gridy, int gridu, int gridv) {
    return static_cast<int>((pow(gridu - gridx, 2) + pow(gridv - gridy, 2)));
}

int Cell::hue(int gridu, int gridv) {
    return static_cast<int>((pow(gridu - gridx, 2) + pow(gridv - gridy, 2)));
}


stack<Cell *> autoPilotG(int gx, int gy, int gridx, int gridy) {
    int angleWt = 1, distanceWt = 1;
    int nei[][2] = {
            {+1, 0},
            {0,  +1},
            {0,  -1},
            {-1, 0},
            {1,  1},
            {-1, -1},
            {1,  -1},
            {-1, 1}
    };
    stack<Cell *> ret;
    if (!Model::coolMan.unsafe(gridx, gridy)) {
        vector<Cell *> openSet, closedSet;
        auto *start = new Cell(gx, gy);
        openSet.push_back(start);
        start->g = 0;
        start->f = start->g + start->hue(gridx, gridy);
        Cell *res = nullptr;
        while (!openSet.empty()) {
            Cell *current = openSet[0];
            for (auto node : openSet)
                if (node->f <= current->f)
                    current = node;
            if (current->f == INT_MAX) break;
            if (current->gridx == gridx && current->gridy == gridy) {
                res = current;
                break;
            }
            openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
            closedSet.push_back(current);
            for (auto &i : nei) {
                gx = current->gridx + i[0];
                gy = current->gridy + i[1];
                if (gx < 0 || gx >= nCellsX || gy < 0 || gy >= nCellsY) continue;
                if (Model::coolMan.unsafe(gx, gy)) continue;
                if (i[0] != 0 && i[1] != 0) {
                    if (Model::coolMan.unsafe(current->gridx, current->gridy + i[1])) continue;
                    if (Model::coolMan.unsafe(current->gridx + i[0], current->gridy)) continue;
                }
                for (auto node : closedSet)
                    if (node->gridx == gx && node->gridy == gy)
                        continue;
                Cell *neighbour = nullptr;
                for (auto node : openSet)
                    if (node->gridx == gx && node->gridy == gy) {
                        neighbour = node;
                        break;
                    }
                if (neighbour == nullptr) {
                    neighbour = new Cell(gx, gy);
                    openSet.push_back(neighbour);
                }
                auto tentativeG = static_cast<int>(current->g + pow(i[0], 2) + pow(i[1], 2));
                if (neighbour->g != INT_MAX && tentativeG >= neighbour->g) continue;
                neighbour->prev = current;
                neighbour->g = tentativeG;
                neighbour->f = neighbour->g + neighbour->hue(gridx, gridy);
            }
        }
        while (res != nullptr) {
            ret.push(res);
            res = res->prev;
        }
    }
    return ret;
}

