#include "stm32f0xx_conf.h"
#include "board_button.h"
#include "board_led.h"
#include "sysdelay.h"
#include <stdbool.h>

void SysClk_24MHz(void);

void SysTick_Handler(void)
{
    SysDelay_IncTicks();
}

int main(void)	
{
	Button_Initialize();
	LED_Initialize();
	SysTick_Config(SystemCoreClock / 1000);   // 1 ms pro Tick
	
	bool isAlarm = false;
	bool is24Mhz = false;
	
	while(1)
	{
		uint32_t button = Button_GetState();
		// button masks are 001 for USER0, 010 for USER1, 100 for WAKEUP
		// XXX & 001 != 0 -> first bit is set
		if((button & BUTTON_USER0_MASK) != 0) // alarmzone 1
		{
			isAlarm = true;
			LED_On(LED4);
		}
		// XXX & 010 != 0 -> second bit is set
		if ((button & BUTTON_USER1_MASK) != 0) // alarmzone 2
		{
			isAlarm = true;
			LED_On(LED5);
		}
		// XXX & 100 != 0 -> third bit is set
		if ((button & BUTTON_WAKEUP_MASK) != 0) // alarm reset button
		{
			if(!is24Mhz)
			{
				SysClk_24MHz(); // set clock to 24Mhz
				is24Mhz = true;
			}
			isAlarm = false; // reset alarm
			LED_Off(LED4); // all leds off
			LED_Off(LED5);
		}
		
		
		if(isAlarm) // is alarm on? -> flash led
		{
			LED_On(LED3);
			SysDelay_Delay(200);
			LED_Off(LED3);
			SysDelay_Delay(200);
		}
		
	}
}



void SysClk_24MHz(void)
{
    // select HSI
    RCC->CFGR &= ~RCC_CFGR_SW;
		// wait for selection
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
		// disable PLL
    RCC->CR &= ~RCC_CR_PLLON;
		// wait for PLL 
    while (RCC->CR & RCC_CR_PLLRDY);

    //	Reset PLLSRC und PLLMUL
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL);
		// Set PLLSRC to HSI / 2 and PLLMUL to 6
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL6;

    // enable PLL
    RCC->CR |= RCC_CR_PLLON;
		// wait for PLL
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Sysclk to PLL
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
		// wait for change
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // update systemcore clock
    SystemCoreClockUpdate();
}


