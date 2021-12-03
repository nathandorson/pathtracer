# Seven Segment Control Makefile

main: main.o Devices.o DE1SoCfpga.o Pathtracer.o RayMath.o
	g++ -o main main.o Devices.o DE1SoCfpga.o Pathtracer.o RayMath.o

main.o: Devices.h main.cpp DE1SoCfpga.h Pathtracer.h RayMath.h Camera.h
	g++ -g -Wall -c -std=c++0x main.cpp

RayMath.o: RayMath.h RayMath.cpp
	g++ -g -Wall -c -std=c++0x RayMath.cpp

DE1SoCfpga.o: DE1SoCfpga.h DE1SoCfpga.cpp
	g++ -g -Wall -c DE1SoCfpga.cpp

Devices.o: Devices.h Devices.cpp DE1SoCfpga.h
	g++ -g -Wall -c Devices.cpp

Pathtracer.o: Pathtracer.h Pathtracer.cpp Devices.h DE1SoCfpga.h RayMath.h Camera.h
	g++ -g -Wall -c -std=c++0x Pathtracer.cpp

clean:
	rm DE1SoCfpga.o Pathtracer.o Devices.o RayMath.o main.o main
