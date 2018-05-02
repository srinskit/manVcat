/*
    Made with ❤ by srinSkit.
    Created on 29 April 2018.
*/

#ifndef MANVCAT_BUSH_H
#define MANVCAT_BUSH_H

#include "Model.h"

class Bush : public Model {
public:
    Bush(int localOriginX, int localOriginY, bool isStaticModel, bool hide);

};

Bush *makeBush(int, int);

void loadBushModule();

void unloadBushModule();

#endif //MANVCAT_BUSH_H
