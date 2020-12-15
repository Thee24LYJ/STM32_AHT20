#include "led.h"
#include "usart.h"
#include "temhum.h"

 int main(void)
 {	 	
    u32 CT_data[2]={0};
    volatile float  hum=0,tem=0;     
     
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
    LED_Init();			     //LED�˿ڳ�ʼ��
    temphum_init();     //ATH20��ʼ��    
	
	while(1)
	{
        AHT20_Read_CTdata(CT_data);       //������CRCУ�飬ֱ�Ӷ�ȡAHT20���¶Ⱥ�ʪ������ 

        hum = CT_data[0]*100*10/1024/1024;  //����õ�ʪ��ֵ���Ŵ���10����
        tem = CT_data[1]*200*10/1024/1024-500;//����õ��¶�ֵ���Ŵ���10����

        printf("ʪ��:%.1f%%\r\n",(hum/10));
        printf("�¶�:%.1f��\r\n",(tem/10));
        printf("\r\n");
        
        //��ʱ2s,LED��˸��ʾ���ڷ���״̬
        LED=0;
        delay_ms(1000);
        LED=1;
        delay_ms(1000);
	 }

}
 
