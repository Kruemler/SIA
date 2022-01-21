/*
 *  
 *
 *
 *
 */


#ifndef Motor_h
#define Motor_h

#include <avr/io.h>
#include <avr/interrupt.h>

#include <inttypes.h>


#define Motor_VERSION           1     // software version of this library
 
#define MAX_MOTOR	8

#define MOTOR1    	0
#define MOTOR2    	1
#define MOTOR3    	2
#define MOTOR4    	3

#define MOTOR_ID_1 (1<<MOTOR1)
#define MOTOR_ID_2 (1<<MOTOR2)
#define MOTOR_ID_3 (1<<MOTOR3)
#define MOTOR_ID_4 (1<<MOTOR4)

#define MOTOR_ALL (MOTOR_ID_1 | MOTOR_ID_2 | MOTOR_ID_3 | MOTOR_ID_4)

/****** Pins definitions *************/

#define MOTORSHIELD_IN1   22
#define MOTORSHIELD_IN2   23
#define MOTORSHIELD_IN3   24
#define MOTORSHIELD_IN4   25

/****** ************* *************/

#define FORWARD     1
#define MINUS_POL   FORWARD
 
#define BACKWARD    0
#define PLUS_POL	BACKWARD 

/****** ************* *************/

typedef struct {
  uint8_t isActive :1 ;
  int8_t Speed;
  uint8_t Dir;
} motor_t;

class Motor
{
public:
	Motor();
	    
	uint8_t driverGo(	uint8_t motorIDs, 
						int8_t mSpeed
					);
					
	uint8_t driverGo(	uint8_t motorIDs, 
						int8_t m1Speed,
						int8_t m2Speed,
						int8_t m3Speed, 
						int8_t m4Speed						
				);
  void driverStop(uint8_t motor_id);
 
private:
   uint8_t motorIndex;               // index into the data for this motor
    
};

#endif
