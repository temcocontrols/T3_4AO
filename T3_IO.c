#include "c8051f120.h"
#include "LibIO_T3IO.h"
#include "define.h"
#include "T3_IO.h"
#include "math.h"		// to use the abs function, August Ron
#include  "pic.h"
 unsigned char xdata filter[10];
//unsigned char xdata gucPreviousInput[8] _at_ 0x2F0;
unsigned char xdata gucPreviousInput[10];
extern unsigned char xdata gucReverseOutput;
extern unsigned int xdata  guiDeadmasterTime;
extern unsigned char xdata gucDeadmaster;
unsigned char xdata Switch_state_buffer[SWITCH_NUMBER];
unsigned int xdata guiBuffer[10][SAMPLENUMBER];
//unsigned int xdata flash_data_mod[21];
extern unsigned int modbus_data[12]; 
unsigned int xdata guiAnalogInput[10];
unsigned int xdata  Adc_data[10] ;
// unsigned char xdata Input_timer[10];
extern unsigned char xdata Swtich_state[3];
 //int xdata guiBuffer_old[10]={1023,1023,1023,1023,1023,1023,1023,1023,256,256};
unsigned int xdata guiBuffer_old[10]={0,0,0,0,0,0,0,0,0,0};
extern signed int RangeConverter(unsigned char function, signed int para,signed int cal,unsigned char i);
void LightOutput(unsigned char overOut);
unsigned int xdata perious_input[10]={4095,4095,4095,4095,4095,4095,4095,4095,4095,4095};
//unsigned int xdata test_timer;
extern bit rang_flag;
extern unsigned char xdata pic_version; 
extern unsigned char data SERIAL_RECEIVE_TIMEOUT;
bit new_heartbeat;
bit High_speed_count,LED_flag; 
bit dac; 
bit D16_Flash;//flag_I2C;
unsigned char count1=0;
unsigned char count_dac=0;
unsigned int D16_Flash_Pulse;
unsigned char RELAY_STATUS;
unsigned char temp_storepulse = 0;
unsigned int xdata baud_rate;
unsigned char xdata info[20];
bit baudrate_flag = 0; 
sfr16 DAC0     = 0xd2;                 // DAC0 data
sfr16 DAC1     = 0xd2;                 // DAC1 data
// timer0: timer
// timer1: uart0
// timer2: ADC0,ADC2   
// timer3: DAC
// timer4: uart0   


void Main_Oscillator_Init(void)
{                        
   unsigned int i;  
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;
   OSCICN = 0X83; 
   RSTSRC = 0X04;
   //RSTSRC  |=0X02;
   OSCXCN = 0x60;
   OSCXCN |= 0x07;
   for(i =13000; i > 0; i--);
   while((OSCXCN & 0x80) == 0);//wait until the  Oscillator run stable
   CLKSEL = 0x01;     //
   //OSCICN &= ~0X80; 
   SFRPAGE = SFRPAGE_SAVE;
}

void timer0_init()
{
    char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
    SFRPAGE = TIMER01_PAGE;
    TMOD= 0X01;
    TL0 = 0x00;
  	TH0 = 0x80;  
  	
  	TR0 = 1;		// start up time0 count
  	ET0 = 1;		// start up time0 interrupt	 
  	//PT0 = 1;		// GIVE PRIORITY TO THE TIMER0 INTERRUPT... THIS REALLY HELPS PREVENT DISPLAY FLICKER 
    SFRPAGE = SFRPAGE_SAVE;
}

void UART0_Init(void)
{
    char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
	if(info[15] == 1)
	{
    	SFRPAGE   = TMR4_PAGE;
      	TMR4CN    = 0x00; 
        TMR4CF    = 0x08; //user SYSCLK
        RCAP4L    = 0xDC;   //set baud rate as 192000
        RCAP4H    = 0xFF;			// RCAP2 = - ((long) SYSTEMCLOCK/BAUDRATE/16)
        TR4= 1;            //start timer
        EIE2 &= ~0x04;
		SERIAL_RECEIVE_TIMEOUT = 3;
	}
	else if(info[15] == 0)
	{   SFRPAGE   = TMR4_PAGE;
        TMR4CN    = 0x00; 
        TMR4CF    = 0x08; //user SYSCLK
        RCAP4H = 0xFF;
        RCAP4L = 0XB8;
        TR4= 1;            //start timer
        EIE2 &= ~0x04;
		SERIAL_RECEIVE_TIMEOUT = 6;
	}

    SFRPAGE   = UART0_PAGE;
    SCON0     = 0x50;   // mode 1:  set uart0 as 9bit uart  fixed baud rate . enable receive
    SSTA0     = 0x0F;   // set timer 4 overflow as  tx and rx  baud rate   double the baud rate
    
    ES0 = 1;            // turn on the inturrput  
    PS = 1;             //prior the uart
    SFRPAGE = SFRPAGE_SAVE;
}
 
/*
void UART1_Init(void)
{


} 
*/
//-----------------------------------------------------------------------------
// Timer2_Init to control ad convert
//-----------------------------------------------------------------------------
// Configure Timer3 to auto-reload at interval specified by <counts> (no 
// interrupt generated) using SYSCLK as its time base.

#if 1

void Timer2_Init(void)   //interrupt per 1000us for ADC0   ADC1
{
    char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
    SFRPAGE   = TMR2_PAGE;
  	TMR2CF = 0x08;                      // use SYSCLK as timebase
  	//RCAP2L = 0X4D;                  // Init reload values
  	//RCAP2H = 0XDC; 
   // TMR2L = 0x4D;                  // set to reload immediately
  //	TMR2H = 0XDC; 

    RCAP2L = 0X66;                  // Init reload values
  	RCAP2H = 0XEA;
    TMR2L = 0x66;                  // set to reload immediately
  	TMR2H = 0XEA; 
    IE &= ~0x20;            // disable the Timer 2 interrupt 
  	TMR2CN |= 0x04;   				//start timer2   auto_reload       
    SFRPAGE = SFRPAGE_SAVE;
} 
    
void ADC_Init(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = ADC0_PAGE;
   ADC0CN = 0x8C;  //set ADC start bye timer2 overflow  right-justified
   ADC0CF = ((SYSCLK/2500000) - 1) << 3; // ADC conversion clock = 2.5MHz
   // set  amplifier gain as 1  //ADC conversion clock =1.25Mhz
   ADC0CF=0x00;//2011.01.13
   AMX0SL = 0x00; //set all AD conversion as single input
   ADC0L = 0;
   ADC0H = 0;
   SFRPAGE = LEGACY_PAGE;
   REF0CN = 0x03;  //set VREF from VREF pin
   EIE2 |= 0x02;
   EIP2 |= 0x02;  //set ADC  proit

   //#if 0
   SFRPAGE = ADC2_PAGE;      //set ADC start bye timer2 overflow  
    ADC2CN= 0x86;  // set ADC2 Start  by  write 1 of adc2busy
   ADC2CF = (((SYSCLK/2500000) - 1) << 3) + 1; // ADC conversion clock = 2.5MHz    gain=1
   
   AMX2SL = 0x06;   
   ADC2 = 0;
   SFRPAGE = LEGACY_PAGE;
   REF0CN = 0x03;
   EIE2 |= 0x10;                             
  EIP2 |= 0x10;
   SFRPAGE = SFRPAGE_SAVE;
}
#endif
#if 1  
void DAC_Init(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page 
   SFRPAGE   = TMR3_PAGE;
   TMR3CN    = 0x00; //  set  Timer3  as auto reload
   TMR3CF    = 0x08;//0000 1000 selcet SYSCLK
  
   RCAP3L = 0x92;
   RCAP3H = 0xFF; //RCAP4 = -(SYSCLK/SAMPLE PULSE);
   
   TMR3L = 0x92;  // Set time 50 US
   TMR3H = 0xFF;  // Set time 30 US

    //TMR3H = 0xff;
   EIE2 |= 0x01; //enable the timer3  interrupt 
   //EIP2 |= 0x01;
   TMR3CN    = 0x04; // start timer
   
   SFRPAGE = DAC0_PAGE;
   DAC0CN = 0x88; //1000 1000 DAC0 enable  output update occur Timer3 overflow
   SFRPAGE = LEGACY_PAGE;
   REF0CN |= 0x03;    // select VREF 3.3V
   SFRPAGE = DAC1_PAGE;
   DAC1CN = 0x88;   //1000  1000 DAC1 enable  output update occur Timer3 overflow
   SFRPAGE = LEGACY_PAGE;
   REF0CN |= 0x03;   //select VREF 3.3V
   SFRPAGE = SFRPAGE_SAVE;
}
#endif

