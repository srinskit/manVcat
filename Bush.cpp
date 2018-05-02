/*
    Made with ❤ by srinSkit.
    Created on 29 April 2018.
*/
#include "Bush.h"
#include "Common.h"

Bush::Bush(int localOriginX, int localOriginY, bool isStaticModel, bool hide) : Model(localOriginX, localOriginY,
                                                                                      isStaticModel, hide) {
}

vector<Element *> bushElements;
Vertex *bushV = nullptr;

Bush *makeBush(int x, int y) {
    if (bushElements.empty())
        return nullptr;
    auto i = random() % (bushElements.size());
    auto *bush = new Bush(x, y, true, false);
    bush->addElement(bushElements[i]);
    return bush;
}

void loadBushModule() {
    auto bushV = new Vertex[17];
    bushV[0].set(0, -2 - 30);
    bushV[1].set(-30, 5 - 30);
    bushV[2].set(-35, 40 - 30);
    bushV[3].set(-25, 20 - 30);
    bushV[4].set(-22, 50 - 30);
    bushV[5].set(-15, 30 - 30);
    bushV[6].set(-10, 65 - 30);
    bushV[7].set(-5, 25 - 30);
    bushV[8].set(0, 55 - 30);
    bushV[9].set(5, 25 - 30);
    bushV[10].set(10, 60 - 30);
    bushV[11].set(15, 30 - 30);
    bushV[12].set(25, 50 - 30);
    bushV[13].set(28, 15 - 30);
    bushV[14].set(35, 40 - 30);
    bushV[15].set(30, 5 - 30);
    bushV[16].set(0, -2 - 30);
    auto bushElement = new Element(17, bushV, GL_POLYGON);
    bushElement->setColor(34, 139, 34);
    bushElements.push_back(bushElement);
    bushElement = new Element(17, bushV, GL_POLYGON);
    bushElement->setColor(0, 100, 0);
    bushElements.push_back(bushElement);
    bushElement = new Element(17, bushV, GL_POLYGON);
    bushElement->setColor(83, 53, 10);
    bushElements.push_back(bushElement);
}

void unloadBushModule() {
    delete[] bushV;
    for (auto ele :bushElements)
        delete ele;
}