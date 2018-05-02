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
#include <stack>
#include <algorithm>
#include <cmath>

#define RADIAN(x) ((x)*M_PI/180.0)

using namespace std;

class CollisionManager;

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

    Element *softCopy();

    static void destroyAll();
};


// Root class for all objects in model
class Model {
    class CollisionManager {
        vector<vector<int>> buff;
        vector<Model *> *staticModels, *dynamicModels;
        stack<int> codes, dHitTracker, sHitTracker;
        int maxCodeSize;
        vector<vector<bool>> approxGrid;

        void toApproxGrid(int x, int y, int &gx, int &gy);

    public:
        CollisionManager(vector<Model *> *, vector<Model *> *);

        void login(Model *model);

        void logout(Model *model);

        void push(Model *model);

        void pop(Model *model);

        void fullCheck();

        bool unsafe(int x, int y);

        vector<vector<bool>> *makeGrid();
    };

protected:
    static int deadCount, maxDeadCount;
    int nE, health;
    vector<Element *> *elements;
    bool active, hide;
public:
    int hl, hr, ht, hb, hCode;
    bool static_model;
    static vector<Model *> staticModels;
    static vector<Model *> dynamicModels;
    static CollisionManager coolMan;

    Model(int localOriginX, int localOriginY, bool isStaticModel, bool hide);

    void adjustHitbox(Element *);

    void destroy();

    void addElement(Element *element);

    virtual void update();

    virtual void render();

    virtual void onHit() ;

    Model *softCopy();

    static void renderStatic();

    static void renderDynamic();

    static void cleanRenderQueue();

    static void nextFrame();

    static void destroyAll();


    GLint y;
    GLint x;
};

bool insideView(int x, int y);

#endif //MANVCAT_MODEL_H
