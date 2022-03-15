#include "Pathtracer.h"
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

//Class constructor implementing Initialize to initialize the class's variables
/**
 * @param raysPerPixel rays shot into the scenes for each pixel
 * @param d depth of search (number of times a ray can bounce)
 * @param branchesPerLevel the number of times each ray splits when it bounces
 **/
Pathtracer::Pathtracer(int raysPerPixel, int d, int branchesPerLevel, float prob)
{
    // Info about the algorithm used to trace paths
    rpp = raysPerPixel;
    depth = d;
    branches = branchesPerLevel;
    probability = prob;
    
    // Info about how everything is displayed
    rows = 360;
    columns = 480;
    fov = 90;
    camera = Camera(Vector(-1.0,0.0,3.0), 0.0f, 0.0f, rows, columns, fov);
    // need some way to write pixels to screen
}

//Class destructor calls to delete the board which implements finalize to release the memory from use
Pathtracer::~Pathtracer()
{
    for(int i = 0; i < scene.size(); i++)
    {
        Shape* s = scene.at(i);
        delete s;
    }
}

void Pathtracer::setup()
{
    //scene.push_back(new Sphere(Vector(5.0, 0.0, 7.0), 1.0f, (float) 0.0, (float) 10.0, Vector(255,255,255)));

    //scene.push_back(new Sphere(Vector(4.0, 1.0, 0.0), 1.0f, (float) 1.0, (float) 0.0, Vector(255, 255, 255)));
    //scene.push_back(new Sphere(Vector(6.0, -2.0, 0.0), 1.0f, (float) 0.0, (float) 0.0, Vector(240, 240, 240)));
    //scene.push_back(new Sphere(Vector(7.0, 0.0, 0.5), 1.5f, (float) 0.0, (float) 0.0, Vector(200, 200, 200)));

    //scene.push_back(new Plane(Vector(0.0, 0.0, 7.0), Vector(0.0,0,-1.0), (float) 0.0, (float) 0.0, Vector(100,100,100)));  //ceiling
    //scene.push_back(new Plane(Vector(8.0, 0.0, 0.0), Vector(-1.0,0.0,0.0), (float) 0.0, (float) 0.0, Vector(200, 255, 200)));//back wall
    //scene.push_back(new Plane(Vector(-1.0, 0.0, 0.0), Vector(1.0,0,0.0), (float) 0.0, (float) 0.0, Vector(255, 255, 255)));//wall behind camera
    //scene.push_back(new Plane(Vector(0.0, 4.0, 0.0), Vector(0.0,-1.0,0.0), (float) 0.0, (float) 0.0, Vector(200,200,255)));//right wall
    //scene.push_back(new Plane(Vector(0.0, -4.0, 0.0), Vector(0.0,1.0,0.0), (float) 0.0, (float) 0.0, Vector(255,200,200)));//left wall

    //scene.push_back(new Plane(Vector(-1.0,0.0,0.0), Vector(1.0,1.0,0.0), (float) 1.0f, 0.0, Vector(255,255,255)));
    scene.push_back(new Plane(Vector(0.0, 0.0, -1.0), Vector(0.0,0,1.0), (float) 0.0, (float) 0.0, Vector(200, 200, 200)));  //floor
    scene.push_back(new Sphere(Vector(20.0, 110.0, 20.0), 45.0f, (float) 0.0, (float) 7.0, Vector(255,255,255)));

    scene.push_back(new Sphere(Vector(4.0, 1.0, 1.0), 3.0f, (float) 1.0, (float) 0.0, Vector(255, 255, 255)));
    //scene.push_back(new Sphere(Vector(6.0, -2.0, 0.0), 1.0f, (float) 0.0, (float) 0.0, Vector(240, 240, 240)));
    scene.push_back(new Sphere(Vector(7.0, 0.0, 0.5), 1.5f, (float) 0.0, (float) 0.0, Vector(200, 200, 200)));
    scene.push_back(new Sphere(Vector(7.0, -5.0, 1.0), 2.0f, (float) 0.5, (float) 0.0, Vector(255, 200, 200)));
    //scene.push_back(new Sphere(Vector(14.0, 2.0, 1.0), 2.0f, (float) 0.5, (float) 0.0, Vector(255, 200, 255)));

}

