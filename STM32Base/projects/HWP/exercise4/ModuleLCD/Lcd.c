#include "Lcd.h"
#include "Gpio.h"
#include <stm32f072xb.h>

#define BYTEMASK 0xFF
#define DATALINE_RESET_OFFSET 16
// 0000 0000 1111 1111 0000 0000 0000 0000
#define CLEAR_DATALINE_MASK (BYTEMASK << DATALINE_RESET_OFFSET)

#define BITMASK 0x1
#define WR_RESET_OFFSET 26
// PE10 = WR Pin, 10 + 16 = 26
// 0000 0100 0000 0000 0000 0000 0000 0000
#define CLEAR_WR_MASK (BITMASK << WR_RESET_OFFSET)

#define WR_SET_OFFSET 10
// 0000 0000 0000 0000 0000 0100 0000 0000 = 0x00000400
#define SET_WR_MASK (BITMASK << WR_SET_OFFSET)

#define LCD_Command 0
#define LCD_Data 1

// Mask for write enable with cleared dataline and reset of write (disable)
static uint32_t const GPIO_LCD_BSSR_WRITE_ON = (uint32_t)(CLEAR_WR_MASK | CLEAR_DATALINE_MASK); // 0x04FF0000
static uint32_t const GPIO_LCD_BSSR_WRITE_OFF = (uint32_t)(SET_WR_MASK);	// 0x00000400

static inline void Lcd_Write(uint8_t data, bool command){
  Gpio_Set(GPIO_ID_TFT_DC, command);  						// Data (1) or command (0) byte		
  GPIOE->BSRR = (GPIO_LCD_BSSR_WRITE_ON | data); 	// enable writing mode + write data
  GPIOE->BSRR = GPIO_LCD_BSSR_WRITE_OFF;   				// disable writing mode
}

