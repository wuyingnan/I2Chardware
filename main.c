#include<msp430f5438.h>
#include"Global.h"
#include"UCS.h"
#include"WDT.h"
#include"TimerA1.h"
#include"UART.h"
#include"I2C.h"
#include"HMC5883.h"
#include"ADXL345.h"
#include"L3G4200.h"
unsigned long nowTime;
unsigned char a;
int mx,my,mz,ax,ay,az,gx,gy,gz;

void main( void )
{
  WDT_init();
  UCS_init();  
  TimerA1_init();
  I2C_init();
  UART_init(UCA1,115200);
  _EINT();
  HMC5883_init();  
  ADXL345_init();
  L3G4200_init();
  
  delay_ms(100);
  while(1)
  {
    if(nowTime!=TimeBase)
    {
      nowTime=TimeBase;
      if ((nowTime % 20 ==0)&&(nowTime < 5000))
      {
        I2C_read(0x1e,0x03,2,(unsigned char *)&mx);
        mx=(unsigned)mx/256+((unsigned)mx%256)*256;
        UART_sendint(UCA1,mx+32768);        
        UART_sendstr(UCA1," ");
        I2C_read(0x1e,0x05,2,(unsigned char *)&my);
        my=(unsigned)my/256+((unsigned)my%256)*256;
        UART_sendint(UCA1,my+32768);        
        UART_sendstr(UCA1," ");        
        I2C_read(0x1e,0x07,2,(unsigned char *)&mz);
        mz=(unsigned)mz/256+((unsigned)mz%256)*256;    
        UART_sendint(UCA1,mz+32768);                
        UART_sendstr(UCA1," ");
        _NOP();
        I2C_read(0x53,0x32,2,(unsigned char *)&ax);
        ax=(unsigned)ax/256+((unsigned)ax%256)*256;
        UART_sendint(UCA1,ax+32768);                
        UART_sendstr(UCA1," ");        
        I2C_read(0x53,0x34,2,(unsigned char *)&ay);
        ay=(unsigned)ay/256+((unsigned)ay%256)*256;
        UART_sendint(UCA1,ay+32768);                
        UART_sendstr(UCA1," ");        
        I2C_read(0x53,0x36,2,(unsigned char *)&az);
        az=(unsigned)az/256+((unsigned)az%256)*256;    
        UART_sendint(UCA1,az+32768);                
        UART_sendstr(UCA1," ");        
        _NOP();
        I2C_read(0x69,0x0f,1,&a);
        I2C_read(0x69,0x28|0x80,2,(unsigned char *)&gx);
        gx=(unsigned)gx/256+((unsigned)gx%256)*256;
        UART_sendint(UCA1,gx+32768);                
        UART_sendstr(UCA1," ");        
        I2C_read(0x69,0x2a|0x80,2,(unsigned char *)&gy);
        gy=(unsigned)gy/256+((unsigned)gy%256)*256;
        UART_sendint(UCA1,gy+32768);               
        UART_sendstr(UCA1," ");        
        I2C_read(0x69,0x2c|0x80,2,(unsigned char *)&gz);
        gz=(unsigned)gz/256+((unsigned)gz%256)*256;    
        UART_sendint(UCA1,gz+32768);                
        UART_sendstr(UCA1," ");
        _NOP();
      }
    }
  }
}
