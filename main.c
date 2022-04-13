#include <avr/io.h>
#define F_CPU 16000000UL


#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)


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
char String[25];

void Initialize(){
	cli();
	//input pins for IR sensors 
	DDRB &= ~(1<<DDB1);
	DDRB &= ~(1<<DDB2);
	DDRB &= ~(1<<DDB3);
	DDRB &= ~(1<<DDB4);
	DDRB &= ~(1<<DDB5);
	//setting them all high initially (comment out later) 
	PORTB |= (1<<PORTB1);
	PORTB |= (1<<PORTB2);
	PORTB |= (1<<PORTB3);
	PORTB |= (1<<PORTB4);
	PORTB |= (1<<PORTB5);
	
	
	
	DDRD |= (1<<DDD6); //set PD6 as output pin for trigger signal
	DDRB &= ~(1<<DDB0); //set PB0 as input pin for echo signal
	
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

/*
int main(void){
	reset();
	board[0][0] = 1; //not letting me access the board from the other file 
	board[0][0] = 1;
	board[0][0] = 1;
	board[0][0] = 1;
	
} */ 

int main(void)
{
	UART_init();
	sprintf(String, "Hello \n");
	UART_putstring(String);
	Initialize();
	sprintf(String, "Player 1's Turn! \n");
	UART_putstring(String);
	while (1)
	{
		
		
		 if(!(PINB & (1<<PINB1))){
			 _delay_ms(400);
			 ballCount++;
			sprintf(String, "Ball scored in column 1 \n");
			UART_putstring(String);
		}
		
		else if(!(PINB & (1<<PINB2))){
			_delay_ms(400);
			ballCount++;
			sprintf(String, "Ball scored in column 2 \n");
			UART_putstring(String);
		}
		else if(!(PINB & (1<<PINB3))){
			_delay_ms(400);
			ballCount++;
			sprintf(String, "Ball scored in column 3 \n");
			UART_putstring(String);
		}
		else if(!(PINB & (1<<PINB4))){
			_delay_ms(400);
			ballCount++;
			sprintf(String, "Ball scored in column 4 \n");
			UART_putstring(String);
		}
		else if(!(PINB & (1<<PINB5))){
			_delay_ms(400);
			ballCount++;
			sprintf(String, "Ball scored in column 5 \n");
			UART_putstring(String);
		}
		
		if(ballCount == 2){
			ballCount = 0; 
			if(playerTurn == 1){
				playerTurn = 2;
				sprintf(String, "Player 2's Turn! \n");
				UART_putstring(String);
			}
			else if(playerTurn == 2){
				playerTurn = 1; 
				sprintf(String, "Player 1's Turn! \n");
				UART_putstring(String);
			}
		}
	//	sprintf(String, "Distance in cm: %u \n", cm);
	//	UART_putstring(String);
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
	PORTD |= (1<<PORTD6);
	_delay_us(20);
	PORTD &= ~(1<<PORTD6);

}
 
