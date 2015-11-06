// *******************T3IOmodbus.h***********************************
// Header file containing all of the register information for modbus 
// serial communications.



// -------------- TEMCO flash registers --------------------------
//	the following are reserved TEMCO registers common to all products
//	range from 0 to 100
//		note for now Robert has added pulsing in this section
//		plan to be removed in the future
// ---------------------------------------------------------------
enum {

	FLASH_SERIALNUMBER_LOWORD       = 0,             
	FLASH_SERIALNUMBER_HIWORD		= 2,
	EEPROM_VERSION_NUMBER			= 4,
	FLASH_SOFTWARE_VERSION_NUMBER	= 5,
	FLASH_ADDRESS					= 6,
	FLASH_PRODUCT_MODEL,
	FLASH_HARDWARE_REV,
	FLASH_PIC_VERSION =  9,
	FLASH_ADDRESS_PLUG_N_PLAY		= 10,
    ISP_VERSION                     = 11,
	//LAST_DISPLAY_ADDRESS,

//	FLASH_SEQUENCE,			// JULY 16th by Ron	
			// SOP = 1  -->	normal operation
			// SOP = 0  -->	test sequence mode
			// SOP = 2  -->	Timer SOP mode
			// SOP = 3  -->	Switch States set to automatic mode
			// SOP = 5  -->	Calibration, produce LUT

 // FLASH_CALIBRATION,
  //FLASH_PWM_TIME_CALIBRATION,
 // FLASH_BAUDRATE		= 13,


	FLASH_DEAD_MASTER       = 12,				// 17  UNITS IS MINUTE, 0 ~ 255,WILL RESET TO AUTO IN THIS TIME NO COMMUNICATION
    FLASH_BAUDRATE		= 15,
	// registers needed for updating status
	FLASH_UPDATE_STATUS	= 16,		// reg 16 August Ron
    //FLASH_init,
	//FLASH_BASE_ADDRESS = 20,



	// special registers for high speed input
	//	TBD move this to lower section of the code
	FLASH_START_PULSE ,

	FLASH_PULSE1_HI_HI = FLASH_START_PULSE ,
	FLASH_PULSE1_LO_LO,

	FLASH_PULSE2_HI_HI,
	FLASH_PULSE2_LO_LO,

	FLASH_PULSE3_HI_HI,
	FLASH_PULSE3_LO_LO,

	FLASH_PULSE4_HI_HI,
	FLASH_PULSE4_LO_LO,

	FLASH_PULSE5_HI_HI,
	FLASH_PULSE5_LO_LO,

	FLASH_PULSE6_HI_HI,
	FLASH_PULSE6_LO_LO,

	FLASH_PULSE7_HI_HI,
	FLASH_PULSE7_LO_LO,

	FLASH_PULSE8_HI_HI,
	FLASH_PULSE8_LO_LO,

 	FLASH_PULSE9_HI_HI,
	FLASH_PULSE9_LO_LO,

  FLASH_PULSE10_HI_HI,
	FLASH_PULSE10_LO_LO,

	FLASH_PULSE1_YEAR,
	FLASH_PULSE1_MONTH,
	FLASH_PULSE1_DATE,
	FLASH_PULSE1_HOUR,
	FLASH_PULSE1_MINUTE,

	FLASH_PULSE2_YEAR,
	FLASH_PULSE2_MONTH,
	FLASH_PULSE2_DATE,
	FLASH_PULSE2_HOUR,
	FLASH_PULSE2_MINUTE,

	FLASH_PULSE3_YEAR,
	FLASH_PULSE3_MONTH,
	FLASH_PULSE3_DATE,
	FLASH_PULSE3_HOUR,
	FLASH_PULSE3_MINUTE,
	
	FLASF_PULSE4_YEAR,
	FLASF_PULSE4_MONTH,
	FLASH_PULSE4_DATE,
	FLASH_PULSE4_HOUR,
	FLASH_PULSE4_MINUTE,

	FLASH_PULSE5_YEAR,
	FLASH_PULSE5_MONTH,
	FLASH_PULSE5_DATE,
	FLASH_PULSE5_HOUR,
	FLASH_PULSE5_MINUTE,

