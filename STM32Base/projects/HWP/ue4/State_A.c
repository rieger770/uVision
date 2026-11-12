#ifndef STATE_A_H
#define STATE_A_H

#include "StateMachine.h"
#include "Gpio.h"
#include "EventQueue.h"


State_t const State_A = {State_A_Enter, State_A_Process, State_A_Exit};

void State_A_Enter(void) {
	Gpio_Set(GPIO_ID_LED3, false);
}

State_t const* State_A_Process(void)
{
	//TODO: Check, whether we want to stay or leave
	// return &State_A;
	
	Event_t event;
	int ret = EventQueue_Get(&event);
	if(ret == 1) // 1 = error
	{
		return &State_A;
	}
	if(event.X == 1)
	{
		return &State_B;
	}

	return &State_A;
	
}

void State_A_Exit(void)
{
	Gpio_Set(GPIO_ID_LED3, true);
}

#endif //STATE_A_H