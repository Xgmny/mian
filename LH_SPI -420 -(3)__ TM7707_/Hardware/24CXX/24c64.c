
#include "24c64.h" 
#include "delay.h"

	extern u8 ljl[];
	extern int32_t LJ;
	extern int32_t LL;
	extern u16 SZ_JZ_Z[], SZ_JZ_F[];                     //   %��У��
	extern u16 SZ_LD_Z, SZ_LD_F, SZ_QC_Z, SZ_QC_F;       //   0��    �г�
	extern int32_t  SZ_LL_Z,SZ_LL_F;		                 //   ����������  
           u8  JNW[8] ;
		   u32 JNS;
           u32 NIAN; 
//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	u8 kl[8];
	I1C_Init();
    WP=0;
	delay_ms(2);
	AT24CXX_Read(0X0100,kl,4);
	if((kl[0]!=0x38)|(kl[1]!=0x36)|(kl[2]!=0x4f)|(kl[3]!=0x4b))
	{
		kl[0]=0x38;kl[1]=0x36;kl[2]=0x4f;kl[3]=0x4b;
		AT24CXX_Write(0X0100,kl,4);
		kl[0]=0x30;kl[1]=0x30;kl[2]=0x30;
		AT24CXX_Write(0X0108,kl,3);
		kl[0]=0x30;kl[2]=0x30;kl[1]=0x2e;kl[3]=0x30;kl[4]=0x30;//����г�
		AT24CXX_Write(0x0110,kl,5);
		AT24CXX_Write(0x0170,kl,5);
		kl[1]=0x30;kl[2]=0x2e;
		AT24CXX_Write(0x0118,kl,5);
		AT24CXX_Write(0x0178,kl,5);
		kl[0]=0x31;kl[1]=0x2e;kl[2]=0x30;kl[3]=0x30;kl[4]=0x30;kl[5]=0x30;kl[6]=0x30;
		AT24CXX_Write(0x0120,kl,5);AT24CXX_Write(0x0180,kl,5);
		AT24CXX_Write(0x0128,kl,5);AT24CXX_Write(0x0130,kl,5);AT24CXX_Write(0x0138,kl,5);
		AT24CXX_Write(0x0140,kl,5);AT24CXX_Write(0x0148,kl,5);AT24CXX_Write(0x0150,kl,5);
		AT24CXX_Write(0x0158,kl,5);AT24CXX_Write(0x0160,kl,5);AT24CXX_Write(0x0168,kl,5);
		AT24CXX_Write(0x0188,kl,5);AT24CXX_Write(0x0190,kl,5);AT24CXX_Write(0x0198,kl,5);
		AT24CXX_Write(0x01a0,kl,5);AT24CXX_Write(0x01a8,kl,5);AT24CXX_Write(0x01b0,kl,5);
		AT24CXX_Write(0x01b8,kl,5);AT24CXX_Write(0x01c0,kl,5);AT24CXX_Write(0x01c8,kl,5);
		AT24CXX_Write(0x0050,kl,5);//WDXS
		kl[0]=0x2B;kl[1]=0x30;kl[2]=0x30;kl[3]=0x2E;kl[4]=0x30;kl[5]=0x30;
		AT24CXX_Write(0x0058,kl,6);//WDLD
		kl[0]=0x2B;kl[1]=0x30;kl[2]=0x30;kl[4]=0x2E;kl[3]=0x30;kl[5]=0x30;
		AT24CXX_Write(0x0060,kl,6);//WDZ
		AT24CXX_Write(0x0070,kl,6);//QJZ
		kl[0]=0x2D;
		AT24CXX_Write(0x0068,kl,6);//WDF
		AT24CXX_Write(0x0078,kl,6);//QJF
		kl[0]=0x32;kl[1]=0x30;kl[2]=0x30;kl[4]=0x30;kl[3]=0x2e;kl[5]=0x30;kl[6]=0x30;
		AT24CXX_Write(0x01D0,kl,6);		
		AT24CXX_Write(0x01D8,kl,6);
		kl[0]=0x30;kl[1]=0x30;kl[2]=0x30;kl[3]=0x30;kl[4]=0x30;kl[5]=0x30;kl[6]=0x2e;kl[7]=0x30;
		AT24CXX_Write(0x01e0,kl,8);
		AT24CXX_Write(0x0200,kl,8);
		kl[0]=0x30;kl[1]=0x30;kl[2]=0x30;kl[3]=0x30;kl[4]=0x30;kl[5]=0x30;kl[6]=0x30;kl[7]=0x30;
	    AT24CXX_Write(0x01f0,kl,8);         //����
		AT24CXX_Write(0x0200+0x0008,kl,8);  //����
	}
		delay_ms(2);
		myid=A_N_24C64(3,kl,0x0108);
		myid*=16;   //16����
		myid=0x11940400+myid;
		NIAN=A_N_24C64(4,kl,0x01f0);   //1����
		NIAN*=16;   //16����
		JNS= A_N_24C64(8,JNW,0x0200+NIAN+0x0008);   //���� ��д����
		BL_24c64();
	WP=1;
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
	I1C_Stop();            //
	I1C_Start();
	I1C_Send_Byte(0XA0);	   //����д����
	I1C_Wait_Ack();
	I1C_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
	I1C_Wait_Ack();
    I1C_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	I1C_Wait_Ack();    
	I1C_Start();  	 	   
	I1C_Send_Byte(0XA1);           //�������ģʽ			   
	I1C_Ack();	 
    temp=I1C_Read_Byte(0);		   
    I1C_Stop();//����һ��ֹͣ����	
    
	return temp;
	
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    I1C_Start();  

		I1C_Send_Byte(0XA0);	    //����д����
		I1C_Wait_Ack();
		I1C_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ

	 
	I1C_Wait_Ack();	   
    I1C_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	I1C_Wait_Ack(); 	 										  		   
	I1C_Send_Byte(DataToWrite);     //�����ֽ�							   
	I1C_Wait_Ack();  		    	   
   I1C_Stop();//����һ��ֹͣ���� 
	delay_ms(5);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u8 DataToWrite,u8 Len)
{  	
	u8 t;
	WP=0;
	delay_ms(2);
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
	WP=1;	
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55) return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	//I1C_Stop();            //
	I1C_Start();
	I1C_Send_Byte(0XA0);	   //����д����
	I1C_Wait_Ack();
	I1C_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
	I1C_Wait_Ack();
    I1C_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	I1C_Wait_Ack();    
	I1C_Start();  	 	   
	I1C_Send_Byte(0XA1);           //�������ģʽ	
    I1C_Ack();		

	while(NumToRead)
	{
		*pBuffer=I1C_Read_Byte(NumToRead-1);
		NumToRead--;
		ReadAddr++;
		pBuffer++;
	}	
	 I1C_Stop();//����һ��ֹͣ����	
	delay_ms(10);
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	WP=0;
	I1C_Start();  
		I1C_Send_Byte(0XA0);	    	//����д����
		I1C_Wait_Ack();
		I1C_Send_Byte(WriteAddr>>8);	//���͸ߵ�ַ
     	I1C_Wait_Ack();	   
        I1C_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	    I1C_Wait_Ack(); 	
	while(NumToWrite)
	{
		I1C_Send_Byte(*pBuffer);     //�����ֽ�							   
		I1C_Wait_Ack();  		    	   
		NumToWrite--;
		pBuffer++;
	}
	I1C_Stop();							//����һ��ֹͣ���� 
	
	WP=1;
	delay_ms(10);
}

