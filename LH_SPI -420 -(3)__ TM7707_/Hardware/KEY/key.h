#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"




#define KEYA  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)//��ȡ����+
#define KEYR  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//��ȡ����>
#define KEYY  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//��ȡ����END 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��					    
#endif
