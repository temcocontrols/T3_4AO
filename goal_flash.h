// Header file for Rx2 Flash Variables Library
// Version 1.02 (13-Sep-2000)
// (C) Embedded Systems Academy 2000

// *******************************************************
// User must copy this goal_flash.h file to their directory
//	- the goal_flash.c should never change
//	- in order to change around configuration you can change around block names which will add up to block count
//	- need to copy the enum of block names in your program to make sure using the same enumeration
// *******************************************************

// *******************************************************
//	Goal Flash
//		- can select amount of blocks needed

//		- can select each block size individualy
//		- can select element size within each block individualy
//		- can select starting address
// *******************************************************


// must comply with memory setup in define.h file
// block names
typedef enum
{
	FLASH_MEMORY,
 	BLOCK_COUNT
};


unsigned int const xdata BLOCK_SIZE_CONST[BLOCK_COUNT]=
{
	1024	// FLASH_MEMORY
};


unsigned int const xdata START_BLOCK_CONST[BLOCK_COUNT][2]=
{
	//{30,31},	// FLASH_MEMORY, always requires an extra page for backup
//	{28,29}	// CALIBRATION_MEMORY
    {54,55},	// MEMORY_RS485
	//{100,101},	// MEMORY_DALI

};



unsigned int const	xdata ELEMENT_SIZE_CONST[BLOCK_COUNT]=
{
	2,		// FLASH_MEMORY, for 2 bytes long
};






