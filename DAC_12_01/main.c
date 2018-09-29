#include <msp430f169.h>
#include "Config.h"

//typedef unsigned char uchar;
//typedef unsigned int uint;


/**
 * main.c
 */

void Init_DAC12_0(void){
    ADC12CTL0 = REF2_5V + REFON;
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;
    DAC12_0DAT = 0x0000;
}

void Init_DAC12_1(void){
    ADC12CTL1 = REF2_5V + REFON;
    DAC12_1CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;
    DAC12_1DAT = 0x0000;
}

void angle(uchar nums[]){
    uchar k=0x00;
    for(;k<64;k++){
        DAC12_0DAT=nums[k];
        DAC12_1DAT=nums[(k+16)%64];
        //delay_us(100);
    }

}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//uchar nums[64]={128,140,153,165,177,189,200,210,219,228,235,241,247,251,254,255,255,255,252,249,244,238,231,223,215,205,194,183,171,159,147,134,121,108,96,84,72,61,50,40,32,24,17,11,6,3,0,0,0,1,4,8,14,20,27,36,45,55,66,78,90,102,115,127};
	uchar nums[64]={128,139,151,163,174,185,195,205,213,221,228,234,239,243,246,247,247,247,244,241,237,231,225,217,209,200,190,180,169,157,145,133,122,110,98,86,75,65,55,46,38,30,24,18,14,11,8,8,8,9,12,16,21,27,34,42,50,60,70,81,92,104,116,127};
	Clock_Init();
	P6SEL |= 0xff;
	P6DIR = 0xff;
	Init_DAC12_0();
	Init_DAC12_1();
	P6OUT=0x00;
	while(1){
	    angle(nums);
	}

	return 0;
}
