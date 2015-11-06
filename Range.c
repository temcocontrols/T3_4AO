#include "c8051f120.h"
#include "LibIO_T3IO.h"
#include "define.h"
#include "math.h"
 
bit pic_exists = 1;

#if defined T3_8IN13OUT //MHF 20010_07 COMBINE TWO IFDEFS TO ONE
void start_timer( et_event timer_no , unsigned int timeout ) ;
extern unsigned char xdata gucPreviousInput[10];
extern unsigned char xdata gucTimer[10];
extern unsigned char xdata gucTimerLeft[10];
extern unsigned char xdata  gucOverOutput[10];
extern unsigned int  xdata guiManual;
extern unsigned char xdata  reading_filter_bypass;
extern bit flash_read_int(unsigned char id, unsigned int *value, unsigned char block_select);
extern void stop_timer( et_event timer_no );
extern unsigned long xdata analog_in_buffer[10];
//unsigned char xdata gucTimerFilter[10];
//extern unsigned long xdata previous_pulse_number[10];
extern unsigned int xdata perious_input[10]; 
typedef union   pulse_number_link {
		unsigned long      number_long[8];
		unsigned char    pulse_number[32];
	};
extern union   pulse_number_link xdata   pic,flash; 
//bit rang_flag= 0; //why no comments?

#endif
unsigned char xdata thermistor_type = 0 ;
unsigned char const code def_tab2[11] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 192, 209, 206, 187, 161, 131, 103, 79, 61, 45, 155
			};
//MHF:12-30-05,Added 4 values to make the tstat can measure minus degree
unsigned char const code def_tab_pic_Type2_10K[15] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 25, 41, 61, 83, 102, 113, 112, 101, 85, 67, 51, 38, 28, 21, 65 //MHF 20010_07 REVISE TEMP LOOKUP TABLE PER NATHANEAL
 			};
unsigned char const code def_tab_pic_Type3_10K[15] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 29, 45, 63, 79, 96, 104, 103, 94, 80, 65, 51, 40, 30, 23, 77 
 			};
unsigned char const code def_tab_pic_Type4_10K[15] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 32, 46, 63, 81, 92, 98, 95, 87, 76, 63, 52, 40, 32, 24, 93 
 			};
signed int   look_up_table1(unsigned int count)
{
	int   xdata val;
    char  index=14;
	int   xdata work_var;
 
	if (pic_exists)
	{	
		if(thermistor_type == TYPE2_10K_THERM)
			work_var= def_tab_pic_Type2_10K[index];
		else if(thermistor_type == TYPE3_10K_THERM)
			work_var= def_tab_pic_Type3_10K[index];
		else if(thermistor_type == TYPE4_10K_THERM)
			work_var= def_tab_pic_Type4_10K[index];
	}
	else
		work_var= def_tab2[index];
		  
	if (work_var > count )
		{
			val =  index  * 100 ;
			return ( val );
		  
		}

	do 
		{
			index--;

			if (pic_exists)
			{
				if(thermistor_type == TYPE2_10K_THERM)
				work_var += def_tab_pic_Type2_10K[index];
				else if(thermistor_type == TYPE3_10K_THERM)
				work_var += def_tab_pic_Type3_10K[index];
				else if(thermistor_type == TYPE4_10K_THERM)
				work_var += def_tab_pic_Type4_10K[index];
			}
			else
				work_var += def_tab2[index];

			if( work_var > count)
				{
				val = ( work_var - count )*100;

				if (pic_exists)
				{
					if(thermistor_type == TYPE2_10K_THERM)
					val /= def_tab_pic_Type2_10K[index];
					else if(thermistor_type == TYPE3_10K_THERM)
					val /= def_tab_pic_Type3_10K[index];
					else if(thermistor_type == TYPE4_10K_THERM)
					val /= def_tab_pic_Type4_10K[index];

				}
				else
					val /= def_tab2[index];
				if(index >= 4)
				{
					val +=  (index - 4) * 100;
					val = val & 0x7fff;
				}
				else
				{
					val += index*100;
					val = 400 - val;
					val = val | 0x8000;
				}			 
				return (val);
				}
		} while (index) ;

			val =  33768;

			return ( val );
}

