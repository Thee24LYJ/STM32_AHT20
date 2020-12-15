#ifndef __TEMHUM_H
#define __TEMHUM_H

/*******************
*
*  @file    temhum.h 
*  @author  LYJ
*  @version V1.0
*  @attention
*       用于AHT20温湿度传感器通信的相关函数
*
********************/

#include "myiic.h"
#include "delay.h"

void temphum_init(void);
u8 AHT20_Read_Status(void);
void AHT20_SendAC(void);
void AHT20_Init(void);
void AHT20_Read_CTdata(u32 *ct);


#endif
