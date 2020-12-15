#include "led.h"
#include "usart.h"
#include "temhum.h"

 int main(void)
 {	 	
    u32 CT_data[2]={0};
    volatile float  hum=0,tem=0;     
     
    delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
    LED_Init();			     //LED端口初始化
    temphum_init();     //ATH20初始化    
	
	while(1)
	{
        AHT20_Read_CTdata(CT_data);       //不经过CRC校验，直接读取AHT20的温度和湿度数据 

        hum = CT_data[0]*100*10/1024/1024;  //计算得到湿度值（放大了10倍）
        tem = CT_data[1]*200*10/1024/1024-500;//计算得到温度值（放大了10倍）

        printf("湿度:%.1f%%\r\n",(hum/10));
        printf("温度:%.1f度\r\n",(tem/10));
        printf("\r\n");
        
        //延时2s,LED闪烁提示串口发送状态
        LED=0;
        delay_ms(1000);
        LED=1;
        delay_ms(1000);
	 }

}
 