void Main_Io_init()
{                      
    char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
    SFRPAGE   = CONFIG_PAGE;
    XBR0      = 0x04;
    XBR2      = 0x40;	 
        
    //P0MDOUT = 0xC5;
    P0MDOUT = 0xff;
    P0 = 0x3A;
    
    P1MDIN = 0x00;
    //P1 = 0;
    P2MDOUT = 0xFF;
    P2 = 0x0f;
   
    P3MDOUT = 0xFF;
    //P3 = 0;
      P3 = 0xff;
    P4MDOUT = 0xFF;
    P4 = 0;
    
    P5MDOUT = 0x00;
    P5 = 0xFF;
    
    P6MDOUT = 0x10; //set P6 as drain out
    P6 = 0xFF;
    
    P7MDOUT = 0xFF;
    P7 = 0x00;

    SFRPAGE = SFRPAGE_SAVE;
}


void Main_System_Init(void)
{
    Main_Oscillator_Init();
    Main_Io_init();
    timer0_init();
    Timer2_Init();
    UART0_Init();
    ADC_Init();
    DAC_Init();
    watchdog();
}
 


void timer0(void) interrupt 1 
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page 
   TL0 = 0x66;     //interal :1ms
   TH0 = 0xfc;

   heart_beat++;  

   D16_Flash_Pulse++;
   if(D16_Flash_Pulse >500)
   {
      D16_Flash_Pulse = 0;
      D16_Flash = ~D16_Flash;
      }                
     
   new_heartbeat = TRUE;
   LED_bank++;
   if(LED_bank > MAX_LED_BANK)
   {
     LED_bank = 1;  
   }
  refresh_LEDs();
  // High_speed_count = ~High_speed_count;
   SFRPAGE   = CONFIG_PAGE;
  // read_pic();
 // watchdog();
   SFRPAGE = SFRPAGE_SAVE;
   //SFRPAGE = ADC2_PAGE; 
   //ADC2CN |= 0x10;
  // SFRPAGE = SFRPAGE_SAVE;
   
 }	

void TIMER3_ISR (void) interrupt 14
{
   char SFRPAGESAVE = SFRPAGE;
  // unsigned char count=0;
                                   // Clear Timer3 overflow flag  
   TF3=0;
   if(count_dac<250) 
   {
       DACMUX_enable= 0;
       count_dac++;
   }
   else
   {
   
   DACMUX_enable = 1;
   count_dac=0;
   dac = ~dac;

   Set_DACs();
   SFRPAGE = CONFIG_PAGE;
    DACMUX = dac;
   SFRPAGE = SFRPAGESAVE;
   }
}

void Set_DACs(void)
{

   char SFRPAGESAVE = SFRPAGE;
   SFRPAGE = DAC0_PAGE;
   #if 1
   if(dac)
   {    //DAC0L=4095;
        //DAC0H=4095>> 8;
      DAC0L = (unsigned char)modbus_data[8];
      DAC0H = (unsigned char)(modbus_data[8] >> 8);
   }
   else
   {   //DAC0L=0;
       // DAC0H=0>> 8;
      DAC0L = (unsigned char)modbus_data[9];
      DAC0H = (unsigned char)(modbus_data[9] >> 8);
   }

   SFRPAGE = DAC1_PAGE;
   if(dac)
   {
        //DAC1L=0;
        //DAC1H=0>> 8;
      DAC1L = (unsigned char)modbus_data[10];
      DAC1H = (unsigned char)(modbus_data[10] >> 8); 

   }
   else
   {
        //DAC1L=0;
        //DAC1H=0;
     DAC1L = (unsigned char)modbus_data[11];
      DAC1H = (unsigned char)(modbus_data[11] >> 8);
   }  
   #endif
   SFRPAGE = SFRPAGESAVE;   
}                     


void watchdog(void)
{
  WDTCN=0XA5;
}

void delay_us (unsigned	int time)
{
   while( (time--) !=0 );
}

void main_flash_init(void)                              
{
	unsigned int xdata	pulse_buf ;
	if(!flash_read_int(FLASH_SOFTWARE_VERSION_NUMBER , &pulse_buf, FLASH_MEMORY))
	{
	 	info[5] = 23;	
		flash_write_int(FLASH_SOFTWARE_VERSION_NUMBER, info[5], FLASH_MEMORY);   
	}
	else
	{
	    info[5] = pulse_buf;
		if(info[5] < 23)   //if there is old rev hex in the code,  we should set a flag to reverse the register 15.
		{
			baudrate_flag = 1 ;	
		}   
	}	  
  flash_write_int(FLASH_SOFTWARE_VERSION_NUMBER, 23, FLASH_MEMORY);
  flash_write_int(FLASH_PRODUCT_MODEL, 28, FLASH_MEMORY);
  flash_write_int(FLASH_HARDWARE_REV,14, FLASH_MEMORY);
  flash_write_int(FLASH_PIC_VERSION, 1, FLASH_MEMORY);
  flash_write_int(FLASH_SERIALNUMBER_LOWORD, 255, FLASH_MEMORY);
  flash_write_int(FLASH_SERIALNUMBER_LOWORD+1, 255, FLASH_MEMORY);
  flash_write_int(FLASH_SERIALNUMBER_LOWORD+2, 255, FLASH_MEMORY) ;
  flash_write_int(FLASH_SERIALNUMBER_LOWORD+3, 255, FLASH_MEMORY) ;
  flash_write_int(EEPROM_VERSION_NUMBER, 0, FLASH_MEMORY) ;
}

