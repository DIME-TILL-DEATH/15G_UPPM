#include "uppm_control.h"

ControlPin_t PIN_PhaseShifter[6];
ControlPin_t PIN_Filter[2];

ControlPin_t PIN_ImitStrobe;

void TIM6_IRQHandler(void)  __attribute__((interrupt(/*"WCH-Interrupt-fast"*/)));

void initControlPin(ControlPin_t initializingPin)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = initializingPin.pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(initializingPin.port, &GPIO_InitStructure);
}

uint16_t imitStrobeLength = 20;
uint8_t imitQ = 8;
uint8_t imitQ_cnt = 0;
void UPPM_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);

    PIN_PhaseShifter[0].port = GPIOE;
    PIN_PhaseShifter[0].pin = GPIO_Pin_9;

    PIN_PhaseShifter[1].port = GPIOE;
    PIN_PhaseShifter[1].pin = GPIO_Pin_11;

    PIN_PhaseShifter[2].port = GPIOE;
    PIN_PhaseShifter[2].pin = GPIO_Pin_13;

    PIN_PhaseShifter[3].port = GPIOE;
    PIN_PhaseShifter[3].pin = GPIO_Pin_15;

    PIN_PhaseShifter[4].port = GPIOD;
    PIN_PhaseShifter[4].pin = GPIO_Pin_9;

    PIN_PhaseShifter[5].port = GPIOD;
    PIN_PhaseShifter[5].pin = GPIO_Pin_11;

    for(uint8_t i=0; i<6; i++)
    {
        initControlPin(PIN_PhaseShifter[i]);
    }

    PIN_Filter[0].port = GPIOB;
    PIN_Filter[0].pin = GPIO_Pin_2;
    initControlPin(PIN_Filter[0]);

    PIN_Filter[1].port = GPIOE;
    PIN_Filter[1].pin = GPIO_Pin_8;
    initControlPin(PIN_Filter[1]);

    PIN_ImitStrobe.port = GPIOE;
    PIN_ImitStrobe.pin = GPIO_Pin_10;
    initControlPin(PIN_ImitStrobe);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = imitStrobeLength-1;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000)-1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM6, ENABLE);
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    NVIC_EnableIRQ(TIM6_IRQn);
}

void UPPM_SetPhaseShifter(uint8_t value)
{
    GPIO_WriteBit(PIN_PhaseShifter[0].port, PIN_PhaseShifter[0].pin, value & 0x01);
    GPIO_WriteBit(PIN_PhaseShifter[1].port, PIN_PhaseShifter[1].pin, value & 0x02);
    GPIO_WriteBit(PIN_PhaseShifter[2].port, PIN_PhaseShifter[2].pin, value & 0x04);
    GPIO_WriteBit(PIN_PhaseShifter[3].port, PIN_PhaseShifter[3].pin, value & 0x08);
    GPIO_WriteBit(PIN_PhaseShifter[4].port, PIN_PhaseShifter[4].pin, value & 0x10);
    GPIO_WriteBit(PIN_PhaseShifter[5].port, PIN_PhaseShifter[5].pin, value & 0x20);
}

void UPPM_SetFilter(UPPM_Filter_t filter)
{
    GPIO_WriteBit(PIN_Filter[0].port, PIN_Filter[0].pin, filter & 0x01);
    GPIO_WriteBit(PIN_Filter[1].port, PIN_Filter[1].pin, filter & 0x02);
}

void TIM6_IRQHandler(void)
{
    if(imitQ_cnt == imitQ-1)
    {
        imitQ_cnt = 0;
        GPIO_WriteBit(PIN_ImitStrobe.port, PIN_ImitStrobe.pin, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(PIN_ImitStrobe.port, PIN_ImitStrobe.pin, Bit_RESET);
        imitQ_cnt++;
    }
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}