void Pathtracer::main()
{

    int algo = 0; //use monte carlo algorithm w/ no optimization by default
    char key = 'z';   

    cout << "You can interact with the pathtracer via the keyboard!" << endl;
    cout << "Available commands:" << endl;
    cout << "\t(1) - Move camera left" << endl;
    cout << "\t(2) - Move camera right" << endl;
    cout << "\t(3) - Pivot camera left" << endl;
    cout << "\t(4) - Pivot camera right" << endl;
    cout << "\t(5) - Pitch camera up" << endl;
    cout << "\t(6) - Pitch camera down" << endl;
    cout << "\t(7) - Move light left" << endl;
    cout << "\t(8) - Move light right" << endl;
    cout << "\t(9) - Move light away from camera" << endl;
    cout << "\t(0) - Move light closer to camera" << endl;
    cout << "\t(a) - Set pathtracer to use unoptimized algorithm (default)" << endl;
    cout << "\t(b) - Set pathtracer to use optimized algorithm" << endl;
    cout << "\t(c) - Stop program" << endl;
    cout << "\t(r) - Render a new frame" << endl;
    cout << "------------------------------------------------------" << endl;

    sf::RenderWindow window(sf::VideoMode(480, 360), "Pathtracer");

    while(key != 'c')
    {
        cin >> key;
        switch(key){
            case (-1):
                break;
            case ('1'):
                moveCamera(Vector(0,-1.0f,0));
                cout << "Moved camera left\n";
                break;
            case ('2'):
                moveCamera(Vector(0,1.0f,0));
                cout << "Moved camera right\n";
                break;
            case ('3'):
                rotateCamera(0,-0.1f);
                cout << "Rotated camera left\n";
                break;
            case ('4'):
                rotateCamera(0,0.1f);
                cout << "Rotated camera right\n";
                break;
            case ('5'):
                rotateCamera(-0.1f,0);
                cout << "Pitched camera up\n";
                break;
            case ('6'):
                rotateCamera(0.1f,0);
                cout << "Pitched camera down\n";
                break;
            case ('7'):
                moveLight(Vector(0.0,-1.0f,0.0));
                cout << "Moved light left\n";
                break;
            case ('8'):
                moveLight(Vector(0.0,1.0f,0.0));
                cout << "Moved light right\n";
                break;
            case ('9'):
                moveLight(Vector(1.0f,0.0,0.0));
                cout << "Moved light away\n";
                break;
            case ('0'):
                moveLight(Vector(-1.0f,0.0,0.0));
                cout << "Moved light closer\n";
                break;
            case ('a'):
                algo = 0;
                cout << "Using default Monte Carlo pathtracer algorithm\n";
                break;
            case ('b'):
                algo = 1;
                cout << "Now using interpolation algorithm\n";
                break;
            case ('r'):
                {
                    cout << "Rendering scene..." << endl;

                    if(algo == 0){
                        renderScene(window);
                    }                    
                    if(algo == 1){
                        renderSceneInterpolate();
                    }

                    cout << "Ready for more input..." << endl;
                }
                break;
            default:
                break;
        }
    }
    char userIn;
    cout << "Input any key to stop...\n";
    cin >> userIn;
    return;
}

void Pathtracer::moveLight(Vector v)
{
    scene[0]->moveBy(v);
}

// helper function to rotate the camera by a pitch and yaw value
void Pathtracer::rotateCamera(float p, float y)
{
    camera.moveBy(Vector(), p, y);
}

// helper function to displace the camera in 3d space by a vector
void Pathtracer::moveCamera(Vector v)
{
    camera.moveBy(v, 0, 0);
}

