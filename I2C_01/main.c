#include <msp430.h> 


/**
 * main.c
 */
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define SDA BIT0
#define SCL BIT1
#define portOut P5OUT
#define portIn P5IN
#define portDir P5DIR
#define portDin portDir &= ~(SDA+SCL)
#define portDout portDir |= SDA+SCL
#define portSdaOut portDir |= SDA
#define portSclIn portDir &= ~SCL
#define portSdaIn portDir &= ~SDA
#define portSclOut portDir |= SCL

#define uchar unsigned char


void Sda(unsigned char a){
    if(a==0){
        portOut &= (~SDA);
    }else{
        portOut |= SDA;
    }
}
void Scl(unsigned char a){
    if(a==0){
        portOut &= (~SCL);
    }else{
        portOut |= SCL;
    }
}


void start(){
    portDout;
    Sda(1);
    delay_us(10);
    Scl(1);
    delay_us(10);
    Sda(0);
    delay_us(10);
    Scl(0);
    delay_us(10);
}

void stop(){
    portDout;
    Sda(0);
    delay_us(10);
    Scl(1);
    delay_us(10);
    Sda(1);
    delay_us(10);
}

unsigned char w(unsigned char d){
    unsigned char a=0,b=0;
    portDout;
    for(a=0;a<8;a++){
        Sda(d>>7);
        d=(d<<1);
        delay_us(10);
        Scl(1);
        delay_us(10);
        Scl(0);
        delay_us(10);
    }
    Sda(1);
    delay_us(10);
    Scl(1);
    portSdaIn;
    while(portIn&SDA){
        b++;
        if(b>200){
            Scl(0);
            delay_us(10);
            return 0;
        }
    }
    Scl(0);
    delay_us(10);
    return 1;
}

unsigned char r(){
    unsigned char a=0,d=0;
    portSdaOut;
    Sda(1);
    portSdaIn;
    delay_us(10);
    for(a=0;a<8;a++){
        Scl(1);
        delay_us(10);
        d <<= 1;
        d |= (portIn&SDA);
        delay_us(10);
        Scl(0);
        delay_us(10);
    }
    return d;
}

unsigned char RC(unsigned char a){
    unsigned char d;
    start();
    w(0xd0);
    w(a);
//    stop();

    start();
    w(0xd1);
    d=r();
    stop();
    return d;
}

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

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	unsigned char a=0;
	Clock_Init();
	P6DIR=0xff;
	P6OUT=0xff;
	while(1){
	    a=~RC(0x00);
	    if(P6OUT>a){
	        P6OUT=a;
	    }
	}

}
