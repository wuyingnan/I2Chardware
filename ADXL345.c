#include "msp430x54x.h"
#include "UCS.h"
#include "ADXL345.h"
#include "I2C.h"
void ADXL345_init(void)
{
  I2C_write(ADXL345_Address,0x31,0x08);   //������Χ,����2g��13λģʽ �ֱ���4mg
  I2C_write(ADXL345_Address,0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
  I2C_write(ADXL345_Address,0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
  //I2C_write(ADXL345_Address,0x2E,0x80);   //ʹ�� DATA_READY �ж�
  I2C_write(ADXL345_Address,0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
  I2C_write(ADXL345_Address,0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
  I2C_write(ADXL345_Address,0x20,0x00);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
  I2C_write(ADXL345_Address,0x2F,0x00);		//����freefall Ϊint1��
  I2C_write(ADXL345_Address,0x28,0x07);
  I2C_write(ADXL345_Address,0x29,0x04);
  I2C_write(ADXL345_Address,0x2E,0x00+BIT2);//����freefall int
  
  
  
  P1DIR&=~BIT0;
  P1IFG = 0;
  P1IES&=~BIT0;//�����ж�
  P1IE |= BIT0;//17��  
  
  delay_ms(10);
}
/*
signed int ADXL345_GetAX()
{
  union
  {
  signed int ax;
  unsigned char n[2];
  };
  n[0]=I2C_read(ADXL345_Address,ACCEL_XOUT_L);
  n[1]=I2C_read(ADXL345_Address,ACCEL_XOUT_H);
  return ax;
}
signed int ADXL345_GetAY()
{
  union
  {
  signed int ay;
  unsigned char n[2];
  };
  n[0]=I2C_read(ADXL345_Address,ACCEL_YOUT_L);
  n[1]=I2C_read(ADXL345_Address,ACCEL_YOUT_H);
  return ay;
}
signed int ADXL345_GetAZ()
{
  union
  {
  signed int az;
  unsigned char n[2];
  };
  n[0]=I2C_read(ADXL345_Address,ACCEL_ZOUT_L);
  n[1]=I2C_read(ADXL345_Address,ACCEL_ZOUT_H);
  return az;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1ISR(void)
{
  switch(P1IV)
  {
  case 0x00:
    break;
  case 0x02:
    _NOP();
    break;
  }
}
*/