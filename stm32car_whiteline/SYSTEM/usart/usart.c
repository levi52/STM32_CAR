#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "oled.h"
#include "usart.h"
uint8_t Serial_TxPacket[4];				//���巢�����ݰ����飬���ݰ���ʽ��FF 01 02 03 04 FE
uint8_t Serial_RxPacket[4];				//����������ݰ�����
uint8_t Serial_RxFlag;					//����������ݰ���־λ

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Serial_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA9���ų�ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA10���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 115200;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART1, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART1, ENABLE);								//ʹ��USART1�����ڿ�ʼ����
}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial_SendByte(Array[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial_SendByte(String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}

/**
  * ��    ����ʹ��printf��Ҫ�ض���ĵײ㺯��
  * ��    ��������ԭʼ��ʽ���ɣ�����䶯
  * �� �� ֵ������ԭʼ��ʽ���ɣ�����䶯
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);			//��printf�ĵײ��ض����Լ��ķ����ֽں���
	return ch;
}

/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	Serial_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

/**
  * ��    �������ڷ������ݰ�
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ô˺�����Serial_TxPacket��������ݽ����ϰ�ͷ��FF����β��FE������Ϊ���ݰ����ͳ�ȥ
  */
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
}

/**
  * ��    ������ȡ���ڽ������ݰ���־λ
  * ��    ������
  * �� �� ֵ�����ڽ������ݰ���־λ����Χ��0~1�����յ����ݰ��󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//�����־λΪ1
	{
		Serial_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ����USART1�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
//		u8 stringt[10] = {0};	//OLED�ַ�����ʾ
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (RxData == 0x2C)			//�������ȷʵ�ǰ�ͷ
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;	//�����ݴ������ݰ������ָ��λ��
			pRxPacket ++;				//���ݰ���λ������
			if (pRxPacket >= 4)			//����չ�4������
			{
				RxState = 2;			//����һ��״̬
			}
			if(Serial_RxPacket[0]==5)
			{
				Serial_RxPacket[0]=2;
			}
			if(Serial_RxPacket[0]==7)
			{
				Serial_RxPacket[0]=1;
			}
			if(Serial_RxPacket[0]==1 || Serial_RxPacket[0]==2)
			{
				serial_flag=1;
			}
			
//			sprintf((char *)stringt,"Num:%d ",Serial_RxPacket[0]);
//			OLED_ShowString(0,6,stringt,16);
		}
		/*��ǰ״̬Ϊ2���������ݰ���β*/
		else if (RxState == 2)
		{
			if (RxData == 0x5B)			//�������ȷʵ�ǰ�β��
			{
				RxState = 0;			//״̬��0
//				Serial_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ
	}
}
