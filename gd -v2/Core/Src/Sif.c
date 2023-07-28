#include "Sif.h"

#define SIF_SYNC  51
 
static uint8_t sif_sync_tosc  = 0;
static uint8_t sif_send_tosc = 0;
static uint8_t state_mode = 0;
static int8_t bit_cnt = 7;
static uint8_t byte_cnt = 0;

enum
{
	  SYNC_SIGNAL,  
    SEND_DATA, 
    SEND_DATA_COMPLET,
};

static uint8_t result[12] = {0x08,0x61,0x00,0x04,0x02,0x80,0x80,0x01,0xF0,0x31,0xD1,0};
static uint8_t length = 11;//数据的长度

void Sif_tb_off()
{
     gpio_bit_write(GPIOC, GPIO_PIN_12, RESET);
     delay_1ms(1);
	   //delay_1ms(10); //延时1ms
 	
}

void Sif_tb_on()
{
    gpio_bit_write(GPIOC, GPIO_PIN_12, SET);
    delay_1ms(1);
	  //delay_1ms(10);

}


void Sif_off()
{    
    gpio_bit_write(GPIOC, GPIO_PIN_12, RESET);
    delay_1us(500);
	  //delay_1ms(5);//延时0.5ms
}

void Sif_on()
{
    gpio_bit_write(GPIOC, GPIO_PIN_12, SET);
    delay_1us(500);
	 // delay_1ms(5); 
}

char Sum_Handle(void)
{
    unsigned char i = 0;  
    unsigned char checkXor = 0;
    for ( i = 0; i < (sizeof(result)/sizeof(result[0])) - 1; i++)
    {
        checkXor = checkXor ^ result[i];
    }
		result[11] = checkXor;
    return checkXor;
}

void exchange(char checkXor)
{
    result[11] = checkXor;
	
}

void Sif_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_12);
	  systick_config();
    Sum_Handle();
    //exchange(Sum_Handle());
	  state_mode = SYNC_SIGNAL;
    while (1)
    {
       if(result[0] != 0)
       {
          sif_send_data_handle(state_mode);     
       }  
       else
      {
           break;
       }   
//           Sif_on();
//           Sif_off();
    }
    
}



void sif_send_data_handle(uint8_t state)
{
    uint8_t data ; 	
		uint8_t *p = (uint8_t *)result;
    data= (p[byte_cnt] >> bit_cnt) & 0x1; 
    switch (state) 
    {
 
	    case SYNC_SIGNAL:  //同步模式
                if (sif_sync_tosc < SIF_SYNC - 1)
                    {
                        Sif_tb_off();
                        sif_sync_tosc++;
                    }  																			
                else if (sif_sync_tosc == SIF_SYNC - 1)                            
                    {
                        Sif_tb_on();
                        sif_sync_tosc++;
                    }                  
                if (sif_sync_tosc >= SIF_SYNC)
                    {		
											  			      
                        state_mode = SEND_DATA;
                        sif_sync_tosc = 0;
                        bit_cnt = 7;
                        byte_cnt = 0;
                        sif_send_tosc = 0;
                    }

                break;
								
        case SEND_DATA://发送数据   
          	
            if (data)
            {
                if (sif_send_tosc == 0)
                {
                    Sif_off();
                    sif_send_tosc++;
                }
                else if (sif_send_tosc == 1)
                {
                    Sif_on();
                    sif_send_tosc++;
                } 
                else if (sif_send_tosc == 2)
                {
                    Sif_on();
                    sif_send_tosc = 0;
                    if(--bit_cnt < 0)
                    {
                        byte_cnt++;
                        bit_cnt = 7;
                    }
                      if (byte_cnt > length)
                    {
											  
                        state_mode = SEND_DATA_COMPLET;
                        break;
                    }
                }
            }
            else
            {
                if (sif_send_tosc == 0)
                {
                    Sif_off();
                    sif_send_tosc++;
                }
                else if (sif_send_tosc == 1)
                {
                    Sif_off();
                    sif_send_tosc++;
                } 
                else if (sif_send_tosc == 2)
                {
                    Sif_on();
                    sif_send_tosc = 0;
                    if (--bit_cnt < 0)
                    {
                        byte_cnt++;
                        bit_cnt = 7;
                    }
                     if (byte_cnt > length)
                    {
											   
                        state_mode = SEND_DATA_COMPLET;
                        break;
                    }
                }
             }
			       break;
        case SEND_DATA_COMPLET://数据发送完成，将标志位清0
            state_mode = 0;
            //Sif_off();
				    //delay_1ms(50);
				
				    byte_cnt = 0;
				    bit_cnt = 7;
//            length = 0;
//            memset(result, 0, sizeof(result));
            break;

        default:
            break;
    }
    
}
/*
void Timer1_Init(void)
{
    rcu_periph_clock_enable(RCU_TIMER1);
    timer_deinit(TIMER1);
    timer_parameter_struct timer_parameter_structre;
    timer_parameter_structre.prescaler = 10800 - 1; 
    timer_parameter_structre.period = 1000 - 1;
    
}
*/

//void NVIC_Configuration(void)
//{
//    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
//    nvic_irq_enable(TIMER1_IRQn, 1, 1);

//}