int Lcd_Initialize(void) {
	Gpio_Initialize(GPIO_ID_TFT_CS, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_DC, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_RD, GPIO_MODE_OUTPUT, GPIO_HIGH);
	Gpio_Initialize(GPIO_ID_TFT_RES, GPIO_MODE_OUTPUT, GPIO_HIGH);
	Gpio_Initialize(GPIO_ID_TFT_WR, GPIO_MODE_OUTPUT, GPIO_HIGH);
	
	Gpio_Initialize(GPIO_ID_TFT_D0, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D1, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D2, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D3, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D4, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D5, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D6, GPIO_MODE_OUTPUT, GPIO_LOW);
	Gpio_Initialize(GPIO_ID_TFT_D7, GPIO_MODE_OUTPUT, GPIO_LOW);		

	Lcd_Write(0x11, LCD_Command); //exit SLEEP mode
	Lcd_Write(0x36, LCD_Command);
	Lcd_Write(0x00, LCD_Data); // MADCTL: memory data access control (portrait/landscape)
	Lcd_Write(0x3A, LCD_Command);
	Lcd_Write(0x65, LCD_Data); // COLMOD: Interface Pixel format
	Lcd_Write(0xB2, LCD_Command);
	Lcd_Write(0x0C, LCD_Data);
	Lcd_Write(0x0C, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x33, LCD_Data);
	Lcd_Write(0x33, LCD_Data); // PORCTRK: Porch setting
	Lcd_Write(0xB7, LCD_Command);
	Lcd_Write(0x35, LCD_Data); // GCTRL: Gate Control
	Lcd_Write(0xBB, LCD_Command);
	Lcd_Write(0x2B, LCD_Data); // VCOMS: VCOM setting
	Lcd_Write(0xC0, LCD_Command);
	Lcd_Write(0x2C, LCD_Data); // LCMCTRL: LCM Control
	Lcd_Write(0xC2, LCD_Command);
	Lcd_Write(0x01, LCD_Data);
	Lcd_Write(0xFF, LCD_Data); // VDVVRHEN: VDV and VRH Command Enable
	Lcd_Write(0xC3, LCD_Command);
	Lcd_Write(0x11, LCD_Data); // VRHS: VRH Set
	Lcd_Write(0xC4, LCD_Command);
	Lcd_Write(0x20, LCD_Data); // VDVS: VDV Set
	Lcd_Write(0xC6, LCD_Command);
	Lcd_Write(0x0F, LCD_Data); // FRCTRL2: Frame Rate control in normal mode
	Lcd_Write(0xD0, LCD_Command);
	Lcd_Write(0xA4, LCD_Data);
	Lcd_Write(0xA1, LCD_Data); // PWCTRL1: Power Control 1
	Lcd_Write(0xE0, LCD_Command);
	Lcd_Write(0xD0, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x05, LCD_Data);
	Lcd_Write(0x0E, LCD_Data);
	Lcd_Write(0x15, LCD_Data);
	Lcd_Write(0x0D, LCD_Data);
	Lcd_Write(0x37, LCD_Data);
	Lcd_Write(0x43, LCD_Data);
	Lcd_Write(0x47, LCD_Data);
	Lcd_Write(0x09, LCD_Data);
	Lcd_Write(0x15, LCD_Data);
	Lcd_Write(0x12, LCD_Data);
	Lcd_Write(0x16, LCD_Data);
	Lcd_Write(0x19, LCD_Data); // PVGAMCTRL: Positive Voltage Gamma control	
	Lcd_Write(0xE1, LCD_Command);
	Lcd_Write(0xD0, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x05, LCD_Data);
	Lcd_Write(0x0D, LCD_Data);
	Lcd_Write(0x0C, LCD_Data);
	Lcd_Write(0x06, LCD_Data);
	Lcd_Write(0x2D, LCD_Data);
	Lcd_Write(0x44, LCD_Data);
	Lcd_Write(0x40, LCD_Data);
	Lcd_Write(0x0E, LCD_Data);
	Lcd_Write(0x1C, LCD_Data);
	Lcd_Write(0x18, LCD_Data);
	Lcd_Write(0x16, LCD_Data);
	Lcd_Write(0x19, LCD_Data); // NVGAMCTRL: Negative Voltage Gamma control
	Lcd_Write(0x29, LCD_Command); // Display: On
	
	for (int i = 0; i < 2; i++) {
		SetToBlack();
		SetToWhite();
	}	
		
	return 0;	
}

void SetToWhite(void) {
	// WRITE CASET with fixed start and stop
	Lcd_Write(0x2A, LCD_Command);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0xEF, LCD_Data); // The address
														 // ranges are X=0 to X=239 (Efh) and Y=0 to Y=319 (13Fh)
	
	// WRITE RASET with fixed start and stop
	Lcd_Write(0x2B, LCD_Command);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x01, LCD_Data); // Bus is only 8 Bit wide so you send the 16 bits seperately
	Lcd_Write(0x3F, LCD_Data); // combines to 13F.
	
	// Memory Write
	Lcd_Write(0x2C, LCD_Command);
	
	for (uint32_t index_y = 0; index_y < LCD_HEIGHT; index_y++) {
		for (uint32_t index_x = 0; index_x < LCD_WIDTH; index_x++) {
			Lcd_Write(0xFF, LCD_Data);
			Lcd_Write(0xFF, LCD_Data);
		}
	}
}

void SetToBlack(void) {
	// WRITE CASET with fixed start and stop
	Lcd_Write(0x2A, LCD_Command);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0xEF, LCD_Data);
	
	// WRITE RASET with fixed start and stop
	Lcd_Write(0x2B, LCD_Command);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x00, LCD_Data);
	Lcd_Write(0x01, LCD_Data);
	Lcd_Write(0x3F, LCD_Data);
	
	// Memory Write
	Lcd_Write(0x2C, LCD_Command);
	
	for (uint32_t index_y = 0; index_y < LCD_HEIGHT; index_y++) {
		for (uint32_t index_x = 0; index_x < LCD_WIDTH; index_x++) {
			Lcd_Write(0x00, LCD_Data);
			Lcd_Write(0x00, LCD_Data);
		}
	}
}

