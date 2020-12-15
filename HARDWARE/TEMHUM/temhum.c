#include "temhum.h"

//��ȡAHT20��״̬�Ĵ���
u8 AHT20_Read_Status(void)
{
	u8 Byte_first,flag;	
	IIC_Start();
    IIC_Send_Byte(0x71);
	flag=IIC_Wait_Ack();
	Byte_first = IIC_Read_Byte(flag);
	IIC_NAck();
	IIC_Stop();

	return Byte_first;
}

//��AHT20����AC����
void AHT20_SendAC(void) 
{
	IIC_Start();
	IIC_Send_Byte(0x70);    //����������͵�01110000 �����1bit��ʾ��/д 0--д,1--����
	IIC_Wait_Ack();
	IIC_Send_Byte(0xac);//0xAC�ɼ����� ��������������ֽڣ���һ���ֽ�Ϊ 0x33���ڶ����ֽ�Ϊ0x00��
	IIC_Wait_Ack();
	IIC_Send_Byte(0x33);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();

}

//��ʼ��AHT20
void AHT20_Init(void)   
{	
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(0x70);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xa8);//0xA8����NOR����ģʽ
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();

	delay_ms(10);//��ʱ10ms����

	IIC_Start();
	IIC_Send_Byte(0x70);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xbe);//0xBE��ʼ�����AHT20�ĳ�ʼ��������0xBE,   AHT10�ĳ�ʼ��������0xE1
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);//��ؼĴ���bit[3]��1��ΪУ׼���
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);//��ʱ10ms����
}

void temphum_init()
{
	delay_ms(40);//���ϵ磬��ʱ40ms�ſ��Զ�ȡ״̬
	
    //���ȷ�0x71��ȡ״̬��bit[3]�����=1,ΪУ׼����������ʼ��!!!��������¶�������״̬��0x1C������0x18,��������0x80��ʾæ״̬;
	if(!((AHT20_Read_Status()&0x08)==0x08))
	{
        AHT20_Init(); //��ʼ��AHT20 
	}
}

//û��CRCУ�飬ֱ�Ӷ�ȡAHT20���¶Ⱥ�ʪ������    
void AHT20_Read_CTdata(u32 *ct) 
{
	volatile u8 Byte_1th=0,Byte_2th=0,Byte_3th=0;
    volatile u8 Byte_4th=0,Byte_5th=0,Byte_6th=0;
	u32 RetuData = 0;
	u16 cnt = 0,flag;
	AHT20_SendAC();//��AHT20����AC����
	delay_ms(80);	//��Լ��ʱ80ms
    
	while(((AHT20_Read_Status()&0x80)==0x80))//ֱ��״̬bit[7]Ϊ0����ʾΪ����״̬����Ϊ1����ʾæ״̬
	{
		delay_ms(1);
		if(cnt++>=100) break;
	}
    
	IIC_Start();
	IIC_Send_Byte(0x71);
	flag=IIC_Wait_Ack();
	Byte_1th = IIC_Read_Byte(flag);//״̬��
	Byte_2th = IIC_Read_Byte(flag);//ʪ��,����ACK(��������)
	Byte_3th = IIC_Read_Byte(flag);//ʪ��
	Byte_4th = IIC_Read_Byte(flag);//ʪ��/�¶�
	Byte_5th = IIC_Read_Byte(flag);//�¶�
	Byte_6th = IIC_Read_Byte(!flag);//�¶�,����NACK(ֹͣ����)  
	IIC_Stop();
    
    //����õ������ݵ�RetuData��
	RetuData = (RetuData|Byte_2th)<<8;  
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//ʪ��
    
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0x0fffff;
	ct[1] =RetuData; //�¶�
}