void Main_Data_init(void)
{
  unsigned char xdata Loop,Loop1,i;
  unsigned int xdata pulse_buf;
  flash_init();
  main_flash_init(); 

  SFRPAGE = CONFIG_PAGE;
	watchdog();	
  RELAY0 = 0;
  RELAY1 = 0;
  RELAY2 = 0;
  RELAY3 = 0;

  RELAY4 = 0;
  RELAY5 = 0;
  RELAY6 = 0;
  RELAY7 = 0; 
     
	KEYPAD_HAND = 1;
	KEYPAD_AUTO = 1;
	LED_DRIVE1 = 0;
	LED_DRIVE2 = 0;
	LED_DRIVE3 = 0;
  // read_pic_version();

    for(i=0;i<2;i++)
     {
       LED_State[i]=0x1FF;
     } 
	 /* init the first 20 register*/
	for(i=0;i<20;i++)
	{
		if(!flash_read_int(FLASH_SERIALNUMBER_LOWORD+i , &pulse_buf, FLASH_MEMORY))
		{
		 	info[i] = 1;	
			flash_write_int(FLASH_SERIALNUMBER_LOWORD+i, info[i], FLASH_MEMORY);   
		}
	    else
	    {	info[i] = pulse_buf;
			if((baudrate_flag)&&(i == 15))	// if it is old rev, we should reverse the register 15 to make compatible.
			{
				if(info[15] == 1)
				{
				 	info[15] = 0;		
				}
				else 
				{
					info[15] = 1;	
				}
				flash_write_int(FLASH_SERIALNUMBER_LOWORD+15, info[15], FLASH_MEMORY); 
				baudrate_flag = 0;	
			}
	            
	    }	
	}

    if(!flash_read_int(FLASH_ADDRESS, &pulse_buf, FLASH_MEMORY) || (pulse_buf == 0xffff) || (pulse_buf == 0))
	{
	 	info[6] = 254;	
		flash_write_int(FLASH_ADDRESS, info[6], FLASH_MEMORY);
	}
    else
    {
        info[6] = pulse_buf;    
    }
  for(i=0;i<10;i++)
	{
		if(!flash_read_int(FLASH_INPUT1_TIMER + i,&pulse_buf,FLASH_MEMORY))
		{
		 	pulse_buf = 60;	
			flash_write_int(FLASH_INPUT1_TIMER + i, 60, FLASH_MEMORY);	
		}
		gucTimer[i] = pulse_buf & 0xff;
		gucTimerLeft[i] = 0;
		gucMinuteCounter[i] = 0;
	 	gucOverOutput[i] = 0;
		gucPreviousInput[i] = 1;
		gucFilterCounter[i] = 0;
		
	}

   dac = 0;
  High_speed_count = 0;
  D16_Flash = 0;
  SFRPAGE = LEGACY_PAGE;

 	for(Loop = 0;Loop < 50;Loop++)
	{
		pulse_buf = 0;
		flash_read_int(FLASH_PULSE1_YEAR + Loop,&pulse_buf,FLASH_MEMORY);			 
		pulse_number[Loop] = pulse_buf & 0xff;
	}
  for(Loop = 0;Loop < 10;Loop++)
  {
      	if (!flash_read_int(FLASH_INPUT1_FILTER + Loop, &pulse_buf, FLASH_MEMORY))
           { 
           pulse_buf= 20;
           flash_write_int(FLASH_INPUT1_FILTER + Loop, 20, FLASH_MEMORY);
           }
        
        
           filter[Loop]=pulse_buf;
        
 } 

  for(Loop = 0;Loop < 12;Loop++)
  {
    Switch_state_buffer[Loop]=0;    
	}
  for(Loop = 0;Loop < 10;Loop++)
  {
    analog_in_buffer[Loop] = 0;
  }
  for(Loop = 0;Loop < 3;Loop++)
  {
    LED_State[Loop] = 0x01FF;
  }
  for(Loop = 0;Loop < 10;Loop++)
  {
    for(Loop1 = 0; Loop1 < SAMPLENUMBER; Loop1++)
    {
      guiBuffer[Loop][Loop1] = 0;
    }
  }
  for(Loop = 0;Loop < 12;Loop++)
  {
     modbus_data[Loop]= 0x00;
  }  
  for(Loop = 0;Loop < 3;Loop++)
  {
    Swtich_state[Loop] = 0;
   
  }               

    for(Loop = 0;Loop < 10;Loop++) //input
	{	
		if(!flash_read_int(FLASH_INPUT1_RANGE + Loop,&pulse_buf,FLASH_MEMORY))			 
			range[Loop] = 0;	
		else
		{
			if(pulse_buf > 7)
			pulse_buf = 0;
	 		range[Loop] = pulse_buf;
			if(range[Loop] == 6)  SetBit(Loop,&channel_type);
		}	
	}
   

  
  	if(!flash_read_int(FLASH_OUTPUT_MANUAL,&guiManual,FLASH_MEMORY))
	{
	 	guiManual = 0;	
		flash_write_int(FLASH_OUTPUT_MANUAL, guiManual, FLASH_MEMORY);
	   
	}

  if(!flash_read_int(FLASH_REVERSE_OUTPUT, &gucReverseOutput, FLASH_MEMORY) )
  {
    gucReverseOutput = 0;
    flash_write_int(FLASH_REVERSE_OUTPUT, gucReverseOutput, FLASH_MEMORY);
  } 
 
  	for(Loop=0; Loop<20; Loop++)
	{
            
        if(!flash_read_int(FLASH_START_PULSE + Loop , &pulse.half[Loop], FLASH_MEMORY) )
         {
           pulse.half[Loop] = 0;
           flash_write_int(FLASH_START_PULSE + Loop , pulse.half[Loop], FLASH_MEMORY);
         } 

    }
  
  for(Loop = 0;Loop < 8;Loop++) //input
	{	
		if(!flash_read_int(FLASH_ZONE_OUTPUT1 + Loop, &pulse_buf ,FLASH_MEMORY))			 
			gucZone[Loop] = 0;	
		else
		{
			if(pulse_buf > 7)
			 pulse_buf = 0;
			gucZone[Loop] = pulse_buf;

		}
    }

  
 	if(!flash_read_int(FLASH_CHANNEL_TYPE,&pulse_buf,FLASH_MEMORY))			 
	   channel_type = 0;
	else
  	 {
     channel_type = pulse_buf & 0x3ff;
     flash_write_int(FLASH_CHANNEL_TYPE,channel_type,FLASH_MEMORY);
     }
	start_timer(REFRESH_INPUTS , DEFAULT_TIMEOUT );
	start_timer(STORE_PULSE,DEFAULT_TIMEOUT);
 	start_timer(REFRESH_OUTPUTS , DEFAULT_TIMEOUT );
    start_timer(CHECK_SWITCHES,DEFAULT_TIMEOUT);
}
void start_timer(et_event timer_no, unsigned int timeout)
{   
	 
	if(timeout == DEFAULT_TIMEOUT)	// DEFAULT_TIMEOUT = 0
	{
		big_ticks[ timer_no] = init_big_ticks[timer_no ];	//load the timer with the default number of ticks
	  
  }
	else
	{
		big_ticks[ timer_no ]= timeout;    
	}

}

void stop_timer(et_event timer_no)
{ 
  big_ticks[ timer_no ] = DEFAULT_TIMEOUT;
}




