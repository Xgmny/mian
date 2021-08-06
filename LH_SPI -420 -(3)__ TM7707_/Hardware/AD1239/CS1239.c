#include "CS1239.h"
//#include "uart.h"


//OUT����������� ��������  PA3
//#define OUT_IN()  {GPIOB->CRH &= 0X0FFFFFFF; GPIOB->CRH|=0x40000000;} 
//0x8 ;1000  ;   ����ģʽ;����/��������ģʽ
//0x4  0100      ����
//#define OUT_OUT() {GPIOB->CRH &= 0X0FFFFFFF; GPIOB->CRH|=0x30000000;} 
//0x3 ;0011  ;   ���ģʽ������ٶ�50MHz
//0xb ;1011       �������

// PB13-----CLK
// PB15-----OUT
void Delay1us(void)
{
	__IO uint32_t t=10;   //5��  1us
	
	while(t--);
}

void Delay1ms(void)
{
	__IO uint32_t t=8000;
	
	while(t--);
}

void delay_ms_cs(__IO uint16_t ms)
{
	do{
		Delay1ms();
	}while(ms--);
}

// ��ʼ��PA2 PA3
// PA2-----CLK
// PA3-----OUT

void OUT_IN(void){

	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_15;		
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IPU; 		
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
ndelay(1);

}

void OUT_OUT(void){

	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
ndelay(1);
}


void CS1239_Init(void)
{	
 // u8 i, a[10]={0x14,0x22,0x40,0x00,0x40,0x03,0x00,0x00,0x00,0x00}; //�¶�
 //	22:100 GND,010 AIN2; 40(02H):40Hz 1���Ŵ� ; 03:VS 3.0V
  //  u8 i, a[10]={0x10,0x00,0x5d,0x00,0x40,0x01,0x10,0x00,0x00,0x00}; //AIN0~AIN1
  //10:����ģʽ ;ADC��ʹ��      5d:40Hz,Buffer�ر�;PGA32,ADGN2(64��)  5c(32��)
	GPIO_InitTypeDef  GPIO_InitStructure;					
//	 u8 i, a[10]={0x14,0x00,0x5d,0x00,0x40,0x01,0x10,0x00,0x00,0x00};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	// PA13 ----- CLK  ����Ϊ���
	// PA15 ----- OUT  ����Ϊ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_12;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_13);	// CLK����
	GPIO_SetBits(GPIOB, GPIO_Pin_15);	// OUT����
	GPIO_SetBits(GPIOB, GPIO_Pin_12);  //д��
	CS_H ();
	SCK_H ();
	SDA_H ();


CS1239_io_write(0x00, 0x14);   // ����ģʽ              ʹ��ADC����
CS1239_io_write(0x01, 0xc0);   // ����ѡ��:AIN0~AIN1    ����Ƶ��:662.22KHZ
CS1239_io_write(0x02, 0x0d);   // 10Hz                  64��   


 Delay1ms();

		  CS1239_io_write(0x00, 0x14);   //����ģʽ   AD����
		  CS1239_io_write(0x01, 0xc0);
		  CS1239_io_write(0x02, 0x5d);	//7:80HZ 5:40HZ     d64��   Buffer��
		  CS1239_io_write(0x02, 0x4d);	//7:80HZ 5:40HZ     d64��   Buffer��
	 Delay1ms();

  ndelay(100);
}


static void ndelay(uint32_t time) 
{           
//    while(time--); //time=1,,500NS
} 
    
      
static void send(uint8_t data)
{ 
    uint8_t i; 
    for(i = 0; i < 8; i++) {
        SCK_H();          //low
        if(data & 0x80) {
            SDA_H();
        } else {
            SDA_L();
        }
        data <<= 1;  
		ndelay(5);
        SCK_L() ;             //high
    } 
     SCK_L();           //low
}

static uint8_t read(void) 
{
    uint8_t i;
    uint8_t data;
    data = 0;
    for(i = 0; i < 8; i++ ) {
        SCK_L();          			//low
        SCK_H() ;               //high
        data <<= 1; 
        ndelay(3);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)) data++;else; 
    }    
    SCK_L();//low
	ndelay(5);   
    return data;        
}



