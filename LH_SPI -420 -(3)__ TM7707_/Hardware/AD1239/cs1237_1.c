#include "cs1237_1.h"
#include "delay.h"
//#include "uart.h"

//���ߣ���С��
//������http://www.jixin.pro
//�ҵ�΢�ţ�253057617


// PA0-----CLK 
// PA1-----OUT

void Delay1us_1(void)
{
	__IO uint32_t t=5;   //5��  1us
	
	while(t--);
}

void Delay1ms_1(void)
{
	__IO uint32_t t=8000;
	
	while(t--);
}

void delay_ms_cs_1(__IO uint16_t ms)
{
	do{
		Delay1ms_1();
	}while(ms--);
}

// ��ʼ��PA2 PA3
// PA0-----CLK
// PA1-----OUT
void CS1237_1_Init_JX(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	// PA0 ----- CLK  ����Ϊ���
	// PA1 ----- OUT  ����Ϊ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK����
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	// OUT����
}

//����CS1237_1оƬ
void CS1237_1_Config(void)
{
	unsigned char i;
	unsigned char dat;
	unsigned int count_i=0;//�����ʱ��
	
	//dat = 0X1C;     //оƬ������ �ڲ�REF(00) ���40HZ(01) PGA=128��(11) ͨ��A 0X1C(00)  
    //dat = 0X18;	  //оƬ������ �ڲ�REF(00) ���40HZ(01) PGA=64��(10)  ͨ��A 0X1C 
	//dat = 0X14;	  //оƬ������ �ڲ�REF(00) ���40HZ(01) PGA=2��(01)   ͨ��A 0X1C
	//dat = 0X10;     //оƬ������ �ڲ�REF(00) ���40HZ(01) PGA=1��(00)   ͨ��A 0X1C
	dat =0x18;
  
/***********************************************************************************/	
	
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //OUT��������
	OUT_IN_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);// ʱ������
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1) //�ȴ�CS237׼����   300��dalay��==0���˳�
	{
		delay_ms(1);
		count_i++;
		if(count_i > 3000)
		{
			OUT_OUT_1();
			GPIO_SetBits(GPIOA, GPIO_Pin_1); // OUT��������
			GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK��������
			return;//��ʱ����ֱ���˳�����
		}
	}
	for(i=0;i<29;i++)// 1 - 29
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
	}
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_SetBits(GPIOA, GPIO_Pin_1);  GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//30     
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_SetBits(GPIOA, GPIO_Pin_1);  GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//31
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_ResetBits(GPIOA, GPIO_Pin_1);GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//32
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_ResetBits(GPIOA, GPIO_Pin_1);GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//33
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_SetBits(GPIOA, GPIO_Pin_1);  GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//34
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_ResetBits(GPIOA, GPIO_Pin_1);GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//35
	GPIO_SetBits(GPIOA, GPIO_Pin_0);Delay1us_1();GPIO_SetBits(GPIOA, GPIO_Pin_1);  GPIO_ResetBits(GPIOA, GPIO_Pin_0);Delay1us_1();//36
	//DA 1100101
	//37     д����0x65
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
	Delay1us_1();
	
	for(i=0;i<8;i++)// 38 - 45�������ˣ�д8λ����        д����
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		if(dat&0x80)
			GPIO_SetBits(GPIOA, GPIO_Pin_1);// OUT = 1
		else
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		dat <<= 1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_1);// OUT = 1
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
	Delay1us_1();
}

// ��ȡоƬ����������
unsigned char Read_Config_1(void)
{
	unsigned char i;
	unsigned char dat=0;//��ȡ��������
	unsigned int count_i=0;//�����ʱ��
//	unsigned char k=0,j=0;//�м����
	
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //OUT��������
	OUT_IN_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);//ʱ������
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//�ȴ�оƬ׼��������
	{
		delay_ms(1);
		count_i++;
		if(count_i > 400)
		{
			OUT_OUT_1();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_1);	// OUT=1;
			return 1;//��ʱ����ֱ���˳�����
		}
	}

	for(i=0;i<29;i++)// ������1��29��ʱ��
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
	}
	
	OUT_OUT_1();
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();// ���ǵ�30��ʱ��
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();// ���ǵ�31��ʱ��
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();//32
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();//33
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();//34
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();//35
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // CLK=0;
	Delay1us_1();//36
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
	Delay1us_1();//37     д��0x56 ��������
	
	dat=0;
	OUT_IN_1();
	for(i=0;i<8;i++)// ��38 - 45�������ˣ���ȡ����
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
		dat <<= 1;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)
			dat++;
	}
	
	//��46������
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
	Delay1us_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
	Delay1us_1();
	
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //OUT��������
	
	return dat;
}

//��ȡADC���ݣ����ص���һ���з�������
int32_t Read_CS1237_1(void)
{
	unsigned char i;
	uint32_t dat=0;//��ȡ��������
	unsigned int count_i=0;//�����ʱ��
	int32_t temp;
	
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //OUT��������
	OUT_IN_1();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);//ʱ������
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//�ȴ�оƬ׼��������
	{
		delay_ms(1);
		count_i++;
		if(count_i > 400)  //֮ǰ��300
		{
			OUT_OUT_1();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_1);	// OUT=1;
			return 1;//��ʱ����ֱ���˳�����
		}
	}
	
	dat=0;
	for(i=0;i<24;i++)//��ȡ24λ��Чת��
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		dat <<= 1;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)
			dat ++;
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
	}
	
	for(i=0;i<3;i++)//����ǰ���ʱ�� ����3��ʱ��
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// CLK=1;
		Delay1us_1();
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// CLK=0;
		Delay1us_1();
	}
	
	OUT_OUT_1();
	GPIO_SetBits(GPIOA, GPIO_Pin_1); // OUT = 1;

	
//	if(dat&0x00800000)// �ж��Ǹ��� ���λ24λ�Ƿ���λ
//	{
//		temp=-(((~dat)&0x007FFFFF) + 1);// �����Դ��
//	}
//	else
		temp=dat; // �����Ĳ������Դ��
	
	return temp;

}




