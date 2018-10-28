#include <msp430.h> 
#include<stdlib.h>

#define CPU_F ((double)8000000)
//#define CPU_F ((double)32768)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char

#define CSN BIT3
#define CE BIT5
#define IRQ BIT7
#define po P1OUT
#define pd P1DIR
#define pi P1IN
#define ps P1SEL
#define CSN_0 po&=(~CSN)
#define CSN_1 po|=CSN
#define CE_0 po&=(~CE)
#define CE_1 po|=CE


//NRF2401
//����
#define READ_REG        0X00    //�����üĴ���
#define WRITE_REG       0X20    //д���üĴ���
#define RD_RX_PLOAD         0X61    //��RX��Ч���ݣ�1~32���ֽ�
#define WR_TX_PLOAD         0XA0    //дTX��Ч���ݣ�1~32���ֽ�
#define FLUSH_TX            0XE1    //���TX FIFO�Ĵ���������ģʽ��
#define FLUSH_RX            0XE2    //���RX FIFO�Ĵ���������ģʽ��
#define REUSE_TX_PL         0XE3    //����ʹ����һ����Ч����
#define NOP1                 0xFF    //�ղ���
//��ַ
#define CONFIG          0X00     //���üĴ���
#define EN_AA           0X01     //ʹ���Զ�Ӧ����
#define EN_RXADDR       0X02     //���յ�ַ����
#define SETUP_AW        0X03     //���õ�ַ���
#define SETUP_RETR      0X04     //�����Զ��ط�
#define RF_CH           0X05     //��Ƶͨ��
#define RF_SETUP        0X06     //��Ƶ�Ĵ���
#define STATUS          0X07     //״̬�Ĵ���
#define OBSERVE_TX      0X08     //���ͼ��Ĵ���
#define CD              0X09     //�ز����
#define RX_ADDR_P0      0X0A     //����ͨ��0���յ�ַ
#define RX_ADDR_P1      0X0B     //����ͨ��1���յ�ַ
#define RX_ADDR_P2      0X0C     //����ͨ��2���յ�ַ
#define RX_ADDR_P3      0X0D     //����ͨ��3���յ�ַ
#define RX_ADDR_P4      0X0E     //����ͨ��4���յ�ַ
#define RX_ADDR_P5      0X0F     //����ͨ��5���յ�ַ
#define TX_ADDR         0X10     //���͵�ַ
#define RX_PW_P0        0X11     //��������ͨ��0��Ч���ݿ��
#define RX_PW_P1        0X12     //��������ͨ��1��Ч���ݿ��
#define RX_PW_P2        0X13     //��������ͨ��2��Ч���ݿ��
#define RX_PW_P3        0X14     //��������ͨ��3��Ч���ݿ��
#define RX_PW_P4        0X15     //��������ͨ��4��Ч���ݿ��
#define RX_PW_P5        0X16     //��������ͨ��5��Ч���ݿ��
#define FIFO_STATUS     0X17     //FIFO ״̬�Ĵ���

unsigned char TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};
unsigned char RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};

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


unsigned char RW(unsigned char dat){
    U0TXBUF=dat;
    while((IFG1&UTXIFG0)==0);
    delay_us(50);
    return U0RXBUF;
}

unsigned char RW_reg(unsigned char reg,unsigned char value){

    unsigned char status1;
    CSN_0;
    status1=RW(reg);
    //delay_us(20);
    RW(value);
    //delay_us(20);
    CSN_1;
    return status1;

}

unsigned char R_reg(unsigned char reg){

    unsigned char value;
    CSN_0;
    RW(reg);
    //delay_us(20);
    //RW(0);
    value=RW(0);
    //delay_us(20);
    CSN_1;
    return value;

}
void setRX(){
    CE_0;
    RW_reg(WRITE_REG+CONFIG,0x0f);
    CE_1;
    delay_ms(3);
}

unsigned char Write_BUF(unsigned char reg,unsigned char * addr,unsigned char width){
    unsigned char status1,ctr;
    CSN_0;
    status1=RW(reg);
    for(ctr=0;ctr<width;ctr++){
        RW(*addr);
        addr++;
    }
    //delay_us(20);
    CSN_1;
    return status1;
}