// renders the world with a monte carlo algorithm
void Pathtracer::renderScene(sf::RenderWindow& window)
{
    sf::Uint8* pixels = new sf::Uint8[360*480*4];
    sf::Texture texture;
    texture.create(480,360);
    sf::Sprite sprite(texture);
    Vector** totalColors = new Vector*[rows];
    for(int r = 0; r < rows; r++) {
        totalColors[r] = new Vector[columns];
    }
    for(int frame = 1; frame <= rpp; frame++) {
        for(int c = 0; c < columns; c++)
        {
            for(int r = 0; r < rows; r++)
            {
                //cout << "Shooting ray " << n << " for row " << r << " and column " << c << endl;
                Ray ray = camera.getRandomRay(r, c);
                Vector color = rayColor(ray, 0);
                totalColors[r][c] = totalColors[r][c] + color; // colorMax(color)
                int pixelIndex = 480*4*r+4*c;
                Vector avgColor = totalColors[r][c] / ((float) frame);
                pixels[pixelIndex] = (sf::Uint8) avgColor.x; 
                pixels[pixelIndex+1] = (sf::Uint8) avgColor.y; 
                pixels[pixelIndex+2] = (sf::Uint8) avgColor.z;
                pixels[pixelIndex+3] = 255; 
            }
            texture.update(pixels);
            //window.clear();
            window.draw(sprite);
            window.display();
        }

    }
}

// renders the world and interpolates
void Pathtracer::renderSceneInterpolate()
{
    Vector colors[rows][columns];
    for(int c = 0; c < columns; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            Vector color = Vector();
            for(int n = 0; n < rpp; n++)
            {
                if(r==rows-1 || c==columns-1 || (c%2==0 && r%2==0))
                {
                    Ray ray = camera.getRandomRay(r, c);
                    color = color + rayColor(ray, 0);
                }
            }
            color = colorMax(color / rpp);
            colors[r][c] = color;
            // write color to pixel //todo
        }
    }
    for(int r = 0; r < rows-2; r+=2)
    {
        for(int c = 0; c < columns-2; c+=2)
        {
            Vector color = colors[r][c];
            Vector nextColor = colors[r+2][c];
            Vector retColor;
            if(colorsSimilar(color, nextColor))
            {
                retColor = colorMax((color + nextColor) / 2.0f);
            }
            else
            {
                Vector color = Vector();
                for(int n = 0; n < rpp; n++)
                {
                    Ray ray = camera.getRandomRay(r+1, c);
                    color = color + rayColor(ray, 0);
                }
                retColor = colorMax(color / rpp);
            }
            colors[r+1][c] = retColor;
            // write color to pixel //todo

            nextColor = colors[r+2][c+2];
            retColor = Vector();
            if(colorsSimilar(color, nextColor))
            {
                retColor = colorMax((color + nextColor) / 2.0f);
            }
            else
            {
                Vector color = Vector();
                for(int n = 0; n < rpp; n++)
                {
                    Ray ray = camera.getRandomRay(r+1, c+1);
                    color = color + rayColor(ray, 0);
                }
                retColor = colorMax(color / rpp);
            }
            colors[r+1][c+1] = retColor;
            // write color to pixel //todo

            nextColor = colors[r][c+2];
            retColor = Vector();
            if(colorsSimilar(color, nextColor))
            {
                retColor = colorMax((color + nextColor) / 2.0f);
             }
            else
            {
                Vector color = Vector();
                for(int n = 0; n < rpp; n++)
                {
                    Ray ray = camera.getRandomRay(r, c+1);
                    color = color + rayColor(ray, 0);
                }
                retColor = colorMax(color / rpp);
            }
            colors[r][c+1] = retColor;
            // write color to pixel //todo
        }
    }
}

// renders the world with a monte carlo algorithm with russian roulette and pixel interpolation and direct lighting
void Pathtracer::renderSceneOptimized()
{
    for(int c = 0; c < columns; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            Vector color = Vector();
            for(int n = 0; n < rpp; n++)
            {
                //cout << "Shooting ray " << n << " for row " << r << " and column " << c << endl;
                Ray ray = camera.getRandomRay(r, c);
                color = color + rayColorRR(ray, 0, probability);
            }
            color = color / rpp; // colorMax(color)
            // write color to pixel //todo
        }
    }
}

