// *********************************************************************************** //
//	goal flash memory management
// *******************************************************
// User must copy this goal_flash.h file to their directory
//	- the goal_flash.c should never change
//	- in order to change around configuration you can change around block names which will add up to block count
//	- need to copy the enum of block names in your program to make sure using the same enumeration
// *******************************************************
// *********************************************************************************** //
// Rx2 Flash Variables Library
// Version 1.10 (14-Sep-2000) by AA
// (C) Embedded Systems Academy 2000

// Requires Rx2IAPLIB

// History:
//	Feb-2006
//		- Robert found some problems and corrected them.  Problems were found when he was working on T3-module 8ch
//
// 	Sept-2005
//		- Robert had found the same flash version on the net and has re-written it according to his own use
//		- RL, trying to move all defines into the goal_flash.h file so that can easily use subroutines
// 	 2-Feb-2004
//		-Modified the read and write routines to automatically adjust for one or 2 bytes
//		-Modified the read routine to return data rather than a pointer to data
//		-Wrote new iap read and write routines in C code rather than assembly
//   Version 1.10 (14-Sep-2000) by AA
//     - Removed fv_xdata_compress_table - too risky to use in reality
//     - Swapped order of id byte and size byte in table entries
//     - Modified flash_write_no_recovery to mark old entry as unused once new
//       entry has been written and write bytes in a fail-safe order
//     - Modified fv_fast_write_byte to mark old entry as unused once
//       new entry has been written and write bytes in a fail-safe order
//     - Modified flash_read to mark older entries with same ID as
//       unused
//     - Modified flash_init to accept two blocks, determine which one holds
//       the current table (if any) and return a pass or fail result
//     - Modified fv_flash_compress_table to use the unused block and
//       execute in a fail-safe way
//     - Renamed fv_flash_compress_table to fv_garbage_collection
//   Version 1.02 (07-Sep-2000) by AA
//     - Changed the way flash variables are read - now pointer is
//       returned, pointing to the value
//     - Commented global variables
//     - Renamed fv_compress_table to fv_xdata_compress_table
//     - Added fv_flash_compress_table
//     - Simplified return values of flash_write
//     - Added fv_fast_write_byte
//   Version 1.01 (30-Aug-2000) by AA
//     - Fixed bug whereby flash_ptr was not properly initialized
//       in flash_init
//   Version 1.00 (23-Aug-2000) by AA
//     - First version

// Format of table:
//   blockstatusbyte,entry,entry,entry,...
//   Possible values of blockstatusbyte (1 byte):
//     0x55 = block contains a valid table
//     0x00 = block contains an invalid table
//     0xff = block is empty or contains an invalid table
// Format of a single entry:
//   n,id,databyte,databyte,...
//   n (1 byte) = number of databytes in entry
//   id (1 byte) = unique flash variable identifier
//   databyte (1 byte) = 1 byte of data that makes up the value 

	 

// include header files
#include "c8051f120.h"
#include "goal_flash.h"


// --- defines --------------------------------------------------
// define constants for the block status byte (first byte in each block)
#define FLASH_BLOCK_IN_USE 		0x77
#define FLASH_BLOCK_INVALID 	0x00

#define FLASH_PROG_ERROR 		1
#define FLASH_TABLE_FULL 		2

#define NO_DEFINE_ADDRESS		0xB4DC
#define NULL					0
//-----------------------------------------------------------------


// ------- function prototype --------------------------------------------------- 
extern void watchdog();
extern void delay_us(unsigned int time);

void flash_init(void);
bit flash_set_block(unsigned char block_select);

bit flash_write_int(unsigned char id, unsigned int value, unsigned char block_select);
bit flash_read_int(unsigned char id, unsigned int *value, unsigned char block_select);
// -------------------------------------------------------------------------------
bit FlashRead_Absolute(unsigned int location, unsigned char *value);