void main (void)
{
//  unsigned int xdata pulse_buf;
//  unsigned int cnt;

	WDTCN = 0xde;
	WDTCN = 0xad;
 //WDTCN &=0X7F;
 //WDTCN |=0X07;
  watchdog();
//  read_pic_version();

 SFRPAGE   = LEGACY_PAGE;
 Main_Data_init();    
 Main_System_Init();
  

 //SFRPAGE   = LEGACY_PAGE;
     
  #if 1

 // Main_Data_init();
  initSerial(); 
    
	EA = 1;  
	while (1)
	{ 
    watchdog();

		if(new_heartbeat) 
		{ 
		  new_heartbeat = FALSE;
			if (serial_receive_timeout_count > 0)
			{
				serial_receive_timeout_count--;
				if(serial_receive_timeout_count == 0)
					serial_restart();
                    delay_us(10);
			}
			else
			{
			   
        if (DealwithTag)
				{  
                 DealwithTag--;
				  if((DealwithTag==1))//&& !Serial_Master )	
					dealwithData();
				}
			}
		}
    if ( heart_beat > 3 ) // The heartbeat flag is set to 1 by the timer0 interrupt function
    {  //
    	heart_beat = 0 ;
    	watchdog ( ) ; 
        
      tabulate_LED_STATE();
     // refresh_LEDs();   
    	// For each event timer, reduce the remaining number of ticks by one.
    	// If the number of ticks has reached 0, push the event into the event queue
    	for(timer_no =0; timer_no < TOTAL_TIMERS; timer_no++)
    	{
    		watchdog ( );
    			
    		temp_unsigned_char = big_ticks[timer_no] ;   //load the remaining ticks of the event timer	    
    
    		// If the event timer is paused (ticks = 0) do not push the event or reduce the ticks
    		if( temp_unsigned_char != 0) 
    		{			  
    			temp_unsigned_char--;		   
    			big_ticks[timer_no] = temp_unsigned_char; // Reduce remaining ticks
         
    			if( temp_unsigned_char == 0) 
    			{	
    				push_event(timer_no); // Add event to event queue
    			}
    		}
      }
    }
    while(event_count)	   
    {   
  			watchdog ();
  			event_count--;
  			switch(event_queue[event_count])
  			{   
           case REFRESH_OUTPUTS :
                                 refresh_inputs();
                      			 start_timer(REFRESH_OUTPUTS , DEFAULT_TIMEOUT );
                    			 break;
   
             // keep looking for pic until one has been found                     
           case REFRESH_INPUTS : 	watchdog();
                                   //tabulate_LED_STATE();
                                  //refresh_inputs();
                                  check_switches();
                                  check_switches1();
                                  Main_Get_Switch_Status();
                                  start_timer(REFRESH_INPUTS , DEFAULT_TIMEOUT );	 													
                    							break;
  	  
           case CHECK_SWITCHES :  watchdog();
                                  read_pic();
                    			  start_timer(CHECK_SWITCHES,DEFAULT_TIMEOUT);
                                  break;   
            case STORE_PULSE :	 watchdog(); 
                                StorePulseToFlash( ) ;
                                start_timer(STORE_PULSE , DEFAULT_TIMEOUT );
                      			break; 			   
                                          

             case INPUT1_TIMER:  
                                            watchdog(); 			
                  							gucMinuteCounter[0]++;
                   							  
                  							if(gucMinuteCounter[0] > MINUTE_COUNTER)
                  							{
                  								gucMinuteCounter[0] = 0;
                   								
                  								if(gucTimerLeft[0] > 0)
                  								{
                  									gucTimerLeft[0]--;
                  								 
                  							 
                  								}
                  								else
                  								{
                                                     								 	
                  										gucTimerLeft[0] = 0;
                  										gucOverOutput[0] = 0;
                  										stop_timer(INPUT1_TIMER);
                  										break;
                  								 
                  								}
                  								
                  								
                  							}
                  							start_timer(INPUT1_TIMER,DEFAULT_TIMEOUT);
                  
                  						break;
			 			case INPUT2_TIMER: watchdog(); 
              							gucMinuteCounter[1]++;
              							if(gucMinuteCounter[1] > MINUTE_COUNTER)
              							{
              								gucMinuteCounter[1] = 0;
              								if(gucTimerLeft[1] > 0)
              								{
              									gucTimerLeft[1]--;
              								 
              							 
              								}
              								else
              								{
              
              							 
              										gucTimerLeft[1] = 0;
              										gucOverOutput[1] = 0;
              										stop_timer(INPUT2_TIMER);
              										break;
              									 
              								}
              								
              							}
              							start_timer(INPUT2_TIMER,DEFAULT_TIMEOUT);

						break;
			 			case INPUT3_TIMER:  watchdog(); 
              							gucMinuteCounter[2]++;
              							if(gucMinuteCounter[2] > MINUTE_COUNTER)
              							{
              								gucMinuteCounter[2] = 0;
              								if(gucTimerLeft[2] > 0)
              								{
              									gucTimerLeft[2]--;
              								 
              							 
              								}
              								else
              								{
              
              								 
              										gucTimerLeft[2] = 0;
              										gucOverOutput[2] = 0;
              										stop_timer(INPUT3_TIMER);
              										break;
              									 
              								}
              								
              							}
              							start_timer(INPUT3_TIMER,DEFAULT_TIMEOUT);

						break;
			 			case INPUT4_TIMER: watchdog(); 
            							gucMinuteCounter[3]++;
            							if(gucMinuteCounter[3] > MINUTE_COUNTER)
            							{
            								gucMinuteCounter[3] = 0;
            								if(gucTimerLeft[3] > 0)
            								{
            									gucTimerLeft[3]--;
            								 
            							 
            								}
            								else
            								{
            
            								 
            										gucTimerLeft[3] = 0;
            										gucOverOutput[3] = 0;
            										stop_timer(INPUT4_TIMER);
            										break;
            									 
            								}
            								
            							}
            							start_timer(INPUT4_TIMER,DEFAULT_TIMEOUT);

						break;
			 			case INPUT5_TIMER: watchdog(); 
            							gucMinuteCounter[4]++;
            							if(gucMinuteCounter[4] > MINUTE_COUNTER)
            							{
            								gucMinuteCounter[4] = 0;
            								if(gucTimerLeft[4] > 0)
            								{
            									gucTimerLeft[4]--;
            								 
            							 
            								}
            								else
            								{
            
            								 
            										gucTimerLeft[4] = 0;
            										gucOverOutput[4] = 0;
            										stop_timer(INPUT5_TIMER);
            										break;
            									 
            								}
            								
            							}
            							start_timer(INPUT5_TIMER,DEFAULT_TIMEOUT);
            
            						break;
			 			case INPUT6_TIMER: watchdog(); 
          							gucMinuteCounter[5]++;
          							if(gucMinuteCounter[5] > MINUTE_COUNTER)
          							{
          								gucMinuteCounter[5] = 0;
          								if(gucTimerLeft[5] > 0)
          								{
          									gucTimerLeft[5]--;
          								 
          							 
          								}
          								else
          								{
          
          								 
          										gucTimerLeft[5] = 0;
          										gucOverOutput[5] = 0;
          										stop_timer(INPUT6_TIMER);
          										break;
          								 
          								}
          								
          							}
          							start_timer(INPUT6_TIMER,DEFAULT_TIMEOUT);
          
          						break;
			 			case INPUT7_TIMER: watchdog(); 
            							gucMinuteCounter[6]++;
            							if(gucMinuteCounter[6] > MINUTE_COUNTER)
            							{
            								gucMinuteCounter[6] = 0;
            								if(gucTimerLeft[6] > 0)
            								{
            									gucTimerLeft[6]--;
            								 
            							 
            								}
            								else
            								{
            
            									 
            										gucTimerLeft[6] = 0;
            										gucOverOutput[6] = 0;
            										stop_timer(INPUT7_TIMER);
            										break;
            								 
            								}
            								
            							}
            							start_timer(INPUT7_TIMER,DEFAULT_TIMEOUT);
            
            						break;
			 			case INPUT8_TIMER: watchdog(); 
              							gucMinuteCounter[7]++;
               
              							if(gucMinuteCounter[7] > MINUTE_COUNTER)
              							{
              								gucMinuteCounter[7] = 0;
               
              								if(gucTimerLeft[7] > 0)
              								{
              									gucTimerLeft[7]--;
              								 
              							 
              								}
              								else
              								{
              
              								 
              										gucTimerLeft[7] = 0;
              										gucOverOutput[7] = 0;
              										stop_timer(INPUT8_TIMER);
              										break;
              								 
              								}
              								
              							}
              							start_timer(INPUT8_TIMER,DEFAULT_TIMEOUT);
              
              						break;

               			 case INPUT9_TIMER: watchdog(); 
              							gucMinuteCounter[8]++;
               
              							if(gucMinuteCounter[8] > MINUTE_COUNTER)
              							{
              								gucMinuteCounter[8] = 0;
               
              								if(gucTimerLeft[8] > 0)
              								{
              									gucTimerLeft[8]--;
              								 
              							 
              								}
              								else
              								{
              
              								 
              										gucTimerLeft[8] = 0;
              										gucOverOutput[8] = 0;
              										stop_timer(INPUT9_TIMER);
              										break;
              								 
              								}
              								
              							}
              							start_timer(INPUT9_TIMER,DEFAULT_TIMEOUT);
              
              						break;
                 		case INPUT10_TIMER:watchdog(); 
              							gucMinuteCounter[9]++;
               
              							if(gucMinuteCounter[9] > MINUTE_COUNTER)
              							{
              								gucMinuteCounter[9] = 0;
               
              								if(gucTimerLeft[9] > 0)
              								{
              									gucTimerLeft[9]--;
              								 
              							 
              								}
              								else
              								{
                              		gucTimerLeft[9] = 0;
              										gucOverOutput[9] = 0;
              										stop_timer(INPUT10_TIMER);
              										break;
              								 
              								}
              								
              							}
              							start_timer(INPUT10_TIMER,DEFAULT_TIMEOUT);
              
              						break;
              
              default :  	break;                                     
         }  
      }
    }
#endif
}                                     


