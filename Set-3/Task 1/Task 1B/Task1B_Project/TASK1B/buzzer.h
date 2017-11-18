/*
 * buzzer.h
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_
#include "buzzer.c"

void buzzer_pin_config (void);
void buzzer_on (void);
void buzzer_off (void);
void buzzer_beep();

#endif /* BUZZER_H_ */