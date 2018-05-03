/*
    Made with ❤ by srinSkit.
    Created on 11 April 2018.
*/
#include "Tank.h"
#include "Common.h"
#include <queue>
#include <climits>
#include <cstdio>
#include <GL/glut.h>
#include "Graphics.h"
#include "AutoPilot.h"

#define DEGREE(x) ((x)*180/M_PI)

Tank::Tank(int localOriginX, int localOriginY, bool isStaticModel, bool hide)
        : Model(localOriginX, localOriginY, isStaticModel, hide) {
    bodyAngle = barrelExcess = 0;
    barrelElements = bodyElements = nullptr;
    health = 7;
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
    myPushMatrix();
    myRotated(bodyAngle, 0, 0, 1);
    for (auto it = bodyElements->begin(); it < bodyElements->end(); ++it) {
        (*it)->render(x, y);
    }
    myRotated(barrelExcess, 0, 0, 1);
    for (auto it = barrelElements->begin(); it < barrelElements->end(); ++it) {
        (*it)->render(x, y);
    }
    myPopMatrix();
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
                autoMove = false;
                while (!moveStack.empty())
                    moveStack.pop();
                break;
            case 'a':
                turn(+1);
                autoMove = false;
                while (!moveStack.empty())
                    moveStack.pop();
                break;
            case 's':
                move(-1);
                autoMove = false;
                while (!moveStack.empty())
                    moveStack.pop();
                break;
            case 'd':
                turn(-1);
                autoMove = false;
                while (!moveStack.empty())
                    moveStack.pop();
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
        auto theta = (int) (-90 + DEGREE(atan2(yt - y, xt - x)));
        if (theta < 0) theta += 360;
        if (bodyAngle < theta) {
            if (theta - bodyAngle < 360 - (theta - bodyAngle))
                bodyAngle = (bodyAngle + 1) % 360;
            else
                bodyAngle = (bodyAngle - 1 + 360) % 360;
        } else if (bodyAngle > theta) {
            if (bodyAngle - theta < 360 - (bodyAngle - theta))
                bodyAngle = (bodyAngle - 1 + 360) % 360;
            else
                bodyAngle = (bodyAngle + 1) % 360;
        } else {
            if (x < xt)
                x++;
            if (x > xt)
                x--;
            if (y < yt)
                y++;
            if (y > yt)
                y--;
        }
        if (x == xt && y == yt) {
            delete cell;
            moveStack.pop();
        }
        if (moveStack.empty())
            autoMove = false;
    }
}

void Tank::onHit() {
    health--;
    if (health <= 0) {
        coolMan.pop(this);
        destroy();
    }
    score -= 8;
}

void Tank::autoMoveEnable() {
    autoMove = true;
}

stack<Cell *> Tank::autoPilot(int gridx, int gridy) {
    int gx = x, gy = y;
    worldToGrid(gx, gy);
    return moveStack = autoPilotG(gx, gy, gridx, gridy);
}

