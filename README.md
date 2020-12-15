# 基于I2C的温湿度采集模块

> 在STM32平台使用I2C通信方式对AHT20进行温湿度的测量，并通过串口将测量得到的数据发送出来

### 管脚连接

+ AHT20

| AHT20 | STM32F10X |
| :---: | :-------: |
|  SCL  |    PB6    |
|  SDA  |    PB7    |
|  VCC  |    3V3    |
|  GND  |    GND    |

+ 串口

| STM32F10X | USB转TTL |
| :-------: | :------: |
|    PA9    |   TXD    |
|   PA10    |   RXD    |
|    3V3    |   3V3    |
|    GND    |   GND    |

