#include "c8051f120.h"
#include "LibIO_T3IO.h"
#include "define.h"
#include  "pic.h"
unsigned char xdata pic_version;
//unsigned int xdata  analog_input[5]  ;
void read_pic(void)
{
   
  char SFRPAGESAVE = SFRPAGE;
  SFRPAGE = CONFIG_PAGE;	
  i2c_pic_start();
  i2c_pic_write( 0xb1 );
  if (GET_ACK())
  {
	i2c_stop();
	return ;
  }
  i2c_pic_write(RELAY_STATUS);
  if (GET_ACK())
  {
	i2c_stop();
	return ;
  }	
  i2c_pic_write(0X69);
  i2c_stop();

   SFRPAGE=SFRPAGESAVE;
}
/*
bit analog_outputadj()
{ 
  unsigned char i;
  unsigned int pic_data_buff[5],checksum;
  char SFRPAGESAVE = SFRPAGE;
  SFRPAGE = CONFIG_PAGE;
	
  i2c_pic_start();
  i2c_pic_write( 0xb3 );
  if (GET_ACK())
  {
	i2c_stop();
	return ;
  }
  for(i=0;i<4;i++)
 {
  pic_data_buff[i]=i2c_pic_read() &0x3ff ;      
  GIVE_PIC_ACK();
  checksum+=pic_data_buff[i];
  delay_us(1);
 }

  pic_data_buff[5]=i2c_pic_read();
  delay_us(1);
  i2c_stop();
  if (pic_data_buff[5] == checksum)
	{
		for(i=0;i<4;i++)
		{
			mul_analog_in_buffer[i]= pic_data_buff[i];
        }
       
		return 1;
	}
	else
		return 0;

   SFRPAGE=SFRPAGESAVE; 
}
*/
bit read_pic_version( void)
{
	unsigned int temp_version, temp_check;
   char SFRPAGESAVE = SFRPAGE;
    SFRPAGE = CONFIG_PAGE;
	i2c_pic_start();
	
	i2c_pic_write( 0xb2 );

	delay_us(2);    
	if (GET_ACK())
    {
		i2c_stop();
		pic_version = 0;
 
		return 0;
	}
    
	delay_us(2);
	temp_version = i2c_pic_read();
	GIVE_PIC_ACK();
	delay_us(2);
	temp_check = i2c_pic_read();
	i2c_stop(); 
	/*if (temp_check == 0x69)
	{
		
       pic_version = temp_version & 0xff;  
    	return 1;
	
	}
	else
	{
		pic_version = 0;
 
		return 0;
	} */
    pic_version = 88;
   SFRPAGE=SFRPAGESAVE;
}
/*
bit refresh_outputs (void)
{   //ET0 = 0 ;
    unsigned char i ;
    unsigned int data_checksum;
	i2c_pic_start();
    i2c_pic_write( 0xb3 );
  	if (GET_ACK())
	{
		i2c_stop();
		return 0;
	}
   
	delay_us(3);
   // EA = 0;
    if(guiDeadmasterTime)
    //if(0)
    {
        i2c_pic_write(Swtich_state[0]);
        if (GET_ACK())
    	{
    		i2c_stop();
    		return 0;
    	}
        i2c_pic_write(Swtich_state[1]);
        if (GET_ACK())
    	{
    		i2c_stop();
    		return 0;
    	} 
    }
    else 
    {
        if(guiDeadmaster_relay== 2)
        {
            i2c_pic_write(0x00);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	}
            i2c_pic_write(0x00);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	} 
        }
        else if(guiDeadmaster_relay ==1)
        {
            i2c_pic_write(0x55);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	}
            i2c_pic_write(0x55);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	} 
        }
        else
        {
            i2c_pic_write(Swtich_state[0]);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	}
            i2c_pic_write(Swtich_state[1]);
            if (GET_ACK())
        	{
        		i2c_stop();
        		return 0;
        	} 
        } 
    } 
    //EA = 1 ;
	

    for(i=0;i<5;i++)
    {
         i2c_pic_write(modbus.registers[EEP_OUTPUT1+i]);
         
         //i2c_pic_write_data (0x3ff); 
        if (GET_ACK())
    	{
    		i2c_stop();
    		return 0;
    	}
        data_checksum += modbus.registers[EEP_OUTPUT1+i];
       
    }
   data_checksum = data_checksum & 0xff;
   i2c_pic_write(data_checksum);
   i2c_stop();


 }  
 
 */


/*;***********************************/
/*;i2c_startup sequence of 24Cxx*/
void i2c_pic_start()
{
	// PIC chip requires a special double start condition in order
	// to initiate communication.  This is to insure the PIC does not 
	// read any false starts.
	// MDF 12/01/04
	
	// 1st start condition
	I2C_SDA = 0;
	delay_us(5);
	I2C_SCL = 0;
	delay_us(2);


		// reset bus
		I2C_SDA = 1;
		I2C_SCL = 1;
		delay_us(1);
	
		// 2nd start condition
		I2C_SDA = 0;
		delay_us(5);
		I2C_SCL = 0;
		delay_us(5);

}



 void GIVE_PIC_ACK( void )
{
 	int j=0;
	// Wait until the data signal goes high
	while (!I2C_SDA){
		j++;
		// If no clock, exit i2c_read routine
		if (j == 100)
			return;
	}
	// Bring the data line low
    I2C_SDA=0;
	// Pulse the clock
    I2C_SCL=1;
    delay_us (1);
    I2C_SCL=0;
	// Bring the data line back high
    I2C_SDA=1;
}

/*;************************************/
/*;send a 8-bit data to 12F675 */
void i2c_pic_write( unsigned char ch )
{
	unsigned char i = 8;
//	if(gbComm) return;
	do
	{    
		delay_us(1);
		I2C_SDA = ( ch & 0x80 );
		I2C_SCL=1;
		delay_us(1);
		I2C_SCL=0;
		ch<<=1;

	} while( --i != 0 );
	
	I2C_SDA=1;
}


/*;**************************************/
/*;receive a 10-bit data from 12F882 */
unsigned int i2c_pic_read( void )
{
	unsigned char i;
	unsigned int data1 = 0;
//	if(gbComm) 	return 0;
	for( i=0; i<10; i++ )
	{
		I2C_SCL = 1;
		delay_us(1);
		data1 = ( data1 << 1 ) | I2C_SDA;
		I2C_SCL = 0;
		delay_us(1);
	}

	return data1;
}


/*;**************************************/
/*;detect the ACK signal to 24Cxx*/	
// returns a 1 for no ACK.  returns a 0 for successful ACK		
bit GET_ACK( void )
{
    unsigned char c=0;
    unsigned char i;
 

	if (1) // only newer versions of the pic have the 2nd start condition.
	{
	    // MDF 12/01/04
	    // Wait for data line to be pulled low.
	    for (i=0; i<10; i++)
	    {
    			c=I2C_SDA;
    			if (c == 0)
          {
    			// if data line is low, pulse the clock.
    			    I2C_SCL=1;
    				delay_us(1);
    			    I2C_SCL=0;
    				return 0;
    			}		
	    }
	    I2C_SCL=0;
	    return 1;
	}

	else
	{
	    I2C_SDA=1;
	    I2C_SCL=1;
	    for (i=0; i<10; i++)
	    {
			c=I2C_SDA;
			if (c == 0){
			    I2C_SCL=0;
				return 0;
			}		
	    }
	    I2C_SCL=0;
	    return 1;
	}
}
void i2c_stop()
{
 	I2C_SDA = 0;
	I2C_SCL = 1; 
	I2C_SDA = 1;

}


