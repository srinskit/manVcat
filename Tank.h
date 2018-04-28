/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_TANK_H
#define MANVCAT_TANK_H

#include "Model.h"
#include "Bullet.h"

class Tank : public Model {
    int bodyAngle, barrelExcess;

public:
    vector<Element *> *barrelElements;
    vector<Element *> *bodyElements;

    Tank(int localOriginX, int localOriginY, bool isStaticModel, bool hide = false);

    void addBodyElement(Element *element);

    void addBarrelElement(Element *element);

    void turn(int dir);

    void move(int dir);

    void rotate(int dir);

    void fire();

    void render() override;

    void update() override;

};

#endif //MANVCAT_TANK_H
