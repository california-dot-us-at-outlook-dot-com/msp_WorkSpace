#include <msp430.h> 

//延时函数，IAR自带，经常使用到
#define CPU_F ((double)1000000)
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

void move(unsigned char direction,unsigned char steps){
    unsigned char a1[4]={0x03,0x06,0x0C,0x09};
    unsigned char i=0;
    unsigned char j=0;
    if(direction==0){
        for(i=0;i<steps;i++){
            for(j=0;j<4;j++){
                P6OUT=a1[j];
                delay_ms(5);
            }
        }
    }else{
        for(i=0;i<steps;i++){
            for(j=0;j<4;j++){
                P6OUT=a1[3-j];
                delay_ms(5);
            }
        }
    }
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P6DIR=0xff;
	while(1){
	    move(1,40);
	}
	return 0;
}
