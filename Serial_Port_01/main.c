#include <msp430.h> 


/**
 * main.c
 */

//USART

#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RXISR(void){
    P6OUT=~P6OUT;
    IFG1 &= ~URXIFG0;
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL1=~XT2OFF;
	BCSCTL2=DIVS_3+SELS;
	P6DIR=0xff;
	P6OUT=0xff;
	P3SEL |= 0x30;
	ME1 |= UTXE0 + URXE0;
	UCTL0 |= CHAR;
	UTCTL0 |= SSEL0;
	UBR00=104;
	UBR10=0x00;
	UMCTL0 = 0x4A;
	UCTL0 &= ~SWRST;
	IE1 |= URXIE0;
	IFG1 &= ~URXIFG0;
	_EINT();
	while(1);
	
	return 0;
}
