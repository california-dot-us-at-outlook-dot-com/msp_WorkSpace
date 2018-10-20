#include <msp430.h> 

#define mosi BIT1
#define miso BIT2
#define scl BIT0
#define portout P5OUT
#define portin P5IN
#define portdir P5DIR
#define ss BIT3

//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char

void init_s(){
    portdir |= miso;
    portdir &= (~mosi);
    portdir &= (~scl);
    portdir &=(~ss);
}

void Miso(unsigned char a){
    if(a&0x80){
        portout|=miso;
    }else{
        portout&=(~miso);
    }
}

unsigned char Mosi(){
    if(portin&mosi){
        return 1;
    }else{
        return 0;
    }
}

unsigned char read_write(unsigned char dat){
    unsigned char i=0;
    unsigned char tmp=0xff;
    unsigned char complete=0;
    unsigned char recv=0;
    unsigned char Dat=dat;
    Miso(Dat);
    Dat<<=1;
    while(!(portin&ss));
    if(ss){
        for(i=0;i<16;i++){
            while((portin&scl)==tmp && portin&ss){
                ;
            }
            if(!(portin&ss)){
                return 0x00;
            }
            tmp=(portin&scl);
            if((portin&scl)){
                recv<<=1;
                if((portin&mosi)){
                    recv|=(Mosi());
                }

                //Miso(Dat);
                //Dat<<=1;
            }
            else{

                Miso(Dat);
                Dat<<=1;
            }
        }
    }
    return recv;

}

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
/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	init_s();
	unsigned char a=0;
	unsigned char tmpLed=0;
	P6DIR=0xff;
	P6OUT=0xff;
	//delay_ms();
	while(1){
	    tmpLed=~(read_write(0xb9));
	    P6OUT=tmpLed;
	    delay_us(2);
	}
}