// --- variables ---------------------------------------------------------------
// flash_ptr always points to the next free location after the top
// of the table
unsigned char code *flash_ptr[BLOCK_COUNT];
// block_base holds the base address of the flash block being used
// next_block_top holds the top address of the flash block being used
unsigned int block_base[BLOCK_COUNT], next_block_top[BLOCK_COUNT];
// flash_unused_block holds the high byte of the base address of the block not in use
unsigned char flash_unused_block[BLOCK_COUNT];

unsigned char iap_program_data_RAM(unsigned char val, unsigned int addr);
void iap_erase_page (unsigned char block);

// ------------------------------------------------------------------------------

unsigned char iap_program_data_RAM(unsigned char val, unsigned int addr)  //it is ok,
{
	unsigned char xdata *pwrite;
    SFRPAGE   = LEGACY_PAGE;
	EA = 0;
	SFRPAGE =  CONFIG_PAGE;	 //swtich to page f
	CCH0CN &= ~0x01;		 //
	SFRPAGE   = LEGACY_PAGE; //swtich to page 0
	PSCTL 	= 0X05;
	//PSCTL 	= 0X05;
	FLSCL	= 0X01 ;
	pwrite = addr;
	*pwrite = val;
	PSCTL 	  &= 0X00 ;
	FLSCL	  &= 0X00 ;
	EA = 1;
//	PSCTL 	  |= (0x01<<2);
//	FLSCL	  |= 0X01 ;
//	PSCTL 	  |= 0x01;
//	pwrite = addr;
//	*pwrite = val;
//	PSCTL 	  &= ~0x01;
//	FLSCL	  &= ~0X01 ;
//	PSCTL 	  &= ~(0x01<<2);
//	EA = 1;
	return 0;

}
void iap_erase_page (unsigned char block)
{
	unsigned char xdata *pwrite; 
    SFRPAGE   = LEGACY_PAGE;
	EA = 0;
  	PSCTL = 0X07;
	FLSCL = 0x01 ;
	pwrite = block*100;
	*pwrite = 0;
	PSCTL = 0X00;
	PSCTL = 0X00;
//	PSCTL |= (0x01<<2);
//	FLSCL |= 0x01 ;
//	PSCTL |= (0x01<<1);
//	PSCTL |= 0x01;
//	pwrite = block*100;
//	*pwrite = 0;
//	PSCTL &= ~(0x01<<1);
//	PSCTL &= ~0x01;
//	FLSCL &= ~0x01 ;
//	PSCTL &= ~(0x01<<2);
	EA = 1;
}



 

 
unsigned char iap_program_data_byte(unsigned char val, unsigned int addr)  //it is ok,
{
	unsigned char xdata *pwrite;
    SFRPAGE   = LEGACY_PAGE;
	EA = 0;
	FLSCL = 0x01;
	PSCTL = 0x01;
	//if(b.eeprom[EEP_CONFIGURATION - MAXEEPCONSTRANGE] == 1 && addr >= 60416 && addr <= 60927)
	//addr -= 512;
 
	pwrite = addr;
	*pwrite = val;
	PSCTL = 0;
	FLSCL = 0x0;

	EA = 1;
	return 0;

}
// 128k , total 128 blocks
void iap_erase_block(unsigned char block)
{
	unsigned char xdata *pwrite; 
    SFRPAGE   = LEGACY_PAGE;
	EA = 0;
    //PSBANK = 0X00; // Select bank1
	//if(b.eeprom[EEP_CONFIGURATION - MAXEEPCONSTRANGE] == 1 && block == 118)
	//block -= 1;

	FLSCL = 0x01;
	PSCTL = 0x03;
	pwrite = (unsigned int)block*1024;
	*pwrite = 0;
	//PSCTL = 0x01;
	PSCTL = 0x00;
	FLSCL = 0x00;

	EA = 1;
}


/*
bit FlashRead_Absolute(unsigned int location, unsigned char *value)
{
	unsigned char code *location_ptr;


	location_ptr = (char *)location;
	*value = (*location_ptr);

	return 1;

}*/

