#ifndef USER_INTERFACES_UPPM_CONTROL_H_
#define USER_INTERFACES_UPPM_CONTROL_H_

#include <stdint.h>

#include "debug.h"

typedef enum
{
    UPPM_FILTER0 = 0,
    UPPM_FILTER1,
    UPPM_FILTER2,
    UPPM_FILTER3
}UPPM_Filter_t;

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
}ControlPin_t;


void UPPM_Init();
void UPPM_SetPhaseShifter(uint8_t value);
void UPPM_SetFilter(UPPM_Filter_t filter);

#endif /* USER_INTERFACES_UPPM_CONTROL_H_ */
