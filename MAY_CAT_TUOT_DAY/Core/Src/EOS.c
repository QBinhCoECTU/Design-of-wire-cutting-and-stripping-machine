#include "EOS.h"

void EOS_init(unsigned int x)
{
	SysTick->LOAD = 72000000/1000*x ; // set reload register
	SysTick->VAL = 0; // Load the SysTick Counter Value
	SysTick->CTRL = 7; // Select Core clock + INT+ENABLE
}