bit FlashRead_Absolute(unsigned int location, unsigned char *value)
{
	unsigned char code *location_ptr;
	SFRPAGE   = LEGACY_PAGE;
	EA = 0 ;
	PSCTL 	= 0X04;
	location_ptr = (char *)location;
	*value = (*location_ptr);
	PSCTL 	= 0X00;
	EA = 1 ;
	return 1;

} 


void flash_find_next_entry(unsigned char code **scan_ptr)
{
  // increase pointer by n + 2
  *scan_ptr += **scan_ptr + 2;
}
// function: flash_table_full
// description: determines if there is enough space in the table for
// another variable of a specific size
// passed: the size of the variable in bytes
// returned: FLASH_TABLE_FULL if no room to fit variable in table
//           0 if there is room
// ron, 05/12/12, added equal sign
bit flash_table_full(unsigned char block_select)
{
	unsigned char size = ELEMENT_SIZE_CONST[block_select];

//	if (flash_ptr[block_select] > ((unsigned char code *)next_block_top[block_select] - (size + 2)) )
	if (flash_ptr[block_select] >= ( (unsigned char code *)(next_block_top[block_select] - (size + 2)) ) )
		return FLASH_TABLE_FULL;
	return 0;

	
}

// function: flash_init
// description: call flash_block_set to initialise all blocks
void flash_init(void)
{
	unsigned char i;

	for(i = 0; i<BLOCK_COUNT; i++)
		flash_set_block(i);
}

// function: flash_block_set
// description: initializes the Flash Variables library
// passed: the crystal frequency rounded down to the nearest integer
// and a value indicating the block to store the flash variables in
// returned: 0 for failure to initialize
//           1 for correctly initialized
//	this must be set for each flash section
bit flash_set_block(unsigned char block_select)
{
	unsigned char block1, block2;
	unsigned int block_size;
	unsigned char code *block_base1, *block_base2; 
	unsigned int block_size1, block_size2;


	block_size = BLOCK_SIZE_CONST[block_select];

	block1 = START_BLOCK_CONST[block_select][0];
	block2 = START_BLOCK_CONST[block_select][1];
	
	
	// calculate base addresses of both block
	block_base1 = (unsigned char code *)((unsigned int)block1 << 10);
	block_base2 = (unsigned char code *)((unsigned int)block2 << 10);

	// determine which block is in use
	
	// if both blocks in use then find which has smallest table
	// this situation will occur if there was a reset or power failure
	// between the new block being marked valid and the old block
	// being marked invalid in flash_garbage_collector
	if (*block_base1 == FLASH_BLOCK_IN_USE && *block_base2 == FLASH_BLOCK_IN_USE)
	{
		// find size of table in block 1
		flash_ptr[block_select] = block_base1 + 1;
		while(*flash_ptr[block_select] != 0xff && flash_ptr[block_select] < block_base2)
		  flash_find_next_entry(&flash_ptr[block_select]);
		
		// record size
		block_size1 = (unsigned int)(flash_ptr[block_select] - block_base1);
		
		// find size of table in block 2
		flash_ptr[block_select] = block_base2 + 1;
		while(*flash_ptr[block_select] != 0xff && flash_ptr[block_select] < block_base2+block_size)
		  flash_find_next_entry(&flash_ptr[block_select]);
		
		block_size2 = flash_ptr[block_select] - block_base2;

		// if block 1 holds the larger table then use block 2 and mark block 1
		// as invalid
		if (block_size2 <= block_size1)
		{      
			// store block 2 base skipping block status byte
			block_base[block_select] = (unsigned int)block_base2 + 1;
			// store block 1 as being unused
			flash_unused_block[block_select] = block1;
			// mark block 1 as invalid
			if (iap_program_data_byte(FLASH_BLOCK_INVALID, (unsigned int)block_base1))
				return 0;
  		}

		// if block 2 holds the larger table then use block 1 and mark block 2
		// as invalid
		else
		{
			// store block 1 base skipping block status byte
			block_base[block_select] = (unsigned int)block_base1 + 1;
			// store block 2 as being unused
			flash_unused_block[block_select] = block2;
			// mark block 2 as invalid
			if (iap_program_data_byte(FLASH_BLOCK_INVALID, (unsigned int)block_base2))
				return 0;
		}  
	}
	// if block 1 is only block in use
	else if(*block_base1 == FLASH_BLOCK_IN_USE)
	{
		// store block 1 base skipping block status byte
		block_base[block_select] = block_base1 + 1;
		// store block 2 as being unused
		flash_unused_block[block_select] = block2;
	}
	// if block 2 is only block in use
	else if(*block_base2 == FLASH_BLOCK_IN_USE)
	{
		// store block 2 base skipping block status byte
		block_base[block_select] = (unsigned int)block_base2 + 1;
		// store block 2 as being unused
		flash_unused_block[block_select] = block1;
	}
	// if neither block is in use
	// this will occur if the code is being executed for the first
	// time in a blank device
	// block 1 will be the first block to be used
	else
	{
		// store block 1 base skipping block status byte
		block_base[block_select] = (unsigned int)block_base1 + 1;
		// store block 2 as being unused
		flash_unused_block[block_select] = block2;
		// mark block 1 as in use
		if (iap_program_data_byte(FLASH_BLOCK_IN_USE, (unsigned int)block_base1))
			return 0;
	}
	
	
	// initialize flash_ptr
	flash_ptr[block_select] =(unsigned char code *) block_base[block_select];

//*************************************************************************
	// calculate top address of next block
	next_block_top[block_select] = block_base[block_select] + block_size;     
										// FOR GOAL VRS1000, page size is 512 bytes (200h)
										// we will use double page blocks.
//*************************************************************************

  
	// set flash_ptr to top of table  
	while(*flash_ptr[block_select] != 0xff)
//	{
//		tmp_char = (unsigned int)flash_ptr + *flash_ptr +2;
//		flash_ptr = (unsigned char code *)tmp_char;
//	}
		flash_find_next_entry(&flash_ptr[block_select]);
	
	// 00initialization was successful
	return 1;
}

