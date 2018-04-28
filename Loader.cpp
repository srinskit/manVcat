/*
    Made with ❤ by srinSkit.
    Created on 11 April 2018.
*/

#include "Loader.h"

extern Element *bulletElement;
Tank *mainTank = nullptr;

void load() {
    mainTank = new Tank(0, 0, true);
    auto v = new Vertex[4];
    v[0].set(-5, 0);
    v[1].set(5, 0);
    v[2].set(5, 70);
    v[3].set(-5, 70);
    auto element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(200, 200, 200);
    element->layer = 1;
    mainTank->addBarrelElement(element);
    v = new Vertex[4];
    v[0].set(-20, -20);
    v[1].set(20, -20);
    v[2].set(20, 20);
    v[3].set(-20, 20);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(200, 50, 0);
    element->layer = 1;
    mainTank->addBarrelElement(element);
    v = new Vertex[4];
    v[0].set(-30, -40);
    v[1].set(30, -40);
    v[2].set(30, 40);
    v[3].set(-30, 40);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(50, 50, 250);
    mainTank->addBodyElement(element);
    v = new Vertex[4];
    v[0].set(-4, -4);
    v[1].set(4, -4);
    v[2].set(4, 4);
    v[3].set(-4, 4);
    bulletElement = new Element(4, v, GL_TRIANGLE_FAN);
    bulletElement->setColor(255, 50, 50);
    Model *bush = new Model(100, 100, true, false);
    v = new Vertex[4];
    v[0].set(-10, -10);
    v[1].set(10, -10);
    v[2].set(10, 10);
    v[3].set(-10, 10);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(50, 255, 50);
    bush->addElement(element);
    v = new Vertex[4];
    v[0].set(0, 0);
    v[1].set(20, 0);
    v[2].set(20, 20);
    v[3].set(0, 20);
    element = new Element(4, v, GL_TRIANGLE_FAN);
    element->setColor(50, 200, 150);
    bush->addElement(element);
    auto *cat = new Cat(-100, -100, false, false);
    v = new Vertex[4];
    v[0].set(-20, -20);
    v[1].set(20, -20);
    v[2].set(20, 20);
    v[3].set(-20, 20);
    element = new Element(4, v, GL_TRIANGLE_FAN);
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
//        cat->reSpawn(width / 2, height / 2);
}

void unload() {
    Element::destroyAll();
    Model::destroyAll();
}

