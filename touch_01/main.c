#include<msp430f169.h>
#include"Config.h"
#include"Touch.h"

//TFT_start
unsigned int Device_code=0x9320;
void Write_Data(unsigned char DH,unsigned char DL)
{

    CS_CLR;
    RS_SET;

    //注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下
    DataPort=DH;            //送低8位数据给573待锁存
    LE_SET;                   //锁存位
    LE_CLR;                   //断开锁存,位选573的Q7~Q0仍保持

    DataPort=DL;            //送高8位数据给TFT

    /*
    //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
    DataPort_L=DL;
    DataPort_H=DH;
    */

    RW_CLR;
    RW_SET;
    CS_SET;
}

void Write_Cmd(unsigned char DH,unsigned char DL)
{
    CS_CLR;
    RS_CLR;
        RD_SET;
        RW_CLR;

    //注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下
    DataPort=DH;            //送低8位命令给573待锁存
    LE_SET;                   //锁存位
    LE_CLR;                   //断开锁存,位选573的Q7~Q0仍保持

    DataPort=DL;            //送高8位命令给TFT

    /*
    //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
    DataPort_L=DL;
    DataPort_H=DH;
    */

    RW_SET;
    CS_SET;
}

void  Write_Cmd_Data (unsigned char x,unsigned int y)
{
    unsigned char m,n;
    m=y>>8;
    n=y;
    Write_Cmd(0x00,x);
    Write_Data(m,n);
}

void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
{

  Write_Cmd_Data(0x50,x0);  // Horizontal GRAM Start Address
  Write_Cmd_Data(0x51,x1);  // Horizontal GRAM End Address
  Write_Cmd_Data(0x52,y0);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x53,y1);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x20,x0);  // GRAM horizontal Address
  Write_Cmd_Data(0x21,y0);  // GRAM Vertical Address
  Write_Cmd (0x00,0x22);    // 0x0022,Start to Write Data to GRAM
}


