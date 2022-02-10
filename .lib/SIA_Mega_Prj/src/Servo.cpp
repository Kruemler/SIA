/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


 
#if defined(ARDUINO_ARCH_AVR)

#include <avr/interrupt.h>
#include <Arduino.h>

#include "Servo.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 2009

//#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

//#define PWM_TOP_T4	 16000	
#define PWM_TOP      40000
#define SERVO_REST   3000
//#define SERVO_REST   0

#define	OC1APIN		11	
#define	OC1BPIN		12	
#define	OC1CPIN		13
#define	OC3APIN		5
#define	OC3BPIN		2
#define	OC3CPIN		3
#define	OC4APIN		6
#define	OC4BPIN		7

//#define	OC5CPIN		44
//#define	OC5BPIN		45

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

uint8_t ServoCount = 0;                                     // the total number of attached servos


// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/

static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
{
  if( Channel[timer] < 0 )
    *TCNTn = 0; // channel set to -1 indicated that refresh interval completed so reset the timer
  else{
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true )
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated
  }

  Channel[timer]++;    // increment to the next channel
  if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    *OCRnA = *TCNTn + SERVO(timer,Channel[timer]).ticks;
    if(SERVO(timer,Channel[timer]).Pin.isActive == true)     // check if activated
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    if( ((unsigned)*TCNTn) + 4 < usToTicks(REFRESH_INTERVAL) )  // allow a few ticks to ensure the next OCR1A not missed
      *OCRnA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      *OCRnA = *TCNTn + 4;  // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
  }
}

#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino
#if defined(_useTimer1)
SIGNAL (TIMER1_COMPA_vect)
{
 // handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif

#if defined(_useTimer3)
SIGNAL (TIMER3_COMPA_vect)
{
 // handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif

#if defined(_useTimer4)
SIGNAL (TIMER4_COMPA_vect)
{
  handle_interrupts(_timer4, &TCNT4, &OCR4A);
}
#endif

#if defined(_useTimer5)
SIGNAL (TIMER5_COMPA_vect)
{
 // handle_interrupts(_timer5, &TCNT5, &OCR5A);
}
#endif

#elif defined WIRING
// Interrupt handlers for Wiring
#if defined(_useTimer1)
void Timer1Service()
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif
#if defined(_useTimer3)
void Timer3Service()
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif
#endif

static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
  if(timer == _timer1) {
    TCCR1A = 0;             // normal counting mode
    TCCR1B = _BV(CS11);     // set prescaler of 8
    TCNT1 = 0;              // clear the timer count
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF1A);      // clear any pending interrupts;
    TIMSK |=  _BV(OCIE1A) ;  // enable the output compare interrupt
#else
    // here if not ATmega8 or ATmega128
    TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
    TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER1OUTCOMPAREA_INT, Timer1Service);
#endif
  }
#endif

#if defined (_useTimer3)
  if(timer == _timer3) {  
    TCCR3A = 0;             // normal counting mode
    TCCR3B = _BV(CS31);     // set prescaler of 8
    TCNT3 = 0;              // clear the timer count
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);     // clear any pending interrupts;
	ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt
#else
    TIFR3 = _BV(OCF3A);     // clear any pending interrupts;
    TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);  // for Wiring platform only
#endif
  }
#endif

#if defined (_useTimer4)
  if(timer == _timer4) {
    TCCR4A = 0;             // normal counting mode
    TCCR4B = _BV(CS41);     // set prescaler of 8
    TCNT4 = 0;              // clear the timer count
    TIFR4 = _BV(OCF4A);     // clear any pending interrupts;
    TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
  }
#endif

#if defined (_useTimer5)
  if(timer == _timer5) {	
    TCCR5A = 0;             // normal counting mode
    TCCR5B = _BV(CS51);     // set prescaler of 8
    TCNT5 = 0;              // clear the timer count
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts;
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
  }
#endif
}

static void finISR(timer16_Sequence_t timer)
{
    //disable use of the given timer
#if defined WIRING   // Wiring
  if(timer == _timer1) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK1 &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #else
    TIMSK &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #endif
    timerDetach(TIMER1OUTCOMPAREA_INT);
  }
  else if(timer == _timer3) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #else
    ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else
    //For arduino - in future: call here to a currently undefined function to reset the timer
#endif
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}


/****************** end of static functions ******************************/

