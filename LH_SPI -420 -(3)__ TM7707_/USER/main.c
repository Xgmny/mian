#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "24c64.h"	
#include "wdg.h"
#include "timer.h"
#include "usart.h"
#include "usmart.h"
#include "oled.h"
#include "setcs.h"
#include "can.h"
#include "gui.h"
#include "pic.h"
#include "CS1239.h"
#include "TM7707.h"
#include "mport.h"
#include "IIC__.h"



    u8 lwd[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 lwd_pa[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 lll[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 ljl[8]={0x2b,0x33,0x32,0x30,0x30,0x32,0x2e,0x30};
	u8 lsl[8]={0x2b,0x33,0x32,0x2e,0x30,0x32};
	u8 cyl[8]={0x2d,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 jdl[8]={0x2b,0x33,0x32,0x2e,0x30,0x32};
	
	int32_t LJ;
	int32_t LL;
	u8 ms1000=0;
	u16 A;
	u16 SZ_JZ_Z[10], SZ_JZ_F[10] ;                     //   %��У��
	u16 SZ_LD_Z, SZ_LD_F, SZ_QC_Z, SZ_QC_F;       //   0��    �г�
	int32_t  SZ_LL_Z,SZ_LL_F;
	
   extern u32 NIAN;
   extern u8 key,se30,sec3,sec,ljks,slj3,slj30,have;
   extern u8 idata,date8[];
   extern u16 QJs;
   u8 rxd_bz,canbuf_rxd[8];
	
 int main(void)
 {	
	 
	u8 cnt=0,old_key=0,hmqh,run;
	u8 canbuf_txd[8]={0x00,0x00,0x32,0x30,0x30,0x30,0x2e,0x30};

	delay_init();	    	 //��ʱ������ʼ��	
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��		
	AT24CXX_Init();			//IIC��ʼ�� 
	TM7707_init( );
	OLED_Init();
	FIRST(); ljks=1;hmqh=0;
	TIM4_init(1999,7199);//��ʱ��4 200����
//	WBZD_Init();

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,0);//CAN��ʼ,������250Kbps 
									//������  mode:0,��ͨģʽ;1,�ػ�ģʽ; //  
//   IWDG_Init(4,6250);      //��10s
	 run=1;      //����
//	LED0=1;        
 	while(1)
	{

		if (rxd_bz)//���յ�������
		{
			if (canbuf_rxd[0]=='O')
			{
				if((canbuf_rxd[1]=='U')&(canbuf_rxd[2]=='T'))
				      {run=1;cnt=0;FIRST();}//����
			}
			else
			{
				if (canbuf_rxd[0]=='$')
				{	
					if (canbuf_rxd[1]=='Y')
						{	run=2;	
							CAN_Answer();
						}
				}	
				else 
					CAN_SAVE();
			}
		}
		if (run<2)
		{
			if(ms200==1)			//200����
			{

				key=KEY_Scan();
				if (key==4) 
					{
						if(have==0)
								{have=1;sec3=0;slj3=0;}
							else
							{
								if(sec3) 
								{
									sec3=0;have=0;
									if(run==1)
									{
										run=0;   SET_IN();   //����
									}
									else 
									{
										BL_24c64();  //??????????
										run=1;  cnt=0; FIRST(); }
									}
							}			
						}		
				else
					{
						if (have)
							have=0;
						if(run==1)
							{
								if((key==0)&(old_key==3))
								{
									if (hmqh>=1)
										{	hmqh=0;FIRST();}
									else
											{hmqh=1;FIRST_2();}
								}else;
							if(ms1000==4)
							  {	
								  ms1000=0;
								if (!hmqh)	
								{	
									 GUI_ShowString(34, 3,lll,8,16,1); // ����
						             GUI_ShowString(34,24,ljl,8,16,1);	
						             GUI_ShowString(34,45,cyl,8,16,1);	
								}
								else{
									GUI_ShowString(4, 21,lwd,5,16,1); 
			//						GUI_ShowNum(34,24,QJs,7,16,1); 
									GUI_ShowString(70,21,lwd_pa ,4,16,1);	//���	jdl				
									GUI_ShowString(34,45,jdl,8,16,1);	//
									}
							  }else ms1000++;
							
							old_key=key;
									
							}
							else
								SET_COME();	//����
						}
				  if (run==1)	
					{
						 Made_Data();		//ADת��	
						{LJLL_Data();se30=0;}			//=1�ۼ�ʹ��  30��
						
						cnt++;
						if (cnt>=255) cnt=0;
						canbuf_txd[0]=cnt;

						canbuf_txd[2]=LL/100>>8;
						canbuf_txd[3]=LL/100;
						canbuf_txd[4]=LJ>>24;
						canbuf_txd[5]=LJ>>16;
						canbuf_txd[6]=LJ>>8;
						canbuf_txd[7]=LJ;
						Can_Send_Msg(myid,canbuf_txd,8);//����8���ֽ� 
					}
				ms200=0;  
			}
		}	
		
	}
}
