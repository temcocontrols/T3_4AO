#include "c8051f120.h"
#include "LibIO_T3IO.h"
#include "define.h"
#include "serial.h"
#include "stdlib.h"
//extern unsigned int xdata temp[8]; 
unsigned char xdata Default_Setting = 0 ;
unsigned char xdata com_beat = 1 ;
extern unsigned char xdata filter[10];
unsigned char xdata Swtich_state[3];
signed int RangeConverter(unsigned char function, signed int para,signed int cal,unsigned char i);
unsigned char  output_range(_OUTPUT_RANGE *range );
//extern unsigned int xdata flash_data_mod[21];
//unsigned char data temp_base_address;
unsigned char xdata gucDeadmaster;
unsigned int xdata  guiDeadmasterTime;
extern unsigned int xdata  Adc_data[10] ;
//extern unsigned char xdata Input_timer[10];
bit serial_no_activity = 1;
bit switch_to_auto  = 0;
//extern unsigned int xdata guiBuffer_old[10];
unsigned char xdata gucBuffer[21];
unsigned int xdata gucZone[8];
unsigned char xdata gucReverseOutput;
void SetBit(unsigned char bit_number,unsigned int *byte);
void ClearBit(unsigned char bit_number,unsigned int *byte);
bit GetBit(unsigned char bit_number,unsigned int *byte);
extern unsigned int xdata perious_input[10]; 
//extern unsigned int  xdata guiBuffer[10][SAMPLENUMBER];
unsigned char xdata gucCounter[10];
unsigned char xdata gucStartFilter;
extern unsigned char xdata gucPreviousInput[10];
unsigned int xdata FilterTemp[10];
unsigned char xdata gucFilterCounter[8];
extern unsigned int LED_State[MAX_LED_BANK];
#define SYS_UPDATE			0xee10
extern unsigned int Filter(unsigned char channel,unsigned input);
extern bit RELAY0, RELAY1,RELAY2,RELAY3,RELAY4,RELAY5,RELAY6,RELAY7;
extern unsigned char xdata info[20];
//extern unsigned char xdata pic_version;

unsigned char data test_register = 123 ;




void SetBit(unsigned char bit_number,unsigned int *byte)
{
	unsigned int mask; 
	mask = 0x01;
	mask = mask << bit_number;
	*byte = *byte | mask;
}
void ClearBit(unsigned char bit_number,unsigned int *byte)
{
	unsigned int mask; 
	mask = 0x01;
	mask = mask << bit_number;
 	mask = ~mask;
	*byte = *byte & mask;
}
bit GetBit(unsigned char bit_number,unsigned int *byte)
{
	unsigned int mask; 
	mask = 0x01;	 
	mask = mask << bit_number;
	return (bit)(*byte & mask);
} 

void LightOutput(unsigned char overOut)
{
  char SFRPAGE_SAVE = SFRPAGE;
  SFRPAGE = CONFIG_PAGE;
    
	if(overOut == 0 && gucZone[0])
  
	{  // RELAY0=1;RELAY1=1; RELAY2=1; RELAY3=1; RELAY4=1; 
 //tbd: MAKE A LOOP FOR THIS CODE, NOT STRIAGHT LINE CODE
		if(GetBit(0,&guiManual) == 1 && gucOverOutput[gucZone[0]-1] == 1)
    // if(1)
		{   
//		//	if (gucReverseOutput)
//          if(1)
//    		{		
//                RELAY0 = 1;
//                //LED_State[1] = LED_State[1] >> 1;
//             }
//    		else
//    		{		
//                RELAY0 = 0;
//               // LED_State[1] = (LED_State[1] >> 1) | 0x100;
//            }
    			gucStatus[0] = 1;
				RELAY0 = 1;
		}
		else 
			gucStatus[0] = 0;
	}
	else if(overOut == 1 && gucZone[1])
	{
		if(GetBit(1,&guiManual) == 1 && gucOverOutput[gucZone[1]-1] == 1)
    	   {
//    		//if (gucReverseOutput)
//    		 if(1)
//           		{
//                    RELAY1 = 1;
//                    //LED_State[1] = LED_State[1] >> 1;
//                }
//    		else
//    			{
//                    RELAY1 = 0;
//                   // LED_State[1] = (LED_State[1] >> 1) | 0x100;
//    			   
//                 }
				RELAY1 = 1;	
                 gucStatus[1] = 1;
    		}
		else 
			gucStatus[1] = 0;	   
        }
	else if(overOut == 2 && gucZone[2])
	{
		if(GetBit(2,&guiManual) == 1 && gucOverOutput[gucZone[2]-1] == 1)
		{
//		//	if (gucReverseOutput)
//			 if(1)	
//                {
//                RELAY2 = 1;
//               // LED_State[1] = LED_State[1] >> 1;
//                }
//			else
//				{
//                 RELAY2 = 0;
//                // LED_State[1] = (LED_State[1] >> 1) | 0x100;
//                 }
			RELAY2 = 1;
			gucStatus[2] = 1;
		}
		else 
			gucStatus[2] = 0;
	}
	else if(overOut == 3 && gucZone[3])
	{
		if(GetBit(3,&guiManual) == 1 && gucOverOutput[gucZone[3]-1] == 1)
		{
//			//if (gucReverseOutput)
//			 if(1)	
//                {
//                RELAY3 = 1;
//                //LED_State[1] = LED_State[1] >> 1;
//                }
//			else
//				{
//                    RELAY3 = 0;
//                    //LED_State[1] = (LED_State[1] >> 1) | 0x100;
//                 }
			RELAY3 = 1;
			gucStatus[3] = 1;
		}
		else 
			gucStatus[3] = 0;
	}
	else if(overOut == 4 && gucZone[4])
	{
		if(GetBit(4,&guiManual) == 1 && gucOverOutput[gucZone[4]-1] == 1)
		{
//			//if (gucReverseOutput)
//			   if(1)
//        	     {
//                  RELAY4 = 1;
//                   // LED_State[1] = LED_State[1] >> 1;
//                }
//			else
//				{
//                   RELAY4 = 0;
//                  //  LED_State[1] = (LED_State[1] >> 1) | 0x100;
//                }
			RELAY4 = 1;
			gucStatus[4] = 1;
		}
		else 
			gucStatus[4] = 0;
	
	}
	else if(overOut == 5 && gucZone[5])
	{
		if(GetBit(5,&guiManual) == 1 && gucOverOutput[gucZone[5]-1] == 1)
		{
//			//if (gucReverseOutput)
//			   if(1)	
//                {
//                RELAY5 = 1;
//               // LED_State[1] = LED_State[1] >> 1;
//			    }
//           else     
//               {
//				RELAY5 = 0;
//			
//               // LED_State[1] = (LED_State[1] >> 1) | 0x100;
//                }
				RELAY5 = 1;
                gucStatus[5] = 1;
		}
		else 
			gucStatus[5] = 0;
      
	}
	else if(overOut == 6 && gucZone[6])
	{
		if(GetBit(6,&guiManual) == 1 && gucOverOutput[gucZone[6]-1] == 1)
		{
//			//if (gucReverseOutput)
//			  if(1)
//            {
//        	    RELAY6 = 1;
//                //LED_State[1] = LED_State[1] >> 1;
//             }
//			else
//				{
//                    RELAY6 = 0;
//                    //LED_State[1] = (LED_State[1] >> 1) | 0x100;
//		        }
				RELAY6 = 1;
               	gucStatus[6] = 1;
		}
		else 
			gucStatus[6] = 0;
	}
	else if(overOut == 7 && gucZone[7])
	{
		if(GetBit(7,&guiManual) == 1 && gucOverOutput[gucZone[7]-1] == 1)
		{
//			//if (gucReverseOutput)
//			  if(1)
//            {	
//                RELAY7 = 1;
//               // LED_State[1] = LED_State[1] >> 1;
//	         }
//			else
//			{
//        	    RELAY7 = 0;
//                //LED_State[1] = (LED_State[1] >> 1) | 0x100;
//             }
			RELAY7 = 1;
			gucStatus[7] = 1;
		}
		else 
			gucStatus[7] = 0;
	}
  SFRPAGE = SFRPAGE_SAVE;
}



