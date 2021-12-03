#include "RayMath.h"
#include <cmath>
#include <random>
#include <iostream>

Vector::Vector(){ x=0; y=0; z=0; }
Vector::Vector(float x,float y,float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector::print()
{
    std::cout << "(" << x <<", "<<y<<", "<<z<<")\n";
}

Ray::Ray(float x,float y,float z,float dx,float dy,float dz)
{
    origin = Vector(x, y, z);
    direction = Vector(dx, dy, dz);
}
Ray::Ray(Vector o, Vector d){
    origin = o;
    direction = d;
}

Vector rotateVectorX(Vector v, float c, float s)
{
    float ny = v.y * c - v.z * s;
    float nz = v.y * s + v.z * c;
    return Vector(v.x, ny, nz);
}
Vector rotateVectorY(Vector v, float c, float s)
{
    float nx = v.x * c + v.z * s;
    float nz = v.z * c - v.x * s;
    return Vector(nx, v.y, nz);
}
Vector rotateVectorZ(Vector v, float c, float s)
{
    float nx = v.x * c + v.y * s;
    float ny = v.x * s - v.y * c;
    return Vector(nx, ny, v.z);
}

float dot(Vector v1, Vector v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector cross(Vector v1, Vector v2)
{
    return Vector(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}

float magnitude(Vector v)
{
    return sqrt(dot(v, v));
}

Vector normalize(Vector v)
{
    float mag = magnitude(v);
    if(mag==0){ return Vector(); }
    return v/mag;
}

Vector reflect(Vector v, Vector n)
{
    return v - (n * 2.0 * dot(v, n));
}

Vector randomVector()
{
    float u = (float) rand()/RAND_MAX;
    float v = (float) rand()/RAND_MAX;
    float theta = u * 2.0 * M_PI;
    float phi = acos(2.0 * v - 1.0);
    float sinPhi = sin(phi);
    return Vector(sinPhi * cos(theta), sinPhi * sin(theta), cos(phi));
}

Vector randomHemisphereVector(Vector n)
{
    Vector v = randomVector();
    if( dot(v, n) < 0) // random vector not in the same hemisphere as normal vector - flip it
    {
        return v * -1.0;
    }
    return v;
}

Vector colorMultiply(Vector c1, Vector c2)
{
    return Vector(c1.x * ((float) c2.x / 255.0), c1.y * ((float) c2.y / 255.0), c1.z * ((float) c2.z / 255.0));
}

Vector colorMax(Vector v)
{
    return Vector(std::max(std::min(v.x, 255.0f),0.0f), std::max(std::min(v.y, 255.0f),0.0f), std::max(std::min(v.z, 255.0f),0.0f));
}

int to16BitColor(Vector color)
{
    int r = (int) (31.0 * color.x / 255.0);
    int g = (int) (63.0 * color.y / 255.0);
    int b = (int) (31.0 * color.z / 255.0);
    return b | (g << 5) | (r << 11);
}

Vector toVectorColor(int i)
{
    float r = std::max(0.0f,(255.0f * ((float)((i >> 11) & 31)) / 31.0f));
    float g = std::max(0.0f,(255.0f * ((float)((i >> 5) & 63)) / 63.0f));
    float b = std::max(0.0f,(255.0f * ((float)(i & 31)) / 31.0f));
    return Vector(r,g,b);
}

bool colorsSimilar(Vector c1, Vector c2)
{
    float rd = c1.x - c2.x;
    float gd = c1.x - c2.x;
    float bd = c1.x - c2.x;
    return (pow(rd,2.0)+pow(gd,2.0)+pow(bd,2.0) < 7500.0);
}