#define  SW_OFF  0
#define  SW_HAND 1
#define  SW_AUTO 2



void check_switches(void)
{
  unsigned char Hand_state_buffer = 0;
  unsigned char Auto_state_buffer = 0; 
  unsigned char Result1,Result2;
  unsigned char Loop; 
  char SFRPAGE_SAVE = SFRPAGE;
  SFRPAGE = CONFIG_PAGE;
           
  //P5 = 0x0f;
  KEYPAD_HAND = 1; 
  KEYPAD_AUTO = 0;
  delay_us(10);
  Hand_state_buffer = P5;

  //P5 = 0xf0;
  KEYPAD_HAND = 0;
  KEYPAD_AUTO = 1;  
  delay_us(10);
  Auto_state_buffer = P5;
  
  for(Loop = 0; Loop < 8;Loop++)
  {
	  Result1 = Hand_state_buffer & (BIT0 << Loop);
	  Result2 = Auto_state_buffer & (BIT0 << Loop);
	//if(Result1 == Result2)
      if((Result1 == (BIT0 << Loop)) && (Result2 == (BIT0 << Loop)))
      {
            Switch_state_buffer[Loop] = SW_OFF;
      }
      else if(Result1 == (BIT0 << Loop)) // from 1 to 0
        {
        Switch_state_buffer[Loop] = SW_HAND;
        }
        else if(Result2 == (BIT0 << Loop)) // from 0 to 1 
        {
        Switch_state_buffer[Loop] = SW_AUTO;
        }
  } 

  SFRPAGE = SFRPAGE_SAVE;
}

void check_switches1(void)
{
  unsigned char Hand_state_buffer = 0;
  unsigned char Auto_state_buffer = 0; 
  unsigned char Result1,Result2;
  unsigned char Loop; 
  char SFRPAGE_SAVE = SFRPAGE;
  SFRPAGE = CONFIG_PAGE;
           
  //P5 = 0x0f;
  KEYPAD_HAND = 1; 
  KEYPAD_AUTO = 0;
  delay_us(10);
  Hand_state_buffer = P6;

  //P5 = 0xf0;
  KEYPAD_HAND = 0;
  KEYPAD_AUTO = 1;  
  delay_us(10);
  Auto_state_buffer = P6;
  for(Loop = 0; Loop < 4;Loop++)
  {
			Result1 = Hand_state_buffer & (BIT0 << Loop);
			Result2 = Auto_state_buffer & (BIT0 << Loop);
			if(Result1 == Result2) 
      {
        Switch_state_buffer[Loop + 8] = SW_OFF;
      }
			else if(Result1 == (BIT0 << Loop)) // from 1 to 0
			{
				Switch_state_buffer[Loop + 8] = SW_HAND;
			}
			else if(Result2 == (BIT0 << Loop))   // from 0 to 1
			{
				Switch_state_buffer[Loop + 8] = SW_AUTO;
			}
  }  

  SFRPAGE = SFRPAGE_SAVE;
}

void push_event(et_event event_type)
{

	if( event_count	< EVENT_QUEUE_SIZE )  //Maximum	of three events	QUEUE'd
	{
		event_queue[event_count++] = event_type;
	}
}

void ADC0_ISR(void) interrupt 15
{
   static unsigned char itemp;
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   unsigned int xdata guiBuffer_new;
   SFRPAGE = ADC0_PAGE;
   #if 1
   switch(AD_Enable)
     {
          case AI0:  //if((channel_type & 0x01) !=1)
                     if((channel_type & 0x200) !=0x200)
                  {       
                      Adc_data[9] =  ADC0 ;
                      guiBuffer[9][itemp] = ADC0;
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0;
                        
                        AD_Enable = AI1;
                        AMX0SL = 0x01;
                      }
                      break;
                   }
                   else
                   {    
                        guiBuffer_new = ADC0;
                        Adc_data[9] =ADC0;
                       if((guiBuffer_old[9] != guiBuffer_new) &&(int)(guiBuffer_old[9] - guiBuffer_new )>800 )
                        {
                           
                             pulse.word[9]++;
                           
                        }
                        guiBuffer_old[9] = guiBuffer_new;


                       AD_Enable = AI1;
                       AMX0SL = 0x01;                       
                       break;
                   }
               
      case AI1: 
                //if((channel_type & 0x02 )!= 0x02)
                  if((channel_type & 0x100) !=0x100)
                  {
                      Adc_data[8] =ADC0;
                      guiBuffer[8][itemp] = ADC0;
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0; 
                        AD_Enable = AI2;
                        AMX0SL = 0x02;
                      }
                      break;
                  }
                  else
                  {     Adc_data[8] =ADC0;
                        guiBuffer_new = ADC0;
                          if((guiBuffer_old[8] != guiBuffer_new)&&(int)(guiBuffer_old[8] - guiBuffer_new )>800)
                        {
                           
                             pulse.word[8]++;
                           
                        }
                        guiBuffer_old[8] = guiBuffer_new;
                        AD_Enable = AI2;
                        AMX0SL = 0x02;    
                    break;
                  }
        case AI2:  
               // if((channel_type & 0x04) !=0x04)
                   if((channel_type & 0x80) !=0x80)
                   {
                   Adc_data[7] =ADC0;
                  guiBuffer[7][itemp] = ADC0;
                  itemp++;
                  if(itemp >= SAMPLENUMBER)
                  {
                    itemp = 0;
                    
                    AD_Enable = AI3;
                    AMX0SL = 0x03;
                  }
                  break;
                  }
                  else
                  {     Adc_data[7] =ADC0;
                        guiBuffer_new = ADC0;                         
                        if((guiBuffer_old[7] != guiBuffer_new) &&(int)(guiBuffer_old[7] - guiBuffer_new )>800 )
                        {
                           
                             pulse.word[7]++;
                           
                        }
                        guiBuffer_old[7] = guiBuffer_new;
                    AD_Enable = AI3; 
                    AMX0SL = 0x03;
                    break;
                  }
       
          case AI3: 
                 //if((channel_type & 0x08) != 0x08)
                 if((channel_type & 0x40) != 0x40)
                  {
                      Adc_data[6] =ADC0;
                      guiBuffer[6][itemp] = ADC0;
                      pulse.word[6] = 0;
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0;
                        
                        AD_Enable = AI4; 
                        AMX0SL = 0x04;
                      }
                      break;
                   }
                   else
                   {    Adc_data[6] =ADC0;
                        guiBuffer_new = ADC0;
                        
                          if((guiBuffer_old[6] != guiBuffer_new) &&(int)(guiBuffer_old[6] - guiBuffer_new )>800)
                        {
                           
                             pulse.word[6]++;
                           
                        }
                        guiBuffer_old[6] = guiBuffer_new;
                       
                    AD_Enable = AI4; 
                    AMX0SL = 0x04;
                    break;
                   }  
        case AI4: //if((channel_type & 0x10) !=0x10)
                   if((channel_type & 0x20) !=0x20)
                  {
                      Adc_data[5] =ADC0;
                     guiBuffer[5][itemp] = ADC0;                      
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0;
                        
                        AD_Enable = AI5; 
                        AMX0SL = 0x05;
                      }
                      break;
                   }
                   else
                   {    Adc_data[5] =ADC0;
                        guiBuffer_new = ADC0;
                       if((guiBuffer_old[5] != guiBuffer_new) &&(int)(guiBuffer_old[5] - guiBuffer_new )>800 )
                        {
                           
                             pulse.word[5]++;
                        }
                        guiBuffer_old[5] = guiBuffer_new;
                        
                    AD_Enable = AI5;  
                    AMX0SL = 0x05;
                    break; 
                   }
        case AI5: if((channel_type & 0x10) !=0x10)
                {
                      Adc_data[4] =ADC0;
                      guiBuffer[4][itemp] = ADC0;
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0;
                        
                        AD_Enable = AI6; 
                        AMX0SL = 0x06;
                      }
                      break;
                 }
                 else
                 {
                        Adc_data[4] =ADC0;
                        guiBuffer_new = ADC0;
                        
                          if((guiBuffer_old[4] != guiBuffer_new) &&(int)(guiBuffer_old[4] - guiBuffer_new )>800 )
                        {
                          
                             pulse.word[4]++;
                           
                        }
                        guiBuffer_old[4] = guiBuffer_new;
                       
                    AD_Enable = AI6; 
                    AMX0SL = 0x06;
                    break;
                 }
        case AI6: //if((channel_type & 0x40) !=0x40)
                  if((channel_type & 0x08) !=0x08)
                {
                      Adc_data[3] =ADC0;
                      guiBuffer[3][itemp] = ADC0;
                      itemp++;
                      if(itemp >= SAMPLENUMBER)
                      {
                        itemp = 0;
                        
                        AD_Enable = AI7; 
                        AMX0SL = 0x07;
                      }
                      break;
                 }
                 else
                 {
                        Adc_data[3] =ADC0;
                        guiBuffer_new = ADC0;
                        if((guiBuffer_old[3] != guiBuffer_new)&& (int)(guiBuffer_old[3] - guiBuffer_new )>800 )
                        {
                           
                             pulse.word[3]++;
                           
                        }
                        guiBuffer_old[3] = guiBuffer_new;
                        
                    AD_Enable = AI7;
                    AMX0SL = 0x07;
                    break;
                 }
        case AI7: //if((channel_type & 0x80) !=0x80)
                    if((channel_type & 0x04) !=0x04)
                {
                  Adc_data[2] =ADC0;
                  guiBuffer[2][itemp] = ADC0;
                  itemp++;
                  if(itemp >= SAMPLENUMBER)
                  {
                    itemp = 0;
                    
                    AD_Enable = AI0;
                    AMX0SL = 0x00;
                  }
                  break;
                 }
                 else
                 {      Adc_data[2] =ADC0;
                        guiBuffer_new = ADC0;
                        
                          if((guiBuffer_old[2] != guiBuffer_new) &&(int)(guiBuffer_old[2] - guiBuffer_new )>800)
                        {
                           
                             pulse.word[2]++;
                           
                        }
                      guiBuffer_old[2] = guiBuffer_new;      
                      
                    AD_Enable = AI0;
                    AMX0SL = 0x00;
                    break; 
                 } 
                
         default :break;
     }    
   
  #endif
   
   AD0INT = 0;                         // Clear ADC conversion complete 
   //SFRPAGE = TMR3_PAGE;
   //TMR3L = 0xDA;                  // set to reload immediately
    //TMR3H = 0XFD; 
  // SFRPAGE = SFRPAGE_SAVE; 
     SFRPAGE = TMR2_PAGE;
     //TMR2L = 0x4D;                  // set to reload immediately
    // TMR2H = 0XDC;
    RCAP2L = 0X66;                  // Init reload values
  	RCAP2H = 0XEA;
        TMR2L = 0x66;                  // set to reload immediately
  	TMR2H = 0XEA; 

     SFRPAGE = SFRPAGE_SAVE; 
}


