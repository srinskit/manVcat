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

Element *Element::loadElements(char *file_location, int &n) {

}

void Element::destroyAll() {
    for (auto it = elementStore.begin(); it < elementStore.end(); ++it) {
        delete ((*it)->v);
        delete (*it);
    }
}

int Model::deadCount = 0, Model::maxDeadCount = 10;
vector<Model *> Model::staticModels, Model::dynamicModels;

Model::Model(int localOriginX, int localOriginY, bool isStaticModel, bool hide) {
    x = localOriginX, y = localOriginY, static_model = isStaticModel;
    elements = nullptr;
    w = h = nE = 0;
    this->active = true;
    this->hide = hide;
    if (static_model)
        staticModels.push_back(this);
    else
        dynamicModels.push_back(this);
}

void Model::detectDimensions() {
    GLint minX = x, maxX = x, minY = y, maxY = y;
    for (auto it = elements->begin(); it < elements->end(); ++it)
        for (auto v = (*it)->v; v < (*it)->v + (*it)->nV; ++v) {
            minX = min(minX, v->x);
            minY = min(minX, v->y);
            maxX = max(minX, v->x);
            maxY = max(minX, v->y);
        }
    w = maxX - minX;
    h = maxY - minY;
}

bool Model::hit(Model *model) {
    int xt = model->x, yt = model->x, wt = model->x, ht = model->x;
    int dx = (wt + w) / 2, dy = (ht + h) / 2;
    return (-dx < xt - x || xt - x < dx) && (-dy < yt - y || yt - y < dy);
}

void Model::destroy() {
    active = false;
    if (elements != nullptr)
        delete (elements);
    elements = nullptr;
    ++deadCount;
}

bool Model::loadElements(char *file_location) {
    return true;
}

void Model::addElement(Element *element) {
    if (elements == nullptr)
        elements = new vector<Element *>();
    elements->push_back(element);
    sort(elements->begin(), elements->end(), [](const Element *e1, const Element *e2) {
        return e1->layer < e2->layer;
    });
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
    for (auto it = staticModels.begin(); it < staticModels.end(); ++it) {
        if (!(*it)->active) continue;
        if ((*it)->hide) continue;
        (*it)->render();
    }
}

void Model::renderDynamic() {
    for (auto it = dynamicModels.begin(); it < dynamicModels.end(); ++it) {
        if (!(*it)->active) continue;
        (*it)->update();
        if ((*it)->hide) continue;
        (*it)->render();
    }
}

void Model::cleanRenderQueue() {
    for (auto it = staticModels.begin(); it < staticModels.end(); ++it)
        if (!(*it)->active) {
            delete *it;
            *it = nullptr;
        }

    for (auto it = dynamicModels.begin(); it < dynamicModels.end(); ++it)
        if (!(*it)->active) {
            delete *it;
            *it = nullptr;
        }
    staticModels.erase(
            remove_if(staticModels.begin(), staticModels.end(), [](Model *model) { return model == nullptr; }),
            staticModels.end());
    dynamicModels.erase(remove_if(dynamicModels.begin(), dynamicModels.end(),
                                  [](Model *model) { return model == nullptr; }), dynamicModels.end());
    deadCount = 0;
}

void Model::nextFrame() {
    if (deadCount > maxDeadCount)
        cleanRenderQueue();
}

void Model::destroyAll() {
    for (auto it = staticModels.begin(); it < staticModels.end(); ++it)
        if (!(*it)->active)
            delete *it;
    for (auto it = dynamicModels.begin(); it < dynamicModels.end(); ++it)
        if (!(*it)->active)
            delete *it;
    staticModels.clear();
    dynamicModels.clear();
    deadCount = 0;
}

bool insideView(int x, int y) {
    int width = 800, height = 800;
    return x > -width / 2 && x < width / 2 && y > -height / 2 && y < height / 2;
}
