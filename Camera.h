#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <cmath>
#include <random>
#include "RayMath.h"

class Camera
{
public:
    Vector position;
    float pitch;
    float yaw;
    // roll omitted
    float fovX;
    float fovY;

    int rows;
    int columns;

    float halfViewWidth;
    float halfViewHeight;
    
    Vector topLeft;
    Vector topRight;
    Vector bottomLeft;

    Vector delHorizontal;
    Vector delVertical;

    Camera(Vector pos, float p, float y, int r, int c, float fov)
    {
        position = pos;
        pitch = p;
        yaw = y;
        rows = r;
        columns = c;
        fovX = fov;
        fovY = fovX * ((float) rows)/ ((float) columns);
        halfViewWidth = tan(fovX/2.0);
        halfViewHeight = tan(fovY/2.0);
        //std::cout << fovX << std::endl;
        //std::cout << fovY << std::endl;
        //std::cout << halfViewWidth << std::endl;
        //std::cout << halfViewHeight << std::endl;
        precomputeVectors();
    }
    Camera()
    {
        position = Vector();
        pitch = 0.0f;
        yaw = 0.0f;

    }

    //precomputes the top left, bottom left, and top right directed vectors coming out of the camera
    void precomputeVectors()
    {
        float pc = cos(pitch);
        float ps = sin(pitch);
        float yc = cos(yaw);
        float ys = sin(yaw);
        topLeft = rotateVectorZ(
            rotateVectorY(
                Vector(1.0, halfViewWidth, -1.0 * halfViewHeight),
                pc, ps
            ),
            yc, ys
        );
        bottomLeft = rotateVectorZ(
            rotateVectorY(
                Vector(1.0, halfViewWidth, halfViewHeight),
                pc, ps
            ),
            yc, ys
        );
        topRight = rotateVectorZ(
            rotateVectorY(
                Vector(1.0, -1.0 * halfViewWidth, -1.0 * halfViewHeight),
                pc, ps
            ),
            yc, ys
        );
        delHorizontal = (topRight - topLeft)/(float)columns;
        delVertical = (bottomLeft - topLeft)/(float)rows;
    }

    void moveBy(Vector v, float p, float y)
    {
        position = position + v;
        pitch = pitch + p;
        yaw = yaw + y;
        precomputeVectors();
    }

    // gets the ray originating at the camera location and pointed towards the given pixel position (can be anywhere within a pixel)
    Ray getRay(float x, float y)
    {
        Vector dir = normalize(topLeft + (delHorizontal * x) + (delVertical * y));
        return Ray(position, dir);
    }

    // gets a random ray that passes through the pixel defined by the coordinates r and c
    Ray getRandomRay(float r, float c)
    {
        return getRay(c + (float) rand()/RAND_MAX, r + (float) rand()/RAND_MAX);
    }

    // get the ray that passes through the middle of the pixel defined by r and c
    Ray getMiddleRay(float r, float c)
    {
        Vector dir = normalize(topLeft + (delHorizontal * (c + 0.5)) + (delVertical * (r + 0.5)));
        return Ray(position, dir);
    }

};

#endif
