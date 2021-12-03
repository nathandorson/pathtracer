#include "Devices.h"
#include "DE1SoCfpga.h"
#include <iostream>
#include <string.h>

using namespace std;

const char *tones[20] = {"C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A6","A#6","B6","C6","C#6","D6","D#6","E6","F6","F#6","G6"};
const int notes[20][3] = {{109, 57, 0}, {109, 1, 57}, {109, 94, 0}, {109, 1, 94}, {109, 121, 0}, {109, 113, 0}, {109, 1, 113}, {109, 61, 0}, {109, 1, 61}, {125, 119, 0}, {125, 1, 119}, {125, 124, 0}, {125, 57, 0}, {125, 1, 57}, {125, 94, 0}, {125, 1, 94}, {125, 121, 0}, {125, 113, 0}, {125, 1, 113}, {125, 61, 0}}; 
const int INT_MAX = 2147483647;

//Class constructor implementing Initialize to initialize the class's variables
Devices::Devices()
{
	initialValueLoadMPCore = RegisterRead(MPCORE_PRIV_TIMER_LOAD_OFFSET);
	initialValueControlMPCore = RegisterRead(MPCORE_PRIV_TIMER_CONTROL_OFFSET);
	initialValueInterruptMPCore = RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET);
}

//Class destructor implementing finalize to release the memory from use
Devices::~Devices()
{
	Hex_ClearAll();
	ClearVGA();
	RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, initialValueLoadMPCore);
	RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, initialValueControlMPCore);
	RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, initialValueInterruptMPCore);
	std::cout << "A Devices object has been destroyed.\n";
}

/** Turns off all the diplays**/
void Devices::Hex_ClearAll()
{
	RegisterWrite(HEX3_HEX0_OFFSET, 0x0);
	RegisterWrite(HEX5_HEX4_OFFSET, 0x0);
	reg0_hexValue = 0x0;
	reg1_hexValue = 0x0;
}

/** Clears a specified 7-segment display
* @param index The index to be cleared
*/
void Devices::Hex_ClearSpecific(int index)
{
	int relativeIndex = 8 * (index % 4);
	if(index > 3){
		RegisterWrite(HEX5_HEX4_OFFSET, (reg1_hexValue & ~(0xFF << relativeIndex)));
		reg1_hexValue = RegisterRead(HEX5_HEX4_OFFSET);
	}else{
		RegisterWrite(HEX3_HEX0_OFFSET, (reg0_hexValue & ~(0xFF << relativeIndex)));
		reg0_hexValue = RegisterRead(HEX3_HEX0_OFFSET);
	}
}

/** Writes a specified 7-segment display
* @param index The index to be written to
* @param value The hex code for the digit to display at that index
*/
void Devices::Hex_WriteSpecific(int index, int value)
{
	int relativeIndex = 8 * (index % 4);
	if(index > 3){
		RegisterWrite(HEX5_HEX4_OFFSET, (reg1_hexValue & ~(0xFF << relativeIndex)) | (value << relativeIndex));
		reg1_hexValue = RegisterRead(HEX5_HEX4_OFFSET);
	}else{
		RegisterWrite(HEX3_HEX0_OFFSET, (reg0_hexValue & ~(0xFF << relativeIndex)) | (value << relativeIndex));
		reg0_hexValue = RegisterRead(HEX3_HEX0_OFFSET);
	}
}

/** Reads the value of a switch
* - Uses base address of I/O
* @param pBase Base address returned by 'mmap'
* @param switchNum Switch number (0 to 9)
* @return Switch value read
*/
int Devices::Read1Switch(int switchNum)
{
    int currentSwitches = RegisterRead(SW_OFFSET);
    return (currentSwitches & (0x1 << switchNum)) == (0x1 << switchNum);
}

/** Sets a pin as input or output
* direction = 0 : pin as input
* direction = 1 : pin as output
* @param pin the pin
* @param direction the direction to set the pin as
**/
void Devices::SetPinDirection(int pin, int direction)
{
	int currentDirections = RegisterRead(DIRECTION_OFFSET);
	if(direction == 0) { RegisterWrite(DIRECTION_OFFSET, currentDirections & ~(0x1 << pin)); }
	if(direction == 1) { RegisterWrite(DIRECTION_OFFSET, currentDirections | (0x1 << pin)); }
}

