/*
    Made with ❤ by srinSkit.
    Created on 17 April 2018.
*/

#ifndef MANVCAT_MODEL_H
#define MANVCAT_MODEL_H

/*
 * A model has a local origin (x,y) and list of elements to be rendered around this origin
 * An element has a shape and vertex array representing that shape
 * A vertex contains the location of the vertex
 */


#include <GL/gl.h>
#include <vector>
#include <algorithm>
#include <math.h>

#define RADIAN(x) ((x)*M_PI/180.0)

using namespace std;

//Encapsulates Vertex data
class Vertex {
public:
    GLint x, y;

    Vertex();

    void set(GLint x, GLint y);
};

//Encapsulate vertices of an element of model
class Element {
public:
    static vector<Element *> elementStore;
    GLenum glShape;
    int nV;
    Vertex *v;
    int layer;
    GLubyte r, g, b;

    Element(int numberOfVertices, Vertex *vertexArray, GLenum elementShape = GL_TRIANGLE_FAN);

    void setColor(GLubyte r, GLubyte g, GLubyte b);

    void loadColor();

    void render(int x, int y);

    static Element *loadElements(char *file_location, int &n);

    static void destroyAll();
};


// Root class for all objects in model
class Model {
protected:
    static vector<Model *> staticModels;
    static vector<Model *> dynamicModels;
    static int deadCount, maxDeadCount;
    int nE;
    vector<Element *> *elements;
    bool active, hide, static_model;
    GLint x, y, w, h;
public:
    Model(int localOriginX, int localOriginY, bool isStaticModel, bool hide);

    void detectDimensions();

    void destroy();

    bool loadElements(char *file_location);

    void addElement(Element *element);

    virtual void update();

    virtual void render();

    bool hit(Model *model);

    static void renderStatic();

    static void renderDynamic();

    static void cleanRenderQueue();

    static void nextFrame();

    static void destroyAll();
};

bool insideView(int x, int y);

#endif //MANVCAT_MODEL_H
