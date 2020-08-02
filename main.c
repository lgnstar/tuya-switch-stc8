#include "stc8.h"
#include "wifi.h"

typedef struct
{

}stKeyStatus;

void hardware_init(void);
void usart4_SendByte(unsigned char dat);


void main()
{
  hardware_init();
  wifi_protocol_init();

  while(1)
  {
    wifi_uart_service();
  }
}

void hardware_init(void)
{
  //��ʼ������1, 115200bps@11.0592MHz, ���log
  P_SW1 = 0x00;     //RXD/P3.0, TXD/P3.1
  SCON = 0x50;		  //8λ����,�ɱ䲨����
	AUXR |= 0x01;		  //����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		  //��ʱ��2ʱ��ΪFosc/12,��12T
	T2L = 0xFE;		    //�趨��ʱ��ֵ
	T2H = 0xFF;		    //�趨��ʱ��ֵ
	AUXR |= 0x10;		  //������ʱ��2
	IE |= 0x10;       //���п�1�ж�����

  //��ʼ������4, 9600bps@11.0592MHz, ��wifiģ��ͨ��
  P_SW2 = 0x04;     //RXD4_2/P5.2, TXD4_2/P5.3
  S4CON = 0x10;		  //8λ����,�ɱ䲨����
	S4CON |= 0x40;		//����4ѡ��ʱ��4Ϊ�����ʷ�����
	T4T3M &= 0xDF;		//��ʱ��4ʱ��ΪFosc/12,��12T
	T4L = 0xE8;		    //�趨��ʱ��ֵ
	T4H = 0xFF;		    //�趨��ʱ��ֵ
	T4T3M |= 0x80;		//������ʱ��4
  IE2 = 0x10;

  //��ʱ������1, 25����@11.0592MHz, ���ڰ���ȥ����
  AUXR &= 0xBF;		  //��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		  //���ö�ʱ��ģʽ
	TL1 = 0x00;		    //���ö�ʱ��ֵ
	TH1 = 0xA6;		    //���ö�ʱ��ֵ
	TF1 = 0;		      //���TF1��־
	TR1 = 1;		      //��ʱ��1��ʼ��ʱ
  ET1 = 1;          //��ʱ������T1����ж�����

  EA = 1;
}

void usart1_SendByte(uint8_t dat)
{
  SBUF = dat;  while(!TI);  TI = 0;
}

void usart1_SendStr(uint8_t *str)
{
  while(*str)
  {
    SBUF = *str;  while(!TI);  TI = 0;   str++;
  }
}

void usart1_SendHexStr(uint8_t *pdat, uint8_t len)
{
  uint8_t data half, dest;
  while(len--)
  {
    half = *pdat >> 4;    half += 0x30;
    dest = (half > 0x39) ? (half + 0x07) :half;
    usart1_SendByte(dest);
    half = *pdat & 0x0f;  half += 0x30;
    dest = (half > 0x39) ? (half + 0x07) :half;
    usart1_SendByte(dest);    usart1_SendByte(' ');
    pdat += 1;
  }
  usart1_SendStr("\r\n");
}

void usart4_SendByte(unsigned char dat)
{
  S4BUF = dat;  while(!(S4CON & 0x02));  S4CON &= ~0x02;
}

void usart4_ISR(void) interrupt 18
{
  if (S4CON & 0x02)   //��������ж������־
  {
    S4CON &= ~0x02;   // ���жϱ�־
  }
  if (S4CON & 0x01)   //��������ж������־
  {
    S4CON &= ~0x01;   // ���жϱ�־
    uart_receive_input(SBUF);
  }
}

void timer1_ISR(void) interrupt 3
{
	TL1 = 0x00;	 TH1 = 0xA6;  TR1 = 1;
}