	FLASH_PULSE6_YEAR,
	FLASH_PULSE6_MONTH,
	FLASH_PULSE6_DATE,
	FLASH_PULSE6_HOUR,
	FLASH_PULSE6_MINUTE,
	
	FLASH_PULSE7_YEAR,
	FLASH_PULSE7_MONTH,
	FLASH_PULSE7_DATE,
	FLASH_PULSE7_HOUR,
	FLASH_PULSE7_MINUTE,
	
	FLASH_PULSE8_YEAR,
	FLASH_PULSE8_MONTH,
	FLASH_PULSE8_DATE,
	FLASH_PULSE8_HOUR,
	FLASH_PULSE8_MINUTE,

  	FLASH_PULSE9_YEAR,
	FLASH_PULSE9_MONTH,
	FLASH_PULSE9_DATE,
	FLASH_PULSE9_HOUR,
	FLASH_PULSE9_MINUTE,

 	 FLASH_PULSE10_YEAR,
	FLASH_PULSE10_MONTH,
	FLASH_PULSE10_DATE,
	FLASH_PULSE10_HOUR,
	FLASH_PULSE10_MINUTE,

	FLASH_CHANNEL_TYPE, 

	FLASH_INPUT1_RANGE,
	FLASH_INPUT2_RANGE,
	FLASH_INPUT3_RANGE,
	FLASH_INPUT4_RANGE,
	FLASH_INPUT5_RANGE,
	FLASH_INPUT6_RANGE,
	FLASH_INPUT7_RANGE,
	FLASH_INPUT8_RANGE,
 	 FLASH_INPUT9_RANGE,
	FLASH_INPUT10_RANGE,


	FLASH_INPUT1_FILTER,
	FLASH_INPUT2_FILTER,
	FLASH_INPUT3_FILTER,
	FLASH_INPUT4_FILTER,
	FLASH_INPUT5_FILTER,
	FLASH_INPUT6_FILTER,
	FLASH_INPUT7_FILTER,
	FLASH_INPUT8_FILTER,
	FLASH_INPUT9_FILTER,
	FLASH_INPUT10_FILTER,

	FLASH_INPUT1_TIMER,
	FLASH_INPUT2_TIMER,
	FLASH_INPUT3_TIMER,
	FLASH_INPUT4_TIMER,
	FLASH_INPUT5_TIMER,
	FLASH_INPUT6_TIMER,
	FLASH_INPUT7_TIMER,
	FLASH_INPUT8_TIMER,
 	FLASH_INPUT9_TIMER,
	FLASH_INPUT10_TIMER,

	FLASH_OUTPUT_MANUAL,

	FLASH_ZONE_OUTPUT1,
	FLASH_ZONE_OUTPUT2,
	FLASH_ZONE_OUTPUT3,       
	FLASH_ZONE_OUTPUT4,
	FLASH_ZONE_OUTPUT5,
	FLASH_ZONE_OUTPUT6,
	FLASH_ZONE_OUTPUT7,
	FLASH_ZONE_OUTPUT8,


	FLASH_REVERSE_OUTPUT,
	FLASH_SERINALNUMBER_WRITE_FLAG ,
	MAX_FLASH_CONSTRANGE,

	CALIBRATION_STORAGE_LOCATION = 300,	// flash data, in modbus will start at 300
	CALIBRATION_GRID = 500				// calibration LUT, in modbus will start at 500
};




// -------------- Registers above 100 are application register --------------------------
//	the following are custom to the code's application registers
//	starting at 100
//
//
// --------------------------------------------------------------------------------------
#define ORIGINALADDRESSVALUE	100 			// Offset for 2nd block of registers

// --------------- 32 inputs module -----------------------------------------------------
// --------------------------------------------------------------------------------------
 
 
enum {
	T38IO_OUTPUT1 = ORIGINALADDRESSVALUE,  		// Output 1, register 100
	T38IO_OUTPUT2  ,                   			// Output 2 
	T38IO_OUTPUT3  ,                   			// Output 3 
	T38IO_OUTPUT4  ,                   			// Output 4 
	T38IO_OUTPUT5  ,                   			// Output 5 
	T38IO_OUTPUT6  ,                   			// Output 6 
	T38IO_OUTPUT7  ,                   			// Output 7 
	T38IO_OUTPUT8  ,                   			// Output 8
    T38IO_OUTPUT9  ,                   			// Output 9 
	T38IO_OUTPUT10  ,                   		// Output 10 
	T38IO_OUTPUT11 ,                   			// Output 11 
	T38IO_OUTPUT12  ,                   		// Output 12  //register 111
 
