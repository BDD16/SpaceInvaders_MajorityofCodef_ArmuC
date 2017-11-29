//****DAC.h*****

//***DAC_Init****
void DAC_Init(void);

//***DAC_Out****
//writes a sample value to DAC
//inputs: 4 bit value
void DAC_Out(unsigned char);

void Sound_Play(const unsigned char *sound);