// function: flash_garbage_collector
// description: removes unused entries from the table
// and compresses the used entries to the start of the table
// therefore allowing maximum storage for new entries
// actually moves table to the unused flash block and
// marks the old flash block as invalid
// passed: nothing
// returned: FLASH_PROG_ERROR for programming error
//           0 for ok

//unsigned char flash_garbage_collector(unsigned char i,  unsigned int block_size)
// Ron
// found some mistakes in here, did not write properly to next page bank
// error was in scan_ptr assignment
unsigned char flash_garbage_collector(unsigned char block_select)
{
	unsigned char code *scan_ptr ;
	unsigned char code *code_ptr;
	unsigned char code *temp_ptr;
	unsigned char xdata b, n, unused_block, temp_trasfer;
	unsigned int xdata block_size;

	scan_ptr = (unsigned char code *)block_base[block_select];
	code_ptr = (unsigned char code*)( (unsigned int)flash_unused_block[block_select] << 10);

	block_size = BLOCK_SIZE_CONST[block_select];

	// erase unused block
	for(b=0; b<(BLOCK_SIZE_CONST[block_select]>>10); b++)
	{
		watchdog();
		iap_erase_block(flash_unused_block[block_select]+b);
	}
	temp_ptr = scan_ptr + block_size - 1;
	// skip block status byte
	// the byte must be programmed after the table has been copied
	code_ptr++;
//	scan_ptr++;

	// scan through entries in table until blank memory reached
// debug, for some reason the first one always gets deleted
	while((*scan_ptr != 0xff) && (scan_ptr < temp_ptr))
	{
		// if this entry is in use then copy to other block...
		// if the value at scan_ptr+1 is 0x00 then the entry is an
		// old one. if the value is 0xff then the entry was not
		// completely programmed due to a reset, crash or power failure
		watchdog();
	
		if (*(scan_ptr + 1) != 0x00 && *(scan_ptr + 1) != 0xff)
		{
			// copy n, id and value to other flash block

			// copy n
			n = *scan_ptr;
			if (iap_program_data_byte(n, (unsigned int)code_ptr)) //copy it to the other block
				return FLASH_PROG_ERROR;
			
			code_ptr++;	// make sure to increment code pointer
			scan_ptr++;

			// copy id
			temp_trasfer = *scan_ptr;
			if (iap_program_data_byte(temp_trasfer, (unsigned int)code_ptr))
				return FLASH_PROG_ERROR;

			code_ptr++;// make sure to increment code pointer
			scan_ptr++;
			
			// copy value
			for (b = 0; b < n; b++)
			{
				temp_trasfer = *scan_ptr;
				if (iap_program_data_byte(temp_trasfer, (unsigned int)code_ptr))
					return FLASH_PROG_ERROR;
				
				code_ptr++;// make sure to increment code pointer
				scan_ptr++;
			}
		}
		// if its an unused entry then skip to next entry
		else
			flash_find_next_entry(&scan_ptr);
	
	}

	// record the top of the compressed table
	flash_ptr[block_select] = code_ptr;
	
	// swap values,  unused_block <--> block_base
	unused_block = (unsigned char)(block_base[block_select] >> 10);
	block_base[block_select] = (unsigned int)(flash_unused_block[block_select] << 10);
	flash_unused_block[block_select] = unused_block;


//*************************************************************************
	// calculate top address of next block
	next_block_top[block_select] = block_base[block_select] + block_size;     
										// FOR GOAL VRS1000, page size is 512 bytes (200h)
										// we will use double page blocks.
//*************************************************************************

	// mark new block as valid
	if (iap_program_data_byte(FLASH_BLOCK_IN_USE, (unsigned int)block_base[block_select]))
	return FLASH_PROG_ERROR;
	
	// mark old block as invalid
	if (iap_program_data_byte(FLASH_BLOCK_INVALID, (unsigned int)(flash_unused_block[block_select] << 10)))
	return FLASH_PROG_ERROR;
	
	// actual base starts after block status byte
	block_base[block_select]++;

	return 0;
}
 
 


