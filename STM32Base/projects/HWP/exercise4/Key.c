// Key.c
#include "Key.h"
#include "Gpio.h"
#include "Timer.h"
#include "EventQueue.h"

#include <stdbool.h>
#include <stdint.h>

#define KEY_TIMER_ID           TIMER_ID_TIM6
#define KEY_CHECK_INTERVAL_MS  50
#define KEY_MIN_PRESS_MS       100
#define KEY_MAX_PRESS_MS       1000
#define KEY_PRESSED            1
#define USER_0                 0
#define USER_1                 1

static uint32_t counterButton0 = 0;
static uint32_t counterButton1 = 0;
static uint32_t counterButtonWakeUp = 0;


void KeyGet(int id)
{
    (void)id;

	bool isPressed = false;
    Gpio_Get(GPIO_ID_BUTTON_USER0, &isPressed);
    if(isPressed) 
    {
        counterButton0++; // increment timer if pressed every 50ms
    } 
    else 
    {
        if(counterButton0 != 0) // if user  stops pressing check time pressed
        {
            uint32_t time = counterButton0*50; // convert counter times 50 to obtain time in ms
            if(KEY_MIN_PRESS_MS <= time && KEY_MAX_PRESS_MS > time) // if time is in given timespan
            {
                Event_t event = {KEY_PRESSED, USER_0, 0, 0}; // create event
                isPressed = false; // reset pressed bool
                EventQueue_Put(&event); // put event on queue
            }
        }
        counterButton0 = 0; // reset button counter
    }
    Gpio_Get(GPIO_ID_BUTTON_USER1, &isPressed); // same for other keys...
    if(isPressed) 
    {
        counterButton1++;
    } 
    else 
    {
        if(counterButton1 != 0)
        {
            uint32_t time = counterButton1*50;
            if(KEY_MIN_PRESS_MS <= time && KEY_MAX_PRESS_MS > time)
            {
                Event_t event = {KEY_PRESSED, USER_1, 0, 0};
                isPressed = false;
                EventQueue_Put(&event);
            }
        }
        counterButton1 = 0;
    }
    Gpio_Get(GPIO_ID_BUTTON_WAKE_UP, &isPressed);
    if(isPressed) 
    {
        counterButtonWakeUp++;
    } 
    else 
    {
        if(counterButtonWakeUp != 0)
        {
            uint32_t time = counterButtonWakeUp*50;
            if(KEY_MIN_PRESS_MS <= time && KEY_MAX_PRESS_MS > time)
            {
                isPressed = false;
            }
        }
        counterButtonWakeUp = 0;
    }
}

int Key_Initialize(void) {
counterButton0 = 0;
counterButton1 = 0;
counterButtonWakeUp = 0;


    Gpio_Initialize(GPIO_ID_BUTTON_WAKE_UP, GPIO_MODE_INPUT, false);
    Gpio_Initialize(GPIO_ID_BUTTON_USER0, GPIO_MODE_INPUT, false);
    Gpio_Initialize(GPIO_ID_BUTTON_USER1, GPIO_MODE_INPUT, false);

    Timer_Initialize(TIMER_ID_TIM6);
    return Timer_Enable(TIMER_ID_TIM6, KeyGet, KEY_CHECK_INTERVAL_MS, TIME_UNIT_MS, true);
}