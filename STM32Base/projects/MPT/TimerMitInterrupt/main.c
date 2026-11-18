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

void EXTI0_1_IRQHandler(void)
{
	if(EXTI->PR & GPIO_Pin_0)
	{
		// we got an interrupt on USER0 pin (PD0)
		// clear the pending flag by writing 0x0001 to the register
		EXTI->PR = GPIO_Pin_0;
		
		// todo toggle LED
	}
	else if(EXTI->PR & GPIO_Pin_1)
	{
		// we got an interrupt on USER1 pin (PD1)
		EXTI->PR = GPIO_Pin_1;
		
		// todo toggle LED
	}
}

void EXTI_Initialize(void)
{
	// activate clock for syscfg register to configure external interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// configure EXTI register in SYSCFG to enable PD0 and PD1 as source for interrupt
	SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0 | SYSCFG_EXTICR1_EXTI1);
	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PD | SYSCFG_EXTICR1_EXTI1_PD);
	
	// configure mask bit to enable IRQ from line 0 and 1
	EXTI->IMR = GPIO_Pin_0 | GPIO_Pin_1;
	
	// configure trigger (Rising edge)
	EXTI->RTSR |= GPIO_Pin_0 | GPIO_Pin_1;
	
	// todo clear pending register
	
	// configure NVIC and enable the EXTI0_1 IRQ
	NVIC_EnableIRQ(EXTI_1_IRQn);
	
}

int main(void)	
{
	Button_Initialize();
	LED_Initialize();
	SysTick_Config(SystemCoreClock / 1000);   // 1 ms pro Tick
	

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


