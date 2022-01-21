#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef __rc5_decoder_h
#define __rc5_decoder_h


#define	PIN_MASK_RC5	(1<<PD2)
#define	DDR_RC5			DDRD
#define	PORT_RC5		PORTD
#define	PIN_RC5			PIND

 
#define MIN_SHORT  	  444
#define MAX_SHORT 	 1333
#define MIN_LONG  	 1334
#define MAX_LONG  	 2222
#define TIME_OUT  	24889

void rc5_init(void);
void rc5_read(	unsigned char *toggle, 
				unsigned char *address, 
				unsigned char *command);

#endif