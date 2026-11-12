#ifndef KEY_H
#define KEY_H

typedef enum Key_Id_e {
  KEY_ID_USER0   = 0,
  KEY_ID_USER1   = 1,
  KEY_ID_WAKE_UP = 2  
} Key_Id_t;


// Initialisiert GPIOs und startet den 50-ms-Timer.
int Key_Initialize(void);

#endif