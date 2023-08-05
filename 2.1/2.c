#include <reg52.h> 		  //包含单片机寄存器的头文件
#include <stdio.h>
#include <math.h>
#include <intrins.h>
#include "JY901.h"

sbit moto1=P1^0;//第一个舵机 
sbit moto2=P1^1;//第二个舵机 
sbit K4=P3^3;//（短接） 复位 

unsigned char Duty=5;	   //初始为5角度0
unsigned char Period=200;	//周期20ms
unsigned char times=0;		//计数
unsigned char Duty1=5;	   //初始为5角度0	   

void delay1s900ms(void)   //误差 -0.000000000432us	1.9秒
{
    unsigned char a,b,c;
    for(c=101;c>0;c--)
        for(b=214;b>0;b--)
            for(a=39;a>0;a--);
}


void delay100us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=47;a>0;a--);
}


void delay400ms(void)   //误差 -0.000000000091us 0.4秒
{
    unsigned char a,b,c;
    for(c=103;c>0;c--)
        for(b=24;b>0;b--)
            for(a=73;a>0;a--);
}


void delay(unsigned int i)
{
	while(i--);
}

void time0init()
{
	TMOD=0X01;
	TL0=0x9D;		//设置定时初值
	TH0=0xFF;		//设置定时初值 0.1ms
	TR0=1;
	ET0=1;
	EA=1;
}

void main(void)
{
	float Vz;//定义速度 
	unsigned char i=0;		
	stcAngle.Angle[0]=stcAngle.Angle[0]/(32768*180);//180为角度 
	stcAngle.Angle[1]=stcAngle.Angle[1]/(32768*180);
	stcAngle.Angle[2]=stcAngle.Angle[2]/(32768*180);

	stcAcc.a[0]=stcAcc.a[0]/(32768*16);//加速度计可测范围16g 
	stcAcc.a[1]=stcAcc.a[1]/(32768*16);
    stcAcc.a[2]=stcAcc.a[2]/(32768*16);

    stcGPSV.lGPSVelocity=(stcGPSV.lGPSVelocity)/1000;//算出陀螺仪速度的变化（积分）	
	Vz=(stcGPSV.lGPSVelocity)*cos(stcAngle.Angle[0])*cos(stcAngle.Angle[1]);//stcGPSV.lGPSVelocity为当前此刻的瞬时速度 

	time0init();
	while(1)
	{
	   if(K4==1&&Vz==0)//当瞬时速度为0 
	   {
		 delay(1000);//延时10ms
		 if(K4==1&&Vz==0)
	   	 {
		 		delay1s900ms();
				Duty=Duty+10;//滑翔机脱离 
				delay400ms();
				Duty1=Duty1+10;//开伞 
		 }
		
	   }
		  delay100us();  // 0.1s    这里是因为陀螺仪不一定能处于最佳位置，不能够保证瞬时速度为0，此处作为强制开伞 
		 if(K4==1&&Vz!=0)
		 delay(1000);
		if(K4==1&&Vz!=0)
	   	 {
		 	
				Duty=Duty+10;
				delay400ms();
				Duty1=Duty1+10;
		 }
		 
	 }
}

void time0() interrupt 1
{
	TL0 = 0x9D;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值  0.1ms
	times++;
	if(times<=Duty)
	{
	   moto1=1;
	}
	else
	{
	   moto1=0;
	} 


	if(times<=Duty1)
	{
	   moto2=1;
	}
	else
	{
	   moto2=0;
	} 


	if(times>=Period)
	{
	   times=0;
	}
}