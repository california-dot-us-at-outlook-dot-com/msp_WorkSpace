#include<msp430f169.h>



#define CPU_F ((double)32768)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char


unsigned char timerA=0;
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void){
    //unsigned long a=0;
    //if(i%1000==0)
    timerA++;

    /*
    for(a=0;a<0xffff;a++){
        _NOP();
    }
    */
}


unsigned char timerB=0;
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B(void){
    //unsigned long a=0;
    //if(i%1000==0)

    if(P5OUT&BIT0){
        P5OUT &= ~BIT0;
        TBR=0;
    }else{
        P5OUT |= BIT0;
        TAR=0;
        timerA=0;
        TBR=9980;
    }


    /*
    for(a=0;a<0xffff;a++){
        _NOP();
    }
    */
}

unsigned long time=0;
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    //P6OUT=~timerA;
    time=timerA*10000+TAR;
    P6OUT=~(((time/100)*17)/10);
    P1IFG &= 0;
}


void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM_2+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

void main(){
    WDTCTL = WDTPW + WDTHOLD;
    Clock_Init();
    P6DIR=0xff;
    //P1 init
    P1DIR &= 0;
    P1IES |= BIT1; //0:�����أ�    1���½���
    P1IE |= BIT0+BIT2;  //�ж�����1Ϊ����
    P1IFG &= 0;  //�жϱ�־��0Ϊ�ɽ����ж�


    TACCTL0=CCIE;//+CM1+CM0;//+CAP;
    TACCR0=20000;
    //CCR0=32768;
    TACTL=TASSEL_2+MC_1+ID_3;

    TBCCTL0=CCIE;//+CM1+CM0;//+CAP;
    TBCCR0=10000;
    //CCR0=32768;
    TBCTL=TBSSEL_2+MC_1+ID_3;


    _EINT();    //�������ж�

    P6DIR=0xff;
    P5DIR=0xff;
    while(1){
        //P6OUT=~timerA;//(((time/1000)*17)/10);
    }


}