void Lcd_SetArea(uint16_t x, uint16_t y,
	uint16_t width, uint16_t height)
{
	// widht and height coordinates must be 1 less than actual value
	uint16_t xEnd = x + width  - 1;
	uint16_t yEnd = y + height - 1;
	
	// CASET: columns
	Lcd_Write(0x2A, LCD_Command);
	Lcd_Write((uint8_t)(x >> 8),      LCD_Data);  // XStart High
	Lcd_Write((uint8_t)(x),    LCD_Data);  // XStart Low
	Lcd_Write((uint8_t)(xEnd >> 8),   LCD_Data);  // XEnd High
	Lcd_Write((uint8_t)(xEnd), LCD_Data);  // XEnd Low

	// RASET: rows
	Lcd_Write(0x2B, LCD_Command);
	Lcd_Write((uint8_t)(y >> 8),      LCD_Data);  // YStart High
	Lcd_Write((uint8_t)(y),    LCD_Data);  // YStart Low
	Lcd_Write((uint8_t)(yEnd >> 8),   LCD_Data);  // YEnd High
	Lcd_Write((uint8_t)(yEnd), LCD_Data);  // YEnd Low

	// RAMWR
	Lcd_Write(0x2C, LCD_Command);
}

// Convert Color to RGB565
static void ConvertColor(RGB565_t const *color, uint8_t *RG, uint8_t *GB)
{
    // Byte 1: RRRRR GGG  (R in Bit 7:3, upper 3 Bit of G in Bit 2:0)
    *RG = (uint8_t)((color->R << 3) | (color->G >> 3));

    // Byte 2: GGG BBBBB  (lower 3 Bit of G in Bit 7:5, B in Bit 4:0)
    *GB = (uint8_t)(((color->G & 0x07) << 5) | color->B);
}

void Lcd_WritePixel(RGB565_t color)
{
    uint8_t RG;
    uint8_t GB;
    ConvertColor(&color, &RG, &GB);

    Lcd_Write(RG, LCD_Data);   // Red + upper Green-Bits
    Lcd_Write(GB, LCD_Data);   // lower Green-Bits + Blue
}

int Lcd_FillRectangle(uint_fast16_t x, uint_fast16_t y,
	uint_fast16_t width, uint_fast16_t height,
	RGB565_t color)
{
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
	{
		return 1; // all pixels outside
	}
	if (width == 0 || height == 0)
	{
		return 2; // no pixels to draw
	}

	uint_fast16_t visibleW = width;
	uint_fast16_t visibleH = height;
	int result = 0;

	// are all pixels in screen?
	if (width + x > LCD_WIDTH)
	{
		visibleW = LCD_WIDTH - x;
		result = 3;
	}
	if (height + y > LCD_HEIGHT)
	{
		visibleH = LCD_HEIGHT - y;
		result = 3;
	}

	Lcd_SetArea(x, y, visibleW, visibleH);

	for (uint_fast16_t row = 0; row < visibleH; row++)
	{
		for (uint_fast16_t col = 0; col < visibleW; col++)
		{
			Lcd_WritePixel(color);
		}
	}

	return result;
}

int Lcd_Draw(uint_fast16_t x, uint_fast16_t y,
             uint_fast16_t width, uint_fast16_t height,
             RGB565_t const * data)
{
	// nullptr check and valid starting point check
	if (data == 0 || x >= LCD_WIDTH || y >= LCD_HEIGHT) { return 1; }
	// nothing to draw
	if (width == 0 || height == 0) { return 2; }

	uint_fast16_t visibleW = width;
	uint_fast16_t visibleH = height;
	int result = 0;

	// determine whether pixels all fit into screen, if not determine visible area
	if (width + x > LCD_WIDTH)   { visibleW = LCD_WIDTH - x;  result = 3; }
	if (height + y > LCD_HEIGHT) { visibleH = LCD_HEIGHT - y; result = 3; }

	Lcd_SetArea(x, y, visibleW, visibleH);

	for (uint_fast16_t row = 0; row < visibleH; row++)
	{
		for (uint_fast16_t col = 0; col < visibleW; col++)
		{
			// draw each pixel of matrix
			Lcd_WritePixel(data[(uint_fast32_t)row * width + col]);
		}
	}
	return result;
}
	
	// Lcd_SetArea mit x, y, width and height
