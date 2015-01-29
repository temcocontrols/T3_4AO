// to select proper module comment-in the hardware model and comment-out the rest
// by default it is set to T3_8IO_REV8 given it is the most recent basic board

//****************tstatio.h************************************

// --- SEQUENCE OF OPERATION ----------------------------------
//	write to FLASH_SEQUENCE register in modbus
//		0 = testing sequence for the 8IO module ONLY
//		1 = normal operation
//		2 = Timer_SOP feature added for input one of 8IO module
//				will control all TSTAT on network
//				*** thinking of removing this given not used anymore ***
//		3 = operation where the T3-modules do not have switches, thus always automatic mode
//			will respond according to output register values


// *************************************************************

// important notes... timers must be changed according to different configuration boards...
//	for instance, 32inputs must go through inputs much faster rate than for 8 inputs...

// ---Analog T3 8Input 8 analog Output board
//#define T3_8IO_A			0x0808



// ---Digital T3 8In 13 relay Out Board
//#define T3_8IO			0x0808	// for hardware rev 5 and below
//#define T3_8IO_REV6		0x0808	// for hardware rev6
//#define T3_8IO_REV8		0x0808	// for hardware rev8 and above
#define T3_8IN13OUT			0x0108 // HARDWARE MODEL 





// ---T3 8In 16Out board
//#define T3_8IN16OUT		0x0810
	// reminder for REX boards...
	//			- No Pull-ups on the board
	//			- No Switches required
	//			- Fast reactive code for calibration.  incraesed readings and reduce filtering



// ---T3 32Input board
//#define T3_32IN			0x2000  
	// for given 32IN requires a lot more ram, cannot use the calibration option simultaneously
	// must set REFRESH_INPUTS ticks to 1 for fast reaction

// ---PWM transducer board
//#define PWM_TRANSDUCER



// -----------------------------------------------------------------
// below are special features which can be applied to different boards

//#define BENNY_FEATURES
//#define REX_FEATURES
//#define FLEXDRIVER_FEATURE
//#define TIMER_SOP_FEATURE
//#define CALIBRATION_OPTION




// -----------------------------------------------------------------
// below are special defines for software characteristics
#define T3_SOFTWARE_VERSION		34

	sbit TX1				 = P0^0;	
  	sbit RX1				 = P0^1;
	sbit TX2				 = P0^2;	
  	sbit RX2				 = P0^3;
  	sbit I2C_SDA		 = P0^4;
	sbit I2C_SCL			 = P0^5;
 	sbit RS232STATE 		 = P0^6;
  sbit RS232STATE2 		 = P0^7;

  sbit LED8         = P2^0;
  sbit LED_DRIVE1		= P2^1;	
	sbit LED_DRIVE2		= P2^2;	// STATUS LED
	sbit LED_DRIVE3		= P2^3;	


 

  sbit SWTICH0    = P5^0;
  sbit SWTICH1    = P5^1;
  sbit SWTICH2    = P5^2;
  sbit SWTICH3    = P5^3;
  sbit SWTICH4    = P5^4;
  sbit SWTICH5    = P5^5;
  sbit SWTICH6    = P5^6;
  sbit SWTICH7    = P5^7;
  sbit SWTICH8    = P6^0;
  sbit SWTICH9    = P6^1;
  sbit SWTICH10   = P6^2;
  sbit SWTICH11   = P6^3;

  sbit RELAY_ENBLE =P7^5;
  sbit DACMUX       = P7^1;
  sbit DACMUX_enable= P2^4;
  sbit TEST       = P7^0;
  sbit KEYPAD_HAND	= P7^7;
	sbit KEYPAD_AUTO	= P7^3;
	#define T3_PRODUCT_MODEL	20
 
















// -----------------------------------------------------------------
// -----------------------------------------------------------------

//----------Hardware definitions ---------------------