// Get 24 bit color of pixel based on ray -- must then convert this to 16 bit color for displaying on vga
// this returns a color divided by the probability that the ray would take that path
Vector Pathtracer::rayColor(Ray r, int bounces)
{

    bool debugMode = false;

    //use monte carlo algorithm and ray bouncing based on surface type. bouncing until we hit light source or run out of bounces
    if(bounces > depth)
    {
        return Vector(0,0,0);
    }

    bounces++;

    Intersection intersect = castRay(r);

    if(!intersect.hit) //did not hit anything
    {
        return Vector(0,0,0);
    }

    Shape* shapeHit = intersect.shape;

    if(shapeHit->emittance >= 1.0)
    {   
        return shapeHit->color * shapeHit->emittance;
    }

    Vector intersectPt = intersect.pt;
    Vector normal = shapeHit->normal(intersectPt); //get the normal to the shape we hit at the point of intersection
    if( dot(normal, r.direction) > 0.0) //normal vector and ray point in the same hemisphere. flip the normal.
    {
        normal = normal * -1.0;
    }

    //calculate diffuse color if shape is diffuse at all
    Vector thisColor = shapeHit->color;
    Vector diffColor = thisColor * shapeHit->emittance; //for ambient lighting
    float specularity = shapeHit->specularity;
    if(specularity < 1.0f)
    {
        Vector diffuseDirection = randomHemisphereVector(normal);
        Vector hitColor = rayColor(Ray(intersectPt, diffuseDirection), bounces);
        diffColor = diffColor + colorMultiply(thisColor, hitColor) * dot(normal, diffuseDirection) * 2.0f;
    }
    //calculate specular color if shape has specular reflections
    Vector specColor = Vector();
    if(specularity > 0.0f)
    {
        Vector bounceDirection = reflect(r.direction, normal);
        Vector hitColor = rayColor(Ray(intersectPt, bounceDirection), bounces);
        specColor = specColor + hitColor;
    }
    return diffColor * (1.0f-specularity) + specColor * specularity;
}

// Get 24 bit color of pixel based on ray -- must then convert this to 16 bit color for displaying on vga
// this returns a color divided by the probability that the ray would take that path
Vector Pathtracer::rayColorRR(Ray r, float p, int bounces)
{

    bool debugMode = false;

    Vector retColor = Vector();

    bounces++;

    Intersection intersect = castRay(r);
    
    if(!intersect.hit) //did not hit anything
    {
        return Vector();
    }

    Shape* shapeHit = intersect.shape;

    if(shapeHit->emittance > 0.0)
    {   
        return shapeHit->color * shapeHit->emittance;
    }
   
    if((float)random()/RAND_MAX < p && bounces > depth)
    {
        return retColor;
    }
 
    Vector intersectPt = intersect.pt;
    Vector normal = shapeHit->normal(intersectPt); //get the normal to the shape we hit at the point of intersection
    if( dot(normal, r.direction) > 0.0) //normal vector and ray point in the same hemisphere. flip the normal.
    {
        normal = normal * -1.0;
    }

    if(shapeHit->specularity == 0.0) //perfectly diffuse object
    {
        //cout << "Hit diffuse object\n";
        //cout << normal.x << " " << normal.y << " " << normal.z << endl;
        //cout << "normal: "; //
        //normal.print(); //
        Vector thisColor = shapeHit->color;
        //thisColor.print();
        for(int n = 0; n < branches; n++)
        {
            Vector diffuseDirection = randomHemisphereVector(normal);
            //cout << "diffuse vector: "; //
            //diffuseDirection.print(); //
            Vector hitColor = rayColorRR(Ray(intersectPt, diffuseDirection), bounces, probability);
            retColor = retColor + colorMultiply(thisColor, hitColor) * dot(normal, diffuseDirection) * 2.0f * (1.0f/p);
        }
        if(debugMode && (retColor.x > 0 || retColor.y > 0 || retColor.z > 0))
        {
            cout << "Ray " << bounces << " that hit:\n";
            r.origin.print();
            r.direction.print();
        }
        return retColor / ((float) branches); // colorMax(retColor);
    }

    if(shapeHit->specularity == 1.0) //perfectly specular object
    {
        for(int n = 0; n < branches; n++)
        {
            Vector bounceDirection = reflect(r.direction, normal);
            Vector hitColor = rayColorRR(Ray(intersectPt, bounceDirection), bounces, probability);
            retColor = retColor + hitColor;
        }
        return retColor / ((float) branches);
    }

    return Vector(0,0,0);
}

