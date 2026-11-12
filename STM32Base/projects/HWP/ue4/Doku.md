## HWP Exercise 2

## Task 1 : Driver Analysis

- Is the driver interface hardware dependent or independent? Provide an explanation.

Das Interface ist großteils hardware-abhängig. Die Headerfiles kennen die Portanzahl A bis F mit je 16 Pins und verwendet auch Boardspezifische Namen wie 'GPIO_ID_LED3' oder 'GPIO_ID_BUTTON_USER0'. Das Interface verwendet zwar keine Register oder Peripherie-Typen, weil diese Hardware Details in die C-File eingebunden sind, jedoch ist der Code bereits stark an die Architektur des STM32F072 angepasst.

- Inspect the Timer_Context_t struct; Why are some fields declared using the const-keyword while others are not?

```c
typedef struct Timer_Context_s {
  volatile uint32_t * const ClockEnableRegister;
  const uint32_t            ClockEnable;
  volatile uint32_t * const ResetRegister;
  const uint32_t            Reset;
  TIM_TypeDef * const       Timer;
  IRQn_Type const           InterruptNumber;
  bool                      Initialized;
  uint16_t:16; // Prevent warning: padding struct 'struct Timer_Context_s' with 2 bytes to align 'Callback' [-Wpadded]
  Timer_Callback_t          Callback;
  uint32_t                  TickCount;
} Timer_Context_t;
```

In diesem struct sind jene Variablen als const markiert, welche Hardware-abhängig sind. Das bedeutet, diese Variablen gibt die Hardware vor, und müssen zur Laufzeit nicht mehr veränderbar sein. Vorteil hierbei ist, dass der Compiler versehentliche Änderungen dieser Variablen erst gar nicht zulässt, und einen Fehler erzeugt. 

Das volatile in volatile uint32_t * const bezieht sich auf das Register, auf das gezeigt wird. Die Hardware kann es unabhängig vom Programm ändern, deshalb darf der Compiler Zugriffe nicht wegoptimieren. Das const hinter dem Stern macht nur den Pointer selbst konstant (die Adresse bleibt fix), der Registerwert bleibt schreibbar.

Das 'bool Initialised' wird zum Beispiel in 'Timer_Initialize' auf true gesetzt, und somit verändert.

## Task 2 : Interrupt Driven Timer Driver

- Implement the interrupt handler functions so that hey just call the Timer_ISR function with the correct id.

Implementierung in `Timer.c`. Behandelt während der Übung.

## Task 3 : Event Queue

- Create the module named EventQueue

Implementierung in `EventQueue.h` und `EventQueue.c` Dabei wird eine simple Ringbuffer Logik implementiert, welche meine Queue darstellt. Während eines Zugriffs auf den Buffer mit Get oder Put werden die Interrupts deaktiviert mit disable_irq damit die Stabilität des Counters gewährleistet wird, falls Get und Put zeitnahe aufgerufen werden würden.

## Task 4 : Key Module

- Implement the Key Module to check the state of each key

Implementierung von GetKey() und KeyInitialise() in `Key.c`. Dabei wird GetKey jede 50ms aufgerufen und zählt diese Aufrufe intern in einem Counter. Dieser Counter wird mit 50ms multipliziert um die Zeiteinheit ms zu erhalten, und dann wird geprüft ob sich der Taster lange genug im gedrückten Zustand befand, um in mein gewünschtes Zeitintervall (von 100ms bis 1000ms) zu passen. Wenn das der Fall ist, wird ein Event erstellt und in die EventQueue gesetzt.

## Task 5 : State Machine

- Implement the state machine (create a separate file for each state) using the already available modules and drivers. Perform the necessary initialization in the main function (before entering the infinite loop) and call StateMachine_Run to start the state machine (note that this function will never return).

Das `StateMachine.c` file wurde während der Stunde behandelt. In `State_A.c` und `State_B.c` wird bei der Funktion Enter die LED eingeschaltet und beim Exit wieder ausgeschaltet. Währenddessen, also beim Process, wird das nächste Event von der Queue mit Get() herausgeholt und verarbeitet, welcher Taster gedrückt wurde.