unsigned char Read_BUF(unsigned char reg,unsigned char *addr,unsigned char width){
    unsigned char status2,ctr;
    CSN_0;
    status2=RW(reg);
    for(ctr=0;ctr<width;ctr++){
       addr[ctr]=RW(0);
    }
    //delay_us(20);
    CSN_1;
    return status2;
}

#define TX_ADR_WIDTH 5
#define RX_ADR_WIDTH 5

#define TX_PLOAD_WIDTH 32
#define RX_PLOAD_WIDTH 32

void NRF2401_init(){
    delay_us(10);
    CE_0;
    CSN_1;
    Write_BUF(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
    Write_BUF(WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);
    RW_reg(WRITE_REG+EN_AA,0x00);
    RW_reg(WRITE_REG+EN_RXADDR,0x01);
    RW_reg(WRITE_REG+RF_CH,0);
    RW_reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
    RW_reg(WRITE_REG+RF_SETUP,0x07);
    RW_reg(WRITE_REG+CONFIG,0x0E);

    delay_ms(10);

}

void send(unsigned char *tx_buf){
    CE_0;
    Write_BUF(WRITE_REG+RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
    Write_BUF(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
    CE_1;
    delay_ms(2);
    CE_0;
}

unsigned char recv(unsigned char*rx_buf){
    unsigned char rev=0;
    unsigned char status1;
    status1=R_reg(STATUS);
    rev=status1;
    if(status1&0x40){
        CE_0;
        Read_BUF(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
        rev=1;
    }

    RW_reg(WRITE_REG+STATUS,status1);

    return rev;

}


void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS+DIVS_3;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    P6OUT=~P6OUT;
    delay_ms(200);
    P6OUT=~P6OUT;
    P1IFG &= 0;
    delay_ms(200);
}

//1602
#define DataDIR         P4DIR                     //���ݿڷ���
#define DataPort        P4OUT                     //P4��Ϊ���ݿ�
//1602
#define RS_CLR          P5OUT &= ~BIT5           //RS�õ�
#define RS_SET          P5OUT |=  BIT5           //RS�ø�

#define RW_CLR          P5OUT &= ~BIT6           //RW�õ�
#define RW_SET          P5OUT |=  BIT6           //RW�ø�

#define EN_CLR          P5OUT &= ~BIT7           //E�õ�
#define EN_SET          P5OUT |=  BIT7           //E�ø�

#define PSB_CLR         P5OUT &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET         P5OUT |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR         P5OUT &= ~BIT1            //RST�õ�
#define RST_SET         P5OUT |= BIT1             //RST�ø�

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
    P4DIR=0xff;
    P4OUT=0x00;
    P5SEL=0x00;
    P5DIR=0xff;
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
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    Clock_Init();
    delay_ms(2000);
    init_spi();

    P1DIR |= BIT3+BIT5;
    P1DIR &= (~BIT7);
    //CSN_1;
    P1IES |= BIT0+BIT1; //0:�����أ�    1���½���
    P1IE |= BIT7;  //�ж�����1Ϊ����
    P1IFG &= 0;  //�жϱ�־��0Ϊ�ɽ����ж�
    _EINT();    //�������ж�

    //P6OUT=~R_reg(0x07);
    NRF2401_init();
    init();
    P6DIR=0xff;
    P6OUT=0x00;
    //P6OUT=~R_reg(0x07);
    unsigned char tx_buf[32]="Hello,world,Hell,World!";
    unsigned char *rx_buf=(unsigned char *)malloc(sizeof(unsigned char)*32);
    U0RXBUF=0;
    rx_buf[0]='e';
    unsigned char status1=0;
    while(1){
        //send(tx_buf);
        //delay_ms(22);
        //P6OUT=~
        status1=0;
                setRX();

                status1=recv(rx_buf);
                if(status1){
                    P6OUT=0xf0;
                    for(status1=0;status1<16;status1++){
                        wc(0x80+status1);
                        wd(rx_buf[status1]);
                    }
                }
        delay_ms(20);
        //P6OUT=~R_reg(0x07);
        //P6OUT=~(R_reg(0x07));
        delay_ms(2);
        //R_reg(FIFO_STATUS);
        //P6OUT=~rx_buf[3];
        //P6OUT=~(R_reg(0x07));
        delay_ms(1000);
    }
    /*
    while(1){
        P6OUT=~Read_BUF(RF_CH,rx_buf,1);
        delay_ms(1000);
    }
    */
}
