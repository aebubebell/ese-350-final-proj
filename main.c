#include <avr/io.h>
#define F_CPU 16000000UL


#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include "lib\ST7735.h"
#include "lib\LCD_GFX.h"


#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "connectFour.c"

volatile int isRising = 1;
volatile int pulseStart = 0;
volatile int pulseEnd = 0;
volatile int tickCount = 0;
int overflowCount = 0;
int ballCount = 0;
int playerTurn = 1; 
int cm = 0; 
int player1Score = 0; 
int player2Score = 0; 

char String[25];

void Initialize(){
	cli();
	
	//input pins for IR sensors 
	DDRD &= ~(1<<DDD2);
	DDRD &= ~(1<<DDD3);
	DDRD &= ~(1<<DDD4);
	DDRD &= ~(1<<DDD5);
	DDRD &= ~(1<<DDD7);
	//setting them all high initially (comment out later) 
	PORTD |= (1<<PORTD2);
	PORTD |= (1<<PORTD3);
	PORTD |= (1<<PORTD4);
	PORTD |= (1<<PORTD5);
	PORTD |= (1<<PORTD7);
	
	
	DDRC &= ~(1<<DDC0); //set PC0 as input pin for bonus distance signal
	//DDRB &= ~(1<<DDB0); //set PB0 as input pin for echo signal
	DDRC |= (1<<DDC5); //set PC5 as output pin for buzzer
	PORTC &= ~(1<<PORTC5); //set buzzer low
	

	
	//set timer 1 to Normal Mode
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM12);
	TCCR1A &= ~(1<<WGM13);
	
	//prescaler timer 1 by 64
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	TIMSK1 |= (1<<TOIE1); //enable overflow interrupt
	TIMSK1 |= (1<<ICIE1); //enable input capture interrupt
	TCCR1B |= (1<ICES1); //enable interrupt to look for rising edge
	
	TIFR1 |= (1<<ICF1);
	TIFR1 |= (1<<TOV1);
	
//	lcd_init();
//	LCD_drawBlock(0,0,159,127,0);
//	LCD_drawCircle(20,20,5,65535);
	
	sei();
	 
}

