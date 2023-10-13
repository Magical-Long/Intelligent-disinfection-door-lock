# Intelligent disinfection door lock
已用的引脚：
1.LCD屏幕：PA0-PA5
A0  SCL
A1   SDA
A2   RES
A3   DC
A4   CS
A5   BLK

2.矩阵键盘：PB8-PB15
PB8-PB11  对应行A B C D    从左到右5 6 7 8
PB12-PB15   对应列1 2 3 A   从左到右1 2 3 4

3.指纹模块:  PA9-PA10 PA6
PA9 TXD
PA10 RXD
PA6  指纹模块感应信号输出

4.RC522射频模块：
CS PA7
SCK PB0
MISO PB4
MOSI PB1
RESET  PA8

5.其他蜂鸣器和IO口，要留出与上面不冲突的GPIO口
外雷达感应输出为PA11    内雷达感应输出为PA15 
蜂鸣器为PA12
两个灯为PC13  PC14，PC13为绿灯 PC14为红灯
外消毒灯为PB5  内消毒灯为PB6
