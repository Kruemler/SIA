/*
 *  Interrupt and PWM utilities for 8 bit Timer3 on ATmega256
 *  
 */

#include <arduino.h>
#include "motor.h"


#define  M1		0
#define  M2		1

TimerThree motor1,motor2;			 // preinstatiate

/*************************************************************************************************

Timer3
 
*************************************************************************************************/
void motor(unsigned char idx, unsigned char dir, unsigned char duty)
{
	switch(idx)
	{
		case M1:				
				DDRK |= (1 << PK6);  	//DIR
				DDRG |= (1 << PG5);		//PWM Out
				pinMode(14, OUTPUT);  	
				motor1.setDir(M1, dir);
				motor1.initialize(M1,1000000);         
				motor1.pwm(M1, duty);                      
			break;
			
		case M2:
				DDRK |= (1 << PK7);  	//Dir		
				DDRE |= (1 << PE3);		//PWM Out
				pinMode(15, OUTPUT); 
				motor2.setDir(M2, dir);
				motor2.initialize(M2,1000000);         
				motor2.pwm(M2, duty);                      

			break;
			
		default:		
			break;
	}  	
}	

void TimerThree::setDir(unsigned char idx, unsigned char dir)
{		

		if(idx == M1)
		{
			if(dir == 1) digitalWrite(14, 1);
//				PORTK |= ( 1 << PK6 ); 
			else if(dir == 0) digitalWrite(14, 0);
//				PORTK &= ~( 1 << PK6 ); 
		}
		else if(idx == M2)
		{
			if(dir == 1) digitalWrite(15, 1);
				//PORTK |= ( 1 << PK7 ); 
			else if(dir == 0) digitalWrite(15, 0);
				//PORTK &= ~( 1 << PK7 ); 		
		}			
}
 
void TimerThree::initialize(unsigned char idx, long microseconds)
{
	switch(idx)
	{
		case M1:
				TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
				// set Timer-0 Register
				TCCR0A = (1 << WGM00);      // Reset TCCR0A Register 
				TCCR0B = 0;      			// Reset TCCR0B Register
			  
				TCCR0A  |= (1 << COM0B1);

				motor1.setPeriod(microseconds);
			break;
			
		case M2:		
				TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
				// set Timer-3 Register
				TCCR3A = (1 << WGM30);      // Reset TCCR3A Register 
				TCCR3B = 0;      			// Reset TCCR3B Register
				  
				TCCR3A  |= (1 << COM3A1);
		
				motor2.setPeriod(microseconds);
			break;
			
		default:		
			break;
	}  
}

void TimerThree::setPeriod(long microseconds)
{  
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum

  clockSelectBits = _BV(CS12) | _BV(CS10);   //Fest               
  pwmPeriod = cycles;                                                     
}


void TimerThree::setPwmDuty(unsigned char idx, int duty)
{
	unsigned long dutyCycle = pwmPeriod;
    
	if(idx == M1)
		OCR0B = (255 * duty )/100;	
	else if(idx == M2)
		OCR3A = (255 * duty )/100;
}

void TimerThree::pwm(unsigned char idx, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
	if(microseconds > 0) setPeriod(microseconds);
  
	// sets data direction register for pwm output pin
	// activates the output pin
	if(idx == M1) { DDRG |= _BV(PORTG5); TCCR0A |= _BV(COM0B1); }
	if(idx == M2) { DDRE |= _BV(PORTE3); TCCR3A |= _BV(COM3A1); }
  
	switch(idx)
	{
		case M1:
					motor1.setPwmDuty(idx, duty);
			break;
			
		case M2:
					motor2.setPwmDuty(idx, duty);
			break;
			
		default:		
			break;
	}  
  
  start(idx);
}

void TimerThree::disablePwm(unsigned char idx)
{
	if(idx == M1) { TCCR0A &= ~_BV(COM0B1); }
	if(idx == M2) { TCCR3A &= ~_BV(COM3A1); }
}

void TimerThree::attachInterrupt(unsigned char idx, void (*isr)(), long microseconds)
{
	if(microseconds > 0) setPeriod(microseconds);
	
	switch(idx)
	{
		case M1:
					motor1.isrCallback = isr;   
			break;
			
		case M2:
					motor2.isrCallback = isr;   
			break;
			
		default:		
			break;
	}  
								// register the user's callback with the real ISR
								// sets the timer overflow interrupt enable bit
  sei();                    	// ensures that interrupts are globally enabled
  start(idx);
}

void TimerThree::detachInterrupt(unsigned char idx)
{
	if(idx == M1)
		TIMSK0 &= ~_BV(OCIE0A);                                   // clears the timer overflow interrupt enable bit 
	else
		TIMSK3 &= ~_BV(OCIE3A);
}

void TimerThree::start(unsigned char idx)
{
	if(idx == M1)
		TCCR0B |= (1 << CS12)|(1 << CS10);
	else if(idx == M2)
		TCCR3B |= (1 << CS12)|(1 << CS10);
	
}

void TimerThree::stop(unsigned char idx)
{
	if(idx == M1)
		TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));          // clears all clock selects bits
	else if(idx == M2)
		TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));          // clears all clock selects bits
}

void TimerThree::restart(unsigned char idx)
{
	if(idx == M1)
		TCNT0 = 0;
	else
		TCNT3 = 0;		
}
