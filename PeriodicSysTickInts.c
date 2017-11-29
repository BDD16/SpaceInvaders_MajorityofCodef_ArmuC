// PeriodicSysTickInts.c
// Runs on LM3S1968
//By Blake De Garza

// oscilloscope or LED connected to PD0 for period measurement
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "SysTickInts.h"
#include "PLL.h"
#include "lm3s1968.h"
#include "LCD.h"
#include "ADC.h"
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
#define N 256
unsigned long ADCMail;	  //defining global variables
unsigned long Status;	 //defining global variables
unsigned long Data;
unsigned long m=1800; //slope in fixed point slope*1024
unsigned long b=8123; //intercept in fixed point    
int main(void){       // bus clock at 50 MHz
  SYSCTL_RCGC2_R |=0x20; //turning on portF clock
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);
  PLL_Init(); //turning on 50MHz bus clock
  LCD_Open();
  LCD_Clear();
  ADC_Init(7);  //setting ADC channel 7
  SysTick_Init(2000000);     // initialize interrupt to run at 25Hz
  EnableInterrupts();

  while(1){                // interrupts every 1ms
    WaitForInterrupt();

	//*****funtion to implement****//
	if(Status==1){
	 LCD_Clear();
	 Data =ADCMail/N; //noise reducing filter
	 Data=((m*Data)+b)>>10; //slope intercept formula y=1.7585x+7.9326
	 LCD_OutFix(Data);
	 Status=0;
	 }
  }
}
