/*
    Made with ❤ by srinSkit.
    Created on 12 April 2018.
*/
#include <GL/glut.h>
#include "Cat.h"
#include "Common.h"
#include "AutoPilot.h"
#include "Graphics.h"

Cat::Cat(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    health = 9;
}


void Cat::update() {
    if (autoMove && !moveStack.empty()) {
        Cell *cell = moveStack.top();
        int xt = cell->gridx, yt = cell->gridy;
        gridToWorld(xt, yt);
        if (x < xt - 1)
            x += 2;
        else if (x < xt)
            x++;
        if (x > xt + 1)
            x -= 2;
        else if (x > xt)
            x--;
        if (y < yt - 1)
            y += 2;
        else if (y < yt)
            y++;
        if (y > yt + 1)
            y -= 2;
        else if (y > yt)
            y--;
        if (x == xt && y == yt) {
            delete cell;
            moveStack.pop();
        }
        while (moveStack.empty()) {
            int gx = rangeRandom(0, 9), gy = rangeRandom(0, 9);
            if (coolMan.unsafe(gx, gy))continue;
            int ix = x, iy = y;
            worldToGrid(ix, iy);
            moveStack = autoPilotG(ix, iy, gx, gy);
        }
    }
    if (!insideView(x, y))
        destroy();
}

void Cat::enableAutoMove() {
    autoMove = true;
    while (moveStack.empty()) {
        int gx = rangeRandom(0, 9), gy = rangeRandom(0, 9);
        if (coolMan.unsafe(gx, gy))continue;
        int ix = x, iy = y;
        worldToGrid(ix, iy);
        moveStack = autoPilotG(ix, iy, gx, gy);
    }
}

void Cat::onHit() {
    health--;
    if (health <= 0) {
        coolMan.pop(this);
        destroy();
    }
    score += 4;
}


Cat *cat = nullptr;

void loadCatMod() {
    cat = new Cat(worldWidth / 4, worldHeight / 3, false, false);
    auto v = new Vertex[8];
    v[0].set(-20, -20);
    v[1].set(20, -20);
    v[2].set(20, 20);
    v[3].set(15, 35);
    v[4].set(10, 20);
    v[5].set(-10, 20);
    v[6].set(-15, 35);
    v[7].set(-20, 20);
    auto element = new Element(8, v, GL_POLYGON);
    element->setColor(50, 255, 50);
    cat->addElement(element);
    v = new Vertex[4];
    v[0].set(5, 5);
    v[1].set(15, 5);
    v[2].set(15, 15);
    v[3].set(5, 15);
    element = new Element(4, v, GL_POLYGON);
    element->setColor(0, 0, 0);
    cat->addElement(element);
    v = new Vertex[4];
    v[0].set(-5, 5);
    v[1].set(-15, 5);
    v[2].set(-15, 15);
    v[3].set(-5, 15);
    element = new Element(4, v, GL_POLYGON);
    element->setColor(0, 0, 0);
    cat->addElement(element);
}

void unloadCatMod() {
    delete cat;
}