// *****************************************************************************************************
// data read from pic and now stored into registers
// analog_input_buffer()
// refresh_inputs()
// *****************************************************************************************************



//---------------------initserial ------------------------------------
//	init serial port .
void initSerial(void)
{
//	unsigned char address_retrieve_flag;

	RS232STATE = RECEIVE;
	rece_count = 0;
	DealwithTag = 0; 
  watchdog();
	// check if during ISP mode if the address has been changed

  
	// if it has not been changed, check the flash memory
  /*  flash_read_char(FLASH_ADDRESS, &laddress, FLASH_MEMORY);
	if( (laddress == 255) || (laddress == 0) )
	{
		
		{
			laddress = 254;
			flash_write_int(FLASH_ADDRESS, laddress, FLASH_MEMORY); 
		}  
 		 
	}
	else
    {
		flash_write_int(FLASH_ADDRESS, laddress, FLASH_MEMORY);
	 
	}  */

	// if data is blank, means first time programming, thus put as default
	// Added by RL 02/11/04
	//if(laddress == 0 || laddress == 255 ) 
		//laddress = 254;

}


//-------------serial_restart -------------------------------
// restart the serial communications . 
void serial_restart(void)
{
	RS232STATE = RECEIVE;
	rece_count = 0;
	DealwithTag = 0;
}

//-------------initsend_com ---------------------------------
// it is ready to send data by serial port . 
void initSend_COM(void)
{
	transmit_finished = 0;
	RS232STATE = SEND;
    delay_us(1000);
}
#if 1
//------------------------serialport ----------------------------------
//	serial port interrupt , deal with all action about serial port. include receive data and 
//		send data and deal with interal.

void SerialPort() interrupt 4
{
  SFRPAGE = UART0_PAGE;
  watchdog();
	if(RI0 == 1)
	{
		// Make sure that you are not in SEND mode and that you do not exceed
		// the length of the data bufer
	RI0 = 0;
    if(rece_count < DATABUFLEN)
	data_buffer[rece_count++] = SBUF0;
	

		// timeout count adjusted to half a packet length
		serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;

		// need to evaluate the packet size
		// once the sixth byte is received we can then figure out what is packet size
		watchdog();
    
  	 
  		if(rece_count == 1 )
		{			 
			// This starts a timer that will reset communication.  If you do not
			// receive the full packet, it insures that the next receive will be fresh.
			// The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
			packet_size = 8;
			serial_receive_timeout_count = 5  ;
		}
				// need to evaluate the packet size
		// once the sixth byte is received we can then figure out what is packet size
		 if(rece_count == 4)
		{
				//check if it is a scan command
			if((unsigned int)(data_buffer[2]<<8) + data_buffer[3] == 0x0a && data_buffer[1] == WRITE_VARIABLES)
			{
					
					packet_size = 12;
					serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;	
			}
		} 
   		else if(rece_count == 7 )
		{

			if(data_buffer[1] == MULTIPLE_WRITE)
				// take the quantity amount, and add 9 more for other information needed
			{
				packet_size = data_buffer[6] + 9;
				serial_receive_timeout_count = packet_size;
			}
		}	
		else if(rece_count == 3 )
		{
			if(data_buffer[1] == CHECKONLINE)
				packet_size = 6;
		}
		// As soon as you receive the final byte, switch to SEND mode
		else if(rece_count == packet_size)		
		{
			// full packet received - turn off serial timeout
			serial_receive_timeout_count = 0;
			DealwithTag = 2;		// making this number big to increase delay
			// if was dealing with a response, must know it is done
			response_receive_finished = 1;
		}


	}
	else if(TI0 == 1)
	{
		TI0=0;
		transmit_finished = 1;
	}
 	watchdog();
    SFRPAGE = LEGACY_PAGE;
	return;
}
#endif