void ADC2_ISR(void) interrupt 18
{
   static unsigned char itemp2;
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page 
   unsigned int xdata guiBuffer_new;
   SFRPAGE = ADC2_PAGE;
      
      switch(AD_Enable2)
      {
        case AI8:  //if((channel_type & 0x100) !=0x100)
                   if((channel_type & 0x02 )!= 0x02)
                  {
                    Adc_data[1] =ADC2*16;  
                    guiBuffer[1][itemp2] =ADC2*16;
                      //  guiBuffer[9][itemp2] =ADC2;
                      itemp2++;
                      if(itemp2 >= SAMPLENUMBER)
                      {
                        itemp2 = 0;
                        AMX2SL = 0x07;
                        AD_Enable2 = AI9;
                      }
                      break;
                   }
                   else
                   { 
                     Adc_data[1] =ADC2*16;
                     guiBuffer_new =ADC2 *16;
                    //if( (guiBuffer_new + guiBuffer_old[9])>200)
                    if((guiBuffer_old[1] != guiBuffer_new) &&(int)(guiBuffer_old[1] -guiBuffer_new )>800)
                    {
                       //guiBuffer_old[9] = guiBuffer_new;
                       //if(guiBuffer_old[9] < 1)
                       //{
                         pulse.word[1]++;
                       //}
                    }
                    guiBuffer_old[1] =guiBuffer_new ;
                    AD_Enable2 = AI9;
                    AMX2SL = 0x07;
                    break;
                   }
                   
        case AI9: //if((channel_type & 0x200) !=0x200)
                   if((channel_type & 0x01 )!= 0x01)
                   {  Adc_data[0] =ADC2*16;
                      guiBuffer[0][itemp2] =ADC2*16;
                      //guiBuffer[8][itemp2] =ADC2;
                      itemp2++;
                      if(itemp2 >= SAMPLENUMBER)
                      {
                        itemp2 = 0;
                        AMX2SL = 0x06;
                        AD_Enable2 = AI8;
                      }
                      break;
                    }
                    else
                    {
                       
                     guiBuffer_new =ADC2 *16;
                     Adc_data[0] =ADC2*16;
                    //if( (guiBuffer_new + guiBuffer_old[9])>200)

                    if((guiBuffer_old[0] != guiBuffer_new) &&(int)(guiBuffer_old[0] -guiBuffer_new )>800)

                    {

                       //guiBuffer_old[9] = guiBuffer_new;

                       //if(guiBuffer_old[9] < 1)

                       //{

                         pulse.word[0]++;

                       //}

                    }
                    guiBuffer_old[0] =guiBuffer_new ;
                    AD_Enable2 = AI8;
                    AMX2SL = 0x06;                    
                    break;
                    }
         default :break;
        } 
     

 AD2INT = 0;                         // Clear ADC conversion complete 
   //SFRPAGE = TMR3_PAGE;
   //TMR3L = 0x92;  // Set time 50 US
   //TMR3H = 0xFF; 
 // SFRPAGE = SFRPAGE_SAVE; 
       
    //TMR2L = 0xDA;                  // set to reload immediately
    //TMR2H = 0XFD; 
    //SFRPAGE = TMR2_PAGE;    
   // TMR2L = 0x60;                  // set to reload immediately
    //TMR2H = 0Xea;
   SFRPAGE = SFRPAGE_SAVE;
}


unsigned int filter_reading(unsigned int Buff[])
{
   unsigned char i;
   unsigned int xdata Temp = 0;
   unsigned int xdata Sum = 0;
   //unsigned int xdata uMin,uMax;
   for(i = 0;i<SAMPLENUMBER;i++)
   {
      Sum += Buff[i];
   }
   Sum /= (SAMPLENUMBER);

   return(Sum);
}


void refresh_inputs (void)
{
   unsigned char i;
   unsigned int xdata temp;
   for(i = 0; i < 10; i++)
	 {
        perious_input[i] = analog_in_buffer[i] ;
        temp = filter_reading(guiBuffer[i]);
        //analog_in_buffer[i] = temp;
		analog_in_buffer[i] = Filter(i,temp)  ;
        guiAnalogInput[i] = RangeConverter(range[i],(analog_in_buffer[i]>>2),128,i);
     } 
 
 }
     