int Lcd_DrawMonochrome(uint_fast16_t   x,
                       uint_fast16_t   y,
                       uint_fast16_t   width,
                       uint_fast16_t   height,
                       uint8_t const * data,
                       RGB565_t        foreground,
                       RGB565_t        background)
{
	// 1: All pixels are outside the available display area.
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
	{
		return 1;
	}
	// 2: No pixels to draw.
	if (width == 0 || height == 0)
	{
		return 2;
	}

	// define visible area
	uint_fast16_t visibleW = width;
	uint_fast16_t visibleH = height;
	int result = 0;

	if (width + x > LCD_WIDTH)
	{
		visibleW = LCD_WIDTH - x;
		result = 3;   // partially outside
	}
	if (height + y > LCD_HEIGHT)
	{
		visibleH = LCD_HEIGHT - y;
		result = 3;
	}

	// set window for visible area
	Lcd_SetArea(x, y, visibleW, visibleH);
	
	for (uint_fast16_t row = 0; row < visibleH; row++)
	{
		for (uint_fast16_t col = 0; col < visibleW; col++)
		{
			// counter for each bit
			uint_fast32_t i = (uint_fast32_t)row * width + col;
			// check each bit of char and continue with next char
			if ((data[i / 8] >> (i % 8)) & 1)
			{
				Lcd_WritePixel(foreground);
			}
			else
			{
				Lcd_WritePixel(background);
			}
		}
}

	return result;   // 0 success, 3 partially drawn
}

int Lcd_PrintCharacter(
 uint_fast16_t * x,
 uint_fast16_t * y,
 char            character,
 RGB565_t        foreground,
 RGB565_t        background,
 Font_t const  * font)
{
	// NULL-check before dereferencing
	if (x == 0 || y == 0 || font == 0)
	{
		return 2;
	}

	// get Glyph once
	Glyph_t const *glyph = font->GetGlyph(character);
	if (glyph == 0)
	{
		return 2;   // nullptr
	}

	// automatic line break if character goes out of screen
	if (*x + glyph->Width > LCD_WIDTH)
	{
		*x = 0;                   // line begin
		*y += font->LineHeight;   // next line
	}

	// get return value of monochrome func
	int result = Lcd_DrawMonochrome(*x, *y,
	                                glyph->Width, font->LineHeight,
	                                glyph->Data,
	                                foreground, background);

	// only increment cursor if character was drawn
	if (result == 0)
	{
		*x += glyph->Width;
	}

	return result;
}
 
 
int Lcd_Print(
 uint_fast16_t   x,
 uint_fast16_t   y,
 char const    * string,
 RGB565_t        foreground,
 RGB565_t        background,
 Font_t const  * font)
{
	if (string == 0)
	{
		return 2;
	}

	uint_fast16_t currX = x;
	uint_fast16_t currY = y;
	
	// while string is not terminated
	while (*string != '\0')
	{
		// line break
		if(*string == '\n')
		{
				currX = 0; // place cursor all the way left
				currY += font->LineHeight; // move cursor to next line below currect line
				string++; // next char
				continue;
		}
		// tabulator
		else if(*string == '\t')
		{
			currX += 10; // move cursor 10 pixels to the right
			string++; // next char
			continue;
		}
		int ret = Lcd_PrintCharacter(&currX, &currY, *string,
		                             foreground, background, font);
		if (ret != 0)
		{
			return ret;   // error or clipping
		}
		string++;         // next char
	}

	return 0;
}
