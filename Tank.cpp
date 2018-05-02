/*
    Made with ❤ by srinSkit.
    Created on 11 April 2018.
*/
#include "Tank.h"
#include "Common.h"
#include <queue>
#include <climits>
#include <cstdio>
#include "Graphics.h"
#include "AutoPilot.h"

Tank::Tank(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    bodyAngle = barrelExcess = 0;
    barrelElements = bodyElements = nullptr;
}

void Tank::addBodyElement(Element *element) {
    if (bodyElements == nullptr)
        bodyElements = new vector<Element *>();
    bodyElements->push_back(element);
    sort(bodyElements->begin(), bodyElements->end(), [](const Element *e1, const Element *e2) {
        return e1->layer < e2->layer;
    });
    coolMan.pop(this);
    adjustHitbox(element);
    coolMan.push(this);
}

void Tank::addBarrelElement(Element *element) {
    if (barrelElements == nullptr)
        barrelElements = new vector<Element *>();
    barrelElements->push_back(element);
    sort(barrelElements->begin(), barrelElements->end(), [](const Element *e1, const Element *e2) {
        return e1->layer < e2->layer;
    });
    coolMan.pop(this);
    adjustHitbox(element);
    coolMan.push(this);
}

void Tank::turn(int dir) {
    bodyAngle += dir < 0 ? -5 : +5;
    if (bodyAngle < 0)bodyAngle = 360 - 1;
    bodyAngle %= 360;
}

void Tank::move(int dir) {
    x += (dir < 0 ? 5 : -5) * sin(RADIAN(bodyAngle));
    y += (dir < 0 ? -5 : +5) * cos(RADIAN(bodyAngle));
    if (!insideView(x, y)) {
        x -= (dir < 0 ? 5 : -5) * sin(RADIAN(bodyAngle));
        y -= (dir < 0 ? -5 : +5) * cos(RADIAN(bodyAngle));
    }
}

void Tank::rotate(int dir) {
    barrelExcess += dir < 0 ? -5 : +5;
    if (barrelExcess < 0)barrelExcess = 360 - 5;
    barrelExcess %= 360;
}

void Tank::render() {
    if (bodyElements == nullptr || barrelElements == nullptr) return;
    glTranslated(x, y, 0);
    glRotated(bodyAngle, 0, 0, 1);
    for (auto it = bodyElements->begin(); it < bodyElements->end(); ++it) {
        (*it)->render(0, 0);
    }
    glRotated(barrelExcess, 0, 0, 1);
    for (auto it = barrelElements->begin(); it < barrelElements->end(); ++it) {
        (*it)->render(0, 0);
    }
    glRotated(-bodyAngle - barrelExcess, 0, 0, 1);
    glTranslated(-x, -y, 0);

}

void Tank::fire() {
    if (bulletElement == nullptr)return;
    auto bullet = new Bullet(0, 0, false, false);
    bullet->addElement(bulletElement);
    bullet->launch(bodyAngle + barrelExcess, x, y);
}

void Tank::update() {
    if (!actionQ.empty()) {
        switch (actionQ.front()) {
            case 'w':
                move(+1);
                break;
            case 'a':
                turn(+1);
                break;
            case 's':
                move(-1);
                break;
            case 'd':
                turn(-1);
                break;
            case 'j':
                rotate(+1);
                break;
            case 'k':
                fire();
                break;
            case 'l':
                rotate(-1);
                break;
            case 'c':
                autoMove = false;
                while (!moveStack.empty())
                    moveStack.pop();
                break;
            default:
                break;
        }
        actionQ.pop();
    }
    if (autoMove && !moveStack.empty()) {
        Cell *cell = moveStack.top();
        int xt = cell->gridx, yt = cell->gridy;
        gridToWorld(xt, yt);
        if (x < xt)
            x++;
        if (x > xt)
            x--;
        if (y < yt)
            y++;
        if (y > yt)
            y--;
        if (x == xt && y == yt) {
            delete cell;
            moveStack.pop();
        }
        if (moveStack.empty())
            autoMove = false;
    }
}

void Tank::onHit() {

}

void Tank::autoMoveEnable() {
    autoMove = true;
}

stack<Cell *> Tank::autoPilot(int gridx, int gridy) {
    int gx = x, gy = y;
    worldToGrid(gx, gy);
    return moveStack = autoPilotG(gx, gy, gridx, gridy);
}