unsigned int Filter(unsigned char channel,unsigned int input)
{
	// -------------FILTERING------------------
	signed int xdata siDelta = 0;
	unsigned int xdata uiResult = 0;
	unsigned char xdata i = 0;
  	unsigned int xdata uiTemp = 0;
	i = channel;
	uiTemp = input;  
	siDelta = uiTemp - analog_in_buffer[i];    //compare new reading and old reading

	// If the difference in new reading and old reading is greater than 5 degrees, implement rough filtering.
  if (( siDelta >= 100 ) || ( siDelta <= -100)) // deg f
	  	analog_in_buffer[i] = analog_in_buffer[i] + (siDelta >> 1);//1 
 			
	// Otherwise, implement fine filtering.
	else
	{			
	  analog_in_buffer[i] =   filter[i]*analog_in_buffer[i];
		analog_in_buffer[i] += (unsigned long)uiTemp;
	 	analog_in_buffer[i] = (unsigned int)(analog_in_buffer[i]/(filter[i]+1));			 	 
	}
	uiResult = analog_in_buffer[i]; 
 

	return uiResult;	
}

/* 			LED ARRAY 
		Led_Group1		Led_Group2		Led_Group3
P0.0		AI1				AO1				      AI10
P0.1		AI2				AO2				      BEAT
P0.2		AI3				AO3				      DO6
P0.3		AI4			  AO4				      DO7
P0.4		AI5				DO1				      DO8
P0.5		AI6				DO2				      TX1
P0.6		AI7				DO3				      RX1
P0.7		AI8				DO4				      TX2 
P1.7		AI9				DO5				      RX2
Led_Group1 contorl by LEDDRIVE1
Led_Group2 contorl by LEDDRIVE2
Led_Group3 contorl by LEDDRIVE3
*/

void refresh_LEDs(void)
{
  switch (LED_bank)
	{
     case 1:    
                LED_DRIVE2 = 1;
        		LED_DRIVE3 = 1;
                //delay_us(2);
                
                LED8= LED_State[0]&BIT8;
                LED_DRIVE1 = 0;
                //LED_State[0]=0;
                P3 = (char)LED_State[0];
                
                break;
    case 2:     LED_DRIVE1 = 1;
                LED_DRIVE3 = 1; 
                //delay_us(2);
                
                
                LED8 = LED_State[1]&BIT8;
                LED_DRIVE2 = 0; 
                P3 = (char)LED_State[1];
               
                break;
     
     case 3:    LED_DRIVE1 = 1;
        		LED_DRIVE2 = 1;
                //delay_us(2);
               
                LED8 = LED_State[2]&BIT8;
                 LED_DRIVE3 = 0;
                P3 = (char)LED_State[2];
               
                break; 
     default:   break;
   }

}

