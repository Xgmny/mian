#ifndef __cs1237_1_H
#define __cs1237_1_H


#include "stm32f10x.h"

//OUT����������� ��������  PA3
#define OUT_IN_1()  {GPIOA->CRL&=0XFFFFFF0F;GPIOB->CRH|=8<<28;} 
//��0���� ;1000  ;   ����ģʽ;����/��������ģʽ
#define OUT_OUT_1() {GPIOA->CRL&=0XFFFFFF0F;GPIOB->CRH|=3<<28;} 
//��1���� ;0011  ;   ���ģʽ������ٶ�50MHz



void Delay1us_1(void);
void Delay1ms_1(void);
void delay_ms_cs_1(__IO uint16_t ms);
void CS1237_1_Init_JX(void);
void CS1237_1_Config(void);
unsigned char Read_Config_1(void);
int32_t Read_CS1237_1(void);

#endif


