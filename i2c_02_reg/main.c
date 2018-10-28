
#include  <msp430x16x.h>
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

unsigned char Read(unsigned char addr){
    I2CNDAT=0x01;
    unsigned char ctlbyte;
    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    U0CTL |= MST;
    I2CIFG &= ~ARDYIFG;
    I2CTCTL &= ~I2CTRX;
    I2CTCTL = I2CSTT+I2CSTP;
    while((I2CIFG&RXRDYIFG)==0);
    ctlbyte=I2CDRB;
    delay_ms(9);
    while((I2CTCTL&I2CSTP)==0x02);
    return ctlbyte;
}

void Clock_Init()
{
  unsigned char i;
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
unsigned char Write(unsigned char addr,unsigned char dat){
    //I2CNDAT=2;
    I2CNDAT=0x02;
    //I2CSA=(0xd0>>1);

    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    //I2CNDAT=0x02;
    //P6OUT=~1;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    //I2CNDAT=0x01;
    //P6OUT=~2;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=dat;
    delay_ms(9);
    //P6OUT=~3;
    while((I2CTCTL&I2CSTP)==0x02);
}

void init(){

    P3SEL|=0x0a;
    P3DIR&=~0x0a;
    U0CTL|=I2C+SYNC;
    U0CTL&=~I2CEN;
    I2CTCTL=I2CSSEL_2;

    //I2CNDAT=0x01;
    I2CSA=(0xd0>>1);
    U0CTL|=I2CEN;

}

void main(){
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    Clock_Init();
    init();
    P6DIR=0xff;
    delay_ms(100);
    Write(0x01,32);
    unsigned char i=0;
    while(1){
        //U0CTL |= MST;                             // Master
       //   I2CTCTL = I2CSTT + I2CSTP;                // Receive, ST, SP (clears MST)
     //Write((0x02),0x03);

        Write(0x01,i);
        i++;
        P6OUT=~Read(0x01);

        delay_ms(2000);
    }

}
