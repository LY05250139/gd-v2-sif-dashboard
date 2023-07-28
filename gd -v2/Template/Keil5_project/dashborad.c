#include "dashboard.h"

uint8_t Addr[9] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8};
uint8_t Data[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

// OxCO  0 ~ 35A
// 0xC1  40 ~ 75A
// 0xC2  80 ~ 100A
// 0xC3  0 ~ 40 line
// 0xC4  40 ~ 80 line
// 0xC5  80 ~ 100 line
// 0xC6  o v E g o  p  BAT !
// 0xC7  MCU M VCU SOC1 ~ SOC5
// 0xC8  SOC6 READY1 READY2

void start()
{
    CLK_HIGH;
    DATA_HIGH;
    delay_1us(1);
    DATA_LOW;
    delay_1us(1);
    CLK_LOW;
    delay_1us(1);
}

void stop()
{
    CLK_LOW;
    DATA_LOW;
    delay_1us(1);
    CLK_HIGH;
    delay_1us(1);
    DATA_HIGH;
    delay_1us(1);
}

void Writebyte(uint8_t data)
{
    uint8_t i;
    uint8_t temp;
    temp = data;
    for (i = 0; i < 8; i++)
    {
        CLK_LOW;
        if (temp & 0x01)
        {
            DATA_HIGH;
        }
        else
        {
            DATA_LOW;
        }
        delay_1us(1);
        CLK_HIGH;
        temp >>= 1;
    }
    CLK_LOW;
    delay_1us(1);
}
void Open()
{

    uint8_t i;
    for (i = 3; i < 6; i++)
    {
        Senddata_t(Addr[i], 0xFF);
    }
    Senddata_t(Addr[7], 0xF8);
    Senddata_t(Addr[8], 0x01);
    delay_1ms(1000);
    clearALL();
    delay_1ms(1000);
    Senddata_t(Addr[6], 0x1F);
}

void TM1460_Init()
{
    start();
    Writebyte(SET_DATA);
    stop();

    start();
    Writebyte(SET_DISPLAY);
    stop();

    clearALL();
}

void dashboard_Init()
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_12);
    gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    systick_config();

    TM1460_Init();

    Open();
    delay_1ms(1000);
    clearALL();
    delay_1ms(1000);
    delay_1us(1000000);

    OpenALL();
    // while(1)
    // {
    //   Senddata(0xC0,Data);
    // }
}

void Senddata_t(uint8_t Addr, uint8_t Data)
{
    start();
    Writebyte(Addr);
    Writebyte(Data);
    stop();
}

void OpenALL()
{
    uint8_t i;
    for (i = 0; i < 9; i++)
    {
        Senddata_t(Addr[i], 0xFF);
    }
}

void Senddata(uint8_t Addr, uint8_t *p)
{
    uint8_t i;
    p = Data;

    for (i = 0; i < 8; i++)
    {
        start();
        Writebyte(Addr);
        Writebyte(p[i]);
        stop();
        delay_1ms(1000);
        start();
        Writebyte(Addr);
        Writebyte(0x00);
        stop();
        delay_1ms(1000);
    }
}
void clearALL() // 清屏
{

    uint8_t i;
    for (i = 0; i < 9; i++)
    {
        start();
        Writebyte(Addr[i]);
        Writebyte(0x00);
        stop();
    }
}
