/********************* serial.h *****************************************/
extern unsigned char iap_program_data_RAM(unsigned char val, unsigned int addr);
extern void iap_erase_page (unsigned char block);



// --- function prototypes ----------------------------------------------
bit checkData(unsigned int address);
void dealwithData(void);
void responseData(unsigned int address);
void internalDeal(unsigned int start_address);

unsigned int CRC16(unsigned char *puchMsg, unsigned char usDataLen);

void InitCRC16(void);
void CRC16_Tstat(unsigned char ch);
extern unsigned long xdata analog_in_buffer[10];//the last byte for on board temp sensor, newly added by Evan
//void SetBit(unsigned char bit_number,unsigned char *byte);
//void ClearBit(unsigned char bit_number,unsigned char *byte);
//bit GetBit(unsigned char bit_number,unsigned char *byte);

// --- variables --------------------------------------------------------
unsigned char xdata packet_size;		// august Ron

unsigned char idata data_buffer[DATABUFLEN];
//unsigned char  send_buffer[SENDPOOLLEN];
unsigned char idata CRClo,CRChi;   // CRC low byte ,high byte
unsigned char idata DealwithTag;          // 0 not to dealwith ,1 dealwith end,2 begin to dealwith ,3 don't need dealwith 4 be to dealwith

unsigned char serial_receive_timeout_count;
unsigned char data SERIAL_RECEIVE_TIMEOUT;


bit transmit_finished;

unsigned char idata rece_count = 0;   // number of data received 

unsigned char idata randval;

unsigned char xdata SNWriteflag ;	// from LSB, low byte SN write
									//			high byte SN write
									//			hardware rev
									//			timer SOP previous state


extern unsigned char xdata reading_filter_bypass;
#ifdef T3_8IN13OUT //MHF 2001_07, CHANGE IFDEFS TO USE ONLY ONE DEFINE FOR EACH MODEL
extern unsigned int channel_type;
unsigned char xdata pulse_number[50];// 
typedef union   pulse_number_link {
		unsigned long      number_long[8];
		unsigned char    pulse_number[32];
	};


extern   union {
         unsigned long  word[10];
         unsigned int   half[20];
         unsigned char  quarter[40];
      }pulse;


union   pulse_number_link  xdata  pic,flash;

typedef union   dual_reading {
		unsigned int      dual_word[8];
		unsigned char    dual_byte[16];
	};
extern union dual_reading xdata adam;

extern unsigned int xdata adam_buffer[8];
extern unsigned char xdata gucTimer[8];
extern unsigned char xdata gucStatus[8];
extern unsigned char xdata gucTimerLeft[8];
extern unsigned int xdata guiManual;
extern unsigned char  xdata gucOverOutput[10];
#endif



unsigned char xdata range[10];

 

//extern unsigned long xdata  analog_filter[10];
extern unsigned char xdata filter[10];
//extern unsigned long xdata previous_pulse_number[8]; 
 

bit	gbClearPulse = 0;
unsigned char xdata gucClearPulse;

bit	gbSetChannel = 0;
unsigned char xdata gucSetChannel;

extern bit WritePicCommand(unsigned char channel);
unsigned char   clear_pulse_channel = 0;
extern unsigned int channel_type ;
extern bit WritePicType(unsigned char channelType);

// --- function prototype main program -----------------------------------
extern void start_timer( et_event timer_no , unsigned int timeout ) ;
extern void stop_timer( et_event timer_no ) ;

extern void delay_us(unsigned int time);
extern void watchdog();
 
// --- variables main program ---------------------------------------------
extern unsigned char com_LED_count,com_LED_count1;
// extern bit com_LED_count;
extern unsigned char xdata Switch_state[MAX_OUTPUT_CHANNELS];

//extern unsigned char xdata output_calibration;
extern unsigned char xdata output_sequence;		// july 21 Ron
//extern unsigned char xdata startup_flag;	// for testing SOP
extern unsigned char xdata testing_error_flag;
extern xdata  struct link modbus;

extern unsigned char idata heart_beat ;  //main heartbeat counter of the system
unsigned int modbus_data[12];


// --- function prototype Flash -------------------------------------------
extern bit flash_write_int(unsigned char id, unsigned int value, unsigned char block_select);
extern bit flash_read_char(unsigned char id, unsigned char *value, unsigned char block_select);
extern bit flash_read_int(unsigned char id, unsigned int *value, unsigned char block_select);

extern unsigned char iap_program_data_byte(unsigned char val, unsigned int addr);
extern void iap_erase_block(unsigned char block);


extern bit FlashRead_Absolute(unsigned int location, unsigned char *value);


// --- variables Flash ----------------------------------------------------



extern unsigned int xdata guiAnalogInput[10];
//extern void SetBit(unsigned char bit_number,unsigned char *byte);
//extern void ClearBit(unsigned char bit_number,unsigned char *byte);
//extern bit GetBit(unsigned char bit_number,unsigned char *byte);
 unsigned char const code auchCRCHi[256];
 unsigned char const code auchCRCLo[256];





unsigned char const code auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;	
	/* Table of CRC values for high�Corder byte */


unsigned char const code  auchCRCLo[256] = {

0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40

} ;	
/* Table of CRC values for low Corder byte */


#define RESPONSERANDVALUE	1




// --- testing flags -------------------------------------------------------


// --- update ISP -----------------------------------------------------------
extern unsigned char xdata hardware_rev;









// --- FLEXDRIVER variables ------------------------------------------------
bit response_receive_finished;

// buffer of the information being transmitted from the com ports
//extern unsigned char xdata DisplayBuffer_Cathode_Receive[DISPLAY_LENGTH];

// initialize to 0xFF so that we control which one to turn OFF
//extern unsigned char xdata DisplayBuffer_Cathode_Blinking1[DISPLAY_LENGTH];
//extern unsigned char xdata DisplayBuffer_Cathode_Blinking2[DISPLAY_LENGTH];



// --- FLEXDRIVER function prototype ---------------------------------------
extern void master_com_serial(unsigned char Flex_id, unsigned int FlexDriver_reg_addr, unsigned int FlexDriver_reg_data);




// --- PIC variables ------------------------------------------------------
extern unsigned int xdata reading_counter;
extern unsigned char pic_type;



// --- calibration ---------------------------------------------------------

#ifdef CALIBRATION_OPTION
//extern void store_data_to_grid(unsigned int grid_location);

extern unsigned int xdata VoltageReading_defines[MAX_INPUT_CHANNELS][DEFINE_TABS];
extern unsigned int xdata serial_input_grid[MAX_INPUT_CHANNELS][2];

unsigned int calibration_address = 0xFFFF;

#endif



#ifdef PWM_TRANSDUCER
	extern unsigned char xdata time_calibration_offset;	// for PWM timer
#endif




// --- HIGH SPEED ---------------------------------------------------------------
#ifdef REX_FEATURES
extern unsigned char high_speed_counter;
#endif
