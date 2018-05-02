/*
    Made with ❤ by srinSkit.
    Created on 10 March 2018.
*/


/*
 * A model has a local origin (x,y) and list of elements to be rendered around this origin
 * An element has a shape and vertex array representing that shape
 * A vertex contains the location of the vertex
 */


#include "Model.h"
#include <GL/gl.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include "Common.h"

#define RADIAN(x) ((x)*M_PI/180.0)

using namespace std;

Vertex::Vertex() = default;

void Vertex::set(GLint x, GLint y) {
    this->x = x;
    this->y = y;
}

vector<Element *> Element::elementStore;

Element::Element(int numberOfVertices, Vertex *vertexArray, GLenum elementShape) {
    glShape = elementShape;
    v = vertexArray;
    nV = numberOfVertices;
    layer = 0;
    r = g = b = 255;
    elementStore.push_back(this);
}

void Element::setColor(GLubyte r, GLubyte g, GLubyte b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Element::loadColor() {
    glColor3ub(r, g, b);
}

void Element::render(int x, int y) {
    glTranslatef(x, y, 0);
    loadColor();
    glVertexPointer(2, GL_INT, sizeof(Vertex), v);
    glDrawArrays(glShape, 0, nV);
    glTranslatef(-x, -y, 0);
}

Element *Element::softCopy() {
    auto element = new Element(nV, v, glShape);
    element->setColor(r, g, b);
    return element;
}

void Element::destroyAll() {
    for (auto &it : elementStore) {
        delete (it->v);
        delete it;
    }
}

int Model::deadCount = 0, Model::maxDeadCount = 20;
vector<Model *> Model::staticModels, Model::dynamicModels;
Model::CollisionManager Model::coolMan(&Model::staticModels, &Model::dynamicModels);

Model::Model(int localOriginX, int localOriginY, bool isStaticModel, bool hide) {
    x = localOriginX, y = localOriginY, static_model = isStaticModel;
    elements = nullptr;
    nE = 0;
    hl = hr = 0;
    ht = hb = 0;
    this->active = true;
    this->hide = hide;
    health = 0;
    hCode = 0;
    if (static_model)
        staticModels.push_back(this);
    else
        dynamicModels.push_back(this);
    coolMan.login(this);
    coolMan.push(this);
}

void Model::adjustHitbox(Element *element) {
    for (auto v = element->v; v < element->v + element->nV; ++v) {
        hl = min(hl, v->x);
        hr = max(hr, v->x);
        hb = min(hb, v->y);
        ht = max(ht, v->y);
    }
}

Model *Model::softCopy() {
    auto clone = new Model(x, y, static_model, hide);
    clone->elements = elements;
    clone->nE = nE;
    return clone;
}

void Model::destroy() {
    coolMan.logout(this);
    active = false;
    if (elements != nullptr)
        delete (elements);
    elements = nullptr;
    ++deadCount;
}

void Model::addElement(Element *element) {
    if (elements == nullptr)
        elements = new vector<Element *>();
    elements->push_back(element);
    sort(elements->begin(), elements->end(), [](const Element *e1, const Element *e2) {
        return e1->layer < e2->layer;
    });
    coolMan.pop(this);
    adjustHitbox(element);
    coolMan.push(this);
}

void Model::update() {

}

void Model::render() {
    if (elements == nullptr || elements->empty()) return;
    for (auto it = elements->begin(); it < elements->end(); ++it) {
        (*it)->render(x, y);
    }
}


void Model::renderStatic() {
    for (auto &staticModel : staticModels) {
        if (!staticModel->active) continue;
        if (staticModel->hide) continue;
        staticModel->render();
    }
}

void Model::renderDynamic() {
    for (auto &dynamicModel : dynamicModels) {
        if (dynamicModel == nullptr) continue;
        if (!dynamicModel->active) continue;
        coolMan.pop(dynamicModel);
        dynamicModel->update();
        coolMan.push(dynamicModel);
        if (dynamicModel->hide) continue;
        dynamicModel->render();
    }
}

void Model::cleanRenderQueue() {
    for (auto &staticModel : staticModels)
        if (!staticModel->active) {
            delete staticModel;
            staticModel = nullptr;
        }

    for (auto &dynamicModel : dynamicModels)
        if (!dynamicModel->active) {
            delete dynamicModel;
            dynamicModel = nullptr;
        }
    staticModels.erase(
            remove_if(staticModels.begin(), staticModels.end(), [](Model *model) { return model == nullptr; }),
            staticModels.end());
    dynamicModels.erase(remove_if(dynamicModels.begin(), dynamicModels.end(),
                                  [](Model *model) { return model == nullptr; }), dynamicModels.end());
    deadCount = 0;
}

void Model::nextFrame() {
    coolMan.fullCheck();
    if (deadCount > maxDeadCount)
        cleanRenderQueue();
}

void Model::destroyAll() {
    for (auto &staticModel : staticModels)
        if (!staticModel->active)
            delete staticModel;
    for (auto &dynamicModel : dynamicModels)
        if (!dynamicModel->active)
            delete dynamicModel;
    staticModels.clear();
    dynamicModels.clear();
    deadCount = 0;
}

void Model::onHit() {
    health--;
    if (health <= 0) {
        coolMan.pop(this);
        destroy();
    }
}


bool insideView(int x, int y) {
    return x >= 0 && x <= worldWidth && y >= 0 && y < worldHeight;
}