void TFT_Initial(void)
{

    RST_SET;
    delay_ms(1);                    // Delay 1ms
    RST_CLR;
    delay_ms(10);                   // Delay 10ms
    RST_SET;
    delay_ms(50);                   // Delay 50 ms

    if(Device_code==0x9320)
     {
        P6OUT=0xf0;
    //************* Start Initial Sequence **********//
    Write_Cmd_Data(0x00,0x0001);   //Set the OSC bit as ‘1’ to start the internal oscillator
    Write_Cmd_Data(0x01,0x0100);   // set SS and SM bit
    Write_Cmd_Data(0x02,0x0700);   // set 1 line inversion
    Write_Cmd_Data(0x03,0x1030);   //set GRAM Write direction and BGR=1
    Write_Cmd_Data(0x04,0x0000);   // Resize register
    Write_Cmd_Data(0x08,0x0202);   // set the back porch and front porch
    Write_Cmd_Data(0x09,0x0000);   // set non-display area refresh cycle ISC[3:0]
    Write_Cmd_Data(0x0A,0x0000);   // FMARK function
    Write_Cmd_Data(0x0C,0x0000);   // RGB interface setting
    Write_Cmd_Data(0x0D,0x0000);   // Frame marker Position
    Write_Cmd_Data(0x0F,0x0000);   // RGB interface polarity
    delay_ms(30);
    //*************Power On sequence ****************//
    Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
    delay_ms(30);
    Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
    Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
    Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
    delay_ms(30);
    Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
    delay_ms(30); // Delay 50ms

    // ----------- Adjust the Gamma Curve ----------//
    Write_Cmd_Data(0x0030, 0x0000);
    Write_Cmd_Data(0x0031, 0x0505);
    Write_Cmd_Data(0x0032, 0x0304);
    Write_Cmd_Data(0x0035, 0x0006);
    Write_Cmd_Data(0x0036, 0x0707);
    Write_Cmd_Data(0x0037, 0x0105);
    Write_Cmd_Data(0x0038, 0x0002);
    Write_Cmd_Data(0x0039, 0x0707);
    Write_Cmd_Data(0x003C, 0x0704);
    Write_Cmd_Data(0x003D, 0x0807);

    //------------------ Set GRAM area ---------------//
    Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
    Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
    Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
    Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
    Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
    Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
    Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
    Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
    Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

    //-------------- Partial Display Control ---------//
    Write_Cmd_Data(0x0080, 0x0000);
    Write_Cmd_Data(0x0081, 0x0000);
    Write_Cmd_Data(0x0082, 0x0000);
    Write_Cmd_Data(0x0083, 0x0000);
    Write_Cmd_Data(0x0084, 0x0000);
    Write_Cmd_Data(0x0085, 0x0000);

    //-------------- Panel Control ---------//
    Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
    Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
    Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3.
    Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
    Write_Cmd_Data(0x97,0x0000);   //
    Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.

    //-------------- Display on ---------//
    Write_Cmd_Data(0x07,0x0173);

    }

    else if(Device_code==0x1505 )
     {

    //************* Start Initial Sequence **********//
    Write_Cmd_Data(0x00,0x0001);   //Set the OSC bit as ‘1’ to start the internal oscillator
    Write_Cmd_Data(0x01,0x0100);   // set SS and SM bit
    Write_Cmd_Data(0x02,0x0700);   // set 1 line inversion
    Write_Cmd_Data(0x03,0x1030);   //set GRAM Write direction and BGR=1
    Write_Cmd_Data(0x04,0x0000);   // Resize register
    Write_Cmd_Data(0x08,0x0202);   // set the back porch and front porch
    Write_Cmd_Data(0x09,0x0000);   // set non-display area refresh cycle ISC[3:0]
    Write_Cmd_Data(0x0A,0x0000);   // FMARK function
    Write_Cmd_Data(0x0C,0x0000);   // RGB interface setting
    Write_Cmd_Data(0x0D,0x0000);   // Frame marker Position
    Write_Cmd_Data(0x0F,0x0000);   // RGB interface polarity
    delay_ms(30);
    //*************Power On sequence ****************//
    Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
    delay_ms(30);
    Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
    Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
    Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
    delay_ms(30);
    Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
    delay_ms(30); // Delay 50ms

    // ----------- Adjust the Gamma Curve ----------//
    Write_Cmd_Data(0x0030, 0x0000);
    Write_Cmd_Data(0x0031, 0x0505);
    Write_Cmd_Data(0x0032, 0x0304);
    Write_Cmd_Data(0x0035, 0x0006);
    Write_Cmd_Data(0x0036, 0x0707);
    Write_Cmd_Data(0x0037, 0x0105);
    Write_Cmd_Data(0x0038, 0x0002);
    Write_Cmd_Data(0x0039, 0x0707);
    Write_Cmd_Data(0x003C, 0x0704);
    Write_Cmd_Data(0x003D, 0x0807);

    //------------------ Set GRAM area ---------------//
    Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
    Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
    Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
    Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
    Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
    Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
    Write_Cmd_Data(0x006A, 0x2700); // set scrolling line
    Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
    Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

    //-------------- Partial Display Control ---------//
    Write_Cmd_Data(0x0080, 0x0000);
    Write_Cmd_Data(0x0081, 0x0000);
    Write_Cmd_Data(0x0082, 0x0000);
    Write_Cmd_Data(0x0083, 0x0000);
    Write_Cmd_Data(0x0084, 0x0000);
    Write_Cmd_Data(0x0085, 0x0000);

    //-------------- Panel Control ---------//
    Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
    Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
    Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3.
    Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
    Write_Cmd_Data(0x97,0x0000);   //
    Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.

    //-------------- Display on ---------//
    Write_Cmd_Data(0x07,0x0173);

    }

    else if(Device_code==0x9328)
     {

    //************* Start Initial Sequence **********//
     Write_Cmd_Data(0x0001,0x0100);   //set SS and SM bit //设置扫描方向
    Write_Cmd_Data(0x0002,0x0700);   //EOR=1 and B/C=1 to set the line inversion  //设置行反转
    Write_Cmd_Data(0x0003,0x1030);   //set Entry Mode  //设置进入模式
    Write_Cmd_Data(0x0004,0x0000);   //
    Write_Cmd_Data(0x00A4,0x0001);
    Write_Cmd_Data(0x0008,0x0202); // set the back porch and front porch
    Write_Cmd_Data(0x0009,0x0000); // set non-display area refresh cycle ISC[3:0]
    Write_Cmd_Data(0x000A,0x0000); // FMARK function
    Write_Cmd_Data(0x000C,0x0000); // RGB interface setting
    Write_Cmd_Data(0x000D, 0x0000); // Frame marker Position
    Write_Cmd_Data(0x000F, 0x0000); // RGB interface polarity



//*************Power On sequence ****************//
    Write_Cmd_Data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    Write_Cmd_Data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    Write_Cmd_Data(0x0012, 0x0000); // VREG1OUT voltage
    Write_Cmd_Data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
    delay_ms(30);
    Write_Cmd_Data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
    Write_Cmd_Data(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
    delay_ms(30);
    Write_Cmd_Data(0x0012, 0x001C); // External reference voltage= Vci;
    delay_ms(30);
    Write_Cmd_Data(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
    Write_Cmd_Data(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
    Write_Cmd_Data(0x002B, 0x000D); // Frame Rate = 91Hz
    delay_ms(30);
    Write_Cmd_Data(0x0020, 0x0000); // GRAM horizontal Address
    Write_Cmd_Data(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
    Write_Cmd_Data(0x0030, 0x0007);
    Write_Cmd_Data(0x0031, 0x0302);
    Write_Cmd_Data(0x0032, 0x0105);
    Write_Cmd_Data(0x0035, 0x0206);
    Write_Cmd_Data(0x0036, 0x0808);
    Write_Cmd_Data(0x0037, 0x0206);
    Write_Cmd_Data(0x0038, 0x0504);
    Write_Cmd_Data(0x0039, 0x0007);
    Write_Cmd_Data(0x003C, 0x0105);
    Write_Cmd_Data(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
    Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
    Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
    Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
    delay_ms(30);
    Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
    delay_ms(30);
    Write_Cmd_Data(0x0060, 0xA700); // Gate Scan Line
    Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
    Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
    Write_Cmd_Data(0x0080, 0x0000);
    Write_Cmd_Data(0x0081, 0x0000);
    Write_Cmd_Data(0x0082,0x0000);
    Write_Cmd_Data(0x0083,0x0000);
    Write_Cmd_Data(0x0084,0x0000);
    Write_Cmd_Data(0x0085,0x0000);
//-------------- Panel Control -------------------//
    Write_Cmd_Data(0x0090, 0x0010);
    Write_Cmd_Data(0x0092, 0x0000);
    Write_Cmd_Data(0x0093, 0x0003);
    Write_Cmd_Data(0x0095, 0x0110);
    Write_Cmd_Data(0x0097, 0x0000);
    Write_Cmd_Data(0x0098, 0x0000);
    Write_Cmd_Data(0x0007, 0x0133); // 262K color and display ON

    }
}
//TFT_end

/********************************************************************
                      主函数
********************************************************************/
void Clock_Inita()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}

//WBM
uchar Getdot() //取采样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
{
    uchar flag=0;
    struct struct1 pix1;
    if (PEN==0)
        {
            pix1=AD7843();
            //pix2=AD7843();
            lx=(uint)(240.0*(pix1.x-Xmin)/(Xmax-Xmin));       //坐标转换，即根据采样值计算实际坐标值
            ly=(uint)(320.0*(pix1.y-Ymin)/(Ymax-Ymin));   //Xmin、max和Ymin、Ymax分别是触摸屏横纵坐标的最小/最大值
            flag=1;
        }
            return flag;

}



void main()
{
  //WDT_Init();                        //看门狗设置
    WDTCTL = WDTPW + WDTHOLD;
    Clock_Inita();                      //系统时钟设置
    P6DIR=0xff;
    P4DIR=0xff;
    P5DIR=0xff;
    P3DIR=0xff;
    P6OUT=0xf0;
    P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;
    start_7843();

    TFT_Initial();
  while(1)
  {
//      P6OUT=P2IN;

    if(Getpix()==1){
        LCD_SetPos(lx,lx,ly,ly);
        Write_Data(0xf8,0x00);
        P6OUT=P2IN;
    }else{
        P6OUT=0xff;
    }
  }
  //flag=0;
   
}
