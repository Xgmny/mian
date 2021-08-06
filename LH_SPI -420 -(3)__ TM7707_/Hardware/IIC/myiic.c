#include "myiic.h"
#include "delay.h"


#if i2c 
//��ʼ��IIC
void I1C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 	//PB5,PB6,PB7 �����
}

void SDA_IN(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //�������

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_7); 	//PB5,PB6,PB7 �����
}
//����IIC��ʼ�ź�
void I1C_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL_1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void I1C_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 I1C_Wait_Ack(void)
{
	
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL_1;delay_us(1);	 
	
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))   //f
//	while(READ_SDA)   //f
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I1C_Stop();
			return 1;
		}
	}
	IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void I1C_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}
//������ACKӦ��		    
void I1C_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I1C_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
		delay_us(2); 
		IIC_SCL_0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 I1C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL_0; 
        delay_us(2);
		IIC_SCL_1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I1C_NAck();//����nACK
    else
        I1C_Ack(); //����ACK   
    return receive;
}
#else
//��ʼ��IIC
void I1C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //5:DA  6ʱ��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 	//PB5,PB6,PB7 �����
}


void SDA_IN(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //�������

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_7); 	//PB5,PB6,PB7 �����  AD
}

//����IIC��ʼ�ź�
void I1C_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;
delay_us(1);	
	IIC_SCL_1;
	delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(5);
}	  
//����IICֹͣ�ź�
void I1C_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_0;
    delay_us(5);
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(5);
	IIC_SCL_1; 
	delay_us(5);
	IIC_SDA=1;//����I2C���߽����ź�
							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 I1C_Wait_Ack(void)
{
	
	u16 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	//IIC_SDA=1;//
	delay_us(1);	 
	
	IIC_SCL_1;//delay_us(1); 
	
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))   //f
		
//	while(READ_SDA)   //
	{
		ucErrTime++;
		if(ucErrTime>205)
		{
			I1C_Stop();
			return 1;
		}
	}

	IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void I1C_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(4);
	IIC_SCL_1;
	delay_us(4);
	IIC_SCL_0;
}
//������ACKӦ��		    
void I1C_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(4);
	IIC_SCL_1;
	delay_us(4);
	IIC_SCL_0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I1C_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {     
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(4);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
		delay_us(4); 
		IIC_SCL_0;	

    }

} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 I1C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL_0; 
        delay_us(4);
		IIC_SCL_1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(4); 
    }					 
    if (!ack)
        I1C_NAck();//����nACK
    else
        I1C_Ack(); //����ACK   
    return receive;
}
#endif


























