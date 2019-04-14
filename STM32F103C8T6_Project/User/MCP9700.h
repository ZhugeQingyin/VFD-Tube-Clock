#ifndef __MCP9700_H
#define __MCP9700_H

#include "stm32f10x.h"
#include <stdint.h>

void MCP9700_Init(void);
uint16_t Get_Adc(void);
int16_t Get_Temperature(void);

#endif
