/*
ÿ�ο���ʹ��ʱ��������Global.h�ж���UCAx_Used_���г�ʼ��ʹ��
ÿ��ʹ��ʱ�����ȵ���UCAx_init();
Ȼ�����ʹ�ö�Ӧ�ĺ���
*/
#include "Global.h"
#ifndef UART_H_
#define UART_H_
#define UCA0 0x01 //�������
#define UCA1 0x02
#define UCA2 0x04
#define UCA3 0x08
#define A0_RXBUF_SIZE 64  //UCA0 FIFO��С
#define A0_TXBUF_SIZE 64
#define A1_RXBUF_SIZE 64  
#define A1_TXBUF_SIZE 64
#define A2_RXBUF_SIZE 64  
#define A2_TXBUF_SIZE 64
#define A3_RXBUF_SIZE 64  
#define A3_TXBUF_SIZE 64
extern void UART_init(unsigned char UCAx,unsigned long int baud);//��ʼ��UCAx��baud��ѡ9600��115200
extern void UART_sendstr(unsigned char UCAx,char *str);//ʹ��UCAx�����ַ���str
extern void UART_sendint(unsigned char UCAx,unsigned int data);//ʹ��UCAx����unsigned int data
//extern void UART_sendlong(unsigned char UCAx,signed long data);//ʹ��UCAx����signed long data
#endif 
#ifdef UCA0_Used_
char UCA0_GET_RXBUFLEN(void);//��ȡRX��FIFO���ַ�����
char UCA0_GET_TXBUFLEN(void);//��ȡTX��FIFO���ַ�����
char UCA0_putchar(unsigned char data);//дһ���ַ���TX FIFO
char UCA0_GET_CHAR(unsigned char *ptr);//��RX FIFO�ж�һ���ֽ�
#endif
#ifdef UCA1_Used_
char UCA1_GET_RXBUFLEN(void);//��ȡRX��FIFO���ַ�����
char UCA1_GET_TXBUFLEN(void);//��ȡTX��FIFO���ַ�����
char UCA1_putchar(unsigned char data);//дһ���ַ���TX FIFO
char UCA1_GET_CHAR(unsigned char *ptr);//��RX FIFO�ж�һ���ֽ�
#endif
#ifdef UCA2_Used_
char UCA2_GET_RXBUFLEN(void);//��ȡRX��FIFO���ַ�����
char UCA2_GET_TXBUFLEN(void);//��ȡTX��FIFO���ַ�����
char UCA2_putchar(unsigned char data);//дһ���ַ���TX FIFO
char UCA2_GET_CHAR(unsigned char *ptr);//��RX FIFO�ж�һ���ֽ�
#endif
#ifdef UCA3_Used_
char UCA3_GET_RXBUFLEN(void);//��ȡRX��FIFO���ַ�����
char UCA3_GET_TXBUFLEN(void);//��ȡTX��FIFO���ַ�����
char UCA3_putchar(unsigned char data);//дһ���ַ���TX FIFO
char UCA3_GET_CHAR(unsigned char *ptr);//��RX FIFO�ж�һ���ֽ�
#endif