/*
    Made with ❤ by srinSkit.
    Created on 11 April 2018.
*/
#include "Bullet.h"
#include "Common.h"
#include <cmath>

const int Bullet::v = 8;

Bullet::Bullet(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    vx = 0, vy = 0, theta = 0;
    health = 1;
}

void Bullet::launch(int theta, int x, int y) {
    this->x = static_cast<GLint>(x + 70 * -sin(RADIAN(theta)));
    this->y = static_cast<GLint>(y + 70 * cos(RADIAN(theta)));
    this->theta = theta;
    vx = static_cast<int>(Bullet::v * -sin(RADIAN(theta)));
    vy = static_cast<int>(Bullet::v * cos(RADIAN(theta)));
    hide = false;
}


void Bullet::update() {
    x += vx;
    y += vy;
    if (!insideView(x, y))
        destroy();
}

Element *bulletElement = nullptr;


