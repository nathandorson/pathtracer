# Seven Segment Control Makefile

main: main.o Pathtracer.o RayMath.o 
	g++ -o main main.o Pathtracer.o RayMath.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp Pathtracer.h RayMath.h Camera.h
	g++ -g -Wall -c -std=c++0x main.cpp

RayMath.o: RayMath.h RayMath.cpp
	g++ -g -Wall -c -std=c++0x RayMath.cpp

Pathtracer.o: Pathtracer.h Pathtracer.cpp RayMath.h Camera.h 
	g++ -g -Wall -c -std=c++0x Pathtracer.cpp

clean:
	rm Pathtracer.o RayMath.o main.o main
