/*
 * Author : Arturo Alvarado
*/ 

//Necessary Libraries
#include <avr/interrupt.h>
#include <avr/eeprom.h> 
#include "Joystick.h"
#include "timer.h"
#include "scheduler.h"
#include "io.h"
#include "max7219.h"
#include "Centipede.h"
#include "Bullet.h"

unsigned short x_value;
uint8_t ic = 0;
unsigned char character = 0x08;
unsigned char button;
unsigned char Reset_Button;
unsigned char CurrScore = 0x00;
unsigned char HScore ;
// Shared Variable for game & game logic
unsigned char Killed_Character;
unsigned char Win_Condition;
unsigned char You[] = {0xA5, 0xA5, 0xE5, 0x47, 0x78, 0x28, 0x28, 0x38};
unsigned char Winner[] = {0xA5, 0xA5, 0xE7, 0xA5, 0x38, 0x10, 0x10,0x38};
unsigned char Loser[] = {0x8E, 0x8A, 0xEE, 0xEF, 0x88, 0xEF, 0x28, 0xEF};

//Initializing Centipede
Centipede First_Bug, Second_Bug, Third_Bug, Fourth_Bug;
Centipede *Bugs[] = {&First_Bug, &Second_Bug, &Third_Bug, &Fourth_Bug};
//Initialize Bullet		
Bullet Shot;

void Reset_Score(){
		HScore = 0;
		eeprom_update_byte((const char*) 1,HScore);
}

void Losing_Screen(){
	
	max7219_clearDisplay(0);
	max7219_clearDisplay(1);	
	for(unsigned char e = 0; e < 8 ; e++){
		max7219_digit(0, e, You[e]);
	}
	for(unsigned char z = 0; z < 8; z++){
		max7219_digit(1, z, Loser[z]);
	}
	character = 0x00;
	delay_ms(10000);
}
void Init_EEPROM(){
	if (eeprom_read_byte((const char*) 1) != 0xFF) {
		HScore = eeprom_read_byte((const char*) 1);
		}else{
		HScore = 0x00;	
	}
}
void NewHighScore(){
	if(CurrScore >= HScore){
		HScore = CurrScore;
		eeprom_update_byte((const char*) 1, HScore);
	}
}
void Winning_Screen(){
	
	max7219_clearDisplay(0);
	max7219_clearDisplay(1);
	for(unsigned char f = 0; f < 8 ; f++){
		max7219_digit(0, f, You[f]);
	}
	for(unsigned char g = 0; g < 8; g++){
		max7219_digit(1, g, Winner[g]);
	}
	delay_ms(10000);
	
}
void Update_Screen(){
	max7219_clearDisplay(0);
	max7219_clearDisplay(1);

		max7219_digit(1, 7, character);	

		max7219_digit(Bugs[0]->Matrix, Bugs[0]->Row, Bugs[0]->Position);	
		max7219_digit(Bugs[1]->Matrix, Bugs[1]->Row, Bugs[1]->Position);
		max7219_digit(Bugs[2]->Matrix, Bugs[2]->Row, Bugs[2]->Position);
		max7219_digit(Bugs[3]->Matrix, Bugs[3]->Row, Bugs[3]->Position);	
		delay_ms(200);
}

