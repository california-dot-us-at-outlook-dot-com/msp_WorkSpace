#include <msp430f169.h>


void init_spi_slave(){
      P3SEL = 0x0E;                             // Setup P3 for SPI mode
      U0CTL = CHAR + SYNC + SWRST;              // 8-bit, SPI, Slave
      U0TCTL = CKPL + STC;                      // Polarity, UCLK, 3-wire
      ME1 = USPIE0;                             // Module enable
      U0CTL &= ~SWRST;                          // SPI enable
      IE1 |= URXIE0;                            // Recieve interrupt enable
      _EINT();                                  // Enable interrupts

}

#pragma vector=USART0RX_VECTOR
__interrupt void SPI0_rx (void){
    unsigned char data=0;
    data=U0RXBUF;
        P6OUT=~data;
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    init_spi_slave();
    P6DIR=0xff;
    P6OUT=0xff;
    while(1);
}
