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
  //初始化串口1, 115200bps@11.0592MHz, 输出log
  P_SW1 = 0x00;     //RXD/P3.0, TXD/P3.1
  SCON = 0x50;		  //8位数据,可变波特率
	AUXR |= 0x01;		  //串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		  //定时器2时钟为Fosc/12,即12T
	T2L = 0xFE;		    //设定定时初值
	T2H = 0xFF;		    //设定定时初值
	AUXR |= 0x10;		  //启动定时器2
	IE |= 0x10;       //串行口1中断允许

  //初始化串口4, 9600bps@11.0592MHz, 与wifi模块通信
  P_SW2 = 0x04;     //RXD4_2/P5.2, TXD4_2/P5.3
  S4CON = 0x10;		  //8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M &= 0xDF;		//定时器4时钟为Fosc/12,即12T
	T4L = 0xE8;		    //设定定时初值
	T4H = 0xFF;		    //设定定时初值
	T4T3M |= 0x80;		//启动定时器4
  IE2 = 0x10;

  //定时计数器1, 25毫秒@11.0592MHz, 用于按键去抖动
  AUXR &= 0xBF;		  //定时器时钟12T模式
	TMOD &= 0x0F;		  //设置定时器模式
	TL1 = 0x00;		    //设置定时初值
	TH1 = 0xA6;		    //设置定时初值
	TF1 = 0;		      //清除TF1标志
	TR1 = 1;		      //定时器1开始计时
  ET1 = 1;          //定时计数器T1溢出中断允许

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
  if (S4CON & 0x02)   //发送完成中断请求标志
  {
    S4CON &= ~0x02;   // 清中断标志
  }
  if (S4CON & 0x01)   //接收完成中断请求标志
  {
    S4CON &= ~0x01;   // 清中断标志
    uart_receive_input(SBUF);
  }
}

void timer1_ISR(void) interrupt 3
{
	TL1 = 0x00;	 TH1 = 0xA6;  TR1 = 1;
}



