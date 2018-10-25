#include <msp430f169.h>

#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
/**
 * main.c
 */

void init_spi(){
    U0CTL |= SWRST;
    U0CTL |= CHAR+SYNC+MM;
    U0TCTL |= SSEL1+SSEL0+STC+CKPH;
    //U0TCTL=CKPH;
    //U0TCTL &= ~CKPL;
    U0BR0=0x02;
    U0BR1=0x00;
    ME1 |= USPIE0;
    U0CTL&=~SWRST;
    IE1&=~UTXIE0;
    IE1&=~URXIE0;
    P3SEL|=0x0e;
    P3DIR|=0x01;
}

void spi_sendByte(unsigned char dat){
    U0TXBUF=dat;
    while((IFG1&UTXIFG0)==0);
    //IFG1&=~UTXIFG0;
}

/*
void delay(void){
    unsigned int i,j;
    for(i=0;i<=500;i++){
        for(j=0;j<=500;j++){
            ;
        }
    }
}*/

void Clock_Init(){
    unsigned char i;
    BCSCTL1&=~XT2OFF;
    BCSCTL2|=(SELM1+SELS);
    do{
        IFG1&=~OFIFG;
        for(i=0;i<100;i++){
            _NOP();
        }
    }
    while((IFG1&OFIFG)!=0);
    IFG1&=~OFIFG;
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	unsigned char i;
	P6DIR=0xff;
	P6OUT=0x00;
	unsigned char data[10]={9,8,7,6,5,4,3,2,1,0};
	Clock_Init();
	init_spi();
	while(1){
	    for(i=0;i<=9;i++){
	        delay_us(5);
	        P6OUT=0xff;
	        delay_us(5);
	        spi_sendByte(data[i]);
	        delay_us(5);
	        P6OUT=0x00;
	    }
	}
	
}
