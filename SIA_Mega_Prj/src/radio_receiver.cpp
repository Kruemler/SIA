#include <avr/io.h>
#include <avr/interrupt.h>
#include <arduino.h>

#define RADIO_RECEIVER_TIMEOUT_CONST 7

#define PPM_PAUSE (1200*3)
#define BUS_PAUSE (((uint32_t)PPM_PAUSE * (uint32_t)10000)/(uint32_t)625)

uint16_t radio_receiver_value[10];
uint16_t avg_radio_receiver_value[10][16];
uint16_t cap_reg;
int8_t   radio_receiver_ch;
uint8_t radio_receiver_bus_pause;
uint8_t radio_receiver_ch_idx = 0;
uint8_t radio_receiver_timeout = 0;
unsigned long radio_receiver_timer0_ovf_cnt = 0;
extern unsigned long timer0_overflow_count;
// if this timer calls, we reached the pause between //
// three PWM packets -> reset channel                  //


// interrupt service routine that wraps a user defined function supplied by attachInterrupt	
ISR (TIMER0_COMPA_vect)
{
  if(radio_receiver_timeout)
	radio_receiver_timeout--;
	
  radio_receiver_bus_pause++;
  if(radio_receiver_bus_pause >= 3)  
  {
		radio_receiver_ch=-1;
  }

}


ISR(INT3_vect)
{
unsigned long m = timer0_overflow_count - radio_receiver_timer0_ovf_cnt ;
  // is PD3 high now (was it an upward slope) ? //
  // if it was an upward slope:                 //
  // 1. it was the FIRST slope of the nine PWMs //
  // 2. it was one of 2..9, then measure time   //
        
  
  			
  radio_receiver_timer0_ovf_cnt = timer0_overflow_count;
  radio_receiver_bus_pause  = 0;
  if (PIND & (1<<PD3)) {      
    // are we inside the 9 PWMs ? //
    // if(radio_receiver_ch == -1)
	
	radio_receiver_timeout = RADIO_RECEIVER_TIMEOUT_CONST;
	
	if(radio_receiver_ch == 0)
		radio_receiver_ch_idx++;
				
	if(radio_receiver_ch>=0 && radio_receiver_ch<9) {
	
		if(TIFR0 & (1 << TOV0))			
		{
			TIFR0 |= (1 << TOV0);
			m++;
		}		
		 
		radio_receiver_value[radio_receiver_ch]= (uint32_t)(m<<8) + TCNT0 - cap_reg  ;
		avg_radio_receiver_value[radio_receiver_ch][radio_receiver_ch_idx & 0x0F] = radio_receiver_value[radio_receiver_ch];
	} 

	cap_reg = TCNT0;
	radio_receiver_ch++; 	 
  }
 
}

