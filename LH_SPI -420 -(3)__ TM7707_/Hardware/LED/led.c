#include "led.h"


//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PC.13 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 }


 void ADCx_Init(void)
 {
     GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����    
     ADC_InitTypeDef       ADC_InitStructure;
     
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
     
     RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
     
     GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//ADC
     GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;    //ģ������
     GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
     GPIO_Init(GPIOA,&GPIO_InitStructure);
     
     ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
     ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ    
     ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ������⣬ʹ���������
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���    
     ADC_InitStructure.ADC_NbrOfChannel = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
     ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
     
     ADC_Cmd(ADC1, ENABLE);//����ADת����
     
     ADC_ResetCalibration(ADC1);//����ָ����ADC��У׼�Ĵ���
     while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ�����״̬
     
     ADC_StartCalibration(ADC1);//��ʼָ��ADC��У׼״̬
     while(ADC_GetCalibrationStatus(ADC1));//��ȡָ��ADC��У׼����
 
     ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
 }
 /*******************************************************************************
 * �� �� ��         : Get_ADC_Value
 * ��������           : ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��     
 * ��    ��         : ch:ͨ�����
                      times:��ȡ����
 * ��    ��         : ͨ��ch��times��ת�����ƽ��ֵ
 *******************************************************************************/
 u16 Get_ADC_Value(u8 ch,u8 times)
 {
     u32 temp_val=0;
     u8 t;
     //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
     ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);    //ADC1,ADCͨ��,239.5������,��߲���ʱ�������߾�ȷ��                
     
     for(t=0;t<times;t++)
     {
         ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ����ADC1�����ת����������    
         while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
         temp_val+=ADC_GetConversionValue(ADC1);
    }
     return temp_val/times;
 } 
  /*******************************************************************************
 * �� �� ��         : PVD����
 * ��������         : ����   �ж�
 * ��    ��         : 
                      
 * ��    ��         : 
 *******************************************************************************/
 
 void PVD_Init(void)
{

EXTI_InitTypeDef EXTI_InitStructure;

//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
//PWR_PVDLevelConfig(PWR_PVDLevel_2V9); 
//PWR_PVDCmd(ENABLE);
	
EXTI_ClearITPendingBit(EXTI_Line16);
EXTI_InitStructure.EXTI_Line = EXTI_Line16;   // PVD���ӵ��ж���16��  
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //ʹ���ж�ģʽ 
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//��ʾ��ѹ�Ӹߵ�ѹ�½��������趨�ĵ�ѹ��ֵ�����ж�
EXTI_InitStructure.EXTI_LineCmd = ENABLE; // ʹ���ж���  
EXTI_Init(&EXTI_InitStructure);   //��ʼ��
	
	PVD_NVIC();
}

void PVD_NVIC(void)
{
NVIC_InitTypeDef NVIC_InitStructure;

/* Configure one bit for preemption priority */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

/* Enable the PVD Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
PWR_PVDLevelConfig(PWR_PVDLevel_2V9); 
PWR_PVDCmd(ENABLE);
}

