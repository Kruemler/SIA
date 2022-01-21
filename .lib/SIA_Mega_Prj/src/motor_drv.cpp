/*
 *  Interrupt and PWM utilities for 8 bit Timer3 on ATmega256
 *  
 */

#include <arduino.h>
#include "motor_drv.h"

static motor_t motor[MAX_MOTOR]; 

#if 0
ISR(TIMER2_OVF_vect)
{
  if(TIMSK2 & (1<<OCIE2B))
  {
    DDRH |= (1<<PH6);
    PORTH |= (1<<PH6);
    OCR2B = motor[MOTOR1].Speed;
  }

  if(TIMSK2 & (1<<OCIE2A))
  {
    DDRB |= (1<<PB4);
    PORTB |= (1<<PB4);
    OCR2A = motor[MOTOR2].Speed;
  }

}

ISR(TIMER5_OVF_vect)
{
digitalWrite(13, !digitalRead(13)); // toggel the LED  

  if(TIMSK5 & (1<<OCIE5B))
  {
	DDRL |= (1<<PL4);
	PORTL &= ~(1<<PL4);
    OCR5B = motor[MOTOR4].Speed;
  }

  if(TIMSK5 & (1<<OCIE5C))
  {
	DDRL |= (1<<PL5);
	PORTL &= ~(1<<PL5);
    OCR5C = motor[MOTOR3].Speed;
  }

}

ISR(TIMER2_COMPA_vect)
{
  DDRB |= (1<<PB4);
  PORTB &= ~(1<<PB4);
}

ISR(TIMER2_COMPB_vect)
{  
   DDRH |= (1<<PH6);
   PORTH &= ~(1<<PH6);
}

ISR(TIMER5_COMPB_vect)
{		
  DDRL |= (1<<PL4);
  PORTL &= ~(1<<PL4);
}

ISR(TIMER5_COMPC_vect)
{ 
   DDRL |= (1<<PL5);
   PORTL &= ~(1<<PL5);
}
#endif

