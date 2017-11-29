 //*****Timer0b Interrupt***
 //Blake De Garza
 #include "lm3s1968.h"
 #include "DAC.h"
 
 extern const unsigned char shoot[]; 
 extern unsigned long k;
 unsigned long TimerCount;
// maximum period is 1.3ms

void Timer0B_Stop(void);

void Timer0B_Init(unsigned short period){ 
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; // 0) activate
  TimerCount = 0;             
  TIMER0_CTL_R &= ~0x00000100;     // 1) disable timer0B 
  TIMER0_CFG_R = 0x00000004;       // 2) 16-bit mode
  TIMER0_TBMR_R = 0x00000002;      // 3) periodic mode
  TIMER0_TBILR_R = period-1;       // 4) reload value
  TIMER0_TBPR_R = 0;               // 5) 20ns timer0B
  TIMER0_ICR_R = 0x00000100;       // 6) clear flag
  TIMER0_IMR_R |= 0x00000100;      // 7) arm timeout
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFFFF00)|0x00000040; 
  NVIC_EN0_R |= NVIC_EN0_INT20;    // 9) enable int 20 C
}

// trigger is Timer0B Time-Out Interrupt
// set periodically
void Timer0B_Handler(void){
  TIMER0_ICR_R = 0x00000100;;      // acknowledge
  TimerCount++;
//Timer0B_Stop();
// run some background stuff here
	if((GPIO_PORTG_DATA_R &0x80 )==0x00 || k>0){
	Sound_Play(shoot);
	}
	if(k==4079){
		Timer0B_Stop();
	}
}

void Timer0B_Stop(void){ 
  TIMER0_CTL_R &= ~0x00000100;     // disable timer0B 
}

void Timer0B_Start(void){ 
  TIMER0_CTL_R |= 0x00000100;      // enable timer0B
}