// cast a ray into the scene : find the intersection with the first object it hits
Intersection Pathtracer::castRay(Ray r)
{
    Intersection foundIntersect = Intersection();
    float closestDist = -1.0f;
    for(int i = 0; i < scene.size(); i++)
    {
        Intersection intersect = scene[i]->intersect(r);
        if(intersect.hit)
        {
            float dist = intersect.dist;
            if((dist < closestDist && dist > 0.0f) || closestDist < 0.0f)
            {
                closestDist = dist;
                foundIntersect = intersect;
            }
        }
    }

    return foundIntersect;
}

// generate an intersection 
Intersection::Intersection(float d, Vector p, Shape* s)
{
    hit = true;
    dist = d;
    pt = p;
    shape = s;
}
// generate an intersection that's actually when something is not hit
Intersection::Intersection()
{
    hit = false;
    dist = -1;
    pt = Vector();
}

Shape::Shape() {}
Shape::~Shape() {
    cout << "Deleted a shape\n";
}

// implementation of spheres as shapes that can be hit in this pathtracer
Sphere::Sphere(Vector c, float r, float s, float e, Vector col)
{
    center = c;
    radius = r;
    specularity = s;
    emittance = e;
    color = col;
}
Sphere::~Sphere() {}
// first point of intersection between sphere and ray, 
Intersection Sphere::intersect(Ray r)
{
//    cout << "Ray " << r.direction.x << ", " << r.direction.y << ", " << r.direction.z << endl;
    float a = dot(r.direction, r.direction);
    float b = 2.0 * dot(r.direction, r.origin - center);
    float c = dot(r.origin - center, r.origin - center) - pow(radius, 2.0);
    float discrim = pow(b, 2.0) - (4.0 * a * c);
    if(discrim < 0)
    {
        return Intersection();
    }
    float t1 = ( -1.0 * b + sqrt(discrim) ) / ( 2.0 * a);
    float t2 = ( -1.0 * b - sqrt(discrim) ) / ( 2.0 * a);

    if(t1 < 0 && t2 < 0)
    {
        return Intersection();
    }

    float t = min(t1, t2);
    if(t < 0)
    {
        t = max(t1, t2);
    }

    Vector intersectionPt = r.origin + (r.direction * t);
    //cout << "Hit at " << intersectionPt.x << ", " << intersectionPt.y << ", " << intersectionPt.z << endl;
    return Intersection(magnitude(intersectionPt - r.origin), intersectionPt, this);
}
Vector Sphere::normal(Vector p)
{
    return normalize(p-center);
}
void Sphere::moveBy(Vector v)
{
    center = center + v;
}

// implementation of planes as shapes that can be hit in this pathtracer
Plane::Plane(Vector p, Vector n, float s, float e, Vector col)
{
    point = p;
    float magnitude = pow(pow(n.x,2)+pow(n.y,2)+pow(n.z,2),2);
    norm = Vector(n.x / magnitude, n.y / magnitude, n.z / magnitude);
    specularity = s;
    emittance = e;
    color = col;
}
Plane::~Plane() {
    cout << "Deleted a shape\n";
}
Intersection Plane::intersect(Ray r) 
{
    float dotDirNorm = dot(r.direction, norm);
    if(abs(dotDirNorm) < 0.000001f)
    {
        return Intersection();
    }
    float t = dot(point - r.origin, norm) / dotDirNorm;
    if(t < 0)
    {
        return Intersection();
    }
    Vector rayToIntersect = r.direction * t;
    Vector intersectionPt = r.origin + rayToIntersect;
    return Intersection(magnitude(rayToIntersect), intersectionPt, this);
}
Vector Plane::normal(Vector p)
{
    return norm;
}
void Plane::moveBy(Vector v)
{
    point = point;
}
