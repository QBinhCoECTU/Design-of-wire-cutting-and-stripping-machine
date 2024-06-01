/** Put this in the src folder **/
#include "main.h"
#include "dongco.h"
#include "encoder.h"
#include "delay.h"

#define PUL_MOTOR3_PORT		GPIOA
#define PUL_MOTOR3_PIN		GPIO_PIN_3

#define xungCat					  16000 //19200; //So xung ma dong co quay cat dut day
#define speedDao					48		
/* Dinh nghia so xung cho qua trinh cat,  truc vitme quay mot vong tuong duong voiw 2mm
Driver TB6600 bbuoc chia 16, 3200 xung step motor quay mot vong tuong duong dao di chuyen 2mm
*/

uint16_t xungSetDao =	12800; //3200 //16000
void setDao(void){
	MOTOR3_LEN;
	for(int i = 0; i < xungSetDao; i++)
		{
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN;
			delay_us(speedDao);
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN<<16u;
			delay_us(speedDao);
		}
}

void catDay(void)
{
	MOTOR3_XUONG;
	for(int i = 0; i < xungCat; i++)
		{
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN;
			delay_us(speedDao);
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN<<16u;
			delay_us(speedDao);
		}
	delay_ms(50);
	MOTOR3_LEN;
	for(int i = 0; i < xungCat; i++)
		{
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN;
			delay_us(speedDao);
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN<<16u;
			delay_us(speedDao);
		}
}

void tuotday_len(uint16_t xungTuot){
	MOTOR3_LEN;
	for(int i = 0; i < xungTuot; i++){
		PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN;
		delay_us(speedDao);
		PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN<<16u;
		delay_us(speedDao);
	}
}

void tuotday_xuong(uint16_t xungTuot){
	MOTOR3_XUONG;
	for(int i = 0; i < xungTuot; i++)
		{
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN;
			delay_us(speedDao);
			PUL_MOTOR3_PORT -> BSRR = PUL_MOTOR3_PIN<<16u;
			delay_us(speedDao);
		}
}
