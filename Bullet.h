/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_BULLET_H
#define MANVCAT_BULLET_H

#include "Model.h"

extern Element *bulletElement;

class Bullet : public Model {
    int vx, vy, theta;
public:
    static const int v;

    Bullet(int localOriginX, int localOriginY, bool isStaticModel, bool hide);

    void launch(int theta, int x, int y);


    void update() override;
};

#endif //MANVCAT_BULLET_H
