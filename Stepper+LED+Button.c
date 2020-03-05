//Bus clock speed = 48 MHz
//Onboard LED (distance) = F0
//External LED (displacement) = L4

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


//make port E output drive row
void PortE0_Init(void){	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		  // activate the clock for Port E
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4) == 0){};	  // allow time for clock to stabilize
	GPIO_PORTE_DEN_R= 0b00000001;
	GPIO_PORTE_DIR_R |= 0b00000001;                           // make PE0 output  
	GPIO_PORTE_DATA_R=0b00000000;                             // setting state to zero to drive the row, one to disable 
	return;
	}

//make port M input read column
void PortM0_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;                 // activate the clock for Port M
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};        // allow time for clock to stabilize 
	GPIO_PORTM_DIR_R |= 0b00000000;       			  // make PM0 an input, PM0 is reading the column 
  GPIO_PORTM_DEN_R |= 0b00000001;
	return;
}	
	
//port K0,K1,K2,K3 for stepper motor
void PortK_Init(void){	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9;		 // activate the clock for Port E
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R9) == 0){};	 // allow time for clock to stabilize
	GPIO_PORTK_DEN_R |= 0b00001111;
	GPIO_PORTK_DIR_R |= 0b00001111;                          // make PK0,PK1,PK2,PK3 output  
	GPIO_PORTK_DATA_R = 0b00000000;                          // setting state to zero to drive the row, one to disable 
	return;

}

//Turns on F0 onboard LED
void PortF0_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;                 //activate the clock for Port F0
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};
	GPIO_PORTF_DIR_R|=0b00000001;                             
	GPIO_PORTF_DEN_R|=0b00000001;
	return;
}

//Turns on L4 external LED
void PortL4_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;                 //activate the clock for Port L4 for external LED
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};
	GPIO_PORTL_DIR_R|=0b00010000;                             
	GPIO_PORTL_DEN_R|=0b00010000;
	return;
}

//motor function - rotate clockwise
void Motor(int direction)
{
	uint32_t 	delay = 1;
	
	if (direction == 0)
	{
		GPIO_PORTK_DATA_R = 0b00001100;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00000110;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00000011;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00001001;
		SysTick_Wait10ms(delay);
	}
	
	//rotate counter clockwise 
	else if (direction == 1)
	{
		GPIO_PORTK_DATA_R = 0b00001001;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00000011;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00000110;
		SysTick_Wait10ms(delay);
		GPIO_PORTK_DATA_R = 0b00001100;
		SysTick_Wait10ms(delay);
	}
}

int main(void){
	
	//initialize ports 
	PLL_Init();
	SysTick_Init();
	PortK_Init();
	PortF0_Init();
	PortL4_Init();
	PortM0_Init();
	PortE0_Init();
	
	uint32_t 	delay = 1;
	int count = 513;
	
	
	//keep checking if the button is pressed
	while(1)
	{
		GPIO_PORTL_DATA_R = 0b00010000;
 
		//if button is pressed	then reset count to 0
		if(((GPIO_PORTM_DATA_R&0b00000001)==0&&(GPIO_PORTE_DATA_R&0b00000001)==0))
		{
			GPIO_PORTL_DATA_R = 0b00000000;
			count = 0;
		}
		
		//start counting 
		while(count <= 512)
		{
			//call motor function in the clockwise direction 
			Motor(0);
			
			//checks if count is 45 deg
			if(count%64 == 0)
			{
				GPIO_PORTF_DATA_R = 0b00000001;		//blink onboard LED
				SysTick_Wait10ms(10*delay);
				GPIO_PORTF_DATA_R = 0b00000000;
			}
			count++; 
		}
		//exit loop and then check button press
	}
}