void motorDriverGo(	uint8_t motorID, 
					uint8_t m1Dir, int8_t m1Speed,
					uint8_t m2Dir, int8_t m2Speed,
					uint8_t m3Dir, int8_t m3Speed,					
					uint8_t m4Dir, int8_t m4Speed
				)
{
  //Motor 1	D9   \ PH6 ( OC2B )	D15  \ PJ0 ( RXD3/PCINT9 )
   if(motorID & MOTOR_ID_1)
   {
		DDRH |= (1<<PH6);
		DDRJ |= (1<<PJ0);
		
		if(m1Speed == 0)  // motor stop
		{
			TCCR2A &=  ~(1<<COM2B1) | (1<<COM2B0);
			PORTH &= ~(1<<PH6);
			PORTJ &= ~(1<<PJ0);        		  			
			motor[MOTOR1].isActive = false;           //Stop
			//	return;
		}
		else
		{
			motor[MOTOR1].isActive = true;               //Start
			
			if(m1Dir)
			{
				motor[MOTOR1].Speed = (m1Speed * 255) / 100;				
				motor[MOTOR1].Dir =	FORWARD;			
				PORTJ |= (1<<PJ0);
			}
			else
			{
				motor[MOTOR1].Dir = BACKWARD;
				motor[MOTOR1].Speed = 255 - ((m1Speed * 255) / 100);
				PORTJ &= ~(1<<PJ0);
			}			
			// initialize TCCR2 as: fast pwm mode, non inverting
			TCCR2A |=  (1<<COM2B1) | (1<<COM2B0) | (1<<WGM21) | (1<<WGM20);
			TCCR2B = (0 << CS22) | (1 << CS21) | (0 << CS20)  ;
			OCR2B = motor[MOTOR1].Speed;			  
		}	   
	}
   //Motor 2	D10 \ PB4 ( OC2A/PCINT4 )	D14 \ PJ1 ( TXD3/PCINT10 )
   if(motorID & MOTOR_ID_2)
	{
		DDRB |= (1<<PB4);
		DDRJ |= (1<<PJ1);
       
		if(m2Speed == 0)  // motor stop
		{
			TCCR2A &=  ~(1<<COM2A1) | (1<<COM2A0);		  
			PORTB &= ~(1<<PH4);
			PORTJ &= ~(1<<PJ1);        		  			
			motor[MOTOR2].isActive = false;           //Stop
		}
		else
		{
			motor[MOTOR2].isActive = true;               //Start
			if(m2Dir)
			{
				motor[MOTOR2].Dir =	FORWARD;
				motor[MOTOR2].Speed = ((m2Speed * 255) / 100);
				PORTJ |= (1<<PJ1);
			}
			else
			{
				motor[MOTOR2].Dir = BACKWARD;
				motor[MOTOR2].Speed = 255 - ((m2Speed * 255) / 100);
				PORTJ &= ~(1<<PJ1);
			}
		   
   			TCCR2A |=  (1<<COM2A1) | (1<<COM2A0) | (1<<WGM21) | (1<<WGM20);
			TCCR2B = (0 << CS22) | (1 << CS21) | (0 << CS20)  ;
			OCR2A = motor[MOTOR2].Speed;			  
		}	   
   }
	//Motor 3	D44 \ PL5 ( OC5C )	A13 \ PK5 ( ADC13/PCINT21 )
	if(motorID & MOTOR_ID_3)
   {      
		DDRL |= (1<<PL4);
        DDRK |= (1<<PK6);

       if(m3Speed == 0)  // motor stop
       {		
			TCCR5A &= ~((1<<COM5B1) | (0<<COM5B0));
			PORTK &= ~(1<<PK6);        		  			
			motor[MOTOR3].isActive = false;           //Stop
       }
       else
	   {
			motor[MOTOR3].isActive = true;               //Start

			if(m3Dir)
			{
				motor[MOTOR3].Dir =	FORWARD;
				//motor[MOTOR3].Speed = 255 - ((m3Speed * 255) / 100);
				motor[MOTOR3].Speed = ((m3Speed * 255) / 100);
				PORTK |= (1<<PK6);
			}
			else
			{
				motor[MOTOR3].Dir = BACKWARD;
				//motor[MOTOR3].Speed = ((m3Speed * 255) / 100);
				motor[MOTOR3].Speed = 255 - ((m3Speed * 255) / 100);
				PORTK &= ~(1<<PK6);
			}	   
			
			Serial.println(motor[MOTOR3].Speed);

			TCCR5A &= ~(1<<WGM50);
   			TCCR5A |= (1<<COM5B1) | (0<<COM5B0) | (1<<WGM51); // Port PINL3 als Fast PWM max. 16Bit (clear on compare),       
			TCCR5B = (1<<WGM53)  | (1<<WGM52)  | (0<< CS52) | (1<<CS51) | (0<<CS50); // Fast PWM, Prescaller = 1

			ICR5   = 0xFF;     // End Counter 255
			OCR5B = motor[MOTOR3].Speed;	
		}   
   }
   
	//Motor 4	D45 \ PL4 ( OC5B )	A14 \ PK6 ( ADC14/PCINT22 ) 
   if(motorID & MOTOR_ID_4)
   {
       DDRL |= (1<<PL5);
       DDRK |= (1<<PK5);
        
       if(m4Speed == 0)  // motor stop
       {
			TCCR5A &= ~((1<<COM5C1) | (0<<COM5C0));
			PORTL &= ~(1<<PL5);
			PORTK &= ~(1<<PK5);        		  
			motor[MOTOR4].isActive = false;           //Stop		
       }
       else
	   {
			motor[MOTOR4].isActive = true;               //Start
			if(m4Dir)
			{
				motor[MOTOR4].Dir =	FORWARD;
				motor[MOTOR4].Speed = 255 - ((m4Speed * 255) / 100);
				PORTK |= (1<<PK5);
			}
			else
			{
				motor[MOTOR4].Dir = BACKWARD;
				motor[MOTOR4].Speed = ((m4Speed * 255) / 100);
				PORTK &= ~(1<<PK5);
			}
			
			TCCR5A &= ~(1<<WGM50);
			TCCR5A |= (1<<COM5C1) | (0<<COM5C0) | (1<<WGM51); // Port PINL3 als Fast PWM max. 16Bit (clear on compare),       		
			TCCR5B = (1<<WGM53)  | (1<<WGM52)  | (0<< CS52) | (1<<CS51) | (0<<CS50); // Fast PWM, Prescaller = 1			
			ICR5   = 255;     // End Counter 255			
			OCR5C = motor[MOTOR4].Speed;	
		}	   
   }
 }


Motor::Motor()
{  
  
}

uint8_t Motor::driverGo(uint8_t motorID, int8_t mSpeed)
{
uint8_t mDir;
	
	
    if((mSpeed > 100) || (mSpeed < -100))
	  return 1; 
	  
    if(mSpeed & 0x80)
	{
	  mDir = FORWARD;
	  mSpeed = -mSpeed;
	}
	else 
	  mDir = BACKWARD;
	   
	if(motorID & MOTOR_ID_1)
		motorDriverGo( MOTOR_ID_1, mDir, mSpeed, 0, 0, 0, 0,0, 0);
	if(motorID & MOTOR_ID_2)
		motorDriverGo( MOTOR_ID_2, 0, 0, mDir, mSpeed, 0, 0,0, 0);
    if(motorID & MOTOR_ID_3)
		motorDriverGo( MOTOR_ID_3, 0, 0, 0, 0 , mDir, mSpeed,0, 0 );
    if(motorID & MOTOR_ID_4)
		motorDriverGo( MOTOR_ID_4, 0, 0, 0, 0 , 0, 0, mDir, mSpeed );

    	
  return 0;
}



uint8_t Motor::driverGo(	uint8_t motorIDs, 
							int8_t m1Speed,
							int8_t m2Speed,
							int8_t m3Speed,
							int8_t m4Speed
						)
{	
 uint8_t m1Dir,m2Dir,m3Dir,m4Dir;
 
    if ((m1Speed > 100) || (m1Speed < -100))
	  return 1;
	  
	if ((m2Speed > 100) || (m2Speed < -100))
	  return 1;
	  
	if ((m3Speed > 100) || (m3Speed < -100))
	  return 1;

	if ((m4Speed > 100) || (m4Speed < -100))
	  return 1;
	  
	if(m1Speed & 0x80)
	{
	  m1Dir = FORWARD;
	  m1Speed = -m1Speed;
	}
	else 
	  m1Dir = BACKWARD;
	  
	if(m2Speed & 0x80)
	{
	  m2Dir = FORWARD;
	  m2Speed = -m2Speed;
	}
	else 
	  m2Dir = BACKWARD; 
	  
	if(m3Speed & 0x80)
	{
	  m3Dir = FORWARD;
	  m3Speed = -m3Speed;
	}
	else 
	  m3Dir = BACKWARD;

	if(m4Speed & 0x80)
	{
	  m4Dir = FORWARD;
	  m4Speed = -m4Speed;
	}
	else 
	  m4Dir = BACKWARD;

  motorDriverGo( 	motorIDs, 
					m1Dir, m1Speed ,
					m2Dir, m2Speed ,
					m3Dir, m3Speed ,					
					m4Dir, m4Speed 
				);
				
return 0;
}

void Motor::driverStop(uint8_t motorIDs)
{
    if(motorIDs & MOTOR_ID_1)
    {
 		DDRH |= (1<<PH6);
		DDRJ |= (1<<PJ0);
				
		TCCR2A &=  ~(1<<COM2B1) | (1<<COM2B0);
		PORTH &= ~(1<<PH6);
		PORTJ &= ~(1<<PJ0);        		  			
		motor[MOTOR1].Speed = 0;
		motor[MOTOR1].isActive = false;           //Stop
    }
	
    if(motorIDs & MOTOR_ID_2)
    {
		TCCR2A &=  ~(1<<COM2A1) | (1<<COM2A0);		  
		PORTB &= ~(1<<PH4);
		PORTJ &= ~(1<<PJ1);    
		motor[MOTOR2].Speed = 0;		
		motor[MOTOR2].isActive = false;           //Stop	
	}

    if(motorIDs & MOTOR_ID_3)
    {
		TCCR5A &= ~((1<<COM5B1) | (0<<COM5B0));
		PORTK &= ~(1<<PK6);     
		motor[MOTOR3].Speed = 0;			
		motor[MOTOR3].isActive = false;           //Stop	
	}

    if(motorIDs & MOTOR_ID_4)
    {
		TCCR5A &= ~((1<<COM5C1) | (0<<COM5C0));
		PORTL &= ~(1<<PL5);
		PORTK &= ~(1<<PK5);   
		motor[MOTOR4].Speed = 0;		
		motor[MOTOR4].isActive = false;           //Stop
	}
	
}



