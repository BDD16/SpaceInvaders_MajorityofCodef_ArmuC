//******Timer0b.h******
//*includes functions used by second interrupt

//----initialize Timer0B----
void Timer0B_Init(unsigned short period);
//----Stop the interrupt----
void Timer0B_Stop(void);
//----Start the interrupt---
void Timer0B_Start(void);