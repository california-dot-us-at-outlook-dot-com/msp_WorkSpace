#include <msp430.h> 
#include"Config.h"

/**
 * main.c
 */

//wd == WriteData
//wc == WriteCommend

void wd(unsigned char data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void wc(unsigned char data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=data;
    delay_ms(5);
    EN_CLR;
}

void init(void){
    wc(0x38);
    delay_ms(5);
    wc(0x08);
    delay_ms(5);
    wc(0x01);
    delay_ms(5);
    wc(0x06);
    delay_ms(5);
    wc(0x0c);
    delay_ms(5);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();                                 //系统时钟设置

	P4DIR=0xff;
	P4OUT=0x00;
	P5SEL=0x00;
	P5DIR=0xff;
	init();
	int i=0;
	for(;i<16;i++){
	    wc(0x80+i);
	    wd('0'+i);
	}
	i=0;
	for(;i<16;i++){
        wc(0xc0+i);
        wd('A'+i);
	}
	//wc(0x01);
	delay_ms(5);

	wc(0x18);
	delay_ms(999);
	wc(0x18);
	delay_ms(999);

	return 0;
}
