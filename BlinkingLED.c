// 2DX4StudioW2E0_SysTickDemo
// This program illustrates the use of SysTick in the C language.
// Note the library headers asscoaited are PLL.h and SysTick.h,
// which define functions and variables used in PLL.c and SysTick.c.
// This program uses code directly from your course textbook.
//
// This example will be extended for Exercise 1 and then furtre in 
// W20E1.
//
//  Written by Tom Doyle
//  January 18, 2020
//  Last Update: January 18, 2020


#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortL_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};	// allow time for clock to stabilize
	GPIO_PORTL_DIR_R |= 0b00010000;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTL_AFSEL_R &= ~0b00010000;     								// disable alt funct on PN0
  GPIO_PORTL_DEN_R |= 0b00010000;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTL_AMSEL_R &= ~0b00010000;     								// disable analog functionality on PN0		
	
	GPIO_PORTL_DATA_R = 0b00010000; 								//hello world!
		
	return;
}



int main(void){
  				
	uint32_t 	delay = 50, 													// this is a multiplier used for SysTick = half of delay 
						count = 0;														// my loop control variable initialized to 0	
	 
	
	
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration

	PortL_Init();																		// Initialize Port N GPIO
	while(count < 100){															// run program for fixed number of steps/samples
		GPIO_PORTL_DATA_R = 0b00010000;							// toggle LED for visualization of process
		SysTick_Wait10ms(delay);											// delay*10ms
		GPIO_PORTL_DATA_R = 0b00000000;							// toggle LED for visualization of process
		SysTick_Wait10ms(delay);
	}

}