// ------------------------dealwithdata -------------------------
// the routine dealwith data ,it has three steps.
// the 3 step is : 1 prepare to send data and init crc for next time
//				   2 dealwith interal
//                 3 organize the data of sending, and send data.

void dealwithData(void)
{	
	unsigned int address;	
	// given this is used in multiple places, decided to put it as an argument
	address = (unsigned int)(data_buffer[2]<<8) + data_buffer[3];
    watchdog();
	if (checkData(address))
	{		
		// Initialize tranmission
		initSend_COM();	
		// Initialize CRC
		InitCRC16();	
		// Store any data being written
		internalDeal(address);
		// Respond with any data requested
		responseData(address);
		//guiDeadmasterTime = (unsigned int)(gucDeadmaster << 8); by jim
		serial_no_activity = 1;
	}	
 	 watchdog();
	// Restart the serial receive.
	serial_restart();

}


//---------------------checkdata ----------------------
//This function calculates and verifies the checksum
bit checkData(unsigned int address)
{
	static unsigned char xdata rand_read_ten_count = 0 ;
	unsigned int crc_val;
	unsigned char minaddr,maxaddr, variable_delay;
	unsigned char i;
   watchdog();
	// check if packet completely received
	if(rece_count != packet_size)
		return FALSE;

	// check if talking to correct device ID
	if(data_buffer[0] != 255 && data_buffer[0] != info[6] && data_buffer[0] != 0)
		return FALSE;	


	//  --- code to verify what is on the network ---------------------------------------------------
	if( data_buffer[1] == CHECKONLINE)
	{
		crc_val = CRC16(data_buffer,4) ;
		if(crc_val != (data_buffer[4]<<8) + data_buffer[5] )
		{
			return FALSE;
		}
		minaddr = (data_buffer[2] >= data_buffer[3] ) ? data_buffer[3] : data_buffer[2] ;	
		maxaddr = (data_buffer[2] >= data_buffer[3] ) ? data_buffer[2] : data_buffer[3] ;	
		if(info[6] < minaddr || info[6] > maxaddr)
			return FALSE;
		else
		{	// in the TRUE case, we add a random delay such that the Interface can pick up the packets
			srand(heart_beat);
			variable_delay = rand() % 20;
			for ( i=0; i<variable_delay; i++)
				delay_us(100);
	
			return TRUE;
		}

	}
	// ------------------------------------------------------------------------------------------------------



	// check that message is one of the following
	if( (data_buffer[1]!=READ_VARIABLES) && (data_buffer[1]!=WRITE_VARIABLES) && (data_buffer[1]!=MULTIPLE_WRITE) )
		return FALSE;
	// ------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------------------
		
	if(data_buffer[2]*256 + data_buffer[3] ==  FLASH_ADDRESS_PLUG_N_PLAY)
	{
		if(data_buffer[1] == WRITE_VARIABLES)
		{
			if(data_buffer[6] != info[0]) 
			return FALSE;
			if(data_buffer[7] != info[1]) 
			return FALSE;
			if(data_buffer[8] != info[2])  
			return FALSE;
			if(data_buffer[9] != info[3]) 
			return FALSE;
		}
		if (data_buffer[1] == READ_VARIABLES)
		{
			randval = rand() % 10 / 2 ;
		}
		if(randval != RESPONSERANDVALUE)
		{
//mhf:12-29-05 if more than 5 times does not response read register 10,reponse manuly.
			rand_read_ten_count++;
			if(rand_read_ten_count%5 == 0)
			{
				rand_read_ten_count = 0;
				randval = RESPONSERANDVALUE;
				variable_delay = rand() % 10;
				for ( i=0; i<variable_delay; i++)
					delay_us(75);
			}
			else
				return FALSE;
		}
		else
		{		
			// in the TRUE case, we add a random delay such that the Interface can pick up the packets
			rand_read_ten_count = 0;
			variable_delay = rand() % 10;
			for ( i=0; i<variable_delay; i++)
				delay_us(75);				
		}
		
	}

	// if trying to write the Serial number, first check to see if it has been already written
	// note this does not take count of multiple-write, thus if try to write into those reg with multiple-write, command will accept
	if( (data_buffer[1]==WRITE_VARIABLES)  && (address<=FLASH_HARDWARE_REV) )
	{
		// Return false if trying to write SN Low word that has already been written
		if(data_buffer[3] < 2)
		{
			if(SNWriteflag & 0x01)                // low byte of SN writed
				return FALSE;
		}
		// Return false if trying to write SN High word that has already been written
		else if (data_buffer[3] < 4)
		{
			if(SNWriteflag & 0x02)                 // high byte of SN writed
				return FALSE;
		}
		else if (data_buffer[3] ==  FLASH_HARDWARE_REV)
		{
			if(SNWriteflag & 0x04)                 // hardware byte writed
				return FALSE;
		}

	}


	crc_val = CRC16(data_buffer,packet_size-2);

	if(crc_val == (data_buffer[packet_size-2]<<8) + data_buffer[packet_size-1] )
		return TRUE;
	else
		return FALSE;

	return TRUE;

 } 


//-------------------responsedata ---------------------------
// the routine organize  data back and send the data to buffer.
void responseData(unsigned int address)

