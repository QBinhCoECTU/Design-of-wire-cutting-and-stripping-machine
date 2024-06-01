#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"
#include "dongco.h"

#define ENC1_START HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
#define ENC2_START HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);
#define ENC1_RESET __HAL_TIM_SET_COUNTER(&htim4,0); //TIM4->CNT = 0;
#define ENC2_RESET __HAL_TIM_SET_COUNTER(&htim2,0); //TIM2->CNT = 0;
#define ENC1_XUNG __HAL_TIM_GET_COUNTER(&htim4); //ENC1_XUNG = TIM4->CNT hay htim4.Instance->CNT
#define ENC2_XUNG __HAL_TIM_GET_COUNTER(&htim2); //ENC2_XUNG = TIM2->CNT hay htim4.Instance->CNT


void setValueEnc1(uint16_t value);
void setValueEnc2(uint16_t value);
int doDaiDayTuot(int lenght);
int doDaiDayCanCat(int lenght);

#endif