/**
  * @fn int32_t CS1239_io_read(uint8_t addr, uint8_t* buf)
  * @brief  Read the sigma-delta ADC register value with address addr and store its value in the buf.
  * @param  addr:8-bit sigma-delta ADC register address value
  * @param  buf: 8-bit sigma-delta ADC register address value output value
  * @return 0:OK 
  */
int32_t CS1239_io_read(uint8_t addr, uint8_t* buf)
{
    SCK_L();
ndelay(1);
    CS_L();
    OUT_OUT();

    send(addr&0x7F);  //д0
    OUT_IN();
	
    buf[0] = read();
    if (addr == 0x09)  //����λ
    {
        buf[1] = read();
        buf[2] = read();
    }
    CS_H();
    ndelay(1);
	SCK_H();
		
//	ndelay(5);//����
//	SCK_L();  
return 0;
}


int32_t CS1239_read(uint8_t addr)
{
	u8 a;
    SCK_L();
    CS_L();
	ndelay(3);
    OUT_OUT();
	ndelay(3);
    send(addr&0x7F);  //д0
    OUT_IN();
    a=read();
    CS_H();
    ndelay(3);
	SCK_H();
	
//	 ndelay(5);//����
//	SCK_L();  
return a;
}

/**
  * @fn int32_t CS1239_io_write(uint8_t addr, uint8_t value)
  * @brief  Write the sigma-delta ADC register value with address addr
  * @param  addr:8-bit sigma-delta ADC register address value
  * @param  value: 8-bit sigma-delta ADC register address value output value
  * @return 0:OK 
  */
int32_t CS1239_io_write(uint8_t addr, uint8_t value)
{
	SCK_L();
    OUT_OUT();
	ndelay(5);
    CS_L();

    send(addr|0x80); //��һλд1 ��д�����
	ndelay(5);
    send(value);

    CS_H();
    OUT_IN();
	ndelay(5);
	SCK_H();
	ndelay(5);
	
//		 ndelay(5);//����
//	SCK_L();  
    return 0;
}


void ADC_EN(void){CS1239_io_write(0x00, 0x14);}  //ADCʹ��

void ADC_DIS(void){CS1239_io_write(0x00, 0x10);}	//ADC�ر�
  
void AD_AD(uint8_t* buf){


//      if(!(((CS1239_read(0x0A))&0x80)))		  //�ж��Ƿ�������
//	  {
          CS1239_io_read(0x09, buf);   //09������

		  CS1239_io_write(0x00, 0x14);   //����ģʽ   AD����
		  CS1239_io_write(0x01, 0x00);
		  CS1239_io_write(0x01, 0x00);
		  CS1239_io_write(0x01, 0x00);
	      CS1239_io_write(0x02, 0x5d);	//5:40HZ    7:80HZ  64�� 
	      CS1239_io_write(0x02, 0x5d);	//7:80HZ  d64�� 
		  CS1239_io_write(0x02, 0x5d);	//7:80HZ  d64�� 

//	  }
	    
}

void AD_WD(uint8_t* buf){
	
	

//      if(!(((CS1239_read(0x0A))&0x80)))		  //�ж��Ƿ�������
//	  {
	    CS1239_io_read(0x09, buf);   //09������

		CS1239_io_write(0x00, 0x14);     //����ģʽ   AD����
	    CS1239_io_write(0x01, 0x22);     //GND   AIN2
		CS1239_io_write(0x01, 0x22);     //GND   AIN2
	    CS1239_io_write(0x01, 0x22);     //GND   AIN2
		CS1239_io_write(0x02, 0x50);	//5:40Hz   2�� 
	    CS1239_io_write(0x02, 0x50);	//7:80Hz   2��    
	    CS1239_io_write(0x02, 0x50);	//7:80Hz   2�� 

//  }

//      if((CS1239_read(0x0A))&0x80)//�ж��Ƿ�������
//	    {
//	    CS1239_io_read(0x09, buf);   //09������
//		}
//    else ;	

	   		
}