{
	unsigned char num, send_buffer;
	unsigned char i=0;
	bit read_time_pulse = 0;
	// --- variables for calibration ---
	#ifdef CALIBRATION_OPTION
		unsigned char input_select=0;
		unsigned int location;
	#endif
    com_LED_count1 = 1;
    SFRPAGE = UART0_PAGE;

	if(data_buffer[1] == WRITE_VARIABLES)
	{
		for (i=0;i<8;i++)
		{
			SBUF0 = data_buffer[i];
			transmit_finished = 0;
			while (!transmit_finished){}
			watchdog();
		}
	}
	else if(data_buffer[1] == READ_VARIABLES)
	{
	 
		i = 0;
        watchdog();
		num = data_buffer[5];					// number of registers to be read

		send_buffer = data_buffer[0];			// re-send first byte of information
		SBUF0 = send_buffer;
		transmit_finished = 0;
		CRC16_Tstat(send_buffer);

		send_buffer = data_buffer[1];			// re-send second byte of information
		while (!transmit_finished){}
		SBUF0 = send_buffer;
		transmit_finished = 0;
		CRC16_Tstat(send_buffer);

		send_buffer = num<<1;					// re-send third byte of information
		while (!transmit_finished){}
		SBUF0 = send_buffer;
		transmit_finished = 0;
		CRC16_Tstat(send_buffer);
		watchdog();

    for (;i<num;i++)
		{		
		  if(address == SYS_UPDATE)               //SYS_UPDATE = 0xee10
          {
                send_buffer = 0;
                while(!transmit_finished){}
                SBUF0 = send_buffer;
                transmit_finished = 0;
                CRC16_Tstat(send_buffer);
    
                send_buffer = 0;
                while(!transmit_finished){}
                SBUF0 = send_buffer;
                transmit_finished = 0;
                CRC16_Tstat(send_buffer);
          }     
			else if ( i+address < 20 )  // read registers below 100, flash memory
			{				
			//	if( !flash_read_int(address+i, &flash_data, FLASH_MEMORY) )
			//	flash_data = 0;   
			//	else 
				send_buffer = (info[address+i] >> 8) & 0xFF;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

				send_buffer = info[address+i] & 0xFF;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			} 
		    else if( i+address == 22 )
			{
				send_buffer = 0;

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

				send_buffer = com_beat;

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			} 
      else if ( i+address < ORIGINALADDRESSVALUE )
			{	
				//if( !flash_read_int(address+i, &flash_data, FLASH_MEMORY) )
				//	flash_data = 0;

				//flash_data = 0;	
				 send_buffer = 0;
				// --- send first byte -------------
				/*if (i+address >= MAX_FLASH_CONSTRANGE)
					
				else
					send_buffer = (flash_data >> 8) & 0xFF;	*/

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

//				// --- send second byte ------------
//				if (i+address >= MAX_FLASH_CONSTRANGE)
//					send_buffer = 1;
//				else
//					send_buffer = flash_data & 0xFF;
				 send_buffer = 0;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}  
            //******************delelt some by jim***********************
            //////////////////////the code ////////////////////////
      else if ( i+address <= T38IO_OUTPUT12 )
      {
         //send_buffer=(perious_input[i+address - ORIGINALADDRESSVALUE-EEP_OUTPUT1]>> 8) & 0xFF;
                send_buffer = (modbus_data[i + address - T38IO_OUTPUT1] >> 8) & 0xFF;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
         //send_buffer = perious_input[i + address - ORIGINALADDRESSVALUE-EEP_OUTPUT1] & 0xFF;
				send_buffer = modbus_data[i + address - T38IO_OUTPUT1] & 0xFF;

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
      }         
      else if ( i+address  <= T38IO_INPUT4)
      {

             send_buffer = 0;
             
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
                 send_buffer = 0;
			
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
      }
      else if ( i+address <= T38IO_SWITCH_STATE3)
      {
                send_buffer = (Swtich_state[i + address - T38IO_SWITCH_STATE1] >> 8) & 0xFF;
           //send_buffer= (0xff >>8) ;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

				send_buffer = Swtich_state[i + address - T38IO_SWITCH_STATE1] & 0xFF;
                // send_buffer = 0xff ;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
      }       
      else if(i + address <= T38IO_PULSE10_LO_WORD) 
			{   
               if(range[(i + address - T38IO_PULSE1_HI_WORD) / 2] == 6)
	           {
                   send_buffer = pulse.quarter[(i + address - T38IO_PULSE1_HI_WORD) << 1];
                    //send_buffer =0;
    				while (!transmit_finished){}
    				SBUF0 = send_buffer;
    				transmit_finished = 0;
    				CRC16_Tstat(send_buffer);				
    
    				send_buffer = pulse.quarter[((i + address - T38IO_PULSE1_HI_WORD) << 1) + 1];
                   // send_buffer=100;
    				while (!transmit_finished){}
    				SBUF0 = send_buffer;
    				transmit_finished = 0;
    				CRC16_Tstat(send_buffer);
                }
                else
                {
                    if((i + address - T38IO_PULSE1_HI_WORD) %2 ==0)
                    {
                        send_buffer =0;
												while (!transmit_finished){}
												SBUF0 = send_buffer;
												transmit_finished = 0;
												CRC16_Tstat(send_buffer);				
								
												send_buffer = 0;
															 // send_buffer=100;
												while (!transmit_finished){}
												SBUF0 = send_buffer;
												transmit_finished = 0;
												CRC16_Tstat(send_buffer);
                    }
                    else
                    {
                        send_buffer = (guiAnalogInput[(i + address -T38IO_PULSE1_HI_WORD)/2] >> 8) & 0xFF;
												while (!transmit_finished){}
												SBUF0 = send_buffer;
												transmit_finished = 0;
												CRC16_Tstat(send_buffer);				
        
        				send_buffer = guiAnalogInput[(i + address - T38IO_PULSE1_HI_WORD)/2] & 0xFF;
                       // send_buffer=100;
        				while (!transmit_finished){}
        				SBUF0 = send_buffer;
        				transmit_finished = 0;
        				CRC16_Tstat(send_buffer); 
                    }
                }
			}
 

      else if(i + address <= T38IO_PULSE10_MINUTE)
			{
			 watchdog();	 
				send_buffer = 0;				 
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			 		 
				send_buffer = pulse_number[i + address - T38IO_PULSE1_YEAR];
			 
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}
      else if(i + address == T38IO_CHANNEL_TYPE)
			{
			    send_buffer = (channel_type>>8)&0x3ff;				 
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

			    send_buffer = channel_type & 0x3ff;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}
      else if(i+address  <=T38IO_INPUT10_READING)
			{					 	 
                              
               send_buffer = ( Adc_data[i + address -T38IO_INPUT1_READING] >> 8) & 0xFF;

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			 		 
	            send_buffer = Adc_data[i + address - T38IO_INPUT1_READING] & 0xFF;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}
      else if(i + address <= 	T38IO_INPUT10_RANGE)
			{					 	 
				send_buffer = 0;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			 		 

                send_buffer = range[i + address-T38IO_INPUT1_RANGE];
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}
    	else if(i + address <= 	T38IO_INPUT10_FILTER)
			{
				send_buffer = 0;				 
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			 		 
				
			
				send_buffer = filter[i + address- T38IO_INPUT1_FILTER] & 0xff;
                 // send_buffer = 88 ;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}

      	else if(i + address <= 	T38IO_INPUT10_TIMER)
				{
						 	 
						send_buffer = 0; 
						while (!transmit_finished){}
						SBUF0 = send_buffer;
						transmit_finished = 0;
						CRC16_Tstat(send_buffer);					 
						send_buffer = gucTimer[i + address-T38IO_INPUT1_TIMER] & 0xff;

						while (!transmit_finished){}
						SBUF0 = send_buffer;
						transmit_finished = 0;
						CRC16_Tstat(send_buffer);
				}
				else if(i + address <= 	T38IO_INPUT10_TIMER_LEFT)
				{
						 	 
								send_buffer = 0;
								 
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
							 
								send_buffer = gucTimerLeft[i + address - T38IO_INPUT1_TIMER_LEFT];
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}
			
      	else if(i + address == 	T38IO_OUTPUT_MANUAL)
				{               // guiManual =1023;
						 	 
								send_buffer = guiManual >> 8;
								  //send_buffer=255>> 8;
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
							    //send_buffer=255 ;
								send_buffer = guiManual & 0xff ;
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}
				else if(i + address <= 	T38IO_ZONE_OUTPUT8) 
				{
						 	 
								send_buffer = 0;
								 
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
							 
								send_buffer = gucZone[i + address - T38IO_ZONE_OUTPUT1]& 0xff;
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}

       	else if(i + address == T38IO_REVERSE_OUTPUT)
				{
						 	 
								send_buffer = 0;
								 
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
								send_buffer = gucReverseOutput ;
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}

        	else if(i + address <= 	T38IO_STATUS_OUTPUT8) 
				{
						 	 
								send_buffer = 0;
								 
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
							 
								send_buffer = gucStatus[i + address - T38IO_STATUS_OUTPUT1];
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}
				else if(i + address == 	T38IO_SERINALNUMBER_WRITE_FLAG) 
				{
						 	 
								send_buffer = 0;
								 
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
							 
							 
								send_buffer = SNWriteflag;
								while (!transmit_finished){}
								SBUF0 = send_buffer;
								transmit_finished = 0;
								CRC16_Tstat(send_buffer);
				}
//				else if(i + address == 	T38IO_DEFAULT_SETTING) 
//				{
//						 	 
//								send_buffer = 0;
//								 
//								while (!transmit_finished){}
//								SBUF0 = send_buffer;
//								transmit_finished = 0;
//								CRC16_Tstat(send_buffer);
//							 
//							 
//								send_buffer = Default_Setting;
//								while (!transmit_finished){}
//								SBUF0 = send_buffer;
//								transmit_finished = 0;
//								CRC16_Tstat(send_buffer);
//				}
//			   	else if(i + address == TEST_REGISTER) 
//				{
//						 	 	FlashRead_Absolute(6, &test_register) ;
//								
//								send_buffer = 0; 
//								while (!transmit_finished){}
//								SBUF0 = send_buffer;
//								transmit_finished = 0;
//								CRC16_Tstat(send_buffer);
//							 
//							 
//								send_buffer = test_register;
//								while (!transmit_finished){}
//								SBUF0 = send_buffer;
//								transmit_finished = 0;
//								CRC16_Tstat(send_buffer);
//				}


	    else
			{
				send_buffer = 0;
				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);

				send_buffer = 1;

				while (!transmit_finished){}
				SBUF0 = send_buffer;
				transmit_finished = 0;
				CRC16_Tstat(send_buffer);
			}
        }
        while (!transmit_finished){}
		SBUF0 = CRChi;
		transmit_finished = 0;
		while (!transmit_finished){}
		SBUF0 = CRClo;
		transmit_finished = 0;
		while (!transmit_finished){}
       

    
  }
  	else if(data_buffer[1] == MULTIPLE_WRITE)
	{

		// --- response to a multiple write function ---
		// the 6 first bits are the same and then send the crc bits
		for ( i=0; i<6; i++)
		{
			send_buffer = data_buffer[i];
			SBUF0 = send_buffer;
			transmit_finished = 0;
			while (!transmit_finished){}
			CRC16_Tstat(send_buffer);
			watchdog();
		
		}
		// send the two last CRC bits
		SBUF0 = CRChi;
		transmit_finished = 0;
		while (!transmit_finished){}
		SBUF0 = CRClo;
		transmit_finished = 0;
		while (!transmit_finished){}

	}
  else if(data_buffer[1] == CHECKONLINE)
  { 
  	// send first byte of information
		send_buffer = data_buffer[0];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);

		// send second byte of information
		send_buffer = data_buffer[1];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);

		// send address of device
		send_buffer = info[6];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);

		// send serie number0
		send_buffer = info[0];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);
		// send serie number1
		send_buffer = info[1];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);
		// send serie number2
		send_buffer = info[2];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);
		// send serie number3
		send_buffer = info[3];
		SBUF0 = send_buffer;
		transmit_finished = 0;
		while (!transmit_finished){}
		CRC16_Tstat(send_buffer);
		// send the two last CRC bits
		SBUF0 = CRChi;
		transmit_finished = 0;
		while (!transmit_finished){}
		SBUF0 = CRClo;
		transmit_finished = 0;
		while (!transmit_finished){}
  }

}

