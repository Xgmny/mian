#ifndef __PROTOCOL_H
#define __PROTOCOL_H 
#include "sys.h"
#include "mport.h"
#include "mcheck.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS Э�����������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 



#define M_MAX_FRAME_LENGTH	261		//���֡����Ϊ261�ֽ� ��ַ+����+֡��+���ݳ���+����+CRC(CRC16��2�ֽ�)
#define M_MIN_FRAME_LENGTH	5		//��С֡����Ϊ5�ֽ� ��ַ+����+֡��+���ݳ���+CRC8/XOR/SUM



//У�鷽ʽ�궨��
#define M_FRAME_CHECK_SUM	0		//У���
#define M_FRAME_CHECK_XOR	1		//���У��
#define M_FRAME_CHECK_CRC8	2		//CRC8У��
#define M_FRAME_CHECK_CRC16	3		//CRC16У��


//���ؽ�����������Ͷ���
typedef enum
{
	MR_OK=0,						//����
	MR_FRAME_FORMAT_ERR = 1,		//֡��ʽ����	 
	MR_FRAME_CHECK_ERR = 2,			//У��ֵ��λ 
	MR_MEMORY_ERR = 3,				//�ڴ���� 
}m_result;

//��modbus֡����
__packed typedef struct
{ 
	u8 address;						//�豸��ַ��0���㲥��ַ��1~255���豸��ַ��
	u8 function;					//֡���ܣ�0~255
	u8 count;						//֡���
	u8 datalen;						//��Ч���ݳ���
	u8 *data;						//���ݴ洢��
	u16 chkval;						//У��ֵ 
}m_frame_typedef;

__packed typedef struct
{ 
	u8 m_1;						//�豸��ַ��0���㲥��ַ��1~255���豸��ַ��
	u8 m_2;					//֡���ܣ�0~255
	u8 m_3;						//֡���
	u8 m_4;						//��Ч���ݳ���
	u8 m_5;						//���ݴ洢��
	u8 m_6;
	u8 m_7;                   //CRC
	u8 m_8;						 //CRC
	u16 chkval;						//У��ֵ 
}m_modbus_typedef;



//��modbusЭ�������
typedef  struct
{  	
	u8* rxbuf;						//���ջ�����
	u16 rxlen;						//�������ݵĳ���
	u8 frameok;						//һ֡���ݽ�����ɱ�ǣ�0����û��ɣ�1�������һ֡���ݵĽ���
	u8 checkmode;					//У��ģʽ��0,У���;1,���;2,CRC8;3,CRC16	
}m_protocol_dev_typedef; 

extern m_protocol_dev_typedef m_ctrl_dev;			//����modbus������
	
m_result mb_unpack_frame(m_frame_typedef *fx);		//����һ֡����
void mb_packsend_frame(m_frame_typedef *fx);		//���һ֡���ݣ�������
m_result mb_init(u8 checkmode);						//��ʼ��MODBUS,�����ڴ�
void mb_destroy(void);								//����MODBUS,�ͷ��ڴ�

void mb_mod(m_modbus_typedef *fx);
#endif












