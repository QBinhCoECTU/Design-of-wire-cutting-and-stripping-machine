#ifndef __DONGCO_H__
#define __DONGCO_H__

#include "main.h"

/** Dinh nghia chan doi chieu quay cho cac dong co
		*Dong co 1 la dong co keo 1
		*Dong co 2 la dong co keo 2
		*Dong co 3 la dong co cat
*/

#define DIR_MOTOR1_PORT	GPIOB
#define DIR_MOTOR1_PIN	GPIO_PIN_0

#define DIR_MOTOR2_PORT	GPIOA
#define DIR_MOTOR2_PIN	GPIO_PIN_6

#define DIR_MOTOR3_PORT	GPIOA
#define DIR_MOTOR3_PIN	GPIO_PIN_2	
#define PUL_MOTOR3_PORT	GPIOA
#define PUL_MOTOR3_PIN	GPIO_PIN_3

//Dinh nghia chieu quay cho cac dong co
#define MOTOR1_QUAYTHUAN 		DIR_MOTOR1_PORT -> BSRR = DIR_MOTOR1_PIN<<16u;
#define MOTOR1_QUAYNGHICH		DIR_MOTOR1_PORT -> BSRR = DIR_MOTOR1_PIN;
#define MOTOR1_DOICHIEU			HAL_GPIO_TogglePin(DIR_MOTOR1_PORT, DIR_MOTOR1_PIN);
#define MOTOR2_QUAYTHUAN 		DIR_MOTOR2_PORT -> BSRR = DIR_MOTOR2_PIN<<16u;
#define MOTOR2_QUAYNGHICH		DIR_MOTOR2_PORT -> BSRR = DIR_MOTOR2_PIN;
#define MOTOR2_DOICHIEU			HAL_GPIO_TogglePin(DIR_MOTOR2_PORT, DIR_MOTOR2_PIN);
#define MOTOR3_XUONG 				DIR_MOTOR3_PORT -> BSRR = DIR_MOTOR3_PIN<<16u;
#define MOTOR3_LEN					DIR_MOTOR3_PORT -> BSRR = DIR_MOTOR3_PIN;

/* Dung dong co */
#define MOTOR1_STOP					HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
#define MOTOR2_STOP 				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);

/* Khoi dong dong co */
#define MOTOR1_START 			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4); // PA7	(DONG CO 2)
#define MOTOR2_START 			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2); // PB1 (DONG CO 1)


void catDay(void);
void setDao(void);
void tuotday_len(uint16_t xungTuot);
void tuotday_xuong(uint16_t xungTuot);

void duongKinhLoiTuot(int diameter); //Ham di chuyen dao cho qua trinh tuot day
void tuotDayDau1(int length1);//length1 là do dai can tuot
void tuotDayDau2(int length2);//lenght2 la do dai can tuot

#endif