// function: flash_write_no_recovery
// description: stores a single byte variable in the flash
// passed: the identifier of the variable and the new value
// returned: 0 for programming error/table full
//           1 for ok
//bit flash_write_no_recovery(unsigned char id, unsigned char *value,unsigned char n,unsigned char i)
bit flash_write_no_recovery(unsigned char id, unsigned char *value,unsigned char block_select)
{
	unsigned char code *scan_ptr;
	unsigned char result,m;
	unsigned char code *old_entry_id = 0 ;// = (unsigned char code *)0;
	unsigned char size = ELEMENT_SIZE_CONST[block_select];


//  scan_ptr = (unsigned int code *)block_base;
	scan_ptr = (unsigned char code *)block_base[block_select];
//  old_entry_id = (unsigned int code *)0;

	// Increment id by one to avoid trying to write register address 0, which is reserved.	 
	id ++;  

	// check for table full
	if (flash_ptr[block_select] >  next_block_top[block_select] - (size + 1))
		return 0;

 
	// check to find old value if stored
	// (continue scanning until you find an ID match or until you reach the first unused entry
	while(*(scan_ptr + 1) != id && *scan_ptr != 0xff)
		flash_find_next_entry(&scan_ptr);
	
	if (*(scan_ptr + 1) == id)  // found old entry so record location of id
		old_entry_id = scan_ptr + 1;
  	
	
	
	// the entry is stored in the order shown incase there is a reset
	// or power failure between programming bytes. In the worst case
	// only the number of data bytes will be programmed which
	// will allow the partly programmed entry to be skipped


	// store size
	watchdog();
	result = iap_program_data_byte(size, (unsigned int)flash_ptr[block_select]); // size byte value
	if (result) 
      return 0;

	flash_ptr[block_select] += 2;  // increase flash_ptr by size + 1

 	// store data
	for(m=0; m<size; m++)
	{
		result = iap_program_data_byte(*(value+m), (unsigned int)(flash_ptr[block_select] + m)); //  
		if (result) 
          return 0;
	}

	// adjust pointer to point to next empty register
 	flash_ptr[block_select]+=size;

	
	// store id
	result = iap_program_data_byte(id, (unsigned int)(flash_ptr[block_select] - (size + 1))); 
	if (result) 
      return 0;
	
	// if there is an old entry then mark it is as unused now new entry has been written
	if (old_entry_id)
		if (iap_program_data_byte(0x00, (unsigned int)old_entry_id))
			return 0;
	
	
	
	return 1;
}


