/*
    Made with ❤ by srinSkit.
    Created on 11 April 2018.
*/

#include "Loader.h"
#include "Graphics.h"
#include <ctime>
#include <cstdio>
#include "Common.h"
#include "Model.h"
#include "Bullet.h"
#include "Bush.h"
#include "Cat.h"

Tank *mainTank = nullptr;


void load() {
    srand(static_cast<unsigned int>(time(nullptr)));
    loadBushModule();
    loadCatMod();
    loadGraphicsMod();
//    Start building bullet
    auto v = new Vertex[4];
    v[0].set(-4, -4);
    v[1].set(4, -4);
    v[2].set(4, 4);
    v[3].set(-4, 4);
    bulletElement = new Element(4, v, GL_POLYGON);
    bulletElement->setColor(255, 50, 50);

    int tx = worldWidth / 2, ty = worldHeight / 2;
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            if (rangeRandom(0, 100) > 90)
                makeBush(j * worldWidth / 10 + worldWidth / 20, i * worldHeight / 10 + worldHeight / 20);
            else if (j * worldWidth / 10 + worldWidth / 20 < worldHeight * 2 / 3 &&
                     i * worldHeight / 10 + worldHeight / 20 < worldHeight * 2 / 3) {
                tx = j * worldWidth / 10 + worldWidth / 20;
                ty = i * worldHeight / 10 + worldHeight / 20;
            }
    //    Start building tank
    mainTank = new Tank(tx, ty, false, false);
//    mainTank->makeImmune();
    v = new Vertex[4];
    v[0].set(-5, 0);
    v[1].set(5, 0);
    v[2].set(5, 70);
    v[3].set(-5, 70);
    auto element = new Element(4, v, GL_POLYGON);
    element->setColor(200, 200, 200);
    element->layer = 1;
    mainTank->addBarrelElement(element);
    v = new Vertex[4];
    v[0].set(-20, -20);
    v[1].set(20, -20);
    v[2].set(20, 20);
    v[3].set(-20, 20);
    element = new Element(4, v, GL_POLYGON);
    element->setColor(200, 50, 0);
    element->layer = 1;
    mainTank->addBarrelElement(element);
    v = new Vertex[4];
    v[0].set(-30, -40);
    v[1].set(30, -40);
    v[2].set(30, 40);
    v[3].set(-30, 40);
    element = new Element(4, v, GL_POLYGON);
    element->setColor(50, 50, 250);
    mainTank->addBodyElement(element);
    cat->enableAutoMove();
}


void unload() {
    unloadGraphicsMod();
    unloadBushModule();
    unloadCatMod();
    Element::destroyAll();
    Model::destroyAll();
}

