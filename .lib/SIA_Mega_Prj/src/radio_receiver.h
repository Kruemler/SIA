#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef __rb14scan_h
#define __rb14scan_h

#define PPM_PAUSE (1200*3)
#define BUS_PAUSE (((uint32_t)PPM_PAUSE * (uint32_t)10000)/(uint32_t)625)
#define RADIO_RECEIVER_DEFAULT	320

#define RC_MAX_OFFSET 475
#define RC_MIN_OFFSET 275

class radio_receiver
{
public:
    // properties

    // methods	
	void init(void);  
	int  channel(uint8_t i);
	int  ready(void);
	int  AverageChannel(uint8_t i);
	void debug(void);  
};

void radio_receiver::debug(void)
{
uint8_t i;  
int mw;

  Serial.println("---------------------------------------------------------");
  for(i=0; i<9; i++)
  {
		Serial.print("CHANNAL[");
		Serial.print(i, DEC);
		Serial.print("] : ");
		Serial.print(radio_receiver::channel(i), DEC);	  
	  
		Serial.print("  Average[");
		Serial.print(i, DEC);
		Serial.print("] : ");
		Serial.println(radio_receiver::AverageChannel(i), DEC);	  

  }  
  
}

void radio_receiver::init(void)
{
	// init external interrupt 4 for PE4  //
		
	// EIMSK=_BV(INT4);                      // enable INT4
	// EICRB= _BV(ISC40) | _BV(ISC41);       // INT3 for any logical change on PE4	
	  
	DDRD &= ~(1<<PD3);
	PORTD &= ~(1<<PD3);
	  
	EIFR   |=  (1<<INTF3);      // Reset Interrupt 3 Flag (Write 1) 
	EIMSK  |=  (1<<INT3);    // External Interrupt 3 enable
	EICRA |= _BV(ISC30) | _BV(ISC31);       // INT3 for any logical change on PD3	
#if 1 	
	TCCR0A = (1<<WGM01) | (1<<WGM00);

	// set timer 0 prescale factor to 8
	// this combination is for the standard 168/328/1280/2560	
	TCCR0B =  (1<<CS01) | (1<<CS00);

	// enable timer 0 overflow and Compare interrupt
	TIMSK0 = (1<<TOIE0) | (1<<OCIE0A);
#endif	 

	
  sei();
}


int radio_receiver::ready(void)
{
extern uint8_t radio_receiver_timeout;

	if(!radio_receiver_timeout)  
		return 0;

return 1;
}



int radio_receiver::channel(uint8_t ch)
{
extern uint16_t radio_receiver_value[];
int8_t value = 0;
  
  if (ch>8) return 0;
  
  value = map(radio_receiver_value[ch],RC_MAX_OFFSET,RC_MIN_OFFSET,-100,100);
   
  if( value > 100 )
	value = 100;
  else if (value < -100)
	value = -100;
	
  return value;
}


int radio_receiver::AverageChannel(uint8_t ch)
{

extern uint16_t avg_radio_receiver_value[10][16];
uint8_t length = 16;
uint16_t array[16];


   memcpy( array, &avg_radio_receiver_value[ch], sizeof(array) );	
   for (uint8_t i = 1; i < length ; i++) 
   {
      for (uint8_t j = 0; j < length - i ; j++) 
      {
          if (array[j] > array[j+1]) 
          {	  int tmp;
              tmp = array[j];
              array[j] = array[j+1];
              array[j+1] = tmp;
          }
      }
   }	

  unsigned long sum = 0;
  for (int i = 4; i < 12; i++)
   sum += array[i];
    
  if (ch>8) return 0;
  return (sum / 8);
}

#endif