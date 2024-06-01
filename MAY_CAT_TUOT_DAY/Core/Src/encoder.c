#include "encoder.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim2;


void setValueEnc1(uint16_t value){
	__HAL_TIM_SET_COUNTER(&htim4,value);
}

void setValueEnc2(uint16_t value){
	__HAL_TIM_SET_COUNTER(&htim2,value);
}
