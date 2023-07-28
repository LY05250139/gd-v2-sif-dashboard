#include "main.h"    
#include "gd32f10x_gpio.h"
#include "gd32f10x.h"
#include "systick.h"
#include "gd32f10x_libopt.h"

#define LOW                   0
#define HIGH                  1


#define REV_BIT_NUM           8   //接收的bit位个数，看是按字节接收还是按字接收，1字节=8bit，1字=2字节=16bit
#define REV_DATA_NUM          12   //接收的数据个数

#define SYNC_L_TIME_NUM         1100    //同步信号低电平时间：50ms = 50000us / 50us = 1000
#define SYNC_H_TIME_NUM_MIN     8      //同步信号高电平最小时间：500-100us = 400us / 50us = 8  
#define SYNC_H_TIME_NUM_MAX     12     //同步信号高电平最大时间：500+100us = 600us / 50us = 12

#define SHORT_TIME_NUM_MIN      9     //一个逻辑周期中短的时间最小值：500-50us = 450us / 50us = 9
#define SHORT_TIME_NUM_MAX      11    //一个逻辑周期中短的时间最大值：500+50us = 550us / 50us = 11

#define LONG_TIME_NUM_MIN       18    //一个逻辑周期中长的时间最小值：1ms-100us = 900us / 50us = 18
#define LONG_TIME_NUM_MAX       22    //一个逻辑周期中长的时间最大值：1ms+100us = 1100us / 50us = 22

#define LOGIC_CYCLE_NUM_MIN     26    //一个逻辑周期最小时间：1.5ms-200us = 1300us / 50us = 26
#define LOGIC_CYCLE_NUM_MAX     34    //一个逻辑周期最大时间：1.5ms+200us = 1700us / 50us = 34

#define HALF_LOGIC_CYCLE_MIN    13    //一个逻辑周期的1/2最小时间：750-100us = 650us / 50us = 13
#define HALF_LOGIC_CYCLE_MAX    17    //一个逻辑周期的1/2最大时间：750+100us = 850us / 50us = 17




//void Timer1_Init(void);     
//void NVIC_Configuration(void);


void Sif_off(void);
void Sif_on(void);
void Sif_Init(void);
void sif_send_data_handle(uint8_t state);
char Sum_Handle(void);
void exchange(char checkXor);
typedef enum
{
    INITIAL_STATE=0,            //初始状态，等待接收同步信号
    SYNC_L_STATE=1,             //接收同步低电平信号状态
    SYNC_H_STATE=2,             //接收同步高电平信号状态
    DATA_REV_STATE=3,           //读取数据码电平状态
    RESTART_REV_STATE=4         //接收过程出错重新接收状态
}REV_STATE_e;

/***********************************************************************************/

