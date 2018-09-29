#include<msp430f169.h>
#include"Config.h"


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



void main(){
    unsigned char dat;
    unsigned char i;
    unsigned char tmp=0;
    unsigned char t[12];
    WDTCTL = WDTPW + WDTHOLD;
    Clock_Init();
    P4DIR=0xff;
    P4OUT=0x00;
    P5SEL=0x00;
    P5DIR=0xff;
    init();
    P6SEL=0x01;
    P6DIR=0xFE;
    P6OUT=0x00;
    ADC12CTL0=ADC12ON+REFON+REF2_5V;

    ADC12CTL1=SHP;
    ADC12MCTL0=SREF_1;
    for(i=0xFFF;i>0;i--);
    ADC12CTL0 |= ENC;

    //ÉèÖÃÊ±ÖÓ
    BCSCTL1&= ~XT2OFF;
    do{
      IFG1 &= ~OFIFG;
      for(i=0xFF;i>0;i--);
    }while((IFG1 & OFIFG)!=0);
    //

    while(1){
        ADC12CTL0 |= ADC12SC;
        while((ADC12IFG & BIT0)==0);
        dat=(long)ADC12MEM0;
        P6OUT=~dat;
        for(i=0;i<12;i++){
            tmp=(dat>>i)%2;
            //wc(0xc0+7-i);

            if(tmp!=t[i]){
                t[i]=tmp;
                if(tmp==1){
                    wc(0xc0+11-i);
                    wd('1');
                }else{
                    wc(0xc0+11-i);
                    wd('0');
                }
            }
//            delay_ms(500);
        }
        //delay_ms(40);
    }

}
