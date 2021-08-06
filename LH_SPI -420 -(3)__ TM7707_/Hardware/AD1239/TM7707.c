#include "TM7707.h"

u8 kan;
u8 TM;  //7705_1  TM=1    7705_2  TM=0 
// Target : TM7707



enum
{
	/* �Ĵ���ѡ��  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* ͨ�żĴ��� */
	REG_SETUP	= 0x10,	/* ���üĴ��� */
	REG_CLOCK_H	= 0x20,	/* �˲����ͼĴ��� �� */
	REG_CLOCK_L	= 0x50,	/* �˲����ͼĴ��� �� */
	REG_DATA	= 0x30,	/* ���ݼĴ��� */
	REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
	REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
	REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
	REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

	/* ��д���� */
	WRITE 		= 0x00,	/* д���� */
	READ 		= 0x08,	/* ������ */

	/* ͨ�� */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};
/* ���üĴ���bit���� */
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ */
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

	GAIN_1			= (0 << 3),	/* ���� */
	GAIN_2			= (1 << 3),	/* ���� */
	GAIN_4			= (2 << 3),	/* ���� */
	GAIN_8			= (3 << 3),	/* ���� */
	GAIN_16			= (4 << 3),	/* ���� */
	GAIN_32			= (5 << 3),	/* ���� */
	GAIN_64			= (6 << 3),	/* ���� */
	GAIN_128		= (7 << 3),	/* ���� */

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* ������ */
};

/* �˲����ͼĴ����� */
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */
       BST_1      = 0x20,     //�����

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
			��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
			��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};
/* �˲����ͼĴ����� */
enum
{
FS7		=(7 << 1),
FS6		=(6 << 1),
FS5		=(5 << 1),
FS4		=(4 << 1),
FS3		=(3 << 1),
FS2		=(2 << 1),	
FS1		=(1 << 1),
FS0		=(1 << 0)	
};

//u8 *intp;
//u8 idata,date8[180];
//void write_byte1(u8 date) //дһ�ֽ�����
//{
// u8 i;
// CS_1=0;    //CS ���ͣ�

//	for(i=0;i<8;i++)
//	{
//		SCLK=0; delay_us(2); //SCLK ����׼��д����
//		
//		if(date&0x80)  DIO=1;
//		else           DIO=0;
//		
//		SCLK=1; delay_us(2); // ���ߣ�д��һλ����
//		date<<=1;
//	}
//	CS=1; //д��һ���ֽں�CS ���ߡ�
//}


//u8 read_byte1 () //��һ�ֽ�����
//{
//	u8 j,b1;
//	CS=0;
//	for(j=0;j<8;j++)
//	{
//		SCLK=0; delay_us(2);
//		b1=(b1<<1)|DII;
//		SCLK=1; delay_us(2);
//	}
//		return b1;
//}


//void save_data(void)
//{
//	u8 i0,i1;
//	intp=date8;
//  for(i0=0;i0<60;i0++)
//     {
//		while(DRDY); //�ȴ�DRDT Ϊ��ʱ,�ɶ�������
//		write_byte1(0x38); //��ͨ�żĴ���д�����ݣ�ѡ��ͨ��1 ��Ϊ��Ч������һ

//		for(i1=0;i1<3;i1++) //�����洢һ�����ݡ�
//		{
//		 *intp=read_byte1();
//		  intp++;
//		}
//		 CS_1=1;
//      }
//}


void TM7707_init(void) //��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	
	                                // 10 CS_1   11 CS_2	 12 REST;    13 SCLK      15 DIN;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 	//	GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_14;
	                                //  8 DRDY_1;  9 DRDY_2;   14 DOUT    
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING ;		
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_SetBits (GPIOB,GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13);   //CS, REST����  
 
	delay_ms(5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	delay_ms(100);
	GPIO_SetBits (GPIOB,GPIO_Pin_12);
	delay_ms(100);
//write_byte1(0X10); //��ͨ�żĴ���д����,ѡ��ͨ��1 ��Ϊ��Чͨ��������һ��������Ϊ�����üĴ�������д������
//write_byte1(0X20); //�����üĴ���д������ѡ��16 ������,��BUF ������,˫���ԡ�
//write_byte1(0X50); //��ͨ�żĴ���д���ݣ�ѡ��ͨ��1 ��Ϊ��Чͨ��,����һ��������Ϊ���˲��ͼĴ�������д������
//write_byte1(0X00); //���˲��ͼĴ���д��0X00��
//write_byte1(0X20); //��ͨ�żĴ���д���ݣ�ѡ��ͨ��1 ��Ϊ��Чͨ��,����һ��������Ϊ���˲��߼Ĵ�������д������
//write_byte1(0X0F); //���˲��߼Ĵ���д��0X0F��
 //	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
//	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1
//	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x50);//ain1  

    TM=1;
	 TM7705_CalibSelf(1);
	 
	TM=0;
	 TM7705_CalibSelf(2);
delay_ms(600);
//	  write_byte1(0X20);  write_byte1(0X25);  //�˲����߼Ĵ���   �����
	
}

