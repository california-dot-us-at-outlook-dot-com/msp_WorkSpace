#include <msp430.h> 
#include"Config.h"

/**
 * main.c
 */

void wc(unsigned char Data){
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(5);
    EN_CLR;
    delay_ms(5);
}

void wd(unsigned char Data){
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort=Data;
    delay_ms(5);
    EN_CLR;
    delay_ms(5);
}

unsigned char a[]="屌爆了";

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P4SEL=0x00;
	P4DIR=0xFF;
	P5SEL=0x00;
	P5DIR=0xFF;
	PSB_SET;
	RST_SET;

	wc(FUN_MODE);            //显示模式设置
	delay_ms(5);
	wc(FUN_MODE);          //显示模式设置
	delay_ms(5);
	wc(DISPLAY_ON);            //显示开
	delay_ms(5);
	wc(CLEAR_SCREEN);          //清屏
	delay_ms(5);
/*
	char i=0;

	wc(0x80+i);
	delay_ms(5);
	wd(a[0]);
	delay_ms(5);
	wd(a[1]);
	delay_ms(50);
    wd(a[2]);
    delay_ms(5);
    wd(a[3]);
    delay_ms(50);
    wd(a[4]);
    delay_ms(5);
    wd(a[5]);
    delay_ms(50);
*/

	//wc(0x34);



	return 0;
}


/********************************************************************
//DM430-L型系统板控制带字库型12864液晶模块显示测试程序，显示汉字字符
//显示模式为汉字模式，直接将12864插入12864接口即可，朝外，液晶接口位于主板上方
//请注意安装位置，左边有字符说明，为靠近1602接口的1X20座子
//注意选择液晶的电源，位于电位器附近，可选5V或3.3V，根据液晶电压进行选择，默认5V
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/
//#include <msp430x14x.h>
//#include "Config.h"

//*************************************************************************
//          初始化IO口子程序
//*************************************************************************
/*
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT5 + BIT6 + BIT7;
  PSB_SET;        //液晶并口方式
  RST_SET;        //复位脚RST置高
}

//***********************************************************************
//  显示屏命令写入函数
//***********************************************************************
void LCD_write_com(unsigned char com)
{
  RS_CLR;
  RW_CLR;
  EN_SET;
  DataPort = com;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//  显示屏数据写入函数
//***********************************************************************
void LCD_write_data(unsigned char data)
{
  RS_SET;
  RW_CLR;
  EN_SET;
  DataPort = data;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//  显示屏清空显示
//***********************************************************************

void LCD_clear(void)
{
  LCD_write_com(0x01);
  delay_ms(5);
}

//***********************************************************************
//函数名称：DisplayCgrom(uchar hz)显示CGROM里的汉字
//***********************************************************************
void DisplayCgrom(uchar addr,uchar *hz)
{
  LCD_write_com(addr);
  delay_ms(5);
  while(*hz != '\0')
  {
    LCD_write_data(*hz);
    hz++;
    delay_ms(5);
  }

}

//****************************************************************
//函数名称：Display()显示测试结果
//****************************************************************
void Display(void)
{
  DisplayCgrom(0x80,"欣世纪电子欢迎你");
  DisplayCgrom(0x88,"旺:jingyehanxing");
  DisplayCgrom(0x90,"www.avrgcc.com  ");
  DisplayCgrom(0x98,"电话057487470625");
}

//***********************************************************************
//  显示屏初始化函数
//***********************************************************************
void LCD_init(void)
{
  LCD_write_com(FUN_MODE);          //显示模式设置
  delay_ms(5);
  LCD_write_com(FUN_MODE);          //显示模式设置
  delay_ms(5);
  LCD_write_com(DISPLAY_ON);            //显示开
  delay_ms(5);
  LCD_write_com(CLEAR_SCREEN);          //清屏
  delay_ms(5);
}

//***********************************************************************
//      主程序
//***********************************************************************
void main(void)
{
  WDT_Init();                                   //看门狗设置
  Clock_Init();                                 //系统时钟设置
  Port_Init();                                  //系统初始化，设置IO口属性
  delay_ms(100);                                //延时100ms
  LCD_init();                                   //液晶参数初始化设置
  LCD_clear();                                  //清屏
  while(1)
  {
    Display();                                  //显示汉字
  }
}
*/
