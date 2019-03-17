//****************************************************************//
//DHT21使用范例
//单片机 AT89S52 或 STC89C52RC
//功能   串口发送温湿度数据 晶振 11.0592M 波特率 9600
//硬件   P2.0口为通讯口连接DHT11,DHT11的电源和地连接单片机的电源和地，单片机串口加MAX232连接电脑
//****************************************************************//

#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4
////
sbit  P2_0  = P1^7 ;		 //温湿度传感器放被窝
sbit P16 = P1^6;	 //	 温湿度传感器放被窝外
sbit fmq = P3^6;		  //蜂鸣器
sbit voice = P2^1;	//声音检测


void delay1ms(uint i)
{
	int j;
	while(i--)
	{
	for(j=115; j>=0; j--);
	}
}
//----------------------------------------------//
//----------------IO口定义区--------------------//
//----------------------------------------------//

//----------------------------------------------//
//----------------定义区--------------------//
//----------------------------------------------//
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H2,U8T_data_L2,U8RH_data_H2,U8RH_data_L2,U8checkdata2;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //定义发送的字节数
U8  indata[5];
U8  count, count_r = 0;
U8  str[5] = {"RS232"};
U8  str2[5] = {"RS232"};
U16 U16temp1,U16temp2;
SendData(U8 *a)
{
	outdata[0] = a[0];
	outdata[1] = a[1];
	outdata[2] = a[2];
	outdata[3] = a[3];
	outdata[4] = a[4];
	count = 1;
	SBUF = outdata[0];
}

void Delay(U16 j)
{
    U8 i;
    for(; j>0; j--)
    {
		for(i=0; i<27; i++) ;
    }
}
void  Delay_10us(void)
{
    U8 i;
    i--;
    i--;
    i--;
    i--;
    i--;
    i--;
}

 void  COM(void)
{

    U8 i;

    for(i=0;i<8;i++)
    {

        U8FLAG=2;
	   	while((!P2_0) && U8FLAG++);
        Delay_10us();
        Delay_10us();
        Delay_10us();
        U8temp = 0;
        if(P2_0)U8temp = 1;
        U8FLAG=2;
        while((P2_0) && U8FLAG++);
	   	//超时则跳出for循环
        if(U8FLAG==1)break;
	   	//判断数据位是0还是1
		// 如果高电平高过预定0高电平值则数据位为 1
        U8comdata<<=1;
        U8comdata|=U8temp;        //0
    }//rof

}

void  COM2(void)
{

    U8 i;

    for(i=0;i<8;i++)
    {

        U8FLAG = 2;
	   	while((!P16) && U8FLAG++);
        Delay_10us();
        Delay_10us();
        Delay_10us();
        U8temp = 0;
        if(P16)U8temp = 1;
        U8FLAG = 2;
        while((P16) && U8FLAG++);
	   	//超时则跳出for循环
        if(U8FLAG == 1) break;
	   	//判断数据位是0还是1
		// 如果高电平高过预定0高电平值则数据位为 1
        U8comdata <<= 1;
        U8comdata|=U8temp;        //0
    }//rof

}
	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM();
	//--------------------------------

void RH1(void)
{
    //主机拉低18ms
    P2_0 = 0;
    Delay(180);
    P2_0 = 1;
    //总线由上拉电阻拉高 主机延时20us
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    //主机设为输入 判断从机响应信号
    P2_0 = 1;
    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
    if(!P2_0)		 //T !
    {
	   U8FLAG = 2;
       //判断从机是否发出 80us 的低电平响应信号是否结束
	   while((!P2_0) && U8FLAG++);
	   U8FLAG = 2;
	   //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P2_0) && U8FLAG++);
	  //数据接收状态
	   COM();
	   U8RH_data_H_temp = U8comdata;
	   COM();
	   U8RH_data_L_temp = U8comdata;
	   COM();
	   U8T_data_H_temp = U8comdata;
	   COM();
	   U8T_data_L_temp = U8comdata;
	   COM();
	   U8checkdata_temp = U8comdata;
	   P2_0 = 1;
	 //数据校验

	   U8temp = (U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp == U8checkdata_temp)
	   {
	   	  U8RH_data_L = U8RH_data_L_temp;
	   	  U8RH_data_H = U8RH_data_H_temp;
		  U8T_data_H = U8T_data_H_temp;
	   	  U8T_data_L = U8T_data_L_temp;
	   	  U8checkdata = U8checkdata_temp;
	   }
    }
}

void RH2(void)
{
    //主机拉低18ms
    P16 = 0;
    Delay(180);
    P16 = 1;
    //总线由上拉电阻拉高 主机延时20us
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    //主机设为输入 判断从机响应信号
    P16 = 1;
    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
    if(!P16)		 //T !
    {
	   U8FLAG = 2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束
	   while((!P16) && U8FLAG++);
	   U8FLAG = 2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P16) && U8FLAG++);
	 //数据接收状态
	   COM2();
	   U8RH_data_H_temp = U8comdata;
	   COM2();
	   U8RH_data_L_temp = U8comdata;
	   COM2();
	   U8T_data_H_temp = U8comdata;
	   COM2();
	   U8T_data_L_temp = U8comdata;
	   COM2();
	   U8checkdata_temp = U8comdata;
	   P16 = 1;
	 //数据校验

	   U8temp = (U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp == U8checkdata_temp)
	   {
	   	  U8RH_data_H2 =U8RH_data_H_temp;
	   	  U8RH_data_L2 = U8RH_data_L_temp;
		  U8T_data_H2 = U8T_data_H_temp;
	   	  U8T_data_L2 = U8T_data_L_temp;
	   	  U8checkdata2 = U8checkdata_temp;
	   }
    }
}

//----------------------------------------------
//main()功能描述:  AT89C51  11.0592MHz 	串口发
//送温湿度数据,波特率 9600
//备注：为保证WiFi能适应新旧路由器的响应时间
//      每次启动是会延时10秒
//----------------------------------------------
void uart_init(void)
{
	TMOD = 0x20;            //定时器/计数器1,工作方式2
	TH1 = 0xfd;             //设置波特率为9600
	TL1 = 0xfd;
	TR1 = 1;                //启动定时器/计数器1
	SCON = 0x50;            //串口工作方式1，允许串行控制
	PCON = 0x00;            //设置SMOD=0
	IE = 0x90;              //CPU允许中断，串行允许中断
	TI = 1;                 //直接使用printf必须加入此句才能实现发送

	delay1ms(3000);         //等待wifi上电完成
    printf("AT+CWMODE=3\n");   //	  设置为ap+stp共存模式
    delay1ms(3000);
    printf("AT+RST\n");      //重启
    delay1ms(10000);
    printf("AT+CWJAP=\"DQGC\",\"njw12345\"");        //连接路由器
    printf("\n");
    delay1ms(10000);
    printf("AT+CIPSTART=\"TCP\",\"192.168.1.103\",9790");	//连接服务器
    printf("\n");
    delay1ms(3000);
    printf("AT+CIPMODE=1\n");								//设置透传模式
    delay1ms(3000);
    printf("AT+CIPSEND\n");                                 //开始透传
    delay1ms(1000);
    printf("Hello,User!\n");
}

void main()
{

	voice = 1;		           //声音检测位
    uart_init();
	while(1)
	{
	   delay1ms(1000);
	   //------------------------
	   //调用温湿度读取子程序
	   RH1();

	   //串口显示程序
	   //--------------------------
	   str[0] = U8RH_data_H;
	   str[1] = U8RH_data_L;
	   str[2] = U8T_data_H;
	   str[3] = U8T_data_L;
	   str[4] = U8checkdata;

	   RH2();
	   str2[0] = U8RH_data_H2;
	   str2[1] = U8RH_data_L2;
	   str2[2] = U8T_data_H2;
	   str2[3] = U8T_data_L2;
	   str2[4] = U8checkdata2;

//	   VoiceScan();			  //声音检测
		if(voice == 0)
		{
		//	delay1ms(1);
		//	if(voice == 0)
		//	{
				printf("Your child may wake up!\n");
		//	}
		}
//	   Bed_wetting();			//尿床检测	   (str2里面{17}-外面str1)
//		if((str2[2]-1) - str[2] >= 20)		   //1A
//		{
//			  printf("Your child may wet your bed!\n");
//		}

//踢被子检测 
	//	if( str[0] - (str2[0]-5) >= 0)
	//	{
	//		  printf("The quilt could be kicked!\n");
	//	}

//发烧检测
//	 	if((str2[0]-5)-str[0] > 0)
//		{
//			   printf("Your baby may have a fever!\n");
//		}

	}

}//end of  main

