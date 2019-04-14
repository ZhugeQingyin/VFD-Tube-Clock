#ifndef __RGBLED_H
#define __RGBLED_H

#include "stm32f10x.h"
#include <stdint.h>

void RGBLED_Init(void);
void RGBLED(unsigned int R, unsigned int G, unsigned int B);
void RGB_breathing_light(void);
void RGB_Light(uint16_t mode, uint16_t value);

#endif
