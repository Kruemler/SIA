/*
 *  Interrupt and PWM utilities for 16 bit Timer3 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION10BIT 0x3FF    // 16 bit



void motor(unsigned char idx, unsigned char dir, unsigned char duty);
#define RESOLUTION 255    // Timer0,3 auf 8-bit


class TimerThree
{
  public:
  
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
	
    // methods
	void setDir(unsigned char idx, unsigned char dir);	
    void initialize(unsigned char idx,long microseconds=500000);
    void start(unsigned char idx);
    void stop(unsigned char idx);
    void restart(unsigned char idx);
    void pwm(unsigned char idx, int duty, long microseconds=-1);
    void disablePwm(unsigned char idx);
    void attachInterrupt(unsigned char idx, void (*isr)(), long microseconds=-1);
    void detachInterrupt(unsigned char idx);
    void setPeriod(long microseconds);
    void setPwmDuty(unsigned char idx, int duty);
    void (*isrCallback)();
};

extern TimerThree motor1,motor2;