// removed this and modified it to return a value rather than an address
/*
// function: flash_read
// description: reads a single byte variable from the flash
// passed: the identifier of the variable
// returned: pointer if successfully read
//           null pointer if not found
unsigned char code *flash_read(unsigned char id, unsigned char block_select)
{
//  unsigned char code *scan_ptr = (unsigned int code *)block_base;
//  unsigned char code *ptr = (unsigned int code *)0;
  unsigned char code *scan_ptr = block_base[block_select];
  unsigned char code *ptr = 0;
//  unsigned char m;

   
	
  // Increment id by one to avoid trying to read register address 0, which is reserved.	
  id ++;

  // scan through entire table marking any older entries with matching
  // identifier as unused
  // this situation will occur if there was a reset or power failure
  // between the new entry being stored and the old entry being marked
  // as unused. This could happen many times in succession so there
  // could be many entries in the table with the same id, however the
  // last one stored is the valid one
  while (*scan_ptr != 0xff && scan_ptr < next_block_top[block_select])
  {
    // if entry found...
    if (*(scan_ptr + 1) == id)
    {
      // if already found an entry...
      if (ptr)
        // mark previously found entry as unused
        if (iap_program_data_byte(0x00, (ptr + 1)))
          return 0;
      // record position of newly found entry
      ptr = scan_ptr;
    }
    // move on to next entry in table
//lei    scan_ptr = flash_find_next_entry(scan_ptr);
 		flash_find_next_entry(&scan_ptr);

  }

  // if an entry found then return pointer to value
  if (ptr) 
  {
    	return (ptr + 2);         	  
  }

  // identifer not found
  return NO_DEFINE_ADDRESS;
}
*/





// function: flash_read_int
// description: reads int(2byte) variable from the flash
// passed: the identifier of the variable
// returned: 1 of found
//           null if not found
bit flash_read_int(unsigned char id, unsigned int *value, unsigned char block_select)
{
//  unsigned char code *scan_ptr = (unsigned int code *)block_base;
//  unsigned char code *ptr = (unsigned int code *)0;
	unsigned char code *scan_ptr = block_base[block_select];
	unsigned char code *ptr = 0;
//  unsigned char m;

    watchdog();
	
	// Increment id by one to avoid trying to read register address 0, which is reserved.	
	id ++;

	// scan through entire table marking any older entries with matching
	// identifier as unused
	// this situation will occur if there was a reset or power failure
	// between the new entry being stored and the old entry being marked
	// as unused. This could happen many times in succession so there
	// could be many entries in the table with the same id, however the
	// last one stored is the valid one
	while (*scan_ptr != 0xff && scan_ptr < next_block_top[block_select])
	{
		// if entry found...
		if (*(scan_ptr + 1) == id)
		{
			// if already found an entry...
			if (ptr)
				// mark previously found entry as unused
				if (iap_program_data_byte(0x00, (ptr + 1)))
				    	return 0;
			// record position of newly found entry
			ptr = scan_ptr;
		}
	// move on to next entry in table
	flash_find_next_entry(&scan_ptr);
	
	}

	// if an entry found then return pointer to value
	if (ptr) 
	{
		*value = *(ptr + 2);
		*value <<= 8;
		*value += *(ptr + 3);
		return 1;	// return a true to indicate value was found
	}

	// identifer not found
	return NULL;
}






