#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdbool.h>
#include "Gpio.h"
#include "EventQueue.h"

typedef void (*StateTransitionFunction_t)(void);
typedef struct State_s const * (*StateFunction_t)(void);

typedef struct State_s {
StateTransitionFunction_t Enter;
StateFunction_t Process;
StateTransitionFunction_t Exit;
} State_t;
void StateMachine_Run(State_t const * state);

extern State_t const State_A;
extern State_t const State_B;

void State_A_Enter(void);
State_t const* State_A_Process(void);
void State_A_Exit(void);

void State_B_Enter(void);
State_t const* State_B_Process(void);
void State_B_Exit(void);

#endif //STATEMACHINE_H