#include "temhum.h"

//读取AHT20的状态寄存器
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

//向AHT20发送AC命令
void AHT20_SendAC(void) 
{
	IIC_Start();
	IIC_Send_Byte(0x70);    //启动传输后发送的01110000 （最后1bit表示读/写 0--写,1--读）
	IIC_Wait_Ack();
	IIC_Send_Byte(0xac);//0xAC采集命令 命令参数有两个字节，第一个字节为 0x33，第二个字节为0x00。
	IIC_Wait_Ack();
	IIC_Send_Byte(0x33);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();

}

//初始化AHT20
void AHT20_Init(void)   
{	
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(0x70);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xa8);//0xA8进入NOR工作模式
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();

	delay_ms(10);//延时10ms左右

	IIC_Start();
	IIC_Send_Byte(0x70);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xbe);//0xBE初始化命令，AHT20的初始化命令是0xBE,   AHT10的初始化命令是0xE1
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);//相关寄存器bit[3]置1，为校准输出
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);//延时10ms左右
}

void temphum_init()
{
	delay_ms(40);//刚上电，延时40ms才可以读取状态
	
    //首先发0x71读取状态字bit[3]，如果=1,为校准输出，无须初始化!!!正常情况下读回来的状态是0x1C或者是0x18,读回来是0x80表示忙状态;
	if(!((AHT20_Read_Status()&0x08)==0x08))
	{
        AHT20_Init(); //初始化AHT20 
	}
}

//没有CRC校验，直接读取AHT20的温度和湿度数据    
void AHT20_Read_CTdata(u32 *ct) 
{
	volatile u8 Byte_1th=0,Byte_2th=0,Byte_3th=0;
    volatile u8 Byte_4th=0,Byte_5th=0,Byte_6th=0;
	u32 RetuData = 0;
	u16 cnt = 0,flag;
	AHT20_SendAC();//向AHT20发送AC命令
	delay_ms(80);	//大约延时80ms
    
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		delay_ms(1);
		if(cnt++>=100) break;
	}
    
	IIC_Start();
	IIC_Send_Byte(0x71);
	flag=IIC_Wait_Ack();
	Byte_1th = IIC_Read_Byte(flag);//状态字
	Byte_2th = IIC_Read_Byte(flag);//湿度,发送ACK(继续发送)
	Byte_3th = IIC_Read_Byte(flag);//湿度
	Byte_4th = IIC_Read_Byte(flag);//湿度/温度
	Byte_5th = IIC_Read_Byte(flag);//温度
	Byte_6th = IIC_Read_Byte(!flag);//温度,发送NACK(停止发送)  
	IIC_Stop();
    
    //保存得到的数据到RetuData中
	RetuData = (RetuData|Byte_2th)<<8;  
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//湿度
    
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0x0fffff;
	ct[1] =RetuData; //温度
}