void UART_init(void){
	//set baud rate
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	
	UCSR0C |= (1<<USBS0); //2 stop bits
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); //8 bits for data
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); //this enables reader and writer
	
}
void UART_send(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
void UART_putstring(char* StringToUse){
	while(*StringToUse != 0b00000000){
		UART_send(*StringToUse);
		StringToUse++;
	}
}
int getPlayer1Score(){
	return player1Score;
}

int getPlayer2Score(){
	return player2Score; 
	};
void setPlayer1Score(int score){
	player1Score = score; 
}
void setPlayer2Score(int score){
	player2Score = score;
}

/*
int main(void){
	reset();
	board[0][0] = 1; //not letting me access the board from the other file 
	board[0][0] = 1;
	board[0][0] = 1;
	board[0][0] = 1;
	
} */ 

/*
int main(void){
	UART_init();
	reset(); 

	setCell(4,0,1); 
	setCell(4,1,2); 
	setCell(4,2,2); 
	setCell(4,3,2); 
	setCell(4,4,1);
	setCell(3,1,1);
	setCell(3,2,1);
	setCell(3,3,1);
	setCell(3,4,2);
	setCell(2,1,2);
	setCell(2,2,2);
	setCell(2,3,2);
	setCell(2,4,1);
	setCell(1,2,2);
	setCell(1,3,1);
	setCell(1,4,2);
	setCell(0,2,1);
	setCell(0,3,2);
	setCell(0,4,2);
	 

	setCell(4,0,1);
	setCell(4,1,1);
	setCell(4,2,1);
	setCell(4,3,2);
	setCell(4,4,2);


	setCell(4,0,2);
	setCell(4,1,2);
	setCell(4,2,2);
	setCell(4,3,1);
	setCell(4,4,1);
	setCell(3,3,2);
	setCell(3,4,2);
	setCell(2,3,1);
	
	int win = checkWinner();
	sprintf(String, " Win: %u ", win);
	UART_putstring(String);
}
 */ 

/*
int main(void){
	UART_init();
	Initialize();
	reset(); 
	lcd_init();
	LCD_drawBlock(0,0,159,127,0);
	LCD_drawCircle(20,20,5,65535);
//	LCD_drawCircle(40,40,10,0);
	
}
*/ 

void drawGrid(){
	for(int row = 0; row < 5; row++){
		for(int col = 0; col < 5; col++){
			LCD_drawSquare(20*col + 20,20*row + 25,10,65535);
		}
	}
}
void drawScore(){
	char a = player1Score + '0';
	char b = player2Score + '0';
	LCD_drawString(35,120, "Round Score: ",65535,0);
	LCD_drawChar(95,120,a,65535,0);
	LCD_drawString(102,120, "-",65535,0);
	LCD_drawChar(109,120,b,65535,0);
}

int main(void)
{
	UART_init();
	sprintf(String, "Hello \n");
	UART_putstring(String);
	Initialize();
	lcd_init();
	reset(); 
	LCD_drawBlock(0,0,159,200,0);
	drawGrid(); 
	drawScore(); 
	//LCD_drawCircle(20,20,5,65535);
	sprintf(String, "Player 1's Turn! \n");
	UART_putstring(String);
//	LCD_setScreen(0); 
	while (1)
	{
	//	if(PINC & (1<<PINC0)){
	//		sprintf(String, "Bonus Distance!");
	//		UART_putstring(String);
	//	}
		
		 if(!(PIND & (1<<PIND7))){
			 _delay_ms(400);
			 if(playTurn(0)==1){
				 int curr = getCurrentPlayer(); 
				 
				 sprintf(String, "Ball scored in column 1 ");
				 UART_putstring(String);
				// int gameOver = getGameOver(); 
				
				 
				 sprintf(String, " Current Player: %u ", curr);
				 UART_putstring(String);
				 // sprintf(String, " Game Over: %u ", gameOver);
				 // UART_putstring(String);
				 
				 ballCount++;
				// LCD_setScreen(0); 
				 drawBoard(); 
				 int win = checkWinner();
				if(win==1){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner!",65535,0);
					LCD_drawString(120,65, "Red",65535,0);
					player1Score++;
					//reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore(); 
				}
				else if(win==2){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner",65535,0);
					LCD_drawString(120,65, "Yellow",65535,0);
					player2Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==0){
					if(PINC & (1<<PINC0)){
						sprintf(String, "Bonus Shot! \n");
						UART_putstring(String);
						if(curr == 0){
							setCurrentPlayer(1);
						}
						else if(curr == 1){
							setCurrentPlayer(0);
						}
					}
				}

			//	LCD_drawCircle(10,10,5,65535);
			 }
			 else{
				 sprintf(String, "Unsuccessful move \n");
				 UART_putstring(String);
			 } 
			
		}
		
		else if(!(PIND & (1<<PIND2))){
			_delay_ms(400);
			if(playTurn(1)==1){
				sprintf(String, "Ball scored in column 2 \n");
				UART_putstring(String);
				int curr = getCurrentPlayer();
				sprintf(String, " Current Player: %u ", curr);
				UART_putstring(String);
			
				//LCD_setScreen(0); 
				drawBoard(); 
				//LCD_drawCircle(20,20,5,65535);
				ballCount++;
				int win = checkWinner();
				if(win==1){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner!",65535,0);
					LCD_drawString(120,65, "Red",65535,0);
					player1Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==2){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner",65535,0);
					LCD_drawString(120,65, "Yellow",65535,0);
					player2Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==0){
					if(PINC & (1<<PINC0)){
						sprintf(String, "Bonus Shot! \n");
						UART_putstring(String);
						if(curr == 0){
							setCurrentPlayer(1);
						}
						else if(curr == 1){
							setCurrentPlayer(0);
						}
					}
				}

			}
			else{
				sprintf(String, "Unsuccessful move \n");
				UART_putstring(String);
			}
		}
		 
		else if(!(PIND & (1<<PIND3))){
			_delay_ms(400);
			if(playTurn(2)==1){
				int curr = getCurrentPlayer(); 
				sprintf(String, "Ball scored in column 3 \n");
				UART_putstring(String);
				ballCount++;
				//LCD_setScreen(0); 
				drawBoard(); 
				//LCD_drawCircle(30,30,5,65535);
				int win = checkWinner();
				if(win==1){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner!",65535,0);
					LCD_drawString(120,65, "Red",65535,0);
					player1Score++;
					//reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==2){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner",65535,0);
					LCD_drawString(120,65, "Yellow",65535,0);
					player2Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==0){
					if(PINC & (1<<PINC0)){
						sprintf(String, "Bonus Shot! \n");
						UART_putstring(String);
						if(curr == 0){
							setCurrentPlayer(1);
						}
						else if(curr == 1){
							setCurrentPlayer(0);
						}
					}
				}
			}
			else{
				sprintf(String, "Unsuccessful move \n");
				UART_putstring(String);
			}
		}
		else if(!(PIND & (1<<PIND4))){
			_delay_ms(400);
			if(playTurn(3)==1){
				int curr = getCurrentPlayer(); 
				sprintf(String, "Ball scored in column 4 \n");
				UART_putstring(String);
				ballCount++;
				//LCD_setScreen(0); 
				drawBoard(); 
				int win = checkWinner();
				if(win==1){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner!",65535,0);
					LCD_drawString(120,65, "Red",65535,0);
					player1Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==2){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner",65535,0);
					LCD_drawString(120,65, "Yellow",65535,0);
					player2Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==0){
					if(PINC & (1<<PINC0)){
						sprintf(String, "Bonus Shot! \n");
						UART_putstring(String);
						if(curr == 0){
							setCurrentPlayer(1);
						}
						else if(curr == 1){
							setCurrentPlayer(0);
						}
					}
				}

				//LCD_drawCircle(40,40,5,65535);
			}
			else{
				sprintf(String, "Unsuccessful move \n");
				UART_putstring(String);
			}
		}
		else if(!(PIND & (1<<PIND5))){
			_delay_ms(400);
			if(playTurn(4)==1){
				int curr = getCurrentPlayer(); 
				sprintf(String, "Ball scored in column 5 \n");
				UART_putstring(String);
				ballCount++;
				//LCD_setScreen(0); 
				drawBoard(); 
				int win = checkWinner();
				if(win==1){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner!",65535,0);
					LCD_drawString(120,65, "Red",65535,0);
					player1Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==2){
					sprintf(String, " Win: %u ", win);
					UART_putstring(String);
					LCD_drawString(120,50, "Winner",65535,0);
					LCD_drawString(120,65, "Yellow",65535,0);
					player2Score++;
				//	reset();
				//	LCD_drawBlock(0,0,159,200,0);
				//	drawGrid();
				//	drawScore();
				}
				else if(win==0){
					if(PINC & (1<<PINC0)){
						sprintf(String, "Bonus Shot! \n");
						UART_putstring(String);
						if(curr == 0){
							setCurrentPlayer(1);
						}
						else if(curr == 1){
							setCurrentPlayer(0);
						}
					}
				}

				//LCD_drawCircle(50,50,5,65535);
			}
			else{
				sprintf(String, "Unsuccessful move \n");
				UART_putstring(String);
				
			}
		}
		
//		if(ballCount == 2){
//			ballCount = 0; 
//			if(playerTurn == 1){
//				playerTurn = 2;
//				sprintf(String, "Player 2's Turn! \n");
//				UART_putstring(String);
//			}
//			else if(playerTurn == 2){
//				playerTurn = 1; 
//				sprintf(String, "Player 1's Turn! \n");
//				UART_putstring(String);
//			}
//		}
		
		
		
	//	sprintf(String, "Distance in cm: %u \n", cm);
	//	UART_putstring(String);
	
	if(checkWinner() > 0){
		reset();
		setGameOver(0); 
		LCD_drawBlock(0,0,159,200,0);
		
		drawGrid();
		drawScore();
	}
	}
	
}
 
 
ISR(TIMER1_CAPT_vect){
	
	if (isRising){
		pulseStart = ICR1;
		isRising = 0;
		TCCR1B &= ~(1<<ICES1); //look for falling edge now
		overflowCount = 0;
	}
	else if (isRising == 0){
		pulseEnd = ICR1;
		isRising = 1;
		TCCR1B |= (1<<ICES1);
		tickCount = pulseEnd - pulseStart;
	    cm = ((tickCount + 65535*overflowCount) * 4)/ 58;
//		sprintf(String, "Distance in cm: %u \n", cm);
//		UART_putstring(String);
		overflowCount = 0;
	}
	
}
ISR(TIMER1_OVF_vect){

	overflowCount++;
	PORTD |= (1<<PORTC0);
	_delay_us(20);
	PORTD &= ~(1<<PORTC0);

}