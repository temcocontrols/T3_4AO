#include "c8051f120.h"
#include "LibIO_T3IO.h"

 #define  FW_VER_LOW 	 (T3_SOFTWARE_VERSION &0xff)
 #define  FW_VER_HIGH	 ((T3_SOFTWARE_VERSION>>8)&0xff)
 
 unsigned char const code infor[20] = {'T', 'e', 'm', 'c', 'o', 'T', '3', '-', '4', 'A', 'O', 0, 0, 0, 0, FW_VER_LOW, FW_VER_HIGH,  0, 0, 64};