/******************************************RangeConverter******************************************/
/*
Description: Convert the  raw data from adc to correspond engineer units.
parameter:	finction,	The engineer units want to get,
			para,		Raw data from ADC
			i, 			Be used for function = 4,customer sensor,because there are only two 
						customer tables,so should check this parameter not bigger than 2 on fun4.
			cal,		calibration data for the correspond input channel
Return:		Changed input to the expected engineer units.	
			
*/
/*********************************RangeConverter funtion start**************************************/
#if 1
signed int RangeConverter(unsigned char function, signed int para,signed int cal,unsigned char i)
{
	signed int xdata siAdcResult;
	unsigned char xdata ucFunction;
 
	signed   int  xdata siInput;
	signed int  xdata uiCal;
	signed   int  xdata siResult;
	unsigned int xdata uiResult;
	bit bAnalogInputStatus;
	char SFRPAGE_SAVE = SFRPAGE;
    SFRPAGE = CONFIG_PAGE;
    ucFunction = function;
	siInput = para;

	uiCal = cal;
	if(ucFunction == 0)
	{   //rang_flag= 0;
	 	siResult = (siInput + (signed int)(uiCal - CALIBRATION_OFFSET))<<2;	
	}	 
	//-----------10K Thermistor---------------
	else if (ucFunction == 1 || ucFunction == 2)
	{
          //rang_flag= 0;
		siAdcResult = look_up_table1(siInput);
 
		//MHF:01-02-06,Added minus temperature display
		if(siAdcResult & 0x8000)
		siInput = -(signed int)(siAdcResult & 0x7fff);
		else
		siInput = siAdcResult;
		//analog_input[i] = adc_result;
		
		if(ucFunction == 2)  
		{	//rang_flag= 0;	 
			siInput = (siInput * 9)/5 +320; 		 
		}

		// Add the calibration term to the input.
		siResult = siInput + (signed int)(uiCal - CALIBRATION_OFFSET); 
		 	
	}
	//-----------0-100%---------------
	else if(ucFunction == 3)  //MHF: Feb 24th 2005 new range setting for analog inputs
	{
		//siResult = (float)(siInput)/1023*100;
       // rang_flag= 0;
    siResult = (unsigned int)((unsigned int)siInput/10.23);
   // RELAY0=1;RELAY1=1; RELAY2=1; RELAY3=1; RELAY4=1;
	}
	//-----------ON/OFF---------------
	else if(ucFunction == 4 || ucFunction == 5)
	{  // rang_flag= 0;
		siAdcResult = (float)(siInput)/1023*50;
		if(siAdcResult <= 24)
		{
			if(ucFunction == 5)
				bAnalogInputStatus = 1; 
			else if(ucFunction == 4)
				bAnalogInputStatus = 0; 
		}
				else if(siAdcResult >= 26)
				{
			if(ucFunction == 5)
				bAnalogInputStatus = 0; 
			else if(ucFunction == 4)
				bAnalogInputStatus = 1; 
		}
		siResult = (unsigned int)(bAnalogInputStatus);
	 	
	}
    else if(ucFunction == 8)
    {
       siResult = (9 *siInput *100 ) /1023 ; 
    }
 	else if(ucFunction == 7 /*&& reading_filter_bypass == 0*/)
	{ 
        //rang_flag= 1;
		switch(i)
		{
			case 0:		
         		if((signed int)(perious_input[i]-analog_in_buffer[i])>150)   //TO judge the swtich is  on or off 
           //if(perious_input[i]!=analog_in_buffer[i])
       
           {	
                                                   
                if(!flash_read_int(FLASH_INPUT1_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				     
						gucOverOutput[i] = 1;                          //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					  
						start_timer(INPUT1_TIMER,DEFAULT_TIMEOUT);
            
					}	
       }
      break;
      case 1:		
 
           	if((signed int)(perious_input[i]-analog_in_buffer[i])>150)      //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT2_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                  //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT2_TIMER,DEFAULT_TIMEOUT);
					
					}	
        }
      break;
      		case 2:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)     //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT3_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                            //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT3_TIMER,DEFAULT_TIMEOUT);
					
					}	
         }
      break;
      			case 3:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)           //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT4_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                    //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT4_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
      break;
      			case 4:		
             if((signed int)(perious_input[i]-analog_in_buffer[i])>150)      //TO judge the swtich is  on or off 
             {


					if(!flash_read_int(FLASH_INPUT5_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                     //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT5_TIMER,DEFAULT_TIMEOUT);
					
					}	
         }
      break;
      case 5:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)     //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT6_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                        //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT6_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
       break;
      	case 6:		
                                                                                 //TO judge the swtich is  on or off 
           if((signed int)(perious_input[i]-analog_in_buffer[i])>150)
           {
					if(!flash_read_int(FLASH_INPUT7_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                                  //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT7_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 7:		
 
             if((signed int)(perious_input[i]-analog_in_buffer[i])>150)           //TO judge the swtich is  on or off 
             {

					if(!flash_read_int(FLASH_INPUT8_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                             //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT8_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 8:				
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)              //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT9_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                             //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT9_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 9:		
           if((signed int)(perious_input[i]-analog_in_buffer[i])>150)            //TO judge the swtich is  on or off 
           {


					if(!flash_read_int(FLASH_INPUT10_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;       //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT10_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
      break;
      			

			default:
			break;
		}
		siResult = siInput;
	}
	else if(ucFunction == NO_USE) 
	siResult = 0;
	else if(ucFunction == V0_3) 
	siResult = (3000L *siInput ) >> 10;
	else if(ucFunction == V0_10) 
	siResult = ( 10000L * siInput) >> 10;
	else if(ucFunction == I0_20ma) 
	siResult = ( 2000L * siInput) >> 10;
    SFRPAGE = SFRPAGE_SAVE;	
 	return siResult;
} 
#endif
#if 0
signed int RangeConverter(unsigned char function, signed int para,signed int cal,unsigned char i)
{
	signed int xdata siAdcResult;
	unsigned char xdata ucFunction;
	signed   int  xdata siInput;
	signed int  xdata uiCal;
	signed   int  xdata siResult;
	unsigned int xdata uiResult;
	const float code  K_A = 4.8876 ;
	const signed int code K_B = 2732 ;
	bit bAnalogInputStatus;
	ucFunction = function;
	siInput = para;
	uiCal = cal;
	switch(ucFunction)
	{
	 case RAW_DATA :
		  siResult = siInput + (signed int)(uiCal - CALIBRATION_OFFSET);
		  break ;		 
	//-----------10K Thermistor---------------
	case  C_TYPE2 :
	case  F_TYPE2 :
	case  C_TYPE3 : 
	case  F_TYPE3 :
	case  C_TYPE4 : 
	case  F_TYPE4 :
		if((ucFunction == C_TYPE2)||(ucFunction == F_TYPE2))
		thermistor_type = TYPE2_10K_THERM ;
		else if((ucFunction == C_TYPE3)||(ucFunction == F_TYPE3))
		thermistor_type = TYPE3_10K_THERM ;
		else
		thermistor_type = TYPE4_10K_THERM ;

       	siAdcResult = look_up_table1(siInput);
		if(siAdcResult & 0x8000)
		siInput = -(signed int)(siAdcResult & 0x7fff);
		else
		siInput = siAdcResult;
		//analog_input[i] = adc_result;
		
		if((ucFunction == F_TYPE2)||(ucFunction == F_TYPE3)||(ucFunction == F_TYPE4))  //F
		{		 
			siInput = (siInput * 9)/5 +320; 		 
		}
		// Add the calibration term to the input.
		siResult = siInput + (signed int)(uiCal - CALIBRATION_OFFSET); 
	break;	 	
	//-----------0-100%---------------
	case PERCENT: 					//MHF: Feb 24th 2005 new range setting for analog inputs
		siResult = (float)(siInput)/1023*100;
		break;

	//-----------ON/OFF---------------
	case ON_OFF : 
	case OFF_ON	:
		siAdcResult = (float)(siInput)/1023*50;
		if(siAdcResult <= 24)
		{
			if(ucFunction == OFF_ON)
				bAnalogInputStatus = 1; 
			else if(ucFunction == ON_OFF)
				bAnalogInputStatus = 0; 
		}
		else if(siAdcResult >= 26)
		{
			if(ucFunction == OFF_ON)
			bAnalogInputStatus = 0; 
			else if(ucFunction == ON_OFF)
			bAnalogInputStatus = 1; 
		}
		siResult = (unsigned int)(bAnalogInputStatus);
	 	break ;
		case LIGHTING :
		{ 
		switch(i)
		{
			case 0:		
         	if((signed int)(perious_input[i]-analog_in_buffer[i])>150)   //TO judge the swtich is  on or off        
           	{	
                                                   
                if(!flash_read_int(FLASH_INPUT1_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				     
						gucOverOutput[i] = 1;                          //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					  
						start_timer(INPUT1_TIMER,DEFAULT_TIMEOUT);
            
					}	
       		}
      break;
      case 1:		
           	if((signed int)(perious_input[i]-analog_in_buffer[i])>150)      //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT2_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                  //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT2_TIMER,DEFAULT_TIMEOUT);
					
					}	
        }
      break;
   	case 2:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)     //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT3_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                            //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT3_TIMER,DEFAULT_TIMEOUT);
					
					}	
         }
      break;
      			case 3:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)           //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT4_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                    //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT4_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
      break;
      			case 4:		
             if((signed int)(perious_input[i]-analog_in_buffer[i])>150)      //TO judge the swtich is  on or off 
             {


					if(!flash_read_int(FLASH_INPUT5_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                     //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT5_TIMER,DEFAULT_TIMEOUT);
					
					}	
         }
      break;
      case 5:		
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)     //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT6_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;
						gucTimerLeft[i] = gucTimer[i];                        //timer left flag . if timer left >0 , flag set to 1.
					 
						start_timer(INPUT6_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
       break;
      	case 6:		
                                                                                 //TO judge the swtich is  on or off 
           if((signed int)(perious_input[i]-analog_in_buffer[i])>150)
           {
					if(!flash_read_int(FLASH_INPUT7_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                                  //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT7_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 7:		
 
             if((signed int)(perious_input[i]-analog_in_buffer[i])>150)           //TO judge the swtich is  on or off 
             {

					if(!flash_read_int(FLASH_INPUT8_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                             //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT8_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 8:				
 
            if((signed int)(perious_input[i]-analog_in_buffer[i])>150)              //TO judge the swtich is  on or off 
            {

					if(!flash_read_int(FLASH_INPUT9_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;                             //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT9_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
            break;
      	case 9:		
           if((signed int)(perious_input[i]-analog_in_buffer[i])>150)            //TO judge the swtich is  on or off 
           {


					if(!flash_read_int(FLASH_INPUT10_TIMER,&uiResult,FLASH_MEMORY))
						gucTimer[i] = 0;
					else
						gucTimer[i] = uiResult;				
					
		                                                                                                                                                                			
					if(gucTimer[i] > 0)
					{
				
						gucOverOutput[i] = 1;       //timer left flag . if timer left >0 , flag set to 1.
						gucTimerLeft[i] = gucTimer[i];
					 
						start_timer(INPUT10_TIMER,DEFAULT_TIMEOUT);
					
					}	
          }
      break;     			
	  default:
			break;
		}
		siResult = siInput;
	}
		case NO_USE: 
		siResult = 0;
		break;
		case V0_5:
		siResult =  ( siInput * 5000L ) >> 10;
		break;
		case V0_10:
		siResult = ( 10000L * siInput ) >> 10;
		break;
		case I0_20ma:
		siResult = ( 2000L * siInput ) >> 10;
		break;
		case I0_100ma:
		siResult = ( 100000L * siInput ) >> 10;
		break;
		case LM235_C:
		case LM235_F:
		siInput = (signed int )(K_A *siInput - K_B) ;
		if((ucFunction == LM235_F))
		siInput = (siInput * 9)/5 +320;
		siResult =  siInput ;
		break;
		}
 	return siResult;
}
#endif



unsigned char  output_range(_OUTPUT_RANGE *range )
 {

		if(range->switch_status == SW_HAND)
		{
			if(range->output_mode == NORMAL_MODE)
			{
					return 1 ;
					
			}
			else if(range->output_mode == REVERSE_MODE)
			{
					return 0 ;	
			}
		}
		else if(range->switch_status == SW_OFF)
		{
			if(range->output_mode == NORMAL_MODE)
			{
					return 0 ;
					
			}
			else if(range->output_mode == REVERSE_MODE)
			{
					return 1 ;	
			}
		}	
		else if(range->switch_status == SW_AUTO)
		{
			if(range->output_mode == NORMAL_MODE)
			{
					if( range->output_value) return 1 ;	
					else	return 0 ;			
			}
			else if(range->output_mode == REVERSE_MODE)
			{
					if( range->output_value) return 0 ;	
					else	return 1 ;		
			}
		}
 }
