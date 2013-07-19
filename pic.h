/*-----------------pic.h---------------------*/


void pic_detect(void);

bit read_pic_original( unsigned char store_location, bit set );
void analog_input_buffer (unsigned int *raw_data, unsigned char length, unsigned char start);

void read_pic(void);
void store_to_registers (unsigned int pic_data, unsigned char start);
 
void read_pulse_pic(void);
void store_pulse_registers(unsigned long data_buffer, unsigned char start);
 


void write_pic( unsigned char addr, unsigned char value );

bit GET_ACK( void );
void GIVE_PIC_ACK( void );
void i2c_pic_start();
void i2c_pic_write( unsigned char ch );
unsigned int i2c_pic_read( void );
void i2c_stop(void);
void delay_us(unsigned int);
extern bit flag_I2C;	
extern unsigned char RELAY_STATUS;
bit read_pic_version( void);






