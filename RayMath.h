#ifndef RAYMATH_H
#define RAYMATH_H

struct Vector{
    float x, y, z;
    Vector();
    Vector(float x,float y,float z);
    Vector operator + (Vector v) { return Vector(x+v.x, y+v.y, z+v.z); }
    Vector operator - (Vector v) { return Vector(x-v.x, y-v.y, z-v.z); }
    Vector operator * (float c) { return Vector(x*c, y*c, z*c); }
    Vector operator / (float c) { return Vector(x/c, y/c, z/c); }
    void print();
};

struct Ray{
    Vector origin;
    Vector direction;
    Ray(float x,float y,float z,float dx,float dy,float dz);
    Ray(Vector o, Vector d);
};

//Rotate a vector around the X axis to a new direction
Vector rotateVectorX(Vector v, float c, float s);

//Rotate a vector around the Y axis to a new direction
Vector rotateVectorY(Vector v, float c, float s);

//Rotate a vector around the Z axis to a new direction
Vector rotateVectorZ(Vector v, float c, float s);

//Take the dot product of two vectors
float dot(Vector v1, Vector v2);

// take the  cross product of two vectors
Vector cross(Vector v1, Vector v2);

// get the magnitude of a vector
float magnitude(Vector v);

// returns the input vector normalized to have length 1
Vector normalize(Vector v);

// reflects a vector over a given normal vector
Vector reflect(Vector v, Vector n);

// function to generate a random unit vector vector
Vector randomVector();

// function to generate a random vector within a hemisphere around a normal vector (normalized to have length 1)
Vector randomHemisphereVector(Vector n);

// scale one color by another color
Vector colorMultiply(Vector c1, Vector c2);

// take a vector and cap the values at 255, 255, 255
Vector colorMax(Vector v);

// maps an rgb color vector to a 16 bit int
int to16BitColor(Vector color);

// changes a 16 bit rgb val to a vector color
Vector toVectorColor(int i);

// checks if two colors are similar enough to interpolate between
bool colorsSimilar(Vector c1, Vector c2);

#endif
