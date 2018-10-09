#include <msp430.h> 


/**
 * main.c
 */
#define uchar unsigned char
//增计数模式
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	P1DIR |= 0x0f;
	P1SEL |= 0x0f;
	CCR0=1000;
	TACTL=TASSEL_2+MC_1;
	while(1){
	    CCR1=0;
	    CCTL1=OUTMOD_3; //先为0，遇到CCR1变为1，遇到CCR0变为0

	    CCR2=1000;
	    CCTL2=OUTMOD_7;//先为1，遇到CCR1变为0，遇到CCR0变为1
	}
	
}
