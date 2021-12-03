#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "DE1SoCfpga.h"
#include <iostream>
using namespace std;

/** 
 * Initialize general-purpose I/O 
 *  - Opens access to physical memory /dev/mem 
 *  - Maps memory into virtual address space 
 * 
 * @param fd	File descriptor passed by reference, where the result 
 *			of function 'open' will be stored. 
 * 			saves addresses to virtual memory which is mapped to physical, or MAP_FAILED on error. 
 */ 
void DE1SoCfpga::Initialize(int *fd) 
{
	// Open /dev/mem to give access to physical addresses
	*fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if (*fd == -1)			//  check for errors in openning /dev/mem
	{
		cout << "ERROR: could not open /dev/mem..." << endl;
		exit(1);
	}

	// Get a mapping from physical addresses to virtual addresses for standard devices
	char *virtual_base = (char *) mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, *fd, LW_BRIDGE_BASE);
	if (virtual_base == MAP_FAILED)		// check for errors
	{
		cout << "ERROR: mmap() failed..." << endl;
		close (*fd);		// close memory before exiting
		return;
	}

	// get a mapping from physical addresses to virtual addresses for vga pixel buffer
	char *vga_pixel_virtual_base = (char *) mmap( NULL, FPGA_PIXEL_SPAN, (PROT_READ | PROT_WRITE ), MAP_SHARED, *fd, FPGA_PIXEL_BASE);	
	if( vga_pixel_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap2() failed...\n" );
		close(*fd);
		return;
	}

	// get a mapping from physical addresses to virtual addresses for vga char buffer
	char *vga_char_virtual_base = (char *) mmap( NULL, FPGA_CHAR_SPAN, (PROT_READ | PROT_WRITE ), MAP_SHARED, *fd, FPGA_CHAR_BASE );	
	if( vga_char_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap3() failed...\n" );
		close(*fd);
		return;
	}

	pBase = virtual_base;
	vgaPixelBase = vga_pixel_virtual_base;
	vgaCharBase = vga_char_virtual_base;
}

/** 
 * Close general-purpose I/O. 
 * 
 * @param pBase	Virtual address where I/O was mapped. 
 * @param fd	File descriptor previously returned by 'open'. 
 */ 
void DE1SoCfpga::Finalize(char *pBase, int fd) 
{
	if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
	{
		cout << "ERROR: munmap() failed..." << endl;
		exit(1);
	}
	if (munmap (vgaPixelBase, FPGA_PIXEL_SPAN) != 0)
	{
		cout << "ERROR: munmap2() failed..." << endl;
		exit(1);
	}
	if (munmap (vgaCharBase, FPGA_CHAR_SPAN) != 0)
	{
		cout << "ERROR: munmap3() failed..." << endl;
		exit(1);
	}
	close (fd); 	// close memory
}

//Class constructor implementing Initialize to initialize the class's variables
DE1SoCfpga::DE1SoCfpga()
{
	Initialize(&fd);
}

//Class destructor implementing finalize to release the memory from use
DE1SoCfpga::~DE1SoCfpga()
{
	Finalize(pBase, fd);
	cout << "A DE1SoCfpga object has been destroyed.\n";
}

/** 
 * Write a 4-byte value at the specified general-purpose I/O location. 
 * 
 * @param offset	Offset where device is mapped. 
 * @param value	Value to be written. 
 */ 
void DE1SoCfpga::RegisterWrite(unsigned int reg_offset, int value) 
{ 
	* (volatile unsigned int *)(pBase + reg_offset) = value; 
} 

/** 
 * Read a 4-byte value from the specified general-purpose I/O location. 
 * 
 * @param offset	Offset where device is mapped. 
 * @return		Value read. 
 */ 
int DE1SoCfpga::RegisterRead(unsigned int reg_offset) 
{ 
	return * (volatile unsigned int *)(pBase + reg_offset); 
}

/** 
 * Write a 2-byte value at the specified pixel buffer location. 
 * 
 * @param offset	Offset where device is mapped. 
 * @param value	Value to be written. 
 */ 
void DE1SoCfpga::VGAPixelWrite(unsigned int reg_offset, int value) 
{ 
	* (volatile int16_t *)(vgaPixelBase + reg_offset) = value; 
} 

/** 
 * Read a 2-byte value from the specified general-purpose I/O location. 
 * 
 * @param offset	Offset where device is mapped. 
 * @return		Value read. 
 */ 
int DE1SoCfpga::VGAPixelRead(unsigned int reg_offset) 
{ 
	return * (volatile int16_t *)(vgaPixelBase + reg_offset); 
}
