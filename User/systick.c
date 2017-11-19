#include "stm32f10x_systick.h"
#include "systick.h"

static uint8_t  delay_fac_us = 0;  
static uint16_t delay_fac_ms = 0;  
static FlagStatus  Status;


void DELAY_Configuration(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
  	RCC_GetClocksFreq(&RCC_ClocksStatus);// get all clocks value, return in Hz
	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);//RCC_SYSCLK_Div1: AHB clock = SYSCLK=72Mhz
    
		//AHB clock divided by 8 selected as SysTick clock source.
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	  SysTick_ITConfig(DISABLE);//disable systick interrupt, we dont' need int when use systick for delay
    
	  //--- RCC_ClocksStatus.HCLK_Frequency= 72 Mhz
	  //--- f_systick= AHB_clk/8= 9*10e6 Hz= 9 Mhz
	  //--- t_step_systick=1/f_systick
	  delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;//=9 count
    delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;//=9000 count      
		
		delay_ms(20);//wait for all components get ready
}
					
         
void delay_us(u32 Nus)
{ 
	  //--- systick_count= nus *10e-6 / f_systick=9*Nus, for specificed 8 Mhz crystal
    SysTick_SetReload(delay_fac_us * Nus);//load the new count value for systick (top value)  
    SysTick_CounterCmd(SysTick_Counter_Clear);//clear systick counter to zero before start count up		
    SysTick_CounterCmd(SysTick_Counter_Enable);//count up		
    do//check until reach top value
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);							
    SysTick_CounterCmd(SysTick_Counter_Disable);//stop count    
}


void delay_ms(uint16_t nms)
{    
	  //systick_count(top value)= nms*10e-3/f_systick= 9000* nms, just for external crystal 8Mhz
    uint32_t temp = delay_fac_ms * nms;

    if (temp > 0x00ffffff)//contrains the value of delay time in ms
    {
        temp = 0x00ffffff;
    }
		
    SysTick_SetReload(temp);//load the new top value for systick 		            
    SysTick_CounterCmd(SysTick_Counter_Clear);//clear systick counter to zero	
    SysTick_CounterCmd(SysTick_Counter_Enable);//start counting up
    do//check overflow flag until it comes	 
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);//Status=SET: completely done			
		
    SysTick_CounterCmd(SysTick_Counter_Disable);//stop counting up
}














