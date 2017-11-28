#include "stm32f10x.h"
//---- list of function --------
void DELAY_Configuration(void);
void delay_us(u32 Nus);
void delay_ms(uint16_t nms);

extern volatile u16 ticks;
extern volatile u16 seconds;
u16 get_ticks(void);
u16 get_seconds(void);
u32 get_full_ticks(void);
void ticks_init(void);
//------------------------------
