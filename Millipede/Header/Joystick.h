// Functions added to initalize the joystick module as well as obtain x-values that controll the character.

/*
 * Functions from lab manual(Init) and https://www.electronicwings.com/ (ADC_Read)
*/ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

//Allows us to initialize the joystick.
void ADC_Init();

// Allows us to read in x-values from the joystick module
unsigned short ADC_Read(unsigned char);

#endif /* JOYSTICK_H_ 
