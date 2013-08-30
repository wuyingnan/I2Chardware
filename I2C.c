#include"msp430f5438.h"
#include"stdint.h"
#define I2C_RXBUF_SIZE 2
#define I2C_TXBUF_SIZE 2

volatile unsigned char I2C_RXBUF[I2C_RXBUF_SIZE];
volatile unsigned char I2C_TXBUF[I2C_TXBUF_SIZE];
volatile unsigned char I2C_RXBUF_LEN,I2C_TXBUF_LEN;
volatile unsigned char I2C_TXBUF_R,I2C_TXBUF_W;
volatile unsigned char I2C_TXFIN,I2C_RXFIN,I2C_RXLEN,I2C_RXCNT;
volatile unsigned char* datadd;
volatile unsigned char isReading;

void I2C_init()
{
    P3SEL &= ~(BIT1+BIT2);
    P3DIR |= BIT2+BIT1;    
    P3OUT &= BIT2;
    P3OUT |= BIT1;
    // ���9��ʱ�� �Իָ�I2C����״̬
    for( char i = 0 ; i < 9 ; i++ )
    {
      P3OUT |= BIT2;
      __delay_cycles(8000);
      P3OUT &= ~BIT2;
      __delay_cycles(8000);
    }  
  
    // Configure pins
    P3SEL     |= BIT1 + BIT2;

    UCB0CTL1 |= UCSWRST;	            //Software reset enabled
    UCB0CTL0 |= UCMODE_3  + UCMST + UCSYNC;//I2C mode, Master mode, sync
    UCB0CTL1 |= UCSSEL_2;                  //SMCLK
    UCB0BR0  =  40;                        //4M/100K=40 
    UCB0BR1  =  0;
    UCB0CTL0 &= ~UCSLA10;
    UCB0CTL1  &=~UCSWRST;
    
    UCB0IE    |= UCRXIE + UCTXIE;
}

void I2C_reset(unsigned char slaveadd)
{
    UCB0CTL1 |= UCSWRST;	            //Software reset enabled
    UCB0CTL0 |= UCMODE_3  + UCMST + UCSYNC;//I2C mode, Master mode, sync
    UCB0CTL1 |= UCSSEL_2;                  //SMCLK
    UCB0BR0  =  10;                        //4M/400K=10 
    UCB0BR1  =  0;
    UCB0CTL0 &= ~UCSLA10;
    UCB0I2CSA = slaveadd;    
    UCB0CTL1  &=~UCSWRST;
    UCB0IE    |= UCRXIE + UCTXIE;
}

void I2C_write(unsigned char slaveadd,unsigned char add,unsigned char data)
{
  while( UCB0CTL1 & UCTXSTP );  
  _DINT();
  if (UCB0I2CSA != slaveadd)
    I2C_reset(slaveadd);
  UCB0CTL1 |= UCTR;                 // дģʽ
  UCB0CTL1 |= UCTXSTT;              // ��������λ  
  isReading=0;
  I2C_TXBUF_LEN=2;
  I2C_TXBUF_R=0;
  I2C_TXBUF[0]=add;
  I2C_TXBUF[1]=data;
  UCB0IE  |= UCTXIE;
  I2C_TXFIN=0;
  _EINT();
  while((!I2C_TXFIN) || (UCB0STAT&UCBUSY));
}

void I2C_read(unsigned char slaveadd,unsigned char regadd,unsigned char len,unsigned char *ramadd)
{
  while( UCB0CTL1 & UCTXSTP );  
  _DINT();
  if (UCB0I2CSA != slaveadd)
    I2C_reset(slaveadd);
  UCB0CTL1 |= UCTR;                 // дģʽ
  UCB0CTL1 |= UCTXSTT;              // ��������λ  
  isReading=1;
  I2C_TXBUF_LEN=1;
  I2C_TXBUF_R=0;
  I2C_TXBUF[0]=regadd;
  UCB0IE  |= UCTXIE;
  datadd=ramadd;
  I2C_RXFIN=0;
  I2C_RXCNT=0;
  I2C_RXLEN=len;
  _EINT();
  while(!I2C_RXFIN);
}

// USCI_B0 Data ISR
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(UCB0IV)
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4:                                 // Vector  4: NACKIFG
    UCB0IFG &=  ~UCNACKIFG;
    _NOP();
    break;  
  case  6:                                 // Vector  6: STTIFG SLAVE MODE
    break;
  case  8: break;                           // Vector  8: STPIFG SLAVE MODE
  case 10:                                  // Vector 10: RXIFG    
