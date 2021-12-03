#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "DE1SoCfpga.h"
#include "Devices.h"
#include "RayMath.h"
#include "Camera.h"

struct Intersection;

class Shape{
    public:
        
        float specularity; //value between 0 and 1. Represents range from fully diffuse to fully specular. Values in the middle appear as glossy.
        float emittance; //value between 0 and 1. Scale factor for amount of light emitted
        Vector color; //color of the shape

        Shape();
        ~Shape();
        
        //returns if the ray intersects the shape, and updates the given ray to be a normal vector at the point of intersection
        virtual Intersection intersect(Ray r) = 0;
        virtual Vector normal(Vector p) = 0;
        virtual void moveBy(Vector v) = 0;
};

// a sphere object in the scene
class Sphere : public Shape{
    public:
        
        // center is the center of the sphere
        Vector center;
        float radius;

        Sphere(Vector c, float r, float s, float e, Vector col);
        ~Sphere();

        virtual Intersection intersect(Ray r);
        virtual Vector normal(Vector p);
        virtual void moveBy(Vector v);
};

// a plane object in the scene
class Plane : public Shape{
    public:

        // point is any point in the plane
        Vector point;
        Vector norm;

        Plane(Vector p, Vector n, float s, float e, Vector col);
        ~Plane();

        virtual Intersection intersect(Ray r);
        virtual Vector normal(Vector p);
        virtual void moveBy(Vector v);
};

struct Intersection
{
    bool hit; //whether this is an actual intersection or not
    float dist; //distance from ray origin that generated this intersection to the point of intersection
    Vector pt; //point of intersection
    Shape* shape;

    Intersection(float d, Vector p, Shape* s);
    Intersection();
};

class Pathtracer{ //todo - pathtracer will have a board w/ devices that it can control after doing the tracing etc w algorithms here
    private:
        int rpp, depth, branches;
        float probability;
        float fov;
        float halfViewWidth, halfViewHeight;
        int rows, columns;
        Devices* board;
        Camera camera;

        // Info about the objects in the scene of the pathtracer
        std::vector <Shape*> scene;
        Shape* light;
    public:

        Pathtracer(int raysPerPixel, int d, int branchesPerLevel, float prob);

        ~Pathtracer();

        void setup();

        void main();

        void rotateCamera(float p, float y);

        void moveCamera(Vector v);

        void moveLight(Vector v);

        void renderScene();

        void renderSceneOptimized();

        void renderSceneInterpolate();

        Intersection castRay(Ray r);

        Vector rayColor(Ray r, int bounces);


        Vector rayColorRR(Ray r, float p, int bounces);

};

#endif
