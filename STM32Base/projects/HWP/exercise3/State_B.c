#ifndef STATE_B_H
#define STATE_B_H

#include "StateMachine.h"

State_t const State_B = {State_B_Enter, State_B_Process, State_B_Exit};

void State_B_Enter(void) {
	Gpio_Set(GPIO_ID_LED4, false);
}

State_t const* State_B_Process(void)
{
	
	Event_t event;
	int ret = EventQueue_Get(&event);
	if(ret == 1) // 1 = error
	{
		return &State_B;
	}
	if(event.X == 0)
	{
		return &State_A;
	}

	return &State_B;
	
}


void State_B_Exit(void)
{
	Gpio_Set(GPIO_ID_LED4, true);
}

#endif //STATE_B_H