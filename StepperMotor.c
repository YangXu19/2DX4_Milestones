

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

void PortK_Init(void){	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9;		  // activate the clock for Port E
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R9) == 0){};	  // allow time for clock to stabilize
	GPIO_PORTK_DEN_R |= 0b00001111;
	GPIO_PORTK_DIR_R |= 0b00001111;                           // make PK0,PK1,PK2,PK3 output  
	GPIO_PORTK_DATA_R = 0b00000000;                             // setting state to zero to drive the row, one to disable 
	return;

}


//Turns on D2
void PortN0_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;                 //activate the clock for Port N to use D2
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};
	GPIO_PORTN_DIR_R|=0b00000011;                             //Here D2 and D1 are ready for output but we will only be using D2
	GPIO_PORTN_DEN_R|=0b00000011;
	return;
}

//rotate clockwise
void Rotate_CW(void)
{
	uint32_t 	delay = 1, count = 0;
	
	while (count < 50)
	{
	GPIO_PORTN_DATA_R = 0b00000001; //turn on LED
	GPIO_PORTK_DATA_R = 0b00001100;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00000110;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00000011;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00001001;
	SysTick_Wait10ms(delay);
	count++;
	}
	
	return;
}

//rotate counter clockwise
void Rotate_CCW(void)
{
	uint32_t 	delay = 1, count = 0;
	
	while (count < 50)
	{
	GPIO_PORTN_DATA_R = 0b00000001; //turn off LED
	GPIO_PORTK_DATA_R = 0b00001001;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00000011;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00000110;
	SysTick_Wait10ms(delay);
	GPIO_PORTK_DATA_R = 0b00001100;
	SysTick_Wait10ms(delay);
	count++;
	}
	
	return;
}

int main(void){
	
	
	PLL_Init();
	SysTick_Init();
	
	PortK_Init();
	PortN0_Init();
	
	Rotate_CW();
	Rotate_CCW();
}


