#include "EventQueue.h"
#define EVENT_QUEUE_SIZE 16

#include <stddef.h>
#include <stm32f072xb.h>

static uint8_t head;
static uint8_t tail;
static uint8_t count;
static Event_t queue[EVENT_QUEUE_SIZE];

int EventQueue_Initialize(void)
{
    EventQueue_Clear();
    return 0;
}

int EventQueue_Put(const Event_t * event)
{
    int ret = 0;

    __disable_irq();
    if(count >= EVENT_QUEUE_SIZE)
    {
        ret = 1; // buffer full
    }
    else
    {
        queue[tail] = *event;
        tail = (tail + 1) % EVENT_QUEUE_SIZE; // buffer logic
        count++;
    }
    __enable_irq();

    return ret;
}

int EventQueue_Get(Event_t * event)
{
    int ret = 0;

    __disable_irq();
    if(count == 0)
    {
        ret = 1; //empty
    }
    else
    {
        *event = queue[head];
        head = (head + 1) % EVENT_QUEUE_SIZE;
        count--;
    }
    __enable_irq();

    return ret;
}

void EventQueue_Clear(void)
{
    __disable_irq();
    head = 0;
    tail = 0;
    count = 0;
    __enable_irq();
}