enum Centipede_STATES{Centipede_Init, Move_It}Centipede_state;
int Centipede_Display(int Centipede_state){
	
	switch(Centipede_state){
		case Centipede_Init:
			Centipede_state = Move_It;
			break;
			
		case Move_It:
			if(Bugs[0]->Direction == 7){
				if(Bugs[0]->Position > 64){
					Bugs[0]->Row++;
					Bugs[0]->Direction = 77;
				}
				else{
					Bugs[0]->Position = Bugs[0]->Position * 2;
				}
			}
			else if(Bugs[0]->Direction == 77){
				if(Bugs[0]->Position > 1){
					Bugs[0]->Position = Bugs[0]->Position / 2;					
				}
				else{
					if(Bugs[0]->Row == 7 && Bugs[0]->Position == 1){
						Bugs[0]->Row = 0;
						Bugs[0]->Matrix = 1;
						Bugs[0]->Direction = 7;
					}
					else{
						Bugs[0]->Row++;
						Bugs[0]->Direction = 7;
					}
				}
			}
			
			if(Bugs[1]->Direction == 7){
				if(Bugs[1]->Position > 64){
					Bugs[1]->Row++;
					Bugs[1]->Direction = 77;
				}
				else{
					Bugs[1]->Position = Bugs[1]->Position * 2;
				}
			}
			else if(Bugs[1]->Direction == 77){
				if(Bugs[1]->Position > 1){
					Bugs[1]->Position = Bugs[1]->Position / 2;
				}
				else{
					if(Bugs[1]->Row == 7 && Bugs[1]->Position == 1){
						Bugs[1]->Row = 0;
						Bugs[1]->Matrix = 1;
						Bugs[1]->Direction = 7;
					}
					else{
						Bugs[1]->Row++;
						Bugs[1]->Direction = 7;
					}
				}
			}			
			if(Bugs[2]->Direction == 7){
				if(Bugs[2]->Position > 64){
					Bugs[2]->Row++;
					Bugs[2]->Direction = 77;
				}
				else{
					Bugs[2]->Position = Bugs[2]->Position * 2;
				}
			}
			else if(Bugs[2]->Direction == 77){
				if(Bugs[2]->Position > 1){
					Bugs[2]->Position = Bugs[2]->Position / 2;
				}
				else{
					if(Bugs[2]->Row == 7 && Bugs[2]->Position == 1){
						Bugs[2]->Row = 0;
						Bugs[2]->Matrix = 1;
						Bugs[2]->Direction = 7;
					}
					else{
						Bugs[2]->Row++;
						Bugs[2]->Direction = 7;
					}
				}
			}			
			if(Bugs[3]->Direction == 7){
				if(Bugs[3]->Position > 64){
					Bugs[3]->Row++;
					Bugs[3]->Direction = 77;
				}
				else{
					Bugs[3]->Position = Bugs[3]->Position * 2;
				}
			}
			else if(Bugs[3]->Direction == 77){
				if(Bugs[3]->Position > 1){
					Bugs[3]->Position = Bugs[3]->Position / 2;
				}
				else{
					if(Bugs[3]->Row == 7 && Bugs[3]->Position == 1){
						Bugs[3]->Row = 0;
						Bugs[3]->Matrix = 1;
						Bugs[3]->Direction = 7;
					}
					else{
						Bugs[3]->Row++;
						Bugs[3]->Direction = 7;
					}
				}
			}		
			
			Centipede_state = Move_It;
			break;
		default:
			break;
	}
	return Centipede_state;
};

enum Button_STATES{Button_Init, Button_Wait, Button_Press}Button_state;
int Button_Display(int Button_state){

	button = ~PINA & 0x02;
	
	switch(Button_state){
		
		case Button_Init:
			Shot.Position = 0;
			Shot.Row = 7;
			Shot.Matrix = 1;
			Shot.Finished_Shooting = 0;
			Button_state = Button_Wait;
			break;
		case Button_Wait: 
			if(!button){
				Button_state = Button_Wait;
			}
			else if(button){
				Button_state = Button_Press;
				Shot.Position = character;
			}
			break;
		case Button_Press:
			if(button || Shot.Finished_Shooting == 0){
				Button_state = Button_Press;
			}
			else if(Shot.Finished_Shooting == 1){
				Button_state = Button_Wait;
				Shot.Finished_Shooting = 0;
				Shot.Row = 7;
				Shot.Position = 0x00;
				Shot.Matrix = 1;
			}
			break;
		default:
			break;
	}	
	switch(Button_state){
		case Button_Init:
			break;
		case Button_Wait:
			break;
		case Button_Press:
		
			max7219_digit(Shot.Matrix, Shot.Row - 1, Shot.Position);
 			Shot.Row--;

 			max7219_clearDisplay(Shot.Matrix);
 			if(Shot.Row - 1	== 0 && Shot.Matrix == 1){
 				Shot.Matrix = 0;
 				Shot.Row = 8;
 			} 
 			if((Shot.Row - 1) == 0 && Shot.Matrix == 0){
 				Shot.Finished_Shooting = 1;
			}
			break;

	}
	return Button_state;
};