void DIO_IN( ) //��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_14;		
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING; 		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DIO_OUT( ) //��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_14;		
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Delay
*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 5; i++);
}

/*


*********************************************************************************************************
*	�� �� ��: TM7705_SyncSPI
*	����˵��: ͬ��TM7705оƬSPI�ӿ�ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_SyncSPI(void)
{
	/* AD7705���нӿ�ʧ�����临λ����λ��Ҫ��ʱ500us�ٷ��� */
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);	
	CS1_1();
	CS2_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Send8Bit(uint8_t _data)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_0();
		_data <<= 1;
		TM7705_Delay();
		SCK_1();	
		TM7705_Delay();	
	
	}

}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t TM7705_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;
	for (i = 0; i < 8; i++)
	{
		SCK_0();
		TM7705_Delay();		
		read = read<<1;
		if (DO_IS_HIGH())
		{
			read++;
		}
		SCK_1();		
		TM7705_Delay();
	}

	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteByte
*	����˵��: д��1���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void TM7705_WriteByte(uint8_t _data)
{
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	
	TM7705_Send8Bit(_data);
	
	CS2_1();
	CS1_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Write3Byte
*	����˵��: д��3���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Write3Byte(uint32_t _data)
{
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	TM7705_Send8Bit((_data >> 16) & 0xFF);
	TM7705_Send8Bit((_data >> 8) & 0xFF);
	TM7705_Send8Bit(_data);
	CS1_1();
	CS2_1();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadByte
*	����˵��: ��ADоƬ��ȡһ���֣�16λ��
*	��    ��: ��
*	�� �� ֵ: ��ȡ���֣�16λ��
*********************************************************************************************************
*/
static uint8_t TM7705_ReadByte(void)
{
	uint8_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	read = TM7705_Recive8Bit();
	CS1_1();
	CS2_1();
	
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read2Byte
*	����˵��: ��2�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�����ݣ�16λ��
*********************************************************************************************************
*/
static uint16_t TM7705_Read2Byte(void)
{
	uint16_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	CS1_1();
	CS2_1();

	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read3Byte
*	����˵��: ��3�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�������ݣ�24bit) ��8λ�̶�Ϊ0.
*********************************************************************************************************
*/
 uint32_t TM7705_Read3Byte(void)
{
	uint32_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	delay_us(1);
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	CS1_1();
	CS2_1();
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WaitDRDY
*	����˵��: �ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 5000000; i++)
	{
		if(TM)
		  {  if (DRDY1_IS_LOW()) {break;}	else;  }	
		else
		  {  if (DRDY2_IS_LOW()) {break;}	else;  }
	}
	if (i >= 2000000)
	{
//		 TM=1;
//	 TM7705_CalibSelf(1);
//	       TM=0;
//	 TM7705_CalibSelf(2);
//	  delay_ms(100);	

	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */		
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK_H:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return;
	}

	TM7705_WriteByte(_RegID | WRITE);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue);
	}
	else	/* 24bit */
	{
		TM7705_Write3Byte(_RegValue);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: �����ļĴ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK_H:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return 0xFFFFFFFF;
	}

	TM7705_WriteByte(_RegID | READ);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 16)
	{
		read = TM7705_Read2Byte();
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte();
	}
	else	/* 24bit */
	{
		read = TM7705_Read3Byte();
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_CalibSelf
*	����˵��: ������У׼. �ڲ��Զ��̽�AIN+ AIN-У׼0λ���ڲ��̽ӵ�Vref У׼��λ���˺���ִ�й��̽ϳ���
*			  ʵ��Լ 180ms
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_CalibSelf(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* ��У׼CH1 */
	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1
	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x10);//ain1
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */		
		TM7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */

	}
	else if (_ch == 2)
	{
		/* ��У׼CH2 */
	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1//
	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x10);//ain1
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */


		
		//		TM7705_WaitDRDY();	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibZero
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-�̽Ӻ�ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_ZERO_CH1) ��  TM7705_ReadReg(REG_ZERO_CH2) ��ȡУ׼������
*	��    ��: _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibZero(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
		TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibFull
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-����������ѹԴ��ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_FULL_CH1) ��  TM7705_ReadReg(REG_FULL_CH2) ��ȡУ׼������
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibFull(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
	//	TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
	//	TM7705_WaitDRDY();	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadAdc1
*	����˵��: ��ͨ��1��2��ADC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint32_t TM7705_ReadAdc(uint8_t _ch)
{
	uint8_t i;
	uint32_t read = 0;
	
	/* Ϊ�˱���ͨ���л���ɶ���ʧЧ����2�� */

	for (i = 0; i < 1; i++)
	{
		TM7705_WaitDRDY();		/* �ȴ�DRDY����Ϊ0 */		

		if (_ch == 1)
		{
			TM7705_WriteByte(0x38);
		}
		else if (_ch == 2)
		{
			TM7705_WriteByte(0x39);
		}

		read = TM7705_Read3Byte();
	}
	return read;	
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

