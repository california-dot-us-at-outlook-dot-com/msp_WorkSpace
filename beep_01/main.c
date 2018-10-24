#include <msp430.h> 
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char

void Clock_Inita()
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

unsigned int list[7]={0,880,988,1109,1175,1319,1480,1661};
void one(unsigned char yin){
    unsigned int i=0;
    unsigned int j=120000/list[yin];
    unsigned int k=0;
    P6DIR=0xff;
    P6OUT=0x00;
    for(i=0;i<list[yin];i++){
        for(k=0;k<j;k++){
            delay_us(1);
        }
        if(yin){
            P6OUT=0xff;
        }
        for(k=0;k<j;k++){
            delay_us(1);
        }
        P6OUT=0x00;
    }

}

unsigned char HLS[]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,0,0,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1};
//unsigned char ALS[]={6,1,3,5,3,2,3,3,3,2,3,6,7,1,3,2,1,6,5,3};
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Inita();
	unsigned char i=0;
	for(i=0;i<32;i++){
	    one(HLS[i]);
	}
	

}
