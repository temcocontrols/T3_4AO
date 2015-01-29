
#include "T3IOmodbus.h"
extern void SetBit(unsigned char bit_number,unsigned int *byte);

#define DEFAULT_TIMEOUT  0

#define MAX_FILTER 				32 //GW: used for filter counter

// --- input channels -------------------

 
// --- output channels ------------------

	#define MAX_OUTPUT_CHANNELS 8

// --- output switch size --------------
#if defined (T3_8IN16OUT)||(T3_8IN13OUT)
	#define OUTPUT_SIZE			2
#else
	#define OUTPUT_SIZE			1
#endif

// --- LED banks ----------------------
#define MAX_LED_BANK	3


#define FLASH_RAM_START 64
	
#define SEND				1
#define RECEIVE				0

#define READ_VARIABLES      3
#define WRITE_VARIABLES     6
#define MULTIPLE_WRITE		16
#define CHECKONLINE			25


// note changing the databuflen larger will make it compile properly but the device will not work properly
// problem observed when length changed to 100
#define DATABUFLEN			25
//#define SENDPOOLLEN         16

#define RECEIVE		0
#define SEND		1

#define FALSE 0
#define TRUE  1

#define LO	0
#define HI	1


#define ORIGINALADDRESSVALUE	100 


// starting point to where to read in the flash for data transfer
// RL 3/11/04
#define DATA_TO_ISP				60928	// 0xEE00 = 60928
//#define DATA_TO_FIRMWARE		61184	// 0xEF00 = 61184
 #define DATA_TO_FIRMWARE		104448


#define EVENT_QUEUE_SIZE 	TOTAL_TIMERS 



// --- defines for reading rectifications -----------------------------------------
/*
#ifdef T3_32IN
	#define INPUT_NOISE_THRESHOLD	1
	#define INPUT_FILTER 			1
#else
//	#define INPUT_NOISE_THRESHOLD	200		// threshold 200/1024 = 1V
//	#define INPUT_FILTER 			10		// set to make an update at every second

	#define INPUT_NOISE_THRESHOLD	100
	#define INPUT_FILTER 			10

#endif
*/



#define INPUT_NOISE_THRESHOLD	100		//200 threshold 200/1024 = 1V //GW
#define INPUT_FILTER 			10		// set to make an update at every second



#define NOISE_TURN_POINT			2

#define BUFFER_DIFFERENCE_THRESHOLD	10
#define BUFFER_DIVIDER				3


#define CALIBRATION_OFFSET 128 //allows us to store FLASH_CALIBRATION as an unsigned char
#define CALIBRATION_PRECISION_POINTS	10	

#define TESTING_INCREMENT_VALUE 100		// testing step size value
//#define TESTING_INCREMENT_BUFFER 500	// testing signal differences
#define TESTING_ERROR_MARGIN 50		// testing to acceptable error range



typedef enum
{
  HEARTBEAT_PULSE = 0,	// 0
  REFRESH_OUTPUTS,
  REFRESH_INPUTS,

  CHECK_SWITCHES,
 
  INPUT1_TIMER,
  INPUT2_TIMER,
  INPUT3_TIMER,
  INPUT4_TIMER,
  INPUT5_TIMER,
  INPUT6_TIMER,
  INPUT7_TIMER,
  INPUT8_TIMER,
  INPUT9_TIMER,
  INPUT10_TIMER,
  
  STORE_PULSE,
 
  TOTAL_TIMERS, 

  SERIAL_COMMANDS,	
  TOTAL_EVENTS,

  START_COMMAND = 128,
  STOP_COMAND 	
} et_event;







 
typedef /*idata*/ enum 
{

  EEP_OUTPUT1  = T38IO_OUTPUT1  - ORIGINALADDRESSVALUE,  				// 00 Output 0  //100
  EEP_OUTPUT2  = T38IO_OUTPUT2  - ORIGINALADDRESSVALUE,				// 01 Output 1 
  EEP_OUTPUT3  = T38IO_OUTPUT3  - ORIGINALADDRESSVALUE,  				// 02 Output 2 
  EEP_OUTPUT4  = T38IO_OUTPUT4  - ORIGINALADDRESSVALUE,				// 03 Output 3 
  EEP_OUTPUT5  = T38IO_OUTPUT5  - ORIGINALADDRESSVALUE,				// 04 Output 4 
  EEP_OUTPUT6  = T38IO_OUTPUT6  - ORIGINALADDRESSVALUE,      			// 05 Output 5 
  EEP_OUTPUT7  = T38IO_OUTPUT7  - ORIGINALADDRESSVALUE,  				// 06 Output 6 
  EEP_OUTPUT8  = T38IO_OUTPUT8  - ORIGINALADDRESSVALUE,      			// 07 Output 7 
  EEP_OUTPUT9  = T38IO_OUTPUT9  - ORIGINALADDRESSVALUE,      			// 05 Output 5 
  EEP_OUTPUT10 = T38IO_OUTPUT10 - ORIGINALADDRESSVALUE,  				// 06 Output 6 
  EEP_OUTPUT11 = T38IO_OUTPUT11 - ORIGINALADDRESSVALUE,      			// 07 Output 7 
  EEP_OUTPUT12 = T38IO_OUTPUT12 - ORIGINALADDRESSVALUE,     

  EEP_INPUT1   = T38IO_INPUT1   - ORIGINALADDRESSVALUE,  	   			// 08 Input 0 //112
  EEP_INPUT2   = T38IO_INPUT2   - ORIGINALADDRESSVALUE, 				// 09 Input 1 
  EEP_INPUT3   = T38IO_INPUT3   - ORIGINALADDRESSVALUE,  				// 10 Input 2 
  EEP_INPUT4   = T38IO_INPUT4   - ORIGINALADDRESSVALUE,					// 11 Input 3 

  EEP_SWITCH_STATE1 = T38IO_SWITCH_STATE1 - ORIGINALADDRESSVALUE,	// 16 First bank of switches  //122
  EEP_SWITCH_STATE2 = T38IO_SWITCH_STATE2 - ORIGINALADDRESSVALUE,	// 17 Second bank of switches
  EEP_SWITCH_STATE3 = T38IO_SWITCH_STATE3 - ORIGINALADDRESSVALUE,	// 18 Second bank of switches  //124


  EEP_INPUT1_RANGE,					//118	Set range for each input. 0 = raw data,1 = 10K Celsius,2 = 10K Fahrenheit		//125	  
  EEP_INPUT2_RANGE,					//		3 = 0 - 100%,4 = ON/OFF,5 = OFF/ON
  EEP_INPUT3_RANGE,
  EEP_INPUT4_RANGE,	
  EEP_INPUT5_RANGE,
  EEP_INPUT6_RANGE,
  EEP_INPUT7_RANGE,
  EEP_INPUT8_RANGE,
  EEP_INPUT9_RANGE,
  EEP_INPUT10_RANGE,         //134

  EEP_INPUT1_FILTER,        //135
  EEP_INPUT2_FILTER,
  EEP_INPUT3_FILTER,
  EEP_INPUT4_FILTER,
  EEP_INPUT5_FILTER,
  EEP_INPUT6_FILTER,
  EEP_INPUT7_FILTER,
  EEP_INPUT8_FILTER,
  EEP_INPUT9_FILTER,
  EEP_INPUT10_FILTER,      //144

  TOTAL_EE_PARAMETERS         //  18


} et_menu_parameter;
 

