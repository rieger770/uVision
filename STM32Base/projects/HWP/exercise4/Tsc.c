#include "Tsc.h"
#include "Gpio.h"

static void Tsc_WriteByte(uint8_t data);
static uint8_t Tsc_ReadByte(void);
static inline void Tsc_Delay(void);

void Tsc_Initialize(void)
{
	// We get Busy, PenIRQ and DOUT
	Gpio_Initialize(GPIO_ID_TOUCH_BUSY, GPIO_MODE_INPUT, false);
	Gpio_Initialize(GPIO_ID_TOUCH_PENIRQ, GPIO_MODE_INPUT, false);
	Gpio_Initialize(GPIO_ID_TOUCH_DOUT, GPIO_MODE_INPUT, false);

	
	// We define DIN, nCS and CLK
	Gpio_Initialize(GPIO_ID_TOUCH_DIN, GPIO_MODE_OUTPUT, false);
	Gpio_Initialize(GPIO_ID_TOUCH_CS, GPIO_MODE_OUTPUT, true);
	Gpio_Initialize(GPIO_ID_TOUCH_DCLK, GPIO_MODE_OUTPUT, false);

}

static void Tsc_WriteByte(uint8_t data)
{
	// Durchlaufen aller Bits und setzen der Leitung
	for (int8_t bit = 7; bit >= 0; bit--)
	{
		// Setzen der DCLK-Line
		Gpio_Set(GPIO_ID_TOUCH_DCLK, false);
		
		if(data & (uint8_t)(0x1 << bit))
		{
			Gpio_Set(GPIO_ID_TOUCH_DIN, true);
		}
		else
		{
			Gpio_Set(GPIO_ID_TOUCH_DIN, false);
		}
		
		Tsc_Delay();
		
		// Setzen der DCLK-Line
		Gpio_Set(GPIO_ID_TOUCH_DCLK, true);
		Tsc_Delay();
	}
	
	Gpio_Set(GPIO_ID_TOUCH_DCLK, false);
	Gpio_Set(GPIO_ID_TOUCH_DIN, false);

}


static uint8_t Tsc_ReadByte(void)
{
	uint8_t data = 0x0;
	
	Gpio_Set(GPIO_ID_TOUCH_DCLK, false);
	
	for(uint8_t bit = 0; bit <= 7; bit++)
	{
		Gpio_Set(GPIO_ID_TOUCH_DCLK, true);
		Tsc_Delay();
		
		// Test for while busy
		bool bitValue = false;
		Gpio_Get(GPIO_ID_TOUCH_DOUT, &bitValue);
		
		data = ((data << 1) | (uint8_t) (bitValue));
		
		Gpio_Set(GPIO_ID_TOUCH_DCLK, false);
		Tsc_Delay();
	}
	return data;

}

static int Tsc_ReadRaw(bool *touched, uint16_t *x, uint16_t *y)
{
	bool penirqHigh = true;
	Gpio_Get(GPIO_ID_TOUCH_PENIRQ, &penirqHigh);
	
	*touched = !penirqHigh;
	
	if(!*touched)
	{
		*x = 0xFFFF;
		*y = 0xFFFF;
		
		return -1;
	}
	
	// We set CS to low to enable writing and reading 
	Gpio_Set(GPIO_ID_TOUCH_CS, false);
	Tsc_Delay();
	
	// X-Pos as 12 Bit Value
	Tsc_WriteByte(0xD0);
	
	uint8_t highByte = Tsc_ReadByte();
	uint8_t lowByte = Tsc_ReadByte();
	
	// Todo x* = high | low correctly shifted and masked
	
	// Y-Pos as 12 Bit Value
	Tsc_WriteByte(0x90);
	
	highByte = Tsc_ReadByte();
	lowByte = Tsc_ReadByte();
	
	// Todo y* = high | low correctly shifted and masked

	Gpio_Set(GPIO_ID_TOUCH_CS, true);
	
	Tsc_Delay();
	
	Gpio_Get(GPIO_ID_TOUCH_PENIRQ, &penirqHigh);
	*touched = !penirqHigh;
	
	if(!*touched)
	{
		return -2;
	}
	
	
	return 0;
}

int Tsc_Read(bool *touched, uint16_t * x, uint16_t * y, uint8_t samples)
{

}

static inline void Tsc_Delay(void)
{
	__asm volatile ("nop"); // 20ns per nop
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");

	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");

}