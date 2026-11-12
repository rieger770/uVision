#include "stm32f072xb.h"
#include "Timer.h"
#include "Gpio.h"
#include "StateMachine.h"
#include "EventQueue.h"
#include "Key.h"

static void Clock_Init48MHz(void)
{
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC)) | RCC_CFGR_PLLMUL12;

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)) {}

    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}

int main()
{
    __enable_irq();
		
		Clock_Init48MHz(); // while testing noticed clock is set default to 8Mhz
											 // causing the timer to have wrong time conversion
											 // -> here set to 48Mhz to operate correctly.
    Gpio_Initialize(GPIO_ID_LED3, GPIO_MODE_OUTPUT, true);   // true = aus
    Gpio_Initialize(GPIO_ID_LED4, GPIO_MODE_OUTPUT, true);
    EventQueue_Initialize();
    Key_Initialize();

    State_t const * state = &State_A;
    state->Enter();

    StateMachine_Run(state);
}