//void A_N_24C64(void)
//{

//   AT24CXX_Read(0X0200,ljl,8);
//    
//          
//		   LJ+=(ljl[1]-0x30)*100000  ;
//	       LJ+=(ljl[2]-0x30)*10000  ;
//		   LJ+=(ljl[3]-0x30)*1000  ;
//		   LJ+=(ljl[4]-0x30)*100  ;
//		   LJ+=(ljl[5]-0x30)*10  ;
//		   LJ+=(ljl[7]-0x30)*1  ;
// 
//       }

//ASCLL~����
//w λ��     aָ���ַ   ��d24c64���ڵ�ַ  
int32_t  A_N_24C64 (u8 w,u8 *a,u16 d)
{
 int32_t  n ,nn;
	u32 bei=1;
 	AT24CXX_Read(d,a,w);
	 for(;w>0;w--)
		{
		  if(a[w-1]<=58&&a[w-1]>=48)			
		  {
			n=bei*(a[w-1]-48);
			bei*=10 ;
			nn +=n ;
		  }
		}return nn;
}
	   
void BL_24c64(void){
	u8 kl[8];
	
	    SZ_LD_Z = A_N_24C64(5,kl,0x0110);     //���
	    SZ_QC_Z = A_N_24C64(5,kl,0x0118);   // �г�
        SZ_LL_Z = A_N_24C64(6,kl,0X01D0);    //��������

		SZ_JZ_Z[0]=A_N_24C64(5,kl,0x0120);	//<10%
		SZ_JZ_Z[1]=A_N_24C64(5,kl,0x0128);	//<~20%
		SZ_JZ_Z[2]=A_N_24C64(5,kl,0x0130);	//<~30%
		SZ_JZ_Z[3]=A_N_24C64(5,kl,0x0138);	//<~40%
		SZ_JZ_Z[4]=A_N_24C64(5,kl,0x0140);	//<~50%
		SZ_JZ_Z[5]=A_N_24C64(5,kl,0x0148);	//<~60%
		SZ_JZ_Z[6]=A_N_24C64(5,kl,0x0150);	//<~70%
		SZ_JZ_Z[7]=A_N_24C64(5,kl,0x0158);	//<~80%
		SZ_JZ_Z[8]=A_N_24C64(5,kl,0x0160);	//<~90%
		SZ_JZ_Z[9]=A_N_24C64(5,kl,0x0168);	//>90%	
	
		SZ_LD_F = A_N_24C64(5,kl,0x0170);     //���
	    SZ_QC_F = A_N_24C64(5,kl,0x0178);   // �г�
        SZ_LL_F = A_N_24C64(6,kl,0X01D8);    //��������

		SZ_JZ_F[0]=A_N_24C64(5,kl,0x0180);	//<10%
		SZ_JZ_F[1]=A_N_24C64(5,kl,0x0188);	//<~20%
		SZ_JZ_F[2]=A_N_24C64(5,kl,0x0190);	//<~30%
		SZ_JZ_F[3]=A_N_24C64(5,kl,0x0198);	//<~40%
		SZ_JZ_F[4]=A_N_24C64(5,kl,0x01A0);	//<~50%
		SZ_JZ_F[5]=A_N_24C64(5,kl,0x01A8);	//<~60%
		SZ_JZ_F[6]=A_N_24C64(5,kl,0x01B0);	//<~70%
		SZ_JZ_F[7]=A_N_24C64(5,kl,0x01B8);	//<~80%
		SZ_JZ_F[8]=A_N_24C64(5,kl,0x01C0);	//<~90%
		SZ_JZ_F[9]=A_N_24C64(5,kl,0x01C8);	//>90%	
		
		LJ = A_N_24C64(8,ljl,0x0200+NIAN);  //�ۼ���
}	









