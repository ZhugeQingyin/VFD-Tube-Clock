#ifndef __74HC595_H
#define __74HC595_H

#include "stm32f10x.h"
#include <stdint.h>

void HC595_Pin_Init(void);
void HC595SendData(unsigned char OutData);
void HC595_shift_to_storage(void);
void HC595_OE(unsigned char value);


#endif