	T38IO_INPUT1  ,                   			// Input 1, register 112
	T38IO_INPUT2  ,                   			// Input 2 , register 113
	T38IO_INPUT3  ,                   			// Input 3 , register 114
	T38IO_INPUT4  ,                   			// Input 4  .register 115



  T38IO_SWITCH_STATE1,						// First bank of switches, //register 116
  T38IO_SWITCH_STATE2,						// Second bank of switches register 117
  T38IO_SWITCH_STATE3,						// Second bank of switches //register 118


	// Use a long type data to store pulse number,and the number be divided two integer data 
	T38IO_PULSE1_HI_WORD,						// register 119
	T38IO_PULSE1_LO_WORD,						// register 120
                                                
	T38IO_PULSE2_HI_WORD,                          // register 121
	T38IO_PULSE2_LO_WORD,                           // register 122

	T38IO_PULSE3_HI_WORD,                            // register 123
	T38IO_PULSE3_LO_WORD,                            // register 124

	T38IO_PULSE4_HI_WORD,                             // register 125
	T38IO_PULSE4_LO_WORD,                              // register 126

	T38IO_PULSE5_HI_WORD,                              // register 127
	T38IO_PULSE5_LO_WORD,                               // register 128
                                                        
	T38IO_PULSE6_HI_WORD,                                 // register 129
	T38IO_PULSE6_LO_WORD,                                 // register 130

	T38IO_PULSE7_HI_WORD,                                 // register 131
	T38IO_PULSE7_LO_WORD,                                 // register 132

	T38IO_PULSE8_HI_WORD,                                 // register 133
	T38IO_PULSE8_LO_WORD,				                  // register 134

 	T38IO_PULSE9_HI_WORD,                                 //register 135
	T38IO_PULSE9_LO_WORD,                                 //register 136

	T38IO_PULSE10_HI_WORD,   // register 137
	T38IO_PULSE10_LO_WORD,   // register 138

	T38IO_PULSE1_YEAR,					//139	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE1_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE1_DATE,
	T38IO_PULSE1_HOUR,
	T38IO_PULSE1_MINUTE,
 
	T38IO_PULSE2_YEAR,					//144	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE2_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE2_DATE,
	T38IO_PULSE2_HOUR,
	T38IO_PULSE2_MINUTE,	
 
	T38IO_PULSE3_YEAR,					//149	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE3_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE3_DATE,
	T38IO_PULSE3_HOUR,
	T38IO_PULSE3_MINUTE,
 
	T38IO_PULSE4_YEAR,					//154	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE4_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE4_DATE,
	T38IO_PULSE4_HOUR,
	T38IO_PULSE4_MINUTE,
 
	T38IO_PULSE5_YEAR,					//159	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE5_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE5_DATE,
	T38IO_PULSE5_HOUR,
	T38IO_PULSE5_MINUTE,	
 
	T38IO_PULSE6_YEAR,					//164	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE6_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE6_DATE,
	T38IO_PULSE6_HOUR,
	T38IO_PULSE6_MINUTE,
 
	T38IO_PULSE7_YEAR,					//169	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE7_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE7_DATE,
	T38IO_PULSE7_HOUR,
	T38IO_PULSE7_MINUTE,
 
	T38IO_PULSE8_YEAR,					//174	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE8_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE8_DATE,
	T38IO_PULSE8_HOUR,
	T38IO_PULSE8_MINUTE,

  T38IO_PULSE9_YEAR,					//179	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE9_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE9_DATE,
	T38IO_PULSE9_HOUR,
	T38IO_PULSE9_MINUTE,

  T38IO_PULSE10_YEAR,					//184	//the pulse number is cleared as soon as this register be wrote
	T38IO_PULSE10_MONTH,							//not clear pulse number when write the following 4 registers
	T38IO_PULSE10_DATE,
	T38IO_PULSE10_HOUR,
	T38IO_PULSE10_MINUTE,