// ---------------------- internaldeal ----------------------------
// when had received data ,the routine begin to dealwith internal by command external.

void internalDeal(unsigned int start_address)
{
  unsigned char temp, i;
	/*int itemp;
	unsigned char i, temp,ucTemp,ucTemp1;
	unsigned int address_increment;
	unsigned char packet_counter;

	// --- variables for ISP ---

	unsigned char block_to_erase;

	// --- variables for calibration ---
	unsigned char input_select=0;
   */ // by jim
	unsigned char update_flash = 1;
    com_LED_count =1; 
	if(data_buffer[1] == WRITE_VARIABLES)
	{   
		watchdog();
    	if(start_address  < ORIGINALADDRESSVALUE  )
		{	
             // If writing to Serial number Low word, set the Serial number Low flag
			 if(data_buffer[3] <= FLASH_SERIALNUMBER_LOWORD+1)
			 {	
  				flash_write_int(FLASH_SERIALNUMBER_LOWORD, data_buffer[5], FLASH_MEMORY);
  				flash_write_int(FLASH_SERIALNUMBER_LOWORD+1, data_buffer[4], FLASH_MEMORY);
				info[0] = data_buffer[5];
				info[1] = data_buffer[4];
				SNWriteflag |= 0x01;
				flash_write_int(FLASH_SERINALNUMBER_WRITE_FLAG, SNWriteflag, FLASH_MEMORY);

			 }
            // If writing to Serial number High word, set the Serial number High flag
			 else if(data_buffer[3] <= FLASH_SERIALNUMBER_HIWORD+1)
			 {	
  				flash_write_int(FLASH_SERIALNUMBER_HIWORD, data_buffer[5], FLASH_MEMORY);
  				flash_write_int(FLASH_SERIALNUMBER_HIWORD+1, data_buffer[4], FLASH_MEMORY);
			   	info[2] = data_buffer[5];
				info[3] = data_buffer[4];
				SNWriteflag |= 0x02;
				flash_write_int(FLASH_SERINALNUMBER_WRITE_FLAG, SNWriteflag, FLASH_MEMORY);
			 }
		   else if(data_buffer[3] == FLASH_HARDWARE_REV)
			 {	
  				flash_write_int(data_buffer[3], data_buffer[5]+ (data_buffer[4]<<8), FLASH_MEMORY);
  				SNWriteflag |= 0x04;
  				flash_write_int(EEP_SERINALNUMBER_WRITE_FLAG, SNWriteflag, FLASH_MEMORY);
  				info[8] = data_buffer[5];
			 }
	
			 else if(data_buffer[3] == FLASH_ADDRESS)
			 {
  				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);
  				info[6] = data_buffer[5] ;
			 }
			/* else if(data_buffer[3] == FLASH_CALIBRATION )
			 {
  				flash_write_int(data_buffer[3], data_buffer[5]+ (data_buffer[4]<<8), FLASH_MEMORY);
  				output_calibration = data_buffer[5];
			 } */
      
			 /*else if(data_buffer[3] == FLASH_SEQUENCE )				// july 21 Ron
			 {
  				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);
  				output_sequence = data_buffer[5] ;
  				// turn all LEDs when under testing SOP
  				if (output_sequence == 0)
  					startup_flag = 1;

			 }*/
			 else if(data_buffer[3] == FLASH_UPDATE_STATUS )			// july 21 Ron
			 {
  			//	flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);
  			//		update_flash = data_buffer[5] ;
				info[16] = data_buffer[5] ;

		   }  

			 else if(data_buffer[3] == FLASH_DEAD_MASTER )			//  
			 {
  				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);
  				info[12] = data_buffer[5] ;
			 
			 }
			else if(data_buffer[3] == FLASH_ADDRESS_PLUG_N_PLAY )
			{
				if(randval == RESPONSERANDVALUE)
				{
					flash_write_int(FLASH_ADDRESS, data_buffer[5], FLASH_MEMORY);
					info[6] = 	data_buffer[5];	
					iap_program_data_byte(0, DATA_TO_FIRMWARE + FLASH_ADDRESS);
				}
			}
		   else if( start_address == FLASH_BAUDRATE )
			 {
  				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);
  				info[15] = data_buffer[5];
  				if(data_buffer[5] == 1)
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
  				else if(data_buffer[5] == 0)
  				{   SFRPAGE   = TMR4_PAGE;
                	TMR4CN    = 0x00; 
                    TMR4CF    = 0x08; //user SYSCLK
                    RCAP4H = 0xFF;
                    RCAP4L = 0XB8;
                    TR4= 1;            //start timer
                    EIE2 &= ~0x04;
  					SERIAL_RECEIVE_TIMEOUT = 6;
  				}
			   	else if(data_buffer[5] == 3)
  				{   SFRPAGE   = TMR4_PAGE;
                	TMR4CN    = 0x00; 
                    TMR4CF    = 0x08; //user SYSCLK
                    RCAP4H = 0xFF;
                    RCAP4L = 0XF4;		  //57600
                    TR4= 1;            //start timer
                    EIE2 &= ~0x04;
  					SERIAL_RECEIVE_TIMEOUT = 6;
  				}
			 } 
			/* else if( start_address == FLASH_BASE_ADDRESS )
			 {
  				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);  			
  				if(data_buffer[5] == 1)
  				{
  					PCON  = 0X80 ;
  					SERIAL_RECEIVE_TIMEOUT = 3;
  				}
  				else
  				{
  			 		PCON  = 0X00 ;
  					SERIAL_RECEIVE_TIMEOUT = 6;
  				} 
			 } */
			 else
				flash_write_int(data_buffer[3], data_buffer[5], FLASH_MEMORY);                                     
		  }

      else if(start_address <= CALIBRATION_STORAGE_LOCATION)// thus between 100 and 300
      {                          
         start_address = (unsigned int)(data_buffer[2]<<8) + data_buffer[3];
         if(start_address >= T38IO_OUTPUT1 && start_address <= T38IO_OUTPUT12)
         {
           modbus_data[start_address - T38IO_OUTPUT1] = (data_buffer[4]<<8) + data_buffer[5];
           if((start_address - T38IO_OUTPUT1) <8)
           {
              if(modbus_data[start_address - T38IO_OUTPUT1]  >1)
              modbus_data[start_address - T38IO_OUTPUT1] = 1;   
           }

            else if(modbus_data[start_address - T38IO_OUTPUT1]  >4095)
              modbus_data[start_address - T38IO_OUTPUT1] = 4095;
           
         }

        else if(start_address == T38IO_CHANNEL_TYPE)
				{
					channel_type = ((unsigned int)data_buffer[4]<<8)+data_buffer[5];
					//temp = (data_buffer[4]<<8)+data_buffer[5];
														 
					flash_write_int(FLASH_CHANNEL_TYPE, channel_type, FLASH_MEMORY);
					reading_filter_bypass = 30;

					for(temp=0;temp<10;temp++)         //if the channel is set to 1 so set the channel as 6
					{
						if(GetBit(temp,&channel_type))
						{		
							range[temp] = 6;
							flash_write_int(FLASH_INPUT1_RANGE + temp, range[temp], FLASH_MEMORY);		
						}
                        else
                        {
                           	range[temp] = 0;					
							flash_write_int(FLASH_INPUT1_RANGE + temp, range[temp], FLASH_MEMORY); 
                        }
						
					} 
					 
				}
         else if (start_address >= T38IO_INPUT1_RANGE && start_address <= T38IO_INPUT10_RANGE) // if the address is within the list of parameters
			   { 
           
  				 temp =  start_address - T38IO_INPUT1_RANGE;
  				 if(data_buffer[5] < 20)
  				 {
  					range[temp] = data_buffer[5];	  				 
  			  		flash_write_int(FLASH_INPUT1_RANGE + temp, range[temp], FLASH_MEMORY);
                    if(data_buffer[5] == 6)//pulse input
					{
							SetBit(temp,&channel_type);    //if the channel set to 6, set the channel as 1
						                                   //else set the channel as  0
					}
					else
					  ClearBit(temp,&channel_type);
  				 }  	
			   }
         else if (start_address >= T38IO_INPUT1_FILTER && start_address <= T38IO_INPUT10_FILTER) // if the address is within the list of parameters
				 {
  					temp =  start_address - T38IO_INPUT1_FILTER;
  					if(data_buffer[5] < 101)
  					{
  						filter[temp] = data_buffer[5];	
  				 
  						flash_write_int(FLASH_INPUT1_FILTER + temp, filter[temp], FLASH_MEMORY);
  					}
				} 
     

        	else if(start_address >= T38IO_INPUT1_TIMER && start_address <= T38IO_INPUT8_TIMER)
				{

					temp =  start_address - T38IO_INPUT1_TIMER;				 
					 
					gucTimer[temp] = data_buffer[5];				 
					flash_write_int(FLASH_INPUT1_TIMER + temp, gucTimer[temp], FLASH_MEMORY); 	
				 			 
					 
				} 
			
      	
        else if(start_address == T38IO_OUTPUT_MANUAL) 
				{
 				 
					 
					guiManual= ((unsigned int)data_buffer[4] << 8) + data_buffer[5];
			
					flash_write_int(FLASH_OUTPUT_MANUAL, guiManual, FLASH_MEMORY);
				  		 
					 
				} 

				else if(start_address >= T38IO_ZONE_OUTPUT1 && start_address <= T38IO_ZONE_OUTPUT8 )
				{

					temp =  start_address - T38IO_ZONE_OUTPUT1 ;
					if(data_buffer[5] <11)
					{			 
					 
						gucZone[temp] = data_buffer[5];				 
						flash_write_int(FLASH_ZONE_OUTPUT1 + temp, gucZone[temp], FLASH_MEMORY); 
					}	
				 			 	 
				}  
				else if(start_address == T38IO_REVERSE_OUTPUT )
				{
 				    
					 
					gucReverseOutput = data_buffer[5];				 
					flash_write_int(FLASH_REVERSE_OUTPUT, gucReverseOutput, FLASH_MEMORY); 	
				  		 
					 
				} 

        
        else if(start_address <= T38IO_PULSE10_MINUTE && start_address >= T38IO_PULSE1_YEAR) 
		  	{
    			//if(start_address >= (T38IO_PULSE1_YEAR - ORIGINALADDRESSVALUE))
    				temp = (start_address - T38IO_PULSE1_YEAR)/5;
    				//if(GetBit(temp,&channel_type))
    				
    					if((start_address - T38IO_PULSE1_YEAR) % 5 == 0)
    					{      						
    						pulse.word[temp] = 0;
    						//analog_filter[temp] = 0;
    						//gbClearPulse = 1;
    						//gucClearPulse = temp; 							
    					//	start_timer(STORE_PULSE,1);
    					}
    					pulse_number[start_address - T38IO_PULSE1_YEAR] = data_buffer[5];
    				   flash_write_int(FLASH_PULSE1_YEAR+start_address - T38IO_PULSE1_YEAR, data_buffer[5], FLASH_MEMORY); 
		  	}
				else if(start_address == T38IO_DEFAULT_SETTING )
				{
							if( data_buffer[5] == 1)
							{
									   for(i = 0;i < 10;i++) 
										{				 
											range[i] = 0;	
											flash_write_int(FLASH_INPUT1_RANGE+ i, range[i], FLASH_MEMORY); 
										}
							}				
				}
//		else if(start_address == TEST_REGISTER )
//		{
//			iap_erase_page(0);
//			test_register = data_buffer[5];
//			iap_program_data_RAM(test_register, 6);
//		}
        
      }

    }
	else if(data_buffer[1] == MULTIPLE_WRITE)
	{
		unsigned char data temp1 ;
		temp1 = start_address - T38IO_OUTPUT1;
       	temp = data_buffer[5];	
	    if(start_address >= T38IO_OUTPUT1 && start_address <= T38IO_OUTPUT12)
         {
	           for( i = 0; i< temp;i++)
			  {
			   modbus_data[temp1+i]	= (data_buffer[7+2*i]<<8)+ data_buffer[8+2*i];
	           if((temp1+i) <8)
	           {
	              if(modbus_data[temp1+i]  >1)
	              modbus_data[temp1+i] = 1;   
	           }
	
	            else if(modbus_data[temp1+i]  >4095)
	              modbus_data[temp1+i] = 4095;
			  }
           
         }
		
		else if (start_address >= T38IO_INPUT1_RANGE && start_address <= T38IO_INPUT10_RANGE) // if the address is within the list of parameters
		{ 	  
			temp1 = start_address - T38IO_INPUT1_RANGE	;
       		temp = data_buffer[5];
			for( i = 0; i< temp;i++)
			{
					 if(data_buffer[8+2*i] < 9)
					 {
						range[temp1+i] = data_buffer[8+2*i];	
						//range[ i] =6;  				 
				  		flash_write_int(FLASH_INPUT1_RANGE +temp1+ i, range[ i+temp1], FLASH_MEMORY);
		                 #if 1
		                if(data_buffer[5] == 6)//pulse input
							{
								SetBit( temp1+i, &channel_type);    //if the channel set to 6, set the channel as 1
							                                   //else set the channel as  0
							}
						else
						  ClearBit( temp1+i, &channel_type);
		                  #endif
					 } 
			 } 	
		}
	  else if (start_address >= T38IO_INPUT1_FILTER && start_address <= T38IO_INPUT10_FILTER) // if the address is within the list of parameters
		{
			temp1 =  start_address - T38IO_INPUT1_FILTER;
			temp = data_buffer[5];
			for( i = 0; i< temp;i++)
			{
				if(data_buffer[8+2*i] < 101)
				{
					filter[temp1+i] = data_buffer[8+2*i];	
			 
					flash_write_int(FLASH_INPUT1_FILTER + temp1+i, filter[temp1+i], FLASH_MEMORY);
				}
			}
		} 		
	}  
      
    //watchdog();
     if(start_address == SYS_UPDATE)
    {
     //iap_erase_block(115);
     //while(1);
     //RSTSRC |= (1 << 4); //software reset
     //iap_program_data_byte(0x55, 0xc800);//first byte of page 115
     //iap_program_data_byte(laddress, 0xc801);//first byte of page 115
    }   
   if (info[16] == 0x7F)
    { 
       unsigned int temp; 
      //iap_erase_block(53);
	  iap_erase_page(0);
      for( i=0; i<=20; i++)
	  {
		if( flash_read_int(i,&temp,FLASH_MEMORY ) )
		iap_program_data_RAM((unsigned char)temp, i);
		//iap_program_data_byte((unsigned char)info[i], 0XD400 + i);
        delay_us(100);
        
	  }	     
        EA = 0;
        RSTSRC |= (1 << 4); //software reset
    }
	// --------------- reset board -------------------------------------------
	else if (info[16] == 0xFF)
	{	
		// disable the global interrupts
        EA = 0;
        RSTSRC |= (1 << 4); //software reset

	}
	else if(info[16] == 0x8e)
	{
		SNWriteflag = 0x00;
		flash_write_int(FLASH_SERINALNUMBER_WRITE_FLAG, SNWriteflag, FLASH_MEMORY);
 		EA = 0;
        RSTSRC |= (1 << 4); //software reset
	}
 }


/////------------------------------- initcrc16 ---------------------
///  init crc 
void InitCRC16(void)
{
	CRClo = 0xFF;
	CRChi = 0xFF;
}
//-------------------crc16_tstat ---------------------
// calculate crc with one byte
void CRC16_Tstat(unsigned char ch)
{
	unsigned char uIndex ;
	uIndex = CRChi ^ ch ; // calculate the CRC 
	CRChi = CRClo ^ auchCRCHi[uIndex] ;
	CRClo = auchCRCLo[uIndex] ;
}


/// ----------------------crc16 --------------------------
//calculate crc 
unsigned int CRC16(unsigned char *puchMsg, unsigned char usDataLen)
{
	unsigned int uchCRCHi = 0xFF ; // high byte of CRC initialized 
	unsigned char uchCRCLo = 0xFF ; // low byte of CRC initialized 
	unsigned char uIndex ; // will index into CRC lookup table 
	while (usDataLen--) // pass through message buffer 
		{
			uIndex = uchCRCHi ^ *puchMsg++ ; // calculate the CRC 
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
			uchCRCLo = auchCRCLo[uIndex] ;
		}
	return (uchCRCHi << 8 | uchCRCLo) ;
}




