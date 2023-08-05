#include <reg52.h> 		  //������Ƭ���Ĵ�����ͷ�ļ�
#include <stdio.h>
#include <math.h>
#include <intrins.h>
#include "JY901.h"

sbit moto1=P1^0;//��һ����� 
sbit moto2=P1^1;//�ڶ������ 
sbit K4=P3^3;//���̽ӣ� ��λ 

unsigned char Duty=5;	   //��ʼΪ5�Ƕ�0
unsigned char Period=200;	//����20ms
unsigned char times=0;		//����
unsigned char Duty1=5;	   //��ʼΪ5�Ƕ�0	   

void delay1s900ms(void)   //��� -0.000000000432us	1.9��
{
    unsigned char a,b,c;
    for(c=101;c>0;c--)
        for(b=214;b>0;b--)
            for(a=39;a>0;a--);
}


void delay100us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=47;a>0;a--);
}


void delay400ms(void)   //��� -0.000000000091us 0.4��
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
	TL0=0x9D;		//���ö�ʱ��ֵ
	TH0=0xFF;		//���ö�ʱ��ֵ 0.1ms
	TR0=1;
	ET0=1;
	EA=1;
}

void main(void)
{
	float Vz;//�����ٶ� 
	unsigned char i=0;		
	stcAngle.Angle[0]=stcAngle.Angle[0]/(32768*180);//180Ϊ�Ƕ� 
	stcAngle.Angle[1]=stcAngle.Angle[1]/(32768*180);
	stcAngle.Angle[2]=stcAngle.Angle[2]/(32768*180);

	stcAcc.a[0]=stcAcc.a[0]/(32768*16);//���ٶȼƿɲⷶΧ16g 
	stcAcc.a[1]=stcAcc.a[1]/(32768*16);
    stcAcc.a[2]=stcAcc.a[2]/(32768*16);

    stcGPSV.lGPSVelocity=(stcGPSV.lGPSVelocity)/1000;//����������ٶȵı仯�����֣�	
	Vz=(stcGPSV.lGPSVelocity)*cos(stcAngle.Angle[0])*cos(stcAngle.Angle[1]);//stcGPSV.lGPSVelocityΪ��ǰ�˿̵�˲ʱ�ٶ� 

	time0init();
	while(1)
	{
	   if(K4==1&&Vz==0)//��˲ʱ�ٶ�Ϊ0 
	   {
		 delay(1000);//��ʱ10ms
		 if(K4==1&&Vz==0)
	   	 {
		 		delay1s900ms();
				Duty=Duty+10;//��������� 
				delay400ms();
				Duty1=Duty1+10;//��ɡ 
		 }
		
	   }
		  delay100us();  // 0.1s    ��������Ϊ�����ǲ�һ���ܴ������λ�ã����ܹ���֤˲ʱ�ٶ�Ϊ0���˴���Ϊǿ�ƿ�ɡ 
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
	TL0 = 0x9D;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ  0.1ms
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