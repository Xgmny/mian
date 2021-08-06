#include "24cxx.h" 
#include "delay.h"



//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	u8 kl[8];
	I1C_Init();
	
	AT24CXX_Read(02,kl,4);
	if((kl[0]!=0x31)|(kl[0]!=0x35)|(kl[0]!=0x4f)|(kl[0]!=0x4c))
	{
		kl[0]=0x31;kl[1]=0x35;kl[2]=0x4f;kl[3]=0x4c;
		AT24CXX_Write(02,kl,4);
		kl[0]=0x30;kl[1]=0x30;kl[2]=0x2e;kl[3]=0x30;kl[4]=0x30;
		AT24CXX_Write(0x10,kl,5);AT24CXX_Write(0x18,kl,5);
		AT24CXX_Write(0x70,kl,5);AT24CXX_Write(0x78,kl,5);
		kl[0]=0x31;kl[1]=0x2e;kl[2]=0x30;kl[3]=0x30;kl[4]=0x30;kl[5]=0x30;kl[6]=0x30;
		AT24CXX_Write(0x20,kl,5);AT24CXX_Write(0x80,kl,5);
		AT24CXX_Write(0x28,kl,5);AT24CXX_Write(0x30,kl,5);AT24CXX_Write(0x38,kl,5);
		AT24CXX_Write(0x40,kl,5);AT24CXX_Write(0x48,kl,5);AT24CXX_Write(0x50,kl,5);
		AT24CXX_Write(0x58,kl,5);AT24CXX_Write(0x60,kl,5);AT24CXX_Write(0x68,kl,5);
		AT24CXX_Write(0x88,kl,5);AT24CXX_Write(0x90,kl,5);AT24CXX_Write(0x98,kl,5);
		AT24CXX_Write(0xa0,kl,5);AT24CXX_Write(0xa8,kl,5);AT24CXX_Write(0xb0,kl,5);
		AT24CXX_Write(0xb8,kl,5);AT24CXX_Write(0xc0,kl,5);AT24CXX_Write(0xc8,kl,5);
		kl[0]=0x32;kl[1]=0x30;kl[2]=0x30;kl[3]=0x30;kl[4]=0x2e;kl[5]=0x30;kl[6]=0x30;
		AT24CXX_Write(0xD0,kl,7);
		AT24CXX_Write(0xD8,kl,7);
		kl[0]=0x30;kl[1]=0x30;kl[2]=0x30;kl[3]=0x30;kl[4]=0x30;kl[5]=0x30;kl[6]=0x2e;kl[7]=0x30;
		AT24CXX_Write(0xe0,kl,8);
	}
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    I1C_Start();  
	if(EE_TYPE>AT24C16)
	{
		I1C_Send_Byte(0XA0);	   //����д����
		I1C_Wait_Ack();
		I1C_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		I1C_Wait_Ack();		 
	}else I1C_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	 

	I1C_Wait_Ack(); 
    I1C_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	I1C_Wait_Ack();	    
	I1C_Start();  	 	   
	I1C_Send_Byte(0XA1);           //�������ģʽ			   
	I1C_Wait_Ack();	 
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
	if(EE_TYPE>AT24C16)
	{
		I1C_Send_Byte(0XA0);	    //����д����
		I1C_Wait_Ack();
		I1C_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		I1C_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	I1C_Wait_Ack();	   
    I1C_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	I1C_Wait_Ack(); 	 										  		   
	I1C_Send_Byte(DataToWrite);     //�����ֽ�							   
	I1C_Wait_Ack();  		    	   
    I1C_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u8 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
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
	if(temp==0X55)return 0;		   
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
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
	delay_ms(50);
}
 