enum Game_Logic_STATES{Logic_Init, Gameplay, Lost, Won}Logic_State;
int Game_Logic(int Logic_State){
	
	switch(Logic_State){
		case Logic_Init:
			Init_EEPROM();
			Display_Points(HScore, CurrScore);
			Killed_Character = 0;
			Win_Condition = 0;
			Logic_State = Gameplay ;
			break;
		case Gameplay:
			// Check For Any bug hitting last row
			if((Bugs[0]->Row == 7 && Bugs[0]->Matrix == 1) || (Bugs[1]->Row == 7 && Bugs[1]->Matrix == 1) || 
				(Bugs[2]->Row == 7 && Bugs[2]->Matrix == 1) || (Bugs[3]->Row == 7 && Bugs[3]->Matrix == 1) ){
					
					Killed_Character = 100;
					Logic_State = Lost;
			}
			else if(Bugs[0]->Position == 0 && Bugs[1]->Position == 0 && Bugs[2]->Position == 0 && Bugs[3]->Position == 0 ){
					Win_Condition = 100;
					Logic_State = Won;
			}
			else if(((Shot.Row - 1) == Bugs[0]->Row) && (Shot.Position == Bugs[0]->Position) && (Shot.Matrix == Bugs[0]->Matrix)){
				CurrScore++;
				
				NewHighScore();
				Display_Points(HScore, CurrScore);
				Bugs[0]->Position = 0;
			}
			else if(((Shot.Row - 1) == Bugs[1]->Row) && (Shot.Position == Bugs[1]->Position) && (Shot.Matrix == Bugs[1]->Matrix)){
				CurrScore++;
				NewHighScore();
				Display_Points(HScore, CurrScore);
				Bugs[1]->Position = 0;
			}
			else if(((Shot.Row - 1) == Bugs[2]->Row) && (Shot.Position == Bugs[2]->Position) && (Shot.Matrix == Bugs[2]->Matrix)){
				CurrScore++;
				NewHighScore();
				Display_Points(HScore, CurrScore);	
				Bugs[2]->Position = 0;
			}
			else if(((Shot.Row - 1) == Bugs[3]->Row) && (Shot.Position == Bugs[3]->Position) && (Shot.Matrix == Bugs[3]->Matrix)){
				CurrScore++;
				NewHighScore();
				Display_Points(HScore, CurrScore);	
				Bugs[3]->Position = 0;
			}									
			else{
					Logic_State = Gameplay;
			}
			break;
			
		case Lost:
			//PORTB = 0x08;
			Killed_Character = 100;
			Logic_State = Lost;
			break;
			
		case Won:
			Win_Condition = 100;
			Logic_State = Won;
			break;
			
		default:
			break;
	}		
	return Logic_State;
};

enum Game_STATES{Game_Init, Play_Game, Game_Lost, Game_Won, Wait_for_reset}Game_state;
int Handle_Game(int Game_state){
	Reset_Button = ~PINA & 0x04;
	
	switch(Game_state){
		case Game_Init:
			PORTB = 0x01;
			Game_state = Play_Game;
			break;
		case Play_Game:
			PORTB = 0x02;
			if(Killed_Character == 100){
				Bugs[0]->Position = 0x00;
				Bugs[1]->Position = 0x00;
				Bugs[2]->Position = 0x00;
				Bugs[3]->Position = 0x00;
				Game_state = Game_Lost ;
			}
			else if(Win_Condition == 100){
				Bugs[0]->Position = 0x00;
				Bugs[1]->Position = 0x00;
				Bugs[2]->Position = 0x00;
				Bugs[3]->Position = 0x00;				
				
				Game_state = Game_Won;
			}
			else{
				//DISPLAY NEW SCREEN
				Update_Screen();
				Game_state = Play_Game;
			}
			break;
		case Game_Lost:
			PORTB = 0x04;
			Losing_Screen();
			Game_state = Wait_for_reset;
			// Display frown
			
			break;
		case Game_Won:
			// Display Win
			Winning_Screen();
			Game_state = Wait_for_reset;
			break;
		case Wait_for_reset:
			PORTB = 32;
			if(Reset_Button){
				Reset_Score();
				Display_Points(HScore, CurrScore);
			}
			Game_state = Wait_for_reset;
			break;
		default:
			break;
	}
	return Game_state;
};