/*    if (I2C_RXBUF_LEN == I2C_RXBUF_MAXLEN-1)
    {
      UCB0CTL1 |= UCTXSTP;              // �ڽ������һ���ֽ�֮ǰ����ֹͣλ
    }*/
    datadd[I2C_RXCNT++] = UCB0RXBUF;	//Get data from UCB0RXBUF
    if (I2C_RXLEN-I2C_RXCNT==1)
      UCB0CTL1 |= UCTXSTP;    
    if (I2C_RXLEN-I2C_RXCNT==0)
      I2C_RXFIN=1;
    break;
  case 12:                                  // Vector 12: TXIFG
    if (I2C_TXBUF_LEN > 0)
    {
        I2C_TXBUF_LEN--;
        UCB0TXBUF = I2C_TXBUF[I2C_TXBUF_R++];	//Send data
        if (I2C_TXBUF_R == I2C_TXBUF_SIZE)
            I2C_TXBUF_R = 0;
    }
    else 
    {
      if (!isReading)
      {
        UCB0CTL1 |= UCTXSTP;
        I2C_TXFIN=1;
      }
      else
      {
        UCB0CTL1 &= ~UCTR;                // ��ģʽ        
        UCB0CTL1 |= UCTXSTT;        
        while(UCB0CTL1 & UCTXSTT);
        if (I2C_RXLEN==1)
          UCB0CTL1 |= UCTXSTP;
      }
      UCB0IE &= ~UCTXIE;				//Turn off transmit
    }
    break;
  default: break; 
  }
}

/*
void I2C_init()
{
  P3SEL &= ~BIT2;                         // P3.2@UCB0SCL
  P3DIR |= BIT2;
  P3OUT |= BIT2;
  // ���9��ʱ�� �Իָ�I2C����״̬
  for( uint8_t i = 0 ; i < 9 ; i++ )
  {
    P3OUT |= BIT2;
    __delay_cycles(8000);
    P3OUT &= ~BIT2;
    __delay_cycles(8000);
  }
  
  P3SEL |= (BIT1 + BIT2);                 // P3.1@UCB0SDA P3.2@UCB0SCL
  // P3.1@ISP.1 P3.2@ISP.5
  
  UCB0CTL1 |= UCSWRST;
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC ;  // I2C����ģʽ
  UCB0CTL1 |= UCSSEL_2;                   // ѡ��SMCLK
  UCB0BR0 = 40;
  UCB0BR1 = 0;
  UCB0CTL0 &= ~UCSLA10;                   // 7λ��ַģʽ
  UCB0I2CSA = 0x1e;            // ADXL345
  UCB0CTL1 &= ~UCSWRST;
}

uint8_t eeprom_readbyte( uint8_t word_addr , uint8_t *pword_value )
{
  UCB0CTL1 |= UCTR;                 // дģʽ
  UCB0CTL1 |= UCTXSTT;              // ��������λ��д�����ֽ�

  UCB0TXBUF = word_addr;            // �����ֽڵ�ַ������Ҫ�����TXBUF
  // �ȴ�UCTXIFG=1 ��UCTXSTT=0 ͬʱ�仯 �ȴ�һ����־λ����
  while(!(UCB0IFG & UCTXIFG))
  {
    if( UCB0IFG & UCNACKIFG )       // ����Ӧ�� UCNACKIFG=1
    {
      return 1;
    }
  }                        

  UCB0CTL1 &= ~UCTR;                // ��ģʽ
  UCB0CTL1 |= UCTXSTT;              // ��������λ�Ͷ������ֽ�

  while(UCB0CTL1 & UCTXSTT);        // �ȴ�UCTXSTT=0
  // ����Ӧ�� UCNACKIFG = 1
  UCB0CTL1 |= UCTXSTP;              // �ȷ���ֹͣλ

  while(!(UCB0IFG & UCRXIFG));      // ��ȡ�ֽ�����
  *pword_value = UCB0RXBUF;         // ��ȡBUF�Ĵ����ڷ���ֹͣλ֮��

  while( UCB0CTL1 & UCTXSTP );
  
  return 0; 
}

uint8_t eeprom_writebyte( uint8_t word_addr , uint8_t word_value )
{
  while( UCB0CTL1 & UCTXSTP );
  UCB0CTL1 |= UCTR;                 // дģʽ
  UCB0CTL1 |= UCTXSTT;              // ��������λ

  UCB0TXBUF = word_addr;            // �����ֽڵ�ַ
  // �ȴ�UCTXIFG=1 ��UCTXSTT=0 ͬʱ�仯 �ȴ�һ����־λ����
  while(!(UCB0IFG & UCTXIFG))
  {
    if( UCB0IFG & UCNACKIFG )       // ����Ӧ�� UCNACKIFG=1
    {
      return 1;
    }
  }   

  UCB0TXBUF = word_value;           // �����ֽ�����
  while(!(UCB0IFG & UCTXIFG));      // �ȴ�UCTXIFG=1

  UCB0CTL1 |= UCTXSTP;
  while(UCB0CTL1 & UCTXSTP);        // �ȴ��������
  
  return 0;
}

void I2C_write(unsigned char add,unsigned char data)
{
  eeprom_writebyte(add,data);
}


uint8_t I2C_read(unsigned char add)
{
  uint8_t temp;
  eeprom_readbyte(add,&temp);
  return temp;
}
*/