	T38IO_CHANNEL_TYPE,					//189   //USE 8 bits in one byte represent each channel type.LST is channel0 and 1 means analog,0 means digital 

				
	T38IO_INPUT1_READING,				//190	analog reading for channel 1 through 8,whatever the channel be set analog or high speeed pulse mode		
	T38IO_INPUT2_READING,
	T38IO_INPUT3_READING,
	T38IO_INPUT4_READING,	
	T38IO_INPUT5_READING,
	T38IO_INPUT6_READING,
	T38IO_INPUT7_READING,
	T38IO_INPUT8_READING,
	T38IO_INPUT9_READING,
	T38IO_INPUT10_READING,     //199


	T38IO_INPUT1_RANGE,					//200	Set range for each input. 0 = raw data,1 = 10K Celsius,2 = 10K Fahrenheit			  
	T38IO_INPUT2_RANGE,					//		3 = 0 - 100%,4 = ON/OFF,5 = OFF/ON   ,7 light
	T38IO_INPUT3_RANGE,
	T38IO_INPUT4_RANGE,	
	T38IO_INPUT5_RANGE,
	T38IO_INPUT6_RANGE,
	T38IO_INPUT7_RANGE,
	T38IO_INPUT8_RANGE,
	T38IO_INPUT9_RANGE,
	T38IO_INPUT10_RANGE,      //209

	T38IO_INPUT1_FILTER,			//210  Filter for all inputs,0 through 100.
	T38IO_INPUT2_FILTER,
	T38IO_INPUT3_FILTER,
	T38IO_INPUT4_FILTER,
	T38IO_INPUT5_FILTER,
	T38IO_INPUT6_FILTER,
	T38IO_INPUT7_FILTER,
	T38IO_INPUT8_FILTER,
	T38IO_INPUT9_FILTER,
	T38IO_INPUT10_FILTER,     //219

	T38IO_INPUT1_TIMER,				//220  Timer,how long time the lighting control take over the outputs
	T38IO_INPUT2_TIMER,
	T38IO_INPUT3_TIMER,
	T38IO_INPUT4_TIMER,
	T38IO_INPUT5_TIMER,
	T38IO_INPUT6_TIMER,
	T38IO_INPUT7_TIMER,
	T38IO_INPUT8_TIMER,
    T38IO_INPUT9_TIMER,
    T38IO_INPUT10_TIMER,        //229

	T38IO_INPUT1_TIMER_LEFT,		//230  Timer Left,how much time left for the lighting control
	T38IO_INPUT2_TIMER_LEFT,
	T38IO_INPUT3_TIMER_LEFT,
	T38IO_INPUT4_TIMER_LEFT,
	T38IO_INPUT5_TIMER_LEFT,
	T38IO_INPUT6_TIMER_LEFT,
	T38IO_INPUT7_TIMER_LEFT,
	T38IO_INPUT8_TIMER_LEFT,
  	T38IO_INPUT9_TIMER_LEFT,
  	T38IO_INPUT10_TIMER_LEFT,   //239

 	T38IO_OUTPUT_MANUAL,				//240 light control disable/enable ,each bit correspond to one output
										//	  output1 correspond to least significant bit,0 = disable,1 = enable



	T38IO_ZONE_OUTPUT1,      //241
	T38IO_ZONE_OUTPUT2,
	T38IO_ZONE_OUTPUT3,
	T38IO_ZONE_OUTPUT4,
	T38IO_ZONE_OUTPUT5,
	T38IO_ZONE_OUTPUT6,
	T38IO_ZONE_OUTPUT7,
	T38IO_ZONE_OUTPUT8,
   

	
	T38IO_REVERSE_OUTPUT,				// 249 in override mode, reverse relay output

	T38IO_STATUS_OUTPUT1,				// 250 status for each output to show if the relay output working on override mode
	T38IO_STATUS_OUTPUT2,
	T38IO_STATUS_OUTPUT3,
	T38IO_STATUS_OUTPUT4,
	T38IO_STATUS_OUTPUT5,
	T38IO_STATUS_OUTPUT6,
	T38IO_STATUS_OUTPUT7,
	T38IO_STATUS_OUTPUT8,
    //264
	T38IO_SERINALNUMBER_WRITE_FLAG,
	T38IO_DEFAULT_SETTING = 300,
	//TEST_REGISTER   = 299 


};



