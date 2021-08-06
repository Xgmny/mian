#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
/////////////////////////////////////////////////////////////////////////////////
#define i2c 0
//IO��������
#if i2c
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
 void SDA_IN(void);
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define WP    	   PBout(5)  
//#define IIC_SCL    PBout(6) //SCL
#define IIC_SCL_1    GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define IIC_SCL_0    GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

//IIC���в�������
void I1C_Init(void);                //��ʼ��IIC��IO��				 
void I1C_Start(void);				//����IIC��ʼ�ź�
void I1C_Stop(void);	  			//����IICֹͣ�ź�
void I1C_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 I1C_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 I1C_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void I1C_Ack(void);					//IIC����ACK�ź�
void I1C_NAck(void);				//IIC������ACK�ź�

void I1C_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 I1C_Read_One_Byte(u8 daddr,u8 addr);	  
#else 


//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
 void SDA_IN(void);
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define WP    	   PBout(5)  
//#define IIC_SCL    PBout(6) //SCL
#define IIC_SCL_1    GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define IIC_SCL_0    GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

//IIC���в�������
void I1C_Init(void);                //��ʼ��IIC��IO��				 
void I1C_Start(void);				//����IIC��ʼ�ź�
void I1C_Stop(void);	  			//����IICֹͣ�ź�
void I1C_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 I1C_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 I1C_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void I1C_Ack(void);					//IIC����ACK�ź�
void I1C_NAck(void);				//IIC������ACK�ź�

void I1C_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 I1C_Read_One_Byte(u8 daddr,u8 addr);	  





#endif

#endif
















