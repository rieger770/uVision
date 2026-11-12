## Exercise 3 : LCD - Dokumentation

### Task 1 : Create the module Lcd and implement the function void Lcd_Initialize(void)which is expected to be called once before the application starts. The function should initialize all used GPIOs on the APUS Board. Use Gpio_Initialize().

```c

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

```

### Provide a code excerpt and describe the relevant code.

This code section was covered in class. The Lcd_Write() calls are given in the assignment instructions and configures different settings of the screen. The Gpio_Intialized() function is called for every required Pin, e.g. Chip Select, Data/Command, Read, Write, Reset, D0-D7...

For Initialization each SetToBlack and SetToWhite is called twice for visual feedback.

### Task 2 : Implement the function Lcd_Write. Use the BSRR-Register for all modifications.

```c

static inline void Lcd_Write(uint8_t data, bool command){
  Gpio_Set(GPIO_ID_TFT_DC, command);  						// Data (1) or command (0) byte		
  GPIOE->BSRR = (GPIO_LCD_BSSR_WRITE_ON | data); 	// enable writing mode + write data
  GPIOE->BSRR = GPIO_LCD_BSSR_WRITE_OFF;   				// disable writing mode
}

```

### Provide a code excerpt and describe the relevant sections.

Here the Data/Command bit is set to receiving commands first, then the data is transmittet throw a bitwise `OR` to write it on the bus. Then disables the WR Pin again. The WR Pin is the 10th bit of the mask. The higher 16 bit (31:16) of the mask is used to set the Pins to `LOW` and the lower 16 bit (15:0) are used for setting Pins to `BIT`. Because the WR Pin is low active, the 26th (10+16) bit is set to `LOW` for enabling the Writing Mode.

### Task 3 : Implement the function Lcd_SetArea. Use the function Lcd_Write.

```c

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

```

### Implement the function Lcd_FillRectangle. Use the function Lcd_Write.

```c

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

```

### Implement the function Lcd_Draw. Use the function Lcd_Write.

```c
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
```

### Provide code excerpts for all functions and describe relevant parts in the code.

SetArea defines CASET AND RASET, then enables RAMWR for writing into frame memory. FillRectangle checks if all pixels are in screen of partially in screen. If none pixels are in screen in returns 1. If some pixels are out of screen the boundaries are fitted to the screen edge and only those pixels that are inside the area are printed. Same principle goes for Lcd_Draw, but now i added an index to print the correct pixels of the image for alligned clipping.

For those functions i used a helper function called WritePixel that also uses a helper function ConvertColor. ConvertColor has to convert a RGB565 color to 2x8 Bits to allign with the bus. WritePixel writes these two 8 Bit values on the bus.

### Task 4 : Implement the function Lcd_DrawMonochrome.

```c

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

```

### Implement the function Lcd_PrintCharacter.

```c

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

```

### Implement the function Lcd_Print.

```c

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

```

### Provide code excerpts for all functions and describe the relevant parts of your code.

The DrawMonochrome function implements a similar logic to the previous functions, except now, i had to differentiate between foreground and background. For this, i needed to examine every bit on its own with the following expression:

`((data[i / 8] >> (i % 8)) & 1)`

With this expression and the index i, every bit of the char is checked individually. The index operator increments the element if all of the 8 bits are checked, the modulo part shifts the bits of the char one by one to the right to apply a bitwise `AND` and get the actual bit value to determine whether its a background or foreground. 

The PrintCharacter function treats each character as a monochrome and calls this function to print it. At the end, the cursor x is set to the pixel after the just printed character. Also it is checked, whether the character was successfully drawn. 

Now the Print function receives a char array and prints each char if possible on the screen. Special characters like \n and \t are covered and modify the cursor accordingly. If a called underlying function returns an error code it is catched and returned as well.