enum Matrix_STATES{Loading, Playing, Move_Right, Move_Left, Shoot} Matrix_state;
int Matrix_Display(int Matrix_state){
	
	x_value = ADC_Read(0);
	
	switch(Matrix_state){
		case Loading:
			Matrix_state = Playing;
//			max7219_digit(1, 7, character);
			break;
		case Playing:
			if(x_value > 900){
				if(character > 1){
					character = (character >> 1) ;
				}
				Matrix_state = Move_Right;
//				max7219_digit(1, 7, character);
			}
			else if(x_value < 300){
				if(character < 128){
					character = character << 1;
				}
				Matrix_state = Move_Left;
	//		max7219_digit(1, 7, character);
			}
			else {
				Matrix_state = Playing;
//			max7219_digit(1, 7, character);
			}
				
			break;
		case Move_Right:
			if(x_value > 900){
				Matrix_state = Move_Right;
			}
			else{
				Matrix_state = Playing;
			}
			break;
		case Move_Left:
			if(x_value < 300){
				Matrix_state = Move_Left;
			}
			else{
				Matrix_state = Playing;
			}
			break;
		default:
			break;
	}
	return Matrix_state;	
};

task task1, task2, task3, task4, task5;

int main(void)
{
    //Initialize inputs
	DDRA = 0x00; PORTA = 0xFF;
	//Initialize outputs
	DDRB = 0xFF; PORTB = 0X00;
	// Port D for LED Matrices & LCD Screen
	DDRD = 0xFF; PORTD = 0x00;
	// Port C for LCD Screen
	DDRC = 0xFF; PORTC = 0x00;
	
	task *tasks[] = {&task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	First_Bug.Direction = 7;
	First_Bug.Matrix = 0;
	First_Bug.Row = 0;
	First_Bug.Position = 0x01;
	
	Second_Bug.Direction = 77;
	Second_Bug.Matrix = 0;
	Second_Bug.Row = 1;
	Second_Bug.Position = 128;
	
	Third_Bug.Direction = 7;
	Third_Bug.Matrix = 0;
	Third_Bug.Row = 2;
	Third_Bug.Position = 1;

	Fourth_Bug.Direction = 77;
	Fourth_Bug.Matrix = 0;
	Fourth_Bug.Row = 3;
	Fourth_Bug.Position = 128;
    
	// Task 1 Character Movement
	task1.state = Loading;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Matrix_Display;
	
	//Task 2 Button
	task2.state = Button_Init;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Button_Display;
	
	//Task 3 Centipede
	task3.state = Centipede_Init;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &Centipede_Display;	
	
	// Task 4 Logic
	task4.state = Logic_Init;
	task4.period = 50;
	task4.elapsedTime = task4.period;
	task4.TickFct = &Game_Logic;	

	// Task 5 Gameplay
	task5.state = Game_Init;
	task5.period = 50;
	task5.elapsedTime = task5.period;
	task5.TickFct = &Handle_Game;	
	
	
// 	uint8_t ic = 0;
	// Initialize Matrices
	max7219_init();
	// Clear Matrix 1 and 2
	max7219_clearDisplay(0);
	max7219_clearDisplay(1);	
	//Initialize joystick module
	ADC_init();
	//Initialize LCD
	LCD_init();
	//Initialize Timer
	TimerSet(50);
	TimerOn();

	
	unsigned short i;
	
	for(ic=0; ic<MAX7219_ICNUMBER; ic++) {
		max7219_shutdown(ic, 1); //power on
		max7219_test(ic, 0); //test mode off
		max7219_decode(ic, 0); //use led matrix
		max7219_intensity(ic, 7); //intensity
		max7219_scanlimit(ic, 7); //set number of digit to drive
	}
	     	
	while (1) 
	{
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += tasks[i]->period;
		}
						  
		while(!TimerFlag);
		TimerFlag = 0;
		
	}
	return 0;
} 
