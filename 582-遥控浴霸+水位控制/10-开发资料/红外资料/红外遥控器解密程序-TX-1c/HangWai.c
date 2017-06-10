#include <reg52.h>
#define uchar unsigned char 

//������ӵ��ⲿ�ж�1
sbit dula=P2^6;
sbit wela=P2^7;
sbit LEDR=P2^2;
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                    0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

//ʱ�����
#define Imax 14000    //�˴�Ϊ����Ϊ11.0592ʱ��ȡֵ, 
#define Imin 8000    //��������Ƶ�ʵľ���ʱ,
#define Inum1 1450    //Ҫ�ı���Ӧ��ȡֵ��
#define Inum2 700 
#define Inum3 3000 
//�������
unsigned char Im[4]={0x00,0x00,0x00,0x00};
uchar show[2]={0,0};

//ȫ�ֱ���
uchar f;
unsigned long m,Tc;
unsigned char IrOK;


//��ʱ
void delay(uchar i)
{
  uchar j,k; 
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

//�������ʾ
void display()
{
   dula=0;
   P0=table[show[0]];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[show[1]];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);
}


//�ⲿ�жϽ������_�ⲿ�ж�0
void intersvr1(void) interrupt 2 using 1
{
 TR0=1;
 Tc=TH0*256+TL0;//��ȡ�ж�ʱ����ʱ��
 TH0=0; 
 TL0=0;         //��ʱ�ж���������
 if((Tc>Imin)&&(Tc<Imax))
 { 
	m=0;
	f=1;
	return;
 }       //�ҵ���ʼ��
 if(f==1)
 {
    if(Tc>Inum1&&Tc<Inum3) 
    {
   	Im[m/8]=Im[m/8]>>1|0x80; m++; 
    }
    if(Tc>Inum2&&Tc<Inum1) 
    {
      Im[m/8]=Im[m/8]>>1; m++; //ȡ��
  	 }
  	 if(m==32) 
    {
      m=0;  
      f=0;
      if(Im[2]==~Im[3]) 
      {
           IrOK=1;
			  TR0=0; 
   	}
      else IrOK=0;   //ȡ����ɺ��ж϶����Ƿ���ȷ
    }
               //׼������һ��
 }
 
}


/*��ʾ������*/
void main(void)
{
 unsigned int  a;
   
 m=0;
 f=0;
 EA=1;//�������ж�
 
 IT1=1;//�½�����Ч
 EX1=1;//�ⲿ�ж�1��
 
 TMOD=0x11;//��ʱ����ʼ��  
 TH0=0;//T0����ֵ
 TL0=0;
 TR0=0;//t0��ʼ��ʱ

 while(1)
 {
 
  if(IrOK==1) 
  {
      show[1]=Im[2] & 0x0F;     //ȡ����ĵ���λ
      show[0]=Im[2] >> 4;  
      IrOK=0;
     LEDR=0;
  }
  for(a=100;a>0;a--)
   {
    //display();
   }
 
 }
}