Servo::Servo()
{  
  if( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
	servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}


uint8_t Servo::attach(int pin, int min, int max)
{	
  if(this->servoIndex < MAX_SERVOS ) {
  
    pinMode( pin, OUTPUT);                                   // set servo pin to output	
	digitalWrite( pin, LOW); 
    servos[this->servoIndex].Pin.nbr = pin;
	
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;

	switch(pin)
	{
		case OC1APIN:
					TCCR1A &= ~(1<<WGM10);  
					TCCR1A |= (1 << COM1A1);  		// Clear OC1A pin on each timer
					OCR1A  = SERVO_REST;  			// => 1,5ms
					DDRB |= (1 << 5);				//OC1A pin as outputs
					break;
		case OC1BPIN:
					TCCR1A &= ~(1<<WGM10);  		
					TCCR1A |= (1 << COM1B1);		// Clear OC1B pin on each timer
					OCR1B  = SERVO_REST;  			// => 1,5ms					
					DDRB |= (1 << 6);				//OC1B pin as outputs
					break;
		case OC1CPIN:
					TCCR1A &= ~(1<<WGM10);  		
					TCCR1A |= (1 << COM1C1);		// Clear OC1C pin on each timer	
					OCR1C  = SERVO_REST;  			// => 1,5ms					
					DDRB |= (1 << 7);				//OC1C pin as outputs
					break;				
		case OC3APIN:
					TCCR3A &= ~(1<<WGM30);  		
					TCCR3A |= (1 << COM3A1);		// Clear OC3A pin on each timer		
					OCR3A  = SERVO_REST;  			// => 1,5ms					
					DDRE |= (1 << 3);				// OC3A pin as outputs					
					break;
		case OC3BPIN:
					TCCR3A &= ~(1<<WGM30);  				
					TCCR3A |= (1 << COM3B1);		// Clear OC3B pin on each timer				
					OCR3B  = SERVO_REST;  			// => 1,5ms					
					DDRE |= (1 << 4);				// OC3B pin as outputs										
					break;
		case OC3CPIN:
					TCCR3A &= ~(1<<WGM30);  				
					TCCR3A |= (1 << COM3C1);		// Clear OC3C pin on each timer			
					OCR3C  = SERVO_REST;  			// => 1,5ms					
					DDRE |= (1 << 5);				// OC3C pin as outputs										
					break;					
		case OC4APIN:		
					TCCR4A &= ~(1<<WGM40);        // clear WGM50 and WGM51 at control register A 
					TCCR4B = _BV(WGM43);       		 // set mode as phase and frequency correct pwm, stop the timer		
					TCCR4A |= (1 << COM4A1);		// Clear OC5B pin on each timer			
					OCR4A  = SERVO_REST;  			// => 1,5ms					
					DDRL |= (1 << 5);				// OC3C pin as outputs										
					
					break;					

		case OC4BPIN:		
					TCCR4A &= ~(1<<WGM40);      // clear WGM50 and WGM51 at control register A 
					TCCR4B = _BV(WGM43);       		 // set mode as phase and frequency correct pwm, stop the timer		
					TCCR4A |= (1 << COM4B1);		// Clear OC5C pin on each timer			
					OCR4B  = SERVO_REST;  			// => 1,5ms										
					DDRL |= (1 << 4);				// OC3C pin as outputs										
					break;					
					
	}
	
	
	if( (pin == OC4APIN) || (pin == OC4BPIN) )
	{		
		// PWM mode 14 (fast PWM, TOP=ICR1)
		TCCR4A |= (1 << WGM51);
		TCCR4B |= ((1 << WGM43) | (1 << WGM42));

		// Start timers (prescale=8)
		TCCR4B &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		TCCR4B |= (0 << CS42) | (1 << CS41) | (0 << CS40);
		// Start timers ( No prescale)
		//TCCR4B |= (0 << CS42) | (0 << CS41) | (1 << CS40);

		// Set register values
		ICR4   = PWM_TOP  ;     // => 50Hz (T=20ms)	   
		//ICR4   = 16000  ;
	}
	
	if( (pin == OC1APIN) || (pin == OC1BPIN) || (pin == OC1CPIN) )
	{
		// PWM mode 14 (fast PWM, TOP=ICR1)
				
		TCCR1A |= (1 << WGM11);
		TCCR1B |= ((1 << WGM13) | (1 << WGM12));
		
	   // Start timers (prescale=8)
	   TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));	   
	   TCCR1B |= (1 << CS11);

	   // Set register values
	   ICR1   = PWM_TOP;     // => 50Hz (T=20ms)
		
	}

	if( (pin == OC3APIN) || (pin == OC3BPIN) || (pin == OC3CPIN) )
	{
		// PWM mode 14 (fast PWM, TOP=ICR3)
		TCCR3A |= (1 << WGM31);
		TCCR3B |= ((1 << WGM33) | (1 << WGM32));		

	   // Start timers (prescale=8)
	   TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));	 	   
	   TCCR3B |= (1 << CS11);		
	   
	   // Set register values
	   ICR3   = PWM_TOP;     // => 50Hz (T=20ms)	   
	}	
	  
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
    
  return this->servoIndex ;
}

