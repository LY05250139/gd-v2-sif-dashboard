#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__


#include "gd32f10x_gpio.h"
#include "gd32f10x.h"
#include "systick.h"
#include "gd32f10x_libopt.h"

#define CLK_LOW    gpio_bit_reset(GPIOD,GPIO_PIN_2)
#define CLK_HIGH   gpio_bit_set(GPIOD,GPIO_PIN_2)
#define DATA_LOW   gpio_bit_reset(GPIOC,GPIO_PIN_12)
#define DATA_HIGH  gpio_bit_set(GPIOC,GPIO_PIN_12)

#define SET_DATA_1        0x40      //数据命令设置  地址自动加一
#define SET_DISPLAY       0x8F      //显示命令控制  脉冲宽度14/16
#define SET_DATA          0x44     //数据命令设置  固定地址 
#define SET_DISPLAY_OFF   0x80      //关闭显示


void start();
void clearALL();
void OpenALL();
void stop();
void Open();
void Writebyte(uint8_t data);
void dashboard_Init();
void Senddata(uint8_t Addr,uint8_t * P);
void Senddata_t(uint8_t Addr,uint8_t P);

#endif