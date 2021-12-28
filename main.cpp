#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <iostream>
#include <random>
#include <time.h>
#include "Pathtracer.h"
using namespace std;

int main() 
{ 
	srand(time(0));
    
	cout << "---Pathtracer---" << endl;

	int rpp, depth, branches;
    float p;

	cout << "How many rays do you want to shoot per pixel? : ";
	cin >> rpp;
	cout << "To what depth should we fire rays? : ";
	cin >> depth;
	//cout << "How many branches per bounce? : ";
	//cin >> branches;
	branches = 1;
    //cout << "What should the probability be to fire more rays\nafter the set depth when using the better algorithm? : ";
    //cin >> p;
	p = 0.01f;

	Pathtracer tracer = Pathtracer(rpp, depth, branches, p);

    tracer.setup();
	tracer.main();
	
	//Finish up
	cout << "Terminating...!" << endl;
	
    return 0; 
}
