#ifndef HMC5883_H_
#define HMC5883_H_
#define	SlaveAddress   0x3C	  //����������IIC�����еĴӵ�ַ
extern void HMC5883_init(void);
extern void Multiple_read_HMC5883(unsigned char *BUF);
extern int HMC5883_Get_x(void);//��ȡx����ֵ
extern int HMC5883_Get_y(void);//��ȡy����ֵ
extern int HMC5883_Get_z(void);//��ȡz����ֵ
unsigned int HMC5883_Get_angle(void);//��ȡ�Ƕ�ֵ,û���
#endif 