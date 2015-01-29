#ifndef __IO_OPR_H__
#define __IO_OPR_H__

#define SYSCLK       11059200L          // SYSCLK frequency in Hz
#define SAMPLENUMBER 10
#define SWITCH_NUMBER  12

#define AI0       0
#define AI1       1
#define AI2       2
#define AI3       3
#define AI4       4
#define AI5       5
#define AI6       6
#define AI7       7
#define AI8       8
#define AI9       9



#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80
#define BIT8		0x0100
#define BIT9		0x0200
#define BIT10		0x0400
#define BIT11		0x0800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000
bit RELAY0, RELAY1,RELAY2,RELAY3,RELAY4,RELAY5,RELAY6,RELAY7;


void Main_Get_Switch_Status(void);
void Main_Oscillator_Init(void);
void timer0_init();
void UART0_Init(void);
//void UART1_Init(void);
void Set_DACs(void);
void Main_Io_init();
void Main_System_Init(void);
void Main_Data_init(void);
void main_flash_init(void);   
void start_timer(et_event timer_no,unsigned int timeout);
void stop_timer(et_event timer_no);
void push_event( et_event event_type);
void tabulate_LED_STATE(void);
void Timer2_Init(void);
void delay_us (unsigned	int time);
void watchdog (void);
void start_timer(et_event timer_no, unsigned int timeout); 
void stop_timer(et_event timer_no);
void push_event(et_event event_type);
void refresh_LEDs (void);
void check_switches(void);
void test(void);
void check_switches1(void);
unsigned int Filter(unsigned char channel,unsigned input);
extern unsigned char xdata gucFilterCounter[8];


/*-----------------IO_8.h---------------------*/

// --- function prototypes --------------------------

void initial();
void set_hardware(void);


void refresh_inputs (void);

// void refresh_inputs_SOP0 (void);
extern void refresh_outputs (void);






 
extern void ClearBit(unsigned char bit_number,unsigned char *byte);
extern bit  GetBit(unsigned char bit_number,unsigned char *value);
 
void StorePulseToFlash( void );
#ifdef T3_8IN13OUT  //MHF 20010_07 COMBINE TWO IFDEFS
extern unsigned char xdata pulse_number[50];
typedef union   pulse_number_link {
		unsigned long      number_long[8];
		unsigned char    pulse_number[32];
	};
extern union   pulse_number_link  xdata  pic,flash;

 
extern unsigned char   clear_pulse_channel;
unsigned char AD_Enable = 0;
unsigned char AD_Enable2 = 8;
unsigned long xdata analog_in_buffer[10]={4095,4095,4095,4095,4095,4095,4095,4095,4095,4095};//the last byte for on board temp sensor, newly added by Evan

union {
         unsigned long  word[10];
         unsigned int   half[20];
         unsigned char  quarter[40];
      }pulse;
 
typedef union   dual_reading {
		unsigned int      dual_word[8];
		unsigned char    dual_byte[16];
	};
extern union dual_reading xdata adam;
//extern unsigned char xdata gucTimerFilter[10];
unsigned char idata  gucMinuteCounter[10];
unsigned char xdata gucTimer[10] _at_ 0x02E0;
unsigned char xdata gucTimerLeft[10];
unsigned int xdata guiManual;
//extern unsigned char xdata gucStatus[13];
unsigned char xdata gucStatus[8]={0};
unsigned char xdata  gucOverOutput[10];// = {0};
 
#endif
// --- variables ------------------------------------


bit select_switch = 0;

unsigned char xdata Switch_state[MAX_OUTPUT_CHANNELS];

//unsigned char xdata output_channel ; 	// changed place
unsigned char xdata input_channel ; 

//unsigned char xdata output_calibration;
unsigned char xdata output_sequence = 1;		// july 21 ron

//unsigned int look_up_table(unsigned int count );

unsigned char idata LED_bank = 1;
unsigned int LED_State[MAX_LED_BANK];
unsigned char event_count = 0 ;

et_event event_queue[ EVENT_QUEUE_SIZE ] ;
et_event timer_no ;



extern unsigned char xdata range[10];
extern unsigned int xdata gucZone[8];
extern bit	gbClearPulse ;
extern unsigned char xdata gucClearPulse;
extern bit	gbSetChannel ;
extern unsigned char xdata gucSetChannel;

unsigned int const code init_big_ticks[ TOTAL_TIMERS ] =
{

  10,		// HEARTBEAT_PULSE	0.1sec
  10,		// REFRESH_OUTPUTS	0.20sec  changed by KR//10
  10,		// REFRESH_INPUTS	0.05sec	   
  //50,		// FLEXDRIVER		0.50sec
  //50,		// TIMER_SOP		0.50sec
  //10,		// INPUT_PWM_TIMER	0.10sec
  //5,		// HIGH_SPEED		0.05sec	
  5,		// CHECK_SWITCHES	0.1sec changed by KR

  25,       // INPUT1 TIMER     5.0 sec
  25,       // INPUT2 TIMER     5.0 sec
  25,       // INPUT3 TIMER     5.0 sec
  25,       // INPUT4 TIMER     5.0 sec
  25,       // INPUT5 TIMER     5.0 sec
  25,       // INPUT6 TIMER     5.0 sec
  25,       // INPUT7 TIMER     5.0 sec
  25,       // INPUT8 TIMER     5.0 sec 
  25,       // INPUT9 TIMER     5.0 sec 
  25,       // INPUT10 TIMER     5.0 sec 
  //50000		 // 
  5000    // STORE PULSE 		2.0 minute

};


 

#ifndef T3_32IN
unsigned char hand_state[OUTPUT_SIZE] = 0;
unsigned char auto_state[OUTPUT_SIZE] = 0;
unsigned char off_state[OUTPUT_SIZE] = 0;
unsigned char hand_state_filter[OUTPUT_SIZE] = 0;
unsigned char auto_state_filter[OUTPUT_SIZE] = 0;
unsigned char off_state_filter[OUTPUT_SIZE] = 0;
#endif

 
unsigned char idata heart_beat ;  //main heartbeat counter of the system
unsigned int xdata temp_unsigned_char ;
unsigned int idata big_ticks[TOTAL_TIMERS ];

// variable for hardware digital mode
// august Ron
bit digital_mode = 0;

bit reverse_logic_output = 0;




// --- function prototype Serial Com -----------------
extern void initSerial(void);
extern void serial_restart(void);


extern unsigned int read_feedback(void);
extern unsigned char idata DealwithTag;
extern void dealwithData(void);

// --- functionr for PIC ------------------------------
extern void pic_detect(void);

extern void write_pic( unsigned char addr, unsigned char value );
extern bit read_pic_original( unsigned char store_location, bit set );
extern void read_pic(void);

 
extern void read_pulse_pic(void);
unsigned int xdata channel_type = 0  ;
extern bit WritePicType(unsigned char channelType);


extern unsigned char pic_type;
extern unsigned char input_channel_select;
//extern unsigned char input_bank_select;
// -------------------------------------------------------------------



// --- variables Serial Com --------------------------
unsigned char com_LED_count, com_LED_count1;

bit pulse_flag;

extern unsigned char xdata SNWriteflag ;	

extern unsigned char idata rece_count;
extern unsigned char serial_receive_timeout_count;




// --- function prototype Flash -----------------------
extern void flash_init(void);
extern bit flash_write_int(unsigned char id, unsigned int value, unsigned char block_select);
extern bit flash_read_int(unsigned char id, unsigned int *value, unsigned char block_select);
extern bit flash_read_char(unsigned char id, unsigned char *value, unsigned char block_select);
extern bit FlashRead_Absolute(unsigned int location, unsigned char *value);


// --- variables Flash --------------------------------
unsigned char xdata hardware_rev = 0;





// --- function prototype timer_SOP ------------------------------
extern void timer_serial_signal(unsigned char start_id, unsigned char end_id, unsigned int tstat_reg_addr, unsigned int tstat_reg_data);




// --- variables timer_SOP ----------------------------------------
// varibles used for testing sequence
// august 9 Ron
//unsigned int xdata testing_increment_init	= 300;
unsigned char xdata testing_error_flag 	= 0;
char channel_error_flag 			= 0;
bit switch_error_flag				= 0;
bit Enable_set_output 				= 1;
bit Enable_read_input 				= 0;
//bit incrementing					= 0;

//unsigned int xdata [16];

#ifdef TIMER_SOP_FEATURE
	unsigned char xdata timer_id = START_ID;
#endif

bit switching_flag_previous;	// On mode is 0, AUTO mode is 1
bit switching_flag_present;		// On mode is 0, AUTO mode is 1



	



// --- FLEXDRIVER function prototype ---------------------------------------
extern void master_com_send_table(unsigned char id, unsigned int FlexDriver_reg_addr, unsigned char length);



// --- FLEXDRIVER variables ------------------------------------------------
bit Serial_Master;	// if set to 1, notify I am master and will want to RECEIVE a response

#ifdef FLEXDRIVER_FEATURE

extern unsigned char xdata DisplayBuffer_Pairs[PAIR_LENGTH][3];
unsigned char flex_set_select = 0;
extern unsigned char send_table_counter;
extern bit send_table_error_flag;

#endif





bit new_heartbeat;
//unsigned char xdata startup_flag;

xdata struct link  modbus;


// --- flag testing ----------------------------------------------
unsigned char debug_test_flag = 0;




// --- added flags for Benny's features --------------------------
//unsigned char input_pulsing_counter[3] = {0,0,0};
//char input_holding_counter[3] = {0,0,0};
unsigned char xdata input_holding_counter = 0;
//bit out1 = 0;
//bit out2 = 0;
bit out3 = 0;
//bit out4 = 0;





// --- PWN feature ---------------------------------------------------
#ifdef PWM_TRANSDUCER
	extern unsigned char xdata time_calibration_offset;	// for PWM timer
	extern void check_digital_inputs(void);
	extern void PWM_timer_count(void);
	extern unsigned int xdata digital_in_counter[6];
#endif

// -------------------------------------------------------------------





// --- calibration feature ---------------------------------------------------
#ifdef CALIBRATION_OPTION
	extern void produce_LUT(void);
	extern void store_data_to_grid(unsigned int grid_location);
	extern unsigned int calibration_address;
#endif
// -------------------------------------------------------------------


// --- High Speed feature ---------------------------------------------------
#ifdef REX_FEATURES
	extern unsigned char high_speed_counter;
	extern unsigned char xdata high_speed_buffer[HIGH_BUFFER];		// arbitrarily chosen at 20
	extern unsigned char xdata high_speed_index;
#endif
// -------------------------------------------------------------------










#endif
