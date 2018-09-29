/*
 * maina.c
 *
 *  Created on: 2018年9月26日
 *      Author: Cal
 */


//中断只有 P1和P2


#include<msp430f169.h>


#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    P6OUT=~P6OUT;
    P1IFG &= 0;
}


void main(){
    WDTCTL = WDTPW + WDTHOLD;
    P6DIR=0xff;
    //P1 init
    P1DIR &= 0;
    P1IES |= BIT0+BIT1; //0:上升沿；    1：下降沿
    P1IE |= BIT0+BIT2;  //中断允许，1为允许
    P1IFG &= 0;  //中断标志，0为可接受中断
    _EINT();    //开启总中断
}
