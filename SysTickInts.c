// SysTickInts.c
// Runs on LM3S1968
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano
// June 27, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

   Program 5.12, section 5.7

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope or LED connected to PD0 for period measurement
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
// #include "lm3s1968.h"
#include "LCD.h"
#include "ADC.h"

#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control

#define PortG_2                 (*((volatile unsigned long *) 0x400263FC))
#define PortG_DIR				(*((volatile unsigned long *) 0x40026400))
#define PortG_AFSEL             (*((volatile unsigned long *) 0x40026420))
#define PortG_DEN               (*((volatile unsigned long *) 0x4002651C))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
volatile unsigned long Counts = 0;
unsigned long sum=0;

unsigned short i;
//unsigned long m=1800; //slope in fixed point slope*1024
//unsigned long b=8123; //intercept in fixed point
extern unsigned long ADCMail; //linking global variables
extern unsigned long Status;   //linking  ""
extern char time[];
//extern unsigned long Clock1;
#define N 25
#define GPIO_PORTD0             (*((volatile unsigned long *)0x40007004))

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 20ns
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void SysTick_Init(unsigned long period){
   
  SYSCTL_RCGC2_R |= 0x40; // activate port G 
  Counts = 0;
  PortG_DIR |= 0x04;   // make PG2 Output, LED light
	PortG_DIR &=~0x80;  //make PG7 Input
  PortG_DEN |= 0x84;   // enable digital I/O on PG7and PG2
  PortG_AFSEL &=~0x84; //disabling alternate functions
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}

// Interrupt service routine
// Executed every 20ns*(period)
void SysTick_Handler(void){ //By Blake De Garza
  PortG_2 ^= 0x04;        // toggle PD0
  Counts = Counts + 1;
    sum=0;
	for(i=0;i<N;i++){
	sum=sum+ADC_InSeq3();
	}
	ADCMail=((sum/N)*110)>>10; //putting new data into mailbox
	if((ADCMail%2)==1){
		ADCMail++;
	}
	//setting time for animation ~.5 seconds
	if(Counts==12){
	Status=1;    //setting flag to let program know new data is available
	}
	if(Counts==25){ //used for Animation and Timer clock
	Status=0;
	//Clock1=1;
	Counts=0;
	if(time[7]>0x30){
			time[7]=time[7]-1;
		} 
		if(time[7]==0x30 && time[6]>0x30){
			time[6]=time[6]-1;
			time[7]=0x39;
		} 
    if(time[6]==0x30 && time[5]>0x30){
       time[5]=time[5]-1;
       time[6]=0x39;	
		}
					
	}
}

