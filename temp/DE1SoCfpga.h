#ifndef DE1SOCFPGA_H
#define DE1SOCFPGA_H

// Physical base address of FPGA Devices 
const unsigned int LW_BRIDGE_BASE 	= 0xFF200000;  // Base offset 
// Length of memory-mapped IO window 
const unsigned int LW_BRIDGE_SPAN 	= 0x00DEC700;  // Address map size

// Physical base address of VGA Pixel Buffer 
const unsigned int FPGA_PIXEL_BASE 	= 0xC8000000;  // Base offset 
// Length of memory-mapped IO window 
const unsigned int FPGA_PIXEL_SPAN 	= 0x00040000;  // Address map size

// Physical base address of VGA Char Buffer
const unsigned int FPGA_CHAR_BASE 	= 0xC9000000;  // Base offset 
// Length of memory-mapped IO window 
const unsigned int FPGA_CHAR_SPAN 	= 0x00002000;  // Address map size

// Cyclone V FPGA device addresses
const unsigned int LEDR_OFFSET 		= 0x0; //real ADDRESS of RED LED - LW_BRIDGE_BASE ;
const unsigned int SW_OFFSET 		= 0x40; //real ADDRESS of SWITCH - LW_BRIDGE_BASE ;
const unsigned int KEY_OFFSET 		= 0x50; //real ADDRESS of PUSH BUTTON - LW_BRIDGE_BASE ;
const unsigned int HEX3_HEX0_OFFSET	= 0x20; //real ADDRESS of 7-segment displays 0-3
const unsigned int HEX5_HEX4_OFFSET	= 0x30; //real ADDRESS of 7-segment displays 4-5
const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0xDEC600; //points to LOAD register
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0xDEC604; //points to COUNTER register
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0xDEC608; //points to CONTROL register
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0xDEC60C; //points to INTERRUPT register
const unsigned int DATA_OFFSET = 0x60; //points to JP1 data pins
const unsigned int DIRECTION_OFFSET = 0x64; //points to JP1 direction register

class DE1SoCfpga
{
	public:
		void Initialize(int *fd); 

		void Finalize(char *pBase, int fd); 

		char *pBase;
		char *vgaPixelBase;
		char *vgaCharBase;
		int fd;

		DE1SoCfpga();

		~DE1SoCfpga();

		void RegisterWrite(unsigned int reg_offset, int value); 

		int RegisterRead(unsigned int reg_offset); 

		void VGAPixelWrite(unsigned int reg_offset, int value); 

		int VGAPixelRead(unsigned int reg_offset); 

};

#endif
