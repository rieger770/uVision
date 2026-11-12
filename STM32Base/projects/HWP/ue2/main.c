/**
  ******************************************************************************
  * @file    led.c 
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    30-September-2022
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_conf.h"
#include "sysdelay.h"
#include "board_button.h"
#include "board_led.h"

/* Private typedef -----------------------------------------------------------*/
typedef uint8_t TCounter;
/* Private define ------------------------------------------------------------*/
#define LED2_MASK 0x1
#define LED1_MASK 0x2
#define LED0_MASK 0x4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the sys tick timer (M0 core) which is used for delay.
  * @param  None
  * @retval None
  */
void SysTick_Init(void) 
{
    /* init the sys tick timer to be called every 1ms */
    SysTick_Config(SystemCoreClock / 1000);
} 

/**
  * @brief  This is the SysTick interrupt handler which is called every 1ms. 
            We have to increment the HAL tick counter which is used for SysDelay
  * @param  None
  * @retval None
  */
void SysTick_Handler()
{
    SysDelay_IncTicks();
}

// converts dec to binary for max. binary 3 digits only
uint8_t ConvertBinary(TCounter c)
{
	uint8_t res = 0;
	uint8_t bit = 0;
	for(int8_t digits = 0; digits <= 2; digits++)
	{
		bit = c % 2;
		res |= (bit << digits);
		if(c != 0) {
		c /= 2;
		}
	}
	return res;
}

void DisplayValue(TCounter c)
{
	uint8_t bitsVal = ConvertBinary(c); // 3 digit binary number
	if(LED2_MASK & bitsVal)
	{
		LED_On(2);
	} else
	{
		LED_Off(2);
	}
		if(LED1_MASK & bitsVal)
	{
		LED_On(1);
	} else
	{
		LED_Off(1);
	}
		if(LED0_MASK & bitsVal)
	{
		LED_On(0);
	} else
	{
		LED_Off(0);
	}
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_InitTypeDef initStruct;
 
    /* init the sys tick counter (needed for SysDelay) */
    SysTick_Init();

    Button_Initialize();
		LED_Initialize();
		TCounter counter = 0;
  

uint32_t prev = 0;

while (1)
{
    uint32_t now = Button_GetState();
    uint32_t pressed = now & prev;      // nur neu gedrückte Buttons

    if (pressed & BUTTON_USER0_MASK)
    {
        counter = (counter + 1) & 0x7;   // 0..7, nach 7 wieder 0
    }
    if (pressed & BUTTON_USER1_MASK)
    {
        counter = (counter - 1) & 0x7;   // von 0 geht es auf 7
    }
    prev = now;

    DisplayValue(counter); // LEDs aktualisieren
    SysDelay_Delay(200); // 200ms Delay = 5Hz
}
}