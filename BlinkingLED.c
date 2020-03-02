#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortL_Init(void){
	//Use PortL onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;		// activate clock for Port L
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};	// allow time for clock to stabilize
	GPIO_PORTL_DIR_R |= 0b00010000;        			// make PL0 out 
 	GPIO_PORTL_AFSEL_R &= ~0b00010000;     			// disable alt funct on PL0
  	GPIO_PORTL_DEN_R |= 0b00010000;        			// enable digital I/O on PL0
																									// configure PL4 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTL_AMSEL_R &= ~0b00010000;     									
	
	GPIO_PORTL_DATA_R = 0b00010000; 
		
	return;
}



int main(void){
  				
	uint32_t 	delay = 50, 	// this is a multiplier used for SysTick = half of delay 
			count = 0;	// my loop control variable initialized to 0	
	 
	
	
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration

	PortL_Init();																		// Initialize Port L GPIO
	while(count < 100){															// run program for fixed number of steps/samples
		GPIO_PORTL_DATA_R = 0b00010000;		// toggle LED for visualization of process
		SysTick_Wait10ms(delay);		// delay*10ms
		GPIO_PORTL_DATA_R = 0b00000000;		// toggle LED for visualization of process
		SysTick_Wait10ms(delay);
	}

}