// function: flash_read_int
// description: reads int(2byte) variable from the flash
// passed: the identifier of the variable
// returned: 1 of found
//           null if not found
bit flash_read_char(unsigned char id, unsigned char *value, unsigned char block_select)
{
	unsigned char code *scan_ptr = block_base[block_select];
	unsigned char code *ptr = 0;

	  watchdog();
	// Increment id by one to avoid trying to read register address 0, which is reserved.	
	id ++;

	// scan through entire table marking any older entries with matching
	// identifier as unused
	// this situation will occur if there was a reset or power failure
	// between the new entry being stored and the old entry being marked
	// as unused. This could happen many times in succession so there
	// could be many entries in the table with the same id, however the
	// last one stored is the valid one
	while (*scan_ptr != 0xff && scan_ptr < next_block_top[block_select])
	{
		// if entry found...
		if (*(scan_ptr + 1) == id)
		{
			// if already found an entry...
			if (ptr)
				// mark previously found entry as unused
				if (iap_program_data_byte(0x00, (ptr + 1)))
				    	return 0;
			// record position of newly found entry
			ptr = scan_ptr;
		}
	// move on to next entry in table
	flash_find_next_entry(&scan_ptr);
	
	}

	// if an entry found then return pointer to value
	if (ptr) 
	{
//		*value = *(ptr + 2);
//		*value <<= 8;
//		*value += *(ptr + 2);
		*value = *(ptr + 2);	// return a char
		return 1;	// return a true to indicate value was found
	}

	// identifer not found
	return NULL;
}

// removed this and modified it to take in int rather than an array
/*
// Writes a register to the flash memory
// Returns 1 for success, 0 for failure.
bit flash_write(unsigned char id, unsigned char *value, unsigned char block_select)
{
	unsigned char j = 0;

	watchdog();

	// Make sure flash table is not full
	if (flash_table_full(block_select))
	{
		// If the table was full run the garbage collector routine.
//		while (j < 10 && flash_garbage_collector(block_select, block_size))
		while (j < 10 && flash_garbage_collector(block_select))
			j++;

		delay_us(5);


		if (j >= 10)
			return 0;

		j = 0;
	}

	// Write the register to the table.
//	while (j < 10 && !flash_write_no_recovery(id, value, description_size,block_select))
	while (j < 10 && !flash_write_no_recovery(id, value, block_select))
		j++;

	if (j >= 10)
			return 0;	

	return 1;
}
*/




// function: flash_write_int
// description: write int(2byte) variable into flash
//				normally Robert wrote the routine to enter an array
//				thus we put desired value into array and perform operation as normal
// passed: the identifier of the variable, value and block select
// returned: 1 write complete
//           null if not complete
bit flash_write_int(unsigned char id, unsigned int value, unsigned char block_select)
{
	unsigned char j = 0;
	unsigned char transfer_value[3];	// this will always be size 2 given we always enter integer values
  watchdog();
	// put value into array or char, just the way Robert wrote the code
	transfer_value[1] = value & 0x00FF;		// enter lower bits
	transfer_value[0] = (value>>8) & 0x00FF;	// enter upper bits

//	watchdog();

	// Make sure flash table is not full
	if (flash_table_full(block_select))
	{
		// If the table was full run the garbage collector routine.
		while (j < 10 && flash_garbage_collector(block_select))
			j++;		// if garbage collector failed once, try it out again

		delay_us(5);

		if (j >= 10)	// if garbage collector too many times, we are in trouble
			return 0;


	}

	// Write the register to the table.
	while (j < 10 && !flash_write_no_recovery(id, &transfer_value[0], block_select))
		j++;

	if (j >= 10)
			return 0;	

	return 1;
}


