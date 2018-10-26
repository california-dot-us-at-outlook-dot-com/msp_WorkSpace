#include <msp430.h> 


#define uchar unsigned char
//������ģʽ
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}
unsigned int pwm_1=500;
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    Clock_Init();
    P1DIR |= 0x0f;
    P1SEL |= 0x0f;
    CCR0=20000;
    TACTL=TASSEL_2+MC_1+ID_3;
    unsigned long i=0;
    while(1){
        for(i=0;i<240000;i++){
        CCR1=18500;
        CCTL1=OUTMOD_3; //��Ϊ0������CCR1��Ϊ1������CCR0��Ϊ0

        CCR2=pwm_1;
        CCTL2=OUTMOD_7;//��Ϊ1������CCR1��Ϊ0������CCR0��Ϊ1
        }
        if(pwm_1<2500){
            pwm_1+=500;
        }else{
            pwm_1=500;
        }
    }

}
