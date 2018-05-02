/*
    Made with ❤ by srinSkit.
    Created on 30 April 2018.
*/

#include <cstdio>
#include <iostream>
#include "Model.h"
#include "Common.h"
#include "Graphics.h"
#include <vector>

void Model::CollisionManager::push(Model *model) {
    if (model->hCode < 0 || !model->active) return;
    int yb = model->y + model->hb, yt = model->y + model->ht, xl = model->x + model->hl, xr = model->x + model->hr;
    limitY(yb);
    limitY(yt);
    limitX(xl);
    limitX(xr);
    int gx, gy;
    for (int y = yb; y <= yt; ++y)
        for (int x = xl; x <= xr; ++x) {
            gx = x;
            gy = y;
            worldToGrid(gx, gy);
            approxGrid[gy][gx] = true;
            buff[y][x] += model->hCode;
        }
}


void Model::CollisionManager::pop(Model *model) {
    if (model->hCode < 0 || !model->active) return;
    int yb = model->y + model->hb, yt = model->y + model->ht, xl = model->x + model->hl, xr = model->x + model->hr;
    limitY(yb);
    limitY(yt);
    limitX(xl);
    limitX(xr);
    int gx, gy;
    for (int y = yb; y <= yt; ++y)
        for (int x = xl; x <= xr; ++x) {
            buff[y][x] -= model->hCode;
            if (buff[y][x] < 0) {
                gx = x;
                gy = y;
                worldToGrid(gx, gy);
                approxGrid[gy][gx] = false;
            }
        }
}


Model::CollisionManager::CollisionManager(vector<Model *> *staticModels, vector<Model *> *dynamicModels) {
    buff.resize(static_cast<unsigned long>(worldHeight) + 1,
                vector<int>(static_cast<unsigned long>(worldWidth) + 1, -1));
    approxGrid.resize(10, vector<bool>(10, false));
    maxCodeSize = 0;
    this->staticModels = staticModels;
    this->dynamicModels = dynamicModels;
}

void Model::CollisionManager::login(Model *model) {
    if (codes.empty()) {
        for (int i = 0; i < 20; ++i)
            codes.push(i + maxCodeSize);
        maxCodeSize += 20;
    }
    model->hCode = codes.top();
    codes.pop();
}

void Model::CollisionManager::logout(Model *model) {
    codes.push(model->hCode);
}

vector<vector<bool>> *Model::CollisionManager::makeGrid() {
    auto grid = new vector<vector<bool> >(10, vector<bool>(10));
    for (int i = 0; i < nCellsX; ++i) {
        for (int j = 0; j < nCellsY; ++j)
            (*grid)[i][j] = (buff[i * cellHeight + cellHeight / 2][j * cellWidth + cellWidth / 2] >= 0);
    }
    return grid;
}


void Model::CollisionManager::fullCheck() {
    for (auto i = 0; i < dynamicModels->size(); ++i) {
        auto model = (*dynamicModels)[i];
        if (model->hCode < 0 || !model->active) continue;
        int yb = model->y + model->hb, yt = model->y + model->ht, xl = model->x + model->hl, xr = model->x + model->hr;
        limitY(yb);
        limitY(yt);
        limitX(xl);
        limitX(xr);
        for (int y = yb, x; y <= yt; ++y) {
            for (x = xl; x <= xr; ++x) {
                if (buff[y][x] - model->hCode >= 0) {
                    dHitTracker.push(i);
                    break;
                }
            }
            if (x <= xr)break;
        }
    }
    if (!dHitTracker.empty()) {
        for (auto i = 0; i < staticModels->size(); ++i) {
            auto model = (*staticModels)[i];
            if (model->hCode < 0 || !model->active) continue;
            int yb = model->y + model->hb, yt = model->y + model->ht, xl = model->x + model->hl, xr =
                    model->x + model->hr;
            limitY(yb);
            limitY(yt);
            limitX(xl);
            limitX(xr);
            for (int y = yb, x; y <= yt; ++y) {
                for (x = xl; x <= xr; ++x) {
                    if (buff[y][x] - model->hCode >= 0) {
                        sHitTracker.push(i);
                        break;
                    }
                }
                if (x <= xr)break;
            }
        }
    }
    while (!dHitTracker.empty()) {
        (*dynamicModels)[dHitTracker.top()]->onHit();
        dHitTracker.pop();
    }
    while (!sHitTracker.empty()) {
        (*staticModels)[sHitTracker.top()]->onHit();
        sHitTracker.pop();
    }
}

bool Model::CollisionManager::unsafe(int x, int y) {
    if (x < 0 || y < 0 || x >= nCellsX || y >= nCellsY)return false;
    return approxGrid[y][x];
}