// **************************************************************************************





//#define MINUTE_COUNTER	90 
#define MINUTE_COUNTER	15 








// 199 is the highest address we can use when storing variables in the flash memory
#define EEP_SERINALNUMBER_WRITE_FLAG	MAX_FLASH_CONSTRANGE 





// ---------------- pic const -----------------------------
enum {

PIC_SERIAL_TIMEOUT

};





struct link
{
	unsigned int  registers[ TOTAL_EE_PARAMETERS ] ;//parameter[12]
//  et_jump_function event_function[ TOTAL_EVENTS ] ;//TOTAL_EVENTS=8
//  et_jump_function jump_function[ TOTAL_TIMERS ] ;//TOTAL_TIMERS=6
//  et_event event_queue[ EVENT_QUEUE_SIZE ]  ;//EVENT_QUEUE_SIZE=3
//  unsigned char big_ticks[ TOTAL_TIMERS ] ;//9       //number of heartbeats x small_ticks, high gear notched evey ~ ms
};  





/* --------------- Timer SOP feature ------------------------- */
// set the device ID on the network, where to start and where to end
#ifdef TIMER_SOP_FEATURE
	#define START_ID 	10
	#define END_ID 		30
#endif





/* --------------- Flex Driver feature ------------------------- */
// set the device ID on the network, where to start and where to end
#define DISPLAY_LENGTH	8
#define PAIR_LENGTH	32
#define TABLE_REFRESH_RATE	20	// 20*0.50sec = 10sec.  So every 15 seconds the table is refreshed, chosen arbitrarily

#define STATE_CHANGE_LOWER	250
#define STATE_CHANGE_UPPER	725



// --------------- added defines for Benny's features --------------/
#define PULSE_LENGTH	0
#define HOLDING_LENGTH	50









/* --------------- PWM transducer feature ------------------------- */
#define PORT0_READING_FILTER	10
#define MAX_CALIBRATION_POINTS	20
#define DEFINE_TABS				11








// ************* Flash Memory info **********************************************
//	Flash Memory related variables
// --------------------------------------------------------------------

// must comply with memory setup in goal_flash.h file
typedef enum
{
	FLASH_MEMORY,
	CALIBRATION_MEMORY,
 	BLOCK_COUNT
};






// *** high speed ****************************************************************
#define HIGH_BUFFER		20		// arbitrarily chosen at 20

#define TYPE2_10K_THERM	  0
#define TYPE3_10K_THERM	  1 
#define TYPE4_10K_THERM	  2 
/*range define*/
/*range define*/
#define RAW_DATA	0
#define C_TYPE2		1
#define F_TYPE2		2
#define PERCENT	  	3
#define ON_OFF		4
#define OFF_ON		5
#define PULSE		6
#define LIGHTING	7
#define C_TYPE3		8
#define F_TYPE3		9
#define NO_USE		10
#define V0_3		11
#define V0_10		12
#define I0_20ma		13
#define	I0_100ma	14
#define LM235_C	 	15
#define LM235_F		16
#define C_TYPE4		17
#define F_TYPE4		18







