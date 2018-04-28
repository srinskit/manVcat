/*
    Made with ❤ by srinSkit.
    Created on 12 April 2018.
*/
#include "Cat.h"


Cat::Cat(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    w = h = 0;
}


void Cat::spawn(int x, int y) {

}


void Cat::update() {
    if (!insideView(x, y))
        destroy();
}

