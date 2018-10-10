/********************************************************************
//DM430-L型最小系统板串口测试程序，使用单片机的串口0
//使用板载BSL模块或者用户外接串口线到DB9，要求是直连公对母串口线
//使用串口调试助手发送数据到系统板，比如02,03等
//系统板会将收到的数据再发送到串口调试助手，接收采用中断模块，发送用查询
//板载的BSL模块可以通过跳线帽设置为USB转串口模式，笔记本电脑没有串口可以直接代替
//开发板的BSL_Config区的RXD和TXD0连接，TXD和RXD0连接即可,另外2个跳线帽取下
//使用USB转串口功能时，BSL失效，因此操作前应该先将程序下载至芯片中再调整跳线帽
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/

#include <msp430f169.h>
#include "Config.h"

//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_Init()
{
  U0CTL|=SWRST;               //复位SWRST
  U0CTL|=CHAR;                //8位数据模式
  U0TCTL|=SSEL1;              //SMCLK为串口时钟
  U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //微调寄存器为0，波特率9600bps
  ME1|=UTXE0;                 //UART0发送使能
  ME1|=URXE0;                 //UART0接收使能
  U0CTL&=~SWRST;
  IE1|=URXIE0;                //接收中断使能位

  P3SEL|= BIT4;               //设置IO口为普通I/O模式
  P3DIR|= BIT4;               //设置IO口方向为输出
  P3SEL|= BIT5;
}

//*************************************************************************
//              串口0发送数据函数
//*************************************************************************

void Send_Byte(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //发送寄存器空的时候发送数据
    U0TXBUF=data;
}

//*************************************************************************
//               处理来自串口 0 的接收中断
//*************************************************************************

#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data=0;
  data=U0RXBUF;                       //接收到的数据存起来
  Send_Byte(data);                    //将接收到的数据再发送出去
}

//*************************************************************************
//            处理来自串口 0 的发送中断，预留
//*************************************************************************

#pragma vector=USART0TX_VECTOR
__interrupt void UART0_TX_ISR(void)
{

}

//*************************************************************************
//           主函数
//*************************************************************************
void main(void)
{
  WDT_Init();                         //看门狗设置
  Clock_Init();                       //系统时钟设置
  UART_Init();                        //串口设置初始化
  _EINT();                            //开中断
  P6DIR=0xff;
  while(1)                            //无限循环
    {
      P6OUT=(P3IN & BIT5)+(~BIT5);
    }
}
