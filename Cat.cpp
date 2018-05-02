/*
    Made with ❤ by srinSkit.
    Created on 12 April 2018.
*/
#include "Cat.h"
#include "Common.h"

Cat::Cat(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    health = 9;
}


void Cat::update() {
    if(autoMove){

    }
    if (!insideView(x, y))
        destroy();
}

void Cat::setAutoMove(bool enable) {
    autoMove = enable;
}


Cat *cat = nullptr;

void loadCatMod() {
    cat = new Cat(worldWidth / 4, worldHeight / 3, false, false);
    auto v = new Vertex[9];
    v[0].set(-20, -20);
    v[1].set(20, -20);
    v[2].set(20, 20);
    v[3].set(15, 35);
    v[4].set(10, 20);
    v[5].set(-10, 20);
    v[6].set(-15, 35);
    v[7].set(-20, 20);
    v[0].set(-20, -20);
    auto element = new Element(9, v, GL_POLYGON);
    element->setColor(50, 255, 50);
    cat->addElement(element);
    v = new Vertex[4];
    v[0].set(5, 5);
    v[1].set(15, 5);
    v[2].set(15, 15);
    v[3].set(5, 15);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(0, 0, 0);
    cat->addElement(element);
    v = new Vertex[4];
    v[0].set(-5, 5);
    v[1].set(-15, 5);
    v[2].set(-15, 15);
    v[3].set(-5, 15);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(0, 0, 0);
    cat->addElement(element);
}

void unloadCatMod() {
    delete cat;
}