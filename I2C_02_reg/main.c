#include <msp430.h> 


/**
 * main.c
 */

#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
void i2c_init(){
    P3SEL = BIT1+BIT3;
    P3DIR &= ~(BIT1+BIT3);
    U0CTL |= I2C + SYNC;
    U0CTL &= ~(I2CEN);
    I2CTCTL = I2CSSEL_2;
    I2CSA = BIT6 + BIT4;
    U0CTL |= I2CEN;
}

void write(unsigned char dat1){
    /*
    I2CNDAT = 0x01;
    U0CTL = MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
    */
    I2CDRB=dat1;
    delay_ms(9);

    while((I2CTCTL & I2CSTP)==0x01);
}

unsigned char read(unsigned char addr){
    I2CNDAT=0x01;
    unsigned char Data;
    U0CTL |= MST;
    I2CTCTL |= I2CSTT + I2CSTP + I2CTRX;
    while((I2CIFG & TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    U0CTL |= MST;
    I2CIFG &= ~ARDYIFG;
    I2CTCTL &= ~I2CSTP;
    while((I2CIFG & RXRDYIFG)==0);
    Data=I2CDRB;
    delay_ms(9);
    while((I2CTCTL&I2CSTP)==0x02);
    return Data;
}

void write_clock(unsigned char dat1){

}
unsigned char read_clock(unsigned char dat1){
    write(0x00);
    write(0x00);
    return read(0x00);
}

void write_I2C1602(unsigned char d){
    unsigned char D;

//
    I2CNDAT = 0x05;
    U0CTL = MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
  //
    write(0x4e);
    D=d&0xf0;
    D|=0x0d;
    write(D);
    D&=0xf9;
    write(D);
    D=(d&0x0f)<<4;
    D|=0x0d;
    write(D);
    D&=0xf9;
    write(D);
    //
    //while((I2CTCTL & I2CSTP)==0x02);
        //
}

void write_I2C1602_c(unsigned char d){

    //
        I2CNDAT = 0x05;
        U0CTL = MST;
        I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
        while((I2CIFG&TXRDYIFG)==0);
      //
    unsigned char D;
    write(0x4e);
    D=d&0xf0;
    D|=0x0c;
    write(D);
    D&=0xf8;
    write(D);
    D=(d&0x0f)<<4;
    D|=0x0c;
    write(D);
    D&=0xf8;
    write(D);

    //
       // while((I2CTCTL & I2CSTP)==0x02);
            //
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

void I2C1602_init(){
    delay_ms(10);
    write_I2C1602_c(0x33);
    delay_ms(5);
    write_I2C1602_c(0x32);
    delay_ms(5);
    write_I2C1602_c(0x28);
    delay_ms(5);
    write_I2C1602_c(0x0c);
    delay_ms(5);
    write_I2C1602_c(0x06);
    delay_ms(5);
    write_I2C1602_c(0x01);
    delay_ms(5);
    delay_ms(10);

    delay_ms(10);
    write_I2C1602_c(0x33);
    delay_ms(5);
    write_I2C1602_c(0x32);
    delay_ms(5);
    write_I2C1602_c(0x28);
    delay_ms(5);
    write_I2C1602_c(0x0c);
    delay_ms(5);
    write_I2C1602_c(0x06);
    delay_ms(5);
    write_I2C1602_c(0x01);
    delay_ms(5);
    delay_ms(10);
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	i2c_init();
	Clock_Init();
	I2C1602_init();
	P6DIR=0xff;
	P6OUT=0xf0;
	while(1){
	    write_I2C1602_c(0x80);
	    write_I2C1602('Y');
	    delay_ms(1000);
	    P6OUT=0x0f;
	}
}
