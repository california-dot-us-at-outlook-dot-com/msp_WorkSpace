#include <msp430.h> 


/**
 * main.c
*/

unsigned char i;
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void){
    //unsigned long a=0;
    //if(i%1000==0)
    P6OUT=~(((P6OUT>>1)<<1)+1-(P6OUT%2));
    i++;
    /*
    for(a=0;a<0xffff;a++){
        _NOP();
    }
    */
}

#define CPU_F ((double)32768)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


void main(void)
{   i=0;


	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P6DIR=0xff;
	P6OUT=0x55;

	BCSCTL1&=~XT2OFF;//0xff-DIVA_3-XT2OFF;//+DIVA_3;//-XT5V-7;//-DIVA1-DIVA0;
	/*
	BCSCTL2=SELM_2+SELS+DIVS_3;//+DIVM_3;//+DIVS_3;//SELM0+SELM1;//+SELM0;//+DIVM0+DIVM1+DIVS0+DIVS1;

	TACTL = TASSEL_1;//+ID_3;
	*/
	TACCTL0=CCIE;//+CM1+CM0;//+CAP;

	TACCR0=32768;
	//CCR0=32768;
	TACTL=TASSEL_1+MC_1;
	_EINT();
	while(1){
	    P6OUT = ((P6OUT>>1)<<1)+1-(P6OUT%2);
	    delay_ms(4096);
    }
	//return 0;
}
