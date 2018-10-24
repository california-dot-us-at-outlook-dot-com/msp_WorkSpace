#include <msp430.h> 
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char

void Clock_Inita()
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

unsigned int list[13]={1024,440,494,554,587,659,740,831,880,988,1109,1175,1319,1480,1661};
void one(unsigned char yin){
    unsigned int i=0;
    unsigned int j=120000/list[yin];
    unsigned int k=0;
    P6DIR=0xff;
    P6OUT=0x00;
    for(i=0;i<list[yin];i++){
        for(k=0;k<j;k++){
            delay_us(1);
        }
        if(yin-255){
            P6OUT=0xff;
        }
        for(k=0;k<j;k++){
            delay_us(1);
        }
        P6OUT=0x00;
    }

}

unsigned char HLS[]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,255,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,255,2,2,3,1,2,3,4,3,1,2,3,4,3,2,1,2,5,255,3,3,4,5,5,4,3,4,2,1,2,3,2,1,1,0};
//unsigned char ALS[]={6,1,3,5,3,2,3,3,3,2,3,6,7,1,3,2,1,6,5,3};
unsigned char MLH[]={3,3,5,6,8,8,6,5,5,6,5,255,3,3,5,6,8,8,6,5,5,6,5,255,5,5,5,3,5,6,6,5,255,3,2,3,5,3,2,1,1,2,1,255,3,2,1,3,2,3,5,6,8,5,255,2,3,5,2,3,1,6,5,255,6,1,2,3,1,2,1,6,5,255,0};
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Inita();
	unsigned char i=0;
	for(i=0;i<strlen(HLS);i++){
	    one(HLS[i]);
	}
	

}