void Servo::detach()
{
int pin;

	pin = servos[this->servoIndex].Pin.nbr;
	switch(pin)
	{
		case OC1APIN:
					TCCR1A &= ~((1 << COM1A1)|(1 << COM1A0));  		//Normal port operation, OC1A disconnected
					OCR1A  = 0;  								
					DDRB &= ~(1 << 5);								//OC1A pin as inputs
					break;
		case OC1BPIN:
					TCCR1A &= ~((1 << COM1B1)|(1 << COM1B0));  		//Normal port operation, OC1B disconnected
					OCR1B  = 0;  			
					DDRB &= ~(1 << 6);								//OC1B pin as inputs
					break;
		case OC1CPIN:
					TCCR1A &= ~((1 << COM1C1)|(1 << COM1C0));  		//Normal port operation, OC1C disconnected
					OCR1C  = 0; 
					DDRB &= ~(1 << 7);								//OC1C pin as inputs
					break;				
		case OC3APIN:
					TCCR3A &= ~((1 << COM3A1)|(1 << COM3A0));  		//Normal port operation, OC3A disconnected		
					OCR3A  = 0; 
					DDRE &= ~(1 << 3);								// OC3A pin as inputs					
					break;
		case OC3BPIN:
					TCCR3A &= ~((1 << COM3B1)|(1 << COM3B0));  		//Normal port operation, OC3B disconnected		
					OCR3B  = 0;  			
					DDRE &= ~(1 << 4);								// OC3B pin as inputs										
					break;
		case OC3CPIN:
					TCCR3A &= ~((1 << COM3C1)|(1 << COM3C0));  		//Normal port operation, OC3C disconnected		
					OCR3C  = 0;  			
					DDRE &= ~(1 << 5);								// OC3C pin as inputs										
					break;					

		case OC4APIN:
					TCCR4A &= ~((1 << COM4A1)|(1 << COM4A0));  		//Normal port operation, OC4A disconnected		
					OCR4A  = 0;  			
					DDRH &= ~(1 << 3);								// OC4A pin as inputs										
					break;					
					
		case OC4BPIN:
					TCCR4A &= ~((1 << COM3B1)|(1 << COM4B0));  		//Normal port operation, OC4B disconnected		
					OCR4B  = 0;  			
					DDRH &= ~(1 << 4);								// OC4B pin as inputs										
					break;
					
	}
	
}

#define SERVO_MIN_POS	-90
#define SERVO_MAX_POS	90


void Servo::write(uint8_t pos)
{  
uint32_t value;

  
   if(pos > 180) 
     pos = 180;

  value = map(pos, 0, 180, SERVO_MIN(),  SERVO_MAX());
     
   this->writeMicroseconds(value);
}
void Servo::writeAngle (int pos)
{  
int value;

   if(pos < -90) 
     pos = -90;
	 
   if(pos > 90) 
     pos = 90;
	 
   value = map(pos,SERVO_MIN_POS,SERVO_MAX_POS,0,180);
   
   value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());

  
   this->writeMicroseconds(value);
}

void Servo::setSpeed(int pos)
{  
int value;

   if(pos < -90) 
     pos = -90;
	 
   if(pos > 90) 
     pos = 90;
	 
   value = map(pos,SERVO_MIN_POS,SERVO_MAX_POS,0,180);
   
   value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());

  
   this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
int pin;
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

	pin = servos[this->servoIndex].Pin.nbr;
	switch(pin)
	{
		case OC1APIN:
					OCR1A = value;
					break;
		case OC1BPIN:
					OCR1B = value;		
					break;
		case OC1CPIN:
					OCR1C = value;
					break;				
		case OC3APIN:
					OCR3A = value;
					break;
		case OC3BPIN:
					OCR3B = value;
					break;
		case OC3CPIN:
					OCR3C = value;
					break;					
		case OC4APIN:
					OCR4A = value;
					break;
		case OC4BPIN:
					OCR4B = value ;
					break;					
					
	}
	
    uint8_t oldSREG = SREG;
    cli();	
    servos[channel].ticks = value;
    SREG = oldSREG;
  }
}

int Servo::read() // return the value as degrees
{
  return  map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive ;
}

#endif // ARDUINO_ARCH_AVR

