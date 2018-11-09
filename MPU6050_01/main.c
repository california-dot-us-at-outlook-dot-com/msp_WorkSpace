//#include <msp430.h>

#include  <msp430x16x.h>
#include<stdlib.h>
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))



#define SMPLRT_DIV  0x19    //采样率分频，典型值：0x07(125Hz) */
#define CONFIG   0x1A       // 低通滤波频率，典型值：0x06(5Hz) */
#define GYRO_CONFIG  0x1B   // 陀螺仪自检及测量范围，典型值：0x18(不自检， 2000deg/s) */
#define ACCEL_CONFIG 0x1C  // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不 自检，2G，5Hz) */
#define ACCEL_XOUT_H 0x3B  // 存储最近的 X 轴、Y 轴、Z 轴加速度感应器的测量值 */
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H  0x41   // 存储的最近温度传感器的测量值 */
#define TEMP_OUT_L  0x42
#define GYRO_XOUT_H  0x43 // 存储最近的 X 轴、Y 轴、Z 轴陀螺仪感应器的测量值 */
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1  0x6B // 电源管理，典型值：0x00(正常启用) */
#define WHO_AM_I  0x75 //IIC 地址寄存器(默认数值 0x68，只读) */



unsigned char i2cRead(unsigned char addr){
    I2CNDAT=0x01;
    unsigned char ctlbyte;
    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    U0CTL |= MST;
    I2CIFG &= ~ARDYIFG;
    I2CTCTL &= ~I2CTRX;
    I2CTCTL = I2CSTT+I2CSTP;
    while((I2CIFG&RXRDYIFG)==0);
    ctlbyte=I2CDRB;
    delay_ms(9);
    while((I2CTCTL&I2CSTP)==0x02);
    return ctlbyte;
}

void Clock_Init()
{
  unsigned char i;
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
unsigned char i2cWrite(unsigned char addr,unsigned char dat){
    //I2CNDAT=2;
    I2CNDAT=0x02;
    //I2CSA=(0xd0>>1);

    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    //I2CNDAT=0x02;
    //P6OUT=~1;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(9);
    //I2CNDAT=0x01;
    //P6OUT=~2;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=dat;
    delay_ms(9);
    //P6OUT=~3;
    while((I2CTCTL&I2CSTP)==0x02);
}

void i2c_init(){

    P3SEL|=0x0a;//P3.1   P3.3
    P3DIR&=~0x0a;
    U0CTL|=I2C+SYNC;
    U0CTL&=~I2CEN;
    I2CTCTL=I2CSSEL_2;

    //I2CNDAT=0x01;
    I2CSA=(0xd0>>1);
    U0CTL|=I2CEN;

}

void mpuWrite(unsigned char addr,unsigned char dat){

}

void MPU6050_Init()             //初始化过程 ，其实就是写 5个寄存器
         {       /*
                 MPU6050_I2C_ByteWrite(0xd0,0x00,MPU6050_RA_PWR_MGMT_1);      // reg107, 唤醒，8M内部时钟源
                 MPU6050_I2C_ByteWrite(0xd0,0x07,MPU6050_RA_SMPLRT_DIV);         //采用频率 1000
                 MPU6050_I2C_ByteWrite(0xd0,0x06,MPU6050_RA_CONFIG);
                 MPU6050_I2C_ByteWrite(0xd0,0x01,MPU6050_RA_ACCEL_CONFIG);     //加速度量程 2g
                 MPU6050_I2C_ByteWrite(0xd0,0x18,MPU6050_RA_GYRO_CONFIG);          //角速度量程 2000度/s
                 */
                i2cWrite(PWR_MGMT_1,0x00);
                i2cWrite(SMPLRT_DIV,0x07);
                i2cWrite(CONFIG,0x06);
                i2cWrite(ACCEL_CONFIG,0x01);
                i2cWrite(GYRO_CONFIG,0x18);
         }

unsigned int readMpu16(unsigned char addr){
    unsigned char i,j;
    i=i2cRead(addr);
    j=i2cRead(addr+1);
    return (i<<8)+j;
}

unsigned char *recvBuf;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//recvBuf=(unsigned char *)malloc(sizeof(unsigned char)*2);
	Clock_Init();
	i2c_init();
	MPU6050_Init();
	P6DIR=0xff;
	while(1){
	    P6OUT=~(readMpu16(0x3b)>>8);
	    //delay_ms(500);
	}
	return 0;
}
