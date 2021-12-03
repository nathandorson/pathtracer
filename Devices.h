#ifndef DEVICES_H
#define DEVICES_H

#include "DE1SoCfpga.h"

const unsigned int countLow[20] = {191113,180384,170262,160707,151685,143172,135136,127552,120393,113636,107258,101238,95556,90195,85128,80353,75843,71587,67567,63775};
const unsigned int countHigh[20] = {382226,360768,340524,321414,303370,286344,270272,255104,240786,227272,214516,202476,191112,180390,170256,160706,151686,143174,135135,127550};

class Devices : public DE1SoCfpga{
	private:
		unsigned int reg0_hexValue;
		unsigned int reg1_hexValue;
		int initialValueLoadMPCore;
		int initialValueControlMPCore;
		int initialValueInterruptMPCore;
	public:
		void Initialize(int *fd); 
		void Finalize(char *pBase, int fd); 

		Devices();

		~Devices();

		void Hex_ClearAll();

		void Hex_ClearSpecific(int index);

		void Hex_WriteSpecific(int index, int value);

		int Read1Switch(int switchNum);

		void PlayTone(const char *tone, double duration);

		void SetPinDirection(int pin, int direction);

		void WritePin(int pin, int value);
        
        int ReadPin(int pin);

        void SetUpKeypad();

        int PollKeypad();

        void StartTimer();

        float ReadTimer();

        void StopTimer();

		void WritePixel(int x, int y, int color);

        int ReadPixel(int x, int y);

		void ClearVGA();
};

#endif