/** Writes the given value to the pin
* @param pin the pin number to be written to
* @param value the value to write
*/
void Devices::WritePin(int pin, int value)
{
	int currentPins = RegisterRead(DATA_OFFSET);
	if(value == 0) { RegisterWrite(DATA_OFFSET, currentPins & ~(0x1 << pin)); }
	if(value == 1) { RegisterWrite(DATA_OFFSET, currentPins | (0x1 << pin)); }
}

/** Reads the given pin for its value
* @param pin the pin number to be read
*/
int Devices::ReadPin(int pin)
{
	int currentPins = RegisterRead(DATA_OFFSET);
    return ((currentPins & (0x1 << pin)) >> pin);
}

/** Writes the given color to the screen
* @param x the x coord of the pixel to be written
* @param y the y coord of the pixel to be written
* @param color the 16-bit color to write to that location
*/
void Devices::WritePixel(int x, int y, int color)
{
	int pixelOffset = 0x0 | (x << 1) | (y << 10);
	VGAPixelWrite(pixelOffset, color);
}

// Reads the color of a pixel in the pixel buffer
int Devices::ReadPixel(int x, int y)
{
    int pixelOffset = 0x0 | (x << 1) | (y << 10);
    return VGAPixelRead(pixelOffset);
}

/** Clears the display connected via VGA
*/
void Devices::ClearVGA()
{
	for(int r = 0; r < 320; r++)
	{
		for(int c = 0; c < 240; c++)
		{
			WritePixel(r,c,0);
		} 
	} 
}

/** Plays the given tone
* assumes we have a speaker attached to D0
* must be in the range C5 to G6
* @param tone the tone to be played
* @param duration the duration in seconds to play the tone
*/
void Devices::PlayTone(const char *tone, double duration)
{
	SetPinDirection(0, 1);	
	int index = -1; //index where we find the note in our list of tones to be played
	for(int i = 0; i < 20; i++)
	{
		if(strcmp(tones[i], tone) == 0) { index = i; }
	}
	if(index != -1) //if the input is in the list of playable tones
	{
		int timeleft = (int) (duration*200000000.0);
		int counter = countLow[index];
		RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
		RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
		int cycle = 1;
		for(int i = 0; i < 3; i++)
		{
			Hex_WriteSpecific(i, notes[index][i]);	
		}
		while(timeleft > 0)
		{
			if (RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0) 
			{
				RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flag bit by writing a 1 into its location
				WritePin(0, cycle % 2);
				cycle++;
				timeleft -= counter; //decrement time remaining by time we just waited for flag to go up again
			}
		}
		WritePin(0, 0); //write 0 to the pin to make sure there is no more tone
		cout << "Played note: " << tone << endl;
		Hex_ClearAll();
	}
	else
	{
		cout << "That is an invalid note.\n";
	}

}

// start the timer, up the prescaler so it takes longer to decrement
void Devices::StartTimer()
{
    RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, INT_MAX);
    RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 0x1 | (0xFF << 8)); //set prescaler to decrement counter every 256 cycles. Start counter
}

// return the time (in seconds) that the timer has been counting down
float Devices::ReadTimer()
{
    int val = RegisterRead(MPCORE_PRIV_TIMER_COUNTER_OFFSET);
    float timeRunning = ((float) (INT_MAX - val)) / 781250.0f;
    return timeRunning;
}

// stop the timer from counting down
void Devices::StopTimer()
{

}

// initialize keypad pins
void Devices::SetUpKeypad()
{
    SetPinDirection(9, 1);
    SetPinDirection(11, 1);
    SetPinDirection(13, 1);
    SetPinDirection(14, 1);
    SetPinDirection(15, 0);
    SetPinDirection(17, 0);
    SetPinDirection(19, 0);
    SetPinDirection(21, 0);
}

// polls the keypad for 20 ms. returns a value in the range [-1-15] to represent the button pressed on the keypad (or if none are pressed)
int Devices::PollKeypad()
{
    int delay = 1000000;
    RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, delay);
    RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
    int row = 0;
    while(row <= 3)
    {
        //scan the appropriate row
        WritePin(9, row==0 ? 0 : 1);
        WritePin(11, row==1 ? 0 : 1);
        WritePin(13, row==2 ? 0 : 1);
        WritePin(14, row==3 ? 0 : 1);
        while (RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) == 0) 
        {
            if(ReadPin(15) == 0) { return 0 + (4*row); }
            if(ReadPin(17) == 0) { return 1 + (4*row); }
            if(ReadPin(19) == 0) { return 2 + (4*row); }
            if(ReadPin(21) == 0) { return 3 + (4*row); }
        }
        RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flag bit by writing a 1 into its location
        row++;
    }
    return -1;
}
