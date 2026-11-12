#include <stm32f072xb.h>
#include "Lcd.h"

void Systemconfig_Clock(void);

int main() {
    Systemconfig_Clock();

    Lcd_Initialize();
		//240x320
		// R:5 G:6 B:5 -> max. Values 31, 63, 31
		RGB565_t red = { .R = 31, .G = 0, .B = 0};
		RGB565_t black = { .R = 0, .G = 0, .B = 0};
		RGB565_t white = { .R = 31, .G = 63, .B = 31};
		Lcd_FillRectangle(0,0,150,120, black);
		Lcd_FillRectangle(10,10,120,30, red);
		Lcd_FillRectangle(10,40,120,30, white);
		Lcd_FillRectangle(10,70,120,30, red);
	
		const char* string = "FH Hagenberg\n\tHSD";
		Lcd_Print(10, 150, string,black,white,&Consolas);
		
		Lcd_Draw(10, 180, Image.Width, Image.Height, Image.Data);
    while (true) {}
}

void Systemconfig_Clock(void) {
    // Enable the internal 48 MHz Oscillator
    RCC->CR2 |= RCC_CR2_HSI48ON;
    while ((RCC->CR2 & RCC_CR2_HSI48RDY) == 0) {}

    // Configure flash before setting new clock frequency
    FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE;

    RCC->CFGR = (RCC->CFGR &
                 ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE))
                 | RCC_CFGR_SW_HSI48;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48) {}

    SystemCoreClock = 48000000U;
}