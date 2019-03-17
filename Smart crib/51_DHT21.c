//****************************************************************//
//DHT21ʹ�÷���
//��Ƭ�� AT89S52 �� STC89C52RC
//����   ���ڷ�����ʪ������ ���� 11.0592M ������ 9600
//Ӳ��   P2.0��ΪͨѶ������DHT11,DHT11�ĵ�Դ�͵����ӵ�Ƭ���ĵ�Դ�͵أ���Ƭ�����ڼ�MAX232���ӵ���
//****************************************************************//

#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4
////
sbit  P2_0  = P1^7 ;		 //��ʪ�ȴ������ű���
sbit P16 = P1^6;	 //	 ��ʪ�ȴ������ű�����
sbit fmq = P3^6;		  //������
sbit voice = P2^1;	//�������


void delay1ms(uint i)
{
	int j;
	while(i--)
	{
	for(j=115; j>=0; j--);
	}
}
//----------------------------------------------//
//----------------IO�ڶ�����--------------------//
//----------------------------------------------//

//----------------------------------------------//
//----------------������--------------------//
//----------------------------------------------//
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H2,U8T_data_L2,U8RH_data_H2,U8RH_data_L2,U8checkdata2;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //���巢�͵��ֽ���
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
	   	//��ʱ������forѭ��
        if(U8FLAG==1)break;
	   	//�ж�����λ��0����1
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1
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
	   	//��ʱ������forѭ��
        if(U8FLAG == 1) break;
	   	//�ж�����λ��0����1
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1
        U8comdata <<= 1;
        U8comdata|=U8temp;        //0
    }//rof

}
	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM();
	//--------------------------------

void RH1(void)
{
    //��������18ms
    P2_0 = 0;
    Delay(180);
    P2_0 = 1;
    //������������������ ������ʱ20us
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    //������Ϊ���� �жϴӻ���Ӧ�ź�
    P2_0 = 1;
    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
    if(!P2_0)		 //T !
    {
	   U8FLAG = 2;
       //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
	   while((!P2_0) && U8FLAG++);
	   U8FLAG = 2;
	   //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P2_0) && U8FLAG++);
	  //���ݽ���״̬
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
	 //����У��

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
    //��������18ms
    P16 = 0;
    Delay(180);
    P16 = 1;
    //������������������ ������ʱ20us
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    //������Ϊ���� �жϴӻ���Ӧ�ź�
    P16 = 1;
    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
    if(!P16)		 //T !
    {
	   U8FLAG = 2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
	   while((!P16) && U8FLAG++);
	   U8FLAG = 2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P16) && U8FLAG++);
	 //���ݽ���״̬
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
	 //����У��

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
//main()��������:  AT89C51  11.0592MHz 	���ڷ�
//����ʪ������,������ 9600
//��ע��Ϊ��֤WiFi����Ӧ�¾�·��������Ӧʱ��
//      ÿ�������ǻ���ʱ10��
//----------------------------------------------
void uart_init(void)
{
	TMOD = 0x20;            //��ʱ��/������1,������ʽ2
	TH1 = 0xfd;             //���ò�����Ϊ9600
	TL1 = 0xfd;
	TR1 = 1;                //������ʱ��/������1
	SCON = 0x50;            //���ڹ�����ʽ1�������п���
	PCON = 0x00;            //����SMOD=0
	IE = 0x90;              //CPU�����жϣ����������ж�
	TI = 1;                 //ֱ��ʹ��printf�������˾����ʵ�ַ���

	delay1ms(3000);         //�ȴ�wifi�ϵ����
    printf("AT+CWMODE=3\n");   //	  ����Ϊap+stp����ģʽ
    delay1ms(3000);
    printf("AT+RST\n");      //����
    delay1ms(10000);
    printf("AT+CWJAP=\"DQGC\",\"njw12345\"");        //����·����
    printf("\n");
    delay1ms(10000);
    printf("AT+CIPSTART=\"TCP\",\"192.168.1.103\",9790");	//���ӷ�����
    printf("\n");
    delay1ms(3000);
    printf("AT+CIPMODE=1\n");								//����͸��ģʽ
    delay1ms(3000);
    printf("AT+CIPSEND\n");                                 //��ʼ͸��
    delay1ms(1000);
    printf("Hello,User!\n");
}

void main()
{

	voice = 1;		           //�������λ
    uart_init();
	while(1)
	{
	   delay1ms(1000);
	   //------------------------
	   //������ʪ�ȶ�ȡ�ӳ���
	   RH1();

	   //������ʾ����
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

//	   VoiceScan();			  //�������
		if(voice == 0)
		{
		//	delay1ms(1);
		//	if(voice == 0)
		//	{
				printf("Your child may wake up!\n");
		//	}
		}
//	   Bed_wetting();			//�򴲼��	   (str2����{17}-����str1)
//		if((str2[2]-1) - str[2] >= 20)		   //1A
//		{
//			  printf("Your child may wet your bed!\n");
//		}

//�߱��Ӽ�� 
	//	if( str[0] - (str2[0]-5) >= 0)
	//	{
	//		  printf("The quilt could be kicked!\n");
	//	}

//���ռ��
//	 	if((str2[0]-5)-str[0] > 0)
//		{
//			   printf("Your baby may have a fever!\n");
//		}

	}

}//end of  main

