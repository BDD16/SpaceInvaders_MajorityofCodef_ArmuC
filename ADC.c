//*******ADC.C*******
//INITILIZES ADC
#include "lm3s1968.h"
void ADC_Init(unsigned char channelNum){
      if(channelNum >7){
	  return;//0 to 7 are valid channels on our board
	  }
  	  SYSCTL_RCGC0_R |= 0x00010000;  //ACTIVATE ADC
	  SYSCTL_RCGC0_R &= ~0x00000300;  //CONFIGURE FOR 125K
	  ADC_SSPRI_R = 0x0123;			 //SEQUENCER 3 IS LOWEST PRIORITY
	  ADC_ACTSS_R &= ~0x0001;		 //DISABLE SAMPLE SEQUENCER 3
	  ADC_EMUX_R &= ~0x000F;		 //SEQUENCER 3 IS SOFTWARE TRIGGER
	  ADC_SSMUX0_R &= ~0x0007;		 //CLEAR SS3 FIELD 
	  ADC_SSMUX0_R += channelNum;	 //SET CHANNELL 
	  ADC_SSCTL0_R = 0x0006;         //NO TS0 D0, YES IE0 END0
	  ADC_IM_R &= ~0x0001;           //DISABLE SS3 INTERRUPTS
	  ADC_ACTSS_R |= 0x0001;         //ENABLE SAMPLE SEQUENCER 3
	  }
//*******ADC_InSeq3*********
//BUSY-WAIT ANALOG TO DIGITAL CONVERSION
//INPUT:NONE
//OUTPUT: 10-BIT RESULT OF ADC CONVERSION
unsigned long ADC_InSeq3(void){unsigned long result;
          ADC_PSSI_R = 0x0001;			//INITIATE  SS3
		  while((ADC_RIS_R &0x01)==0){}; //WAIT FOR CONVERSION DONE
		  result = ADC_SSFIFO0_R&0x3FF;  //READ 10bit RESULT
		  ADC_ISC_R = 0x0001;            //ACKNOWLEDGE COMPLETION
		  return result;
		  }