void tabulate_LED_STATE(void)    //add modbus data;
{
    //unsigned char by_loop;
    //bit LED_flag;
    char SFRPAGE_SAVE = SFRPAGE;
    SFRPAGE = CONFIG_PAGE;
  	ET0 =0;
    //-------for the first line-------------------
    if(Adc_data[0] > 2048)  
        LED_State[0] = LED_State[0] >> 1;
         //LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
    
    if(Adc_data[1] > 2048)  
      LED_State[0] = LED_State[0] >> 1;
      //LED_State[0] = (LED_State[0] >> 1) | 0x100;
    else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
   
   if(Adc_data[2] > 2048)  
        LED_State[0] = LED_State[0] >> 1;
      // LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
  if(Adc_data[3] > 2048)  
        LED_State[0] = LED_State[0] >> 1;
       //LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
  
    if(Adc_data[4] > 2048)  
        LED_State[0] = LED_State[0] >> 1;
       //LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100; 
   if(Adc_data[5] > 2048)  
       LED_State[0] = LED_State[0] >> 1;
      // LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
  if(Adc_data[6] > 2048)  
     LED_State[0] = LED_State[0] >> 1;
   // LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
       // LED_State[0] = (LED_State[0] >> 1) | 0x100;
        LED_State[0] = (LED_State[0] >> 1)| 0x100;
  if(Adc_data[7] > 2048)  
       LED_State[0] = LED_State[0] >> 1;
      // LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
  if(Adc_data[8] > 2048)  
      LED_State[0] = LED_State[0] >> 1;
       //LED_State[0] = (LED_State[0] >> 1) | 0x100;
      else
        LED_State[0] = (LED_State[0] >> 1) | 0x100;
   

 
    //----------for the second line---------------
   // for(by_loop = 0; by_loop < 4;by_loop++)
   // {
      if(Switch_state_buffer[8] == 0)
        {
         LED_State[1] = (LED_State[1] >> 1) | 0x100;
	       modbus_data[8] = 0;
         }
      else if(Switch_state_buffer[8] == 1)
         {
         LED_State[1] = LED_State[1] >> 1;
	       modbus_data[8] = 4095;
         }
       else if(Switch_state_buffer[8] == 2)
      {
         if(modbus_data[8] > 2048)
           LED_State[1] = LED_State[1] >> 1;	
         else
           LED_State[1] = (LED_State[1] >> 1) | 0x100;	
      }



         if(Switch_state_buffer[9] == 0)
        {
         LED_State[1] = (LED_State[1] >> 1) | 0x100;
	       modbus_data[9] = 0;
         }
      else if(Switch_state_buffer[9] == 1)
         {
         LED_State[1] = LED_State[1] >> 1;
	       modbus_data[9] = 4095;
         }
       else if(Switch_state_buffer[9] == 2)
      {
         if(modbus_data[9] > 2048)
           LED_State[1] = LED_State[1] >> 1;	
         else
           LED_State[1] = (LED_State[1] >> 1) | 0x100;	
      }


         if(Switch_state_buffer[10] == 0)
        {
         LED_State[1] = (LED_State[1] >> 1) | 0x100;
	       modbus_data[10] = 0;
         }
      else if(Switch_state_buffer[10] == 1)
         {
         LED_State[1] = LED_State[1] >> 1;
	       modbus_data[10] = 4095;
         }
       else if(Switch_state_buffer[10] == 2)
      {
         if(modbus_data[10] > 2048)
           LED_State[1] = LED_State[1] >> 1;	
         else
           LED_State[1] = (LED_State[1] >> 1) | 0x100;	
      }


         if(Switch_state_buffer[11] == 0)
        {
         LED_State[1] = (LED_State[1] >> 1) | 0x100;
	       modbus_data[11] = 0;
         }
      else if(Switch_state_buffer[11] == 1)
         {
         LED_State[1] = LED_State[1] >> 1;
	       modbus_data[11] = 4095;
         }
       else if(Switch_state_buffer[11] == 2)
      {
         if(modbus_data[11] > 2048)
           LED_State[1] = LED_State[1] >> 1;	
         else
           LED_State[1] = (LED_State[1] >> 1) | 0x100;	
      }
    //}

    /*for(by_loop = 0; by_loop < 5;by_loop++)
    {
      if(Switch_state_buffer[by_loop] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY0 = 0;
      }
      else if(Switch_state_buffer[by_loop] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY0 = 1;  
      }
      else if(Switch_state_buffer[by_loop] == 2)
      {
         if(modbus_data[by_loop] > 512)
          {
            LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY0 = 1;
          }
         else
         {
           LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY0 = 0;                           
         }                                        
      }            
    }*/  
    if(Switch_state_buffer[0] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY7 = 0;
      }
      else if(Switch_state_buffer[0] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY7 = 1;  
      }
      else if(Switch_state_buffer[0] == 2)
      {  //if(rang_flag== 0)
         //{
         if(modbus_data[0] > 0)
          {
           // LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY7 = 1;
          }
         else
         {
           //LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY7 = 0;                           
         } 
         //}                              
         
          LightOutput(7);   //light control
          if(RELAY7 == 1) LED_State[1] = LED_State[1] >> 1;
          else LED_State[1] = (LED_State[1] >> 1) | 0x100;
       }            
      
      if(Switch_state_buffer[1] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY6 = 0;
      }
      else if(Switch_state_buffer[1] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY6 = 1;  
      }
      else if(Switch_state_buffer[1] == 2)
      {
        //if(rang_flag== 0)
       // {
        if(modbus_data[1] >0)
          {
            //LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY6 = 1;
          }
         else
         {
           //LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY6 =0;                           
         }                                        
        //}
         LightOutput(6); 
          if(RELAY6 == 1) LED_State[1] = LED_State[1] >> 1;
          else LED_State[1] = (LED_State[1] >> 1) | 0x100;
      }            
      
      if(Switch_state_buffer[2] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY5 = 0;
      }
      else if(Switch_state_buffer[2] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY5 = 1;  
      }
      else if(Switch_state_buffer[2] == 2)
      {
        // if(rang_flag== 0)
        // {
         if(modbus_data[2] > 0)
          {
            //LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY5 = 1;
          }
         else
         {
           //LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY5 = 0;                           
         } 
        // }                              
         LightOutput(5);
           if(RELAY5 == 1) LED_State[1] = LED_State[1] >> 1;
          else LED_State[1] = (LED_State[1] >> 1) | 0x100;
      }            
      
      if(Switch_state_buffer[3] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY4 = 0;
      }
      else if(Switch_state_buffer[3] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY4 = 1;  
      }
      else if(Switch_state_buffer[3] == 2)
      {  
        // if(rang_flag== 0)
        // {
         if(modbus_data[3] > 0)
          {
            //LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY4 = 1;
          }
         else
         {
           //LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY4 = 0;                           
         } 
        // }                              
        LightOutput(4);
          if(RELAY4 == 1) LED_State[1] = LED_State[1] >> 1;
          else LED_State[1] = (LED_State[1] >> 1) | 0x100;

      }            
      
      if(Switch_state_buffer[4] == 0)
      {
        LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
        RELAY3 = 0;
      }
      else if(Switch_state_buffer[4] == 1)
      {
        LED_State[1] = LED_State[1] >> 1;	// relay is ON
        RELAY3 = 1;  
      }
      else if(Switch_state_buffer[4] == 2)
      {
        // if(rang_flag== 0)
        // {
         if(modbus_data[4] > 0)
          {
           // LED_State[1] = LED_State[1] >> 1;	// relay is ON
            RELAY3 = 1;
          }
         else
         {
           //LED_State[1] = (LED_State[1] >> 1) | 0x100;	// relay is OFF
           RELAY3 = 0;                           
         } 
       // }                              
        LightOutput(3);
          if(RELAY3 == 1) LED_State[1] = LED_State[1] >> 1;
          else LED_State[1] = (LED_State[1] >> 1) | 0x100;
      }            
      
   
    //------------------------for the third line----------------------

    if(Adc_data[9] > 2048)  
        LED_State[2] = LED_State[2] >> 1;
      else
        LED_State[2] = (LED_State[2] >> 1) | 0x100;
    
    if(D16_Flash)
      LED_State[2] = LED_State[2] >> 1; //for Light D16
    else
      LED_State[2] = (LED_State[2] >> 1) | 0x100;
    /*for(by_loop = 0; by_loop < 3;by_loop++)
    {
      if(Switch_state_buffer[5+by_loop] == 0)
      {
        LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
        RELAY0 = 0;
      }
      else if(Switch_state_buffer[5+by_loop] == 1)
      {
        LED_State[2] = LED_State[2] >> 1;	// relay is ON
        RELAY0 = 1;  
      }
      else if(Switch_state_buffer[5+by_loop] == 2)
      {
         if(modbus_data[5+by_loop] > 512)
          {
            LED_State[2] = LED_State[2] >> 1;	// relay is ON
            RELAY0 = 1;
          }
         else
         {
           LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
           RELAY0 = 0;                           
         }                                        
      }
    }*/
    if(Switch_state_buffer[5] == 0)
      {
        LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
        RELAY2 = 0;
      }
      else if(Switch_state_buffer[5] == 1)
      {
        LED_State[2] = LED_State[2] >> 1;	// relay is ON
        RELAY2 = 1;  
      }
      else if(Switch_state_buffer[5] == 2)
      {  
         //if(rang_flag== 0)
        // {
         if(modbus_data[5] > 0)
          {
           // LED_State[2] = LED_State[2] >> 1;	// relay is ON
            RELAY2 = 1;
          }
         else
         {
           //LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
           RELAY2 = 0;                           
         } 
       // }                              
        LightOutput(2);
          if(RELAY2 == 1) LED_State[2] = LED_State[2] >> 1;
          else LED_State[2] = (LED_State[2] >> 1) | 0x100;
       }

      if(Switch_state_buffer[6] == 0)
      {
        LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
        RELAY1 = 0;
      }
      else if(Switch_state_buffer[6] == 1)
      {
        LED_State[2] = LED_State[2] >> 1;	// relay is ON
        RELAY1 = 1;  
      }
      else if(Switch_state_buffer[6] == 2)
      {  
         //if(rang_flag== 0)
         //{
         if(modbus_data[6] > 0)
          {
           // LED_State[2] = LED_State[2] >> 1;	// relay is ON
            RELAY1 = 1;
          }
         else
         {
          // LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
           RELAY1 = 0;                           
         }                                        
         //}
       LightOutput(1);
           if(RELAY1 == 1) LED_State[2] = LED_State[2] >> 1;
          else LED_State[2] = (LED_State[2] >> 1) | 0x100;
       
      }

      if(Switch_state_buffer[7] == 0)
      {
        LED_State[2] = (LED_State[2] >> 1) | 0x100;	// relay is OFF
        RELAY0 = 0;
      }
      else if(Switch_state_buffer[7] == 1)
      {
        LED_State[2] = LED_State[2] >> 1;	// relay is ON
        RELAY0 = 1;  
      }
      else if(Switch_state_buffer[7] == 2)
      {
        
         if(modbus_data[7] > 0)
          {
            
            RELAY0 = 1;
          }
         else
         {
           
           RELAY0 = 0;                           
         } 
                                      
           LightOutput(0);
          if(RELAY0 == 1) LED_State[2] = LED_State[2] >> 1;
          else LED_State[2] = (LED_State[2] >> 1) | 0x100;
          
       }
     RELAY_STATUS=  ((unsigned char)RELAY7<<7)+((unsigned char)RELAY6<<6)+((unsigned char)RELAY5<<5)+((unsigned char)RELAY4<<4)+((unsigned char)RELAY3<<3)+((unsigned char)RELAY2<<2)+((unsigned char)RELAY1<<1)+((unsigned char)RELAY0);
    if(com_LED_count>0 ) 
    {
      com_LED_count--;
      LED_State[2]= LED_State[2] >> 1;
      
      
    }
    else
      {
       LED_State[2] = (LED_State[2] >> 1) | 0x100; 
      }
    
    if(com_LED_count1>0 ) 
    {
      com_LED_count1--;
      LED_State[2]= LED_State[2] >> 1;
      
    }
      else
      {
        LED_State[2] = (LED_State[2] >> 1) | 0x100;
      }
     
      
    LED_State[2] = (LED_State[2] >> 1) | 0x100; 
    LED_State[2] = (LED_State[2] >> 1) | 0x100;
    ET0 = 1;
    SFRPAGE = SFRPAGE_SAVE;
} 


void Main_Get_Switch_Status(void)
{
  char i;
  for(i = 0;i < 3;i++)
  {    
     Swtich_state[i] = (Switch_state_buffer[i*4+3]<<6) + (Switch_state_buffer[i*4+2]<<4) + (Switch_state_buffer[i*4+1]<<2) + Switch_state_buffer[i*4]; 
  }  
} 

void StorePulseToFlash( void )
{
    temp_storepulse++;
    if(temp_storepulse==10) temp_storepulse = 0;
	flash_write_int(FLASH_START_PULSE + temp_storepulse*2,pulse.half[temp_storepulse*2],FLASH_MEMORY);
    flash_write_int(FLASH_START_PULSE + temp_storepulse*2+1,pulse.half[temp_storepulse*2+1],FLASH_MEMORY);
    
}




