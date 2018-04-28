/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_CAT_H
#define MANVCAT_CAT_H

#include "Model.h"

class Cat : public Model {
public:

    Cat(int localOriginX, int localOriginY, bool isStaticModel, bool hide);


    void spawn(int x, int y);


    void update() override;
};

#endif //MANVCAT_CAT_H
