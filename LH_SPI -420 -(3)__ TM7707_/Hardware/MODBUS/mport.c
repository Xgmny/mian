#include "protocol.h"
#include "mcheck.h" 
#include "mport.h"

u8 QJ[6] ,QJp=0,JDL[8];
u16 QJs;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS �ӿڲ���������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2017-2027
//All rights reserved	 
////////////////////////////////////////////////////////////////////////////////// 
 

//����1�жϷ�����
//�����ͨ��������ʽ���ܣ��밴Ҫ��ʵ�����ݴ洢
//��һ֡���������Ĵ洢��m_ctrl_dev.rxbuf����,����֡���ݳ��ȴ洢��m_ctrl_dev.rxlen����

//void USART1_IRQHandler(void)
//{
//	u8 res;	
//	if(USART1->SR&(1<<5))			//���յ�����
//	{	 
//		res=USART1->DR; 
//		if(m_ctrl_dev.frameok==0)	//����δ���
//		{ 
//			m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
//			m_ctrl_dev.rxlen++;
//			if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1))m_ctrl_dev.rxlen=0;//�������ݴ���,���¿�ʼ����	  
//  		}  		 									     
//	}else if(USART1->SR&(1<<4))		//�����ж�
//	{
//		res=USART1->DR; 			//��DR�����IDLEλ
//		m_ctrl_dev.frameok=1;		//������һ֡���ݽ���	
//	}
//} 


void USART1_IRQHandler(void)
{
	u8 res;	
		if(USART1->SR&(1<<5))			//���յ�����
	{	 
		res=USART1->DR; 
		if(res==02 && QJp==0)QJp=1;//QJp=1
		else if(res==03 && QJp==1)QJp=2;//QJp=2
		else if(res==04 && QJp==2)QJp=3;//QJp=3
		else if(QJp>2&&QJp<6) {    //3~5
								QJ[QJp-3]=res;
								QJp++;
							   }
		else if(QJp==6) QJp++; //7
		else if(QJp==7) QJp++; //8
		else if(QJp==8) {
						 QJs=(QJ[0]*10000)+	(QJ[1]*100)+(QJ[2]);
		    			 QJp=0;
			
		                 }
	 									     
	}else if(USART1->SR&(1<<4))		//�����ж�
	{
		res=USART1->DR; 			//��DR�����IDLEλ
		m_ctrl_dev.frameok=1;		//������һ֡���ݽ���	
	}
} 






//��ʼ���ӿ�
//��������ʹ�ô���1���ø����Լ�����Ҫ�޸�
//����1���������ÿ����жϣ���֡�����жϣ���������ӿڣ�û�����ƿ���״̬���绰
//�����Լ�ʵ��֡���ݽ�ȡ�������ó�ʱ����
//pclk2��PCLK2ʱ��;
//bound��������


void mp_init(u32 pclk2,u32 bound)
{  	  
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=(pclk2*1000000)/bound;
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ
	USART1->CR1|=1<<4;	  //�����������߿����ж�. 
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
//	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ�  
}


//����ָ�����ȵ����� 
//buf��Ҫ���͵����ݻ������׵�ַ
//len��Ҫ���͵����ݳ���
void mp_send_data(u8* buf, u16 len)		
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		while((USART1->SR&0X40)==0);	//�ȴ���һ�δ������ݷ������  
		USART1->DR=buf[i];      		//дDR,����1����������
	}	
}


void QingJiao_F(void)//��  ��ȡ�Ƕ�����
{
    u8 fx[8];	
	fx[0]=0x02;
	fx[1]=0x03;
	fx[2]=0x00;
	fx[3]=0x00;
	fx[4]=0x00;
	fx[5]=0x02;
	fx[6]=0xc4;
	fx[7]=0x38;
mp_send_data(fx,8)	;
  
	QJp=0;

}	













