/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_CAT_H
#define MANVCAT_CAT_H

#include "Model.h"
#include "AutoPilot.h"


class Cat : public Model {
    bool autoMove;
    stack<Cell *> moveStack;
    int tgx, tgy;
public:

    Cat(int localOriginX, int localOriginY, bool isStaticModel, bool hide);

    void enableAutoMove();

    void update();

    void onHit() override;
};

extern Cat *cat;

void loadCatMod();

void unloadCatMod();

#endif //MANVCAT_CAT_H
