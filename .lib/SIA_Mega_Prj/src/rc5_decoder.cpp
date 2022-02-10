#include <avr/io.h>
#include <avr/interrupt.h>
#include <arduino.h>

#include "rc5_decoder.h"



// RC5 Protocol decoder Arduino code für SIA
/*
   The message of the RC5 protocol is 14-bit long, 2 start bits (always 1), 
   toggle bit, address (5 bits) and command (6 bits). 
   The length of 1 bit is 1778µs which can be divided into two parts of 889µs. 
   A logic 1 is represented by 889µs low and 889µs high. 
   A logic zero is represented by 889µs high and 889µs low.
 
    Einem Start-Bit, das immer logisch 1 ist und es dem Empfänger erlaubt, 
    die korrekte Verstärkung für die nachfolgenden Datenbits zu wählen.
    Einem Field-Bit, das verwendet wird, um zwischen einem unteren Kommandobereich 
    mit einem Wertebereich von 0 bis 63 und einem oberen Kommandobereich mit 
    einem Wertebereich von 64 bis 127 zu unterscheiden.
    Einem Steuerbit, das bei jedem Tastendruck auf der Fernbedienung seinen Zustand wechselt. 
    Damit kann zwischen dem permanenten gedrückt Halten einer Taste und dem wiederholten 
    Drücken und Loslassen unterschieden werden.
    Einer 5 Bit langen Systemadresse zur Auswahl zwischen 32 verschiedenen Gerätearten.
    Einem 6 Bit langen Kommandowert, der eine Funktion aus einem Set von 64 Möglichkeiten 
    (bzw. in Kombination mit dem Field-Bit 128) auswählt. 
    Jeder Funktion, wie beispielsweise dem Verstellen der Lautstärke, 
    ist dabei ein bestimmter Kommandowert zugeordnet.
    
    Die zeitliche Dauer jedes gesendeten Bits beträgt 1,778 ms, 
    die 14 Bits eines Rahmens benötigen 24,889 ms zur Übertragung. 
    Der Datenrahmen wird bei gedrückter Taste alle 113,778 ms wiederholt. 

    Irgendwann fiel dann den Philips-Ingenieuren auf, das 64 Befehle für 
    komplizierte Geräte etwas wenig sein könnten. Man brauchte ein weiteres Kommandobit. 
    Im Interesse weitestgehender Kompatibilität, entschied man sich dafür, 
    das zweite Startbit nun nicht mehr als Startbit, sondern als invertiertes 7. 
    Kommandobit zu nutzen. Für die ersten 64 Kommandos ist es "1", als wäre es ein Startbit, 
    für die 64 neuen Kommandos ist es "0". Damit sind nun für jedes Gerät je 128 unterschiedliche Befehle möglich.
    
*/

#define 	MS_1				(unsigned long)1000
#define   	RC5_INTERVAL         MS_1*113  //ms
 
boolean filed_bit, toggle_bit;
unsigned long rc5_code;
unsigned char cnt_bits;
unsigned char rc5_command, rc5_address;
unsigned long time_stamp, time_stamp_save;
unsigned long previousMicros;

void rc5_init(void)
{
  // Pin Change Interrupt enable on PD2 
  
//   pinMode(19,INPUT); 
  //PCICR |= (1<<PCIE2);
  //PCMSK2 |= (1<<PCINT21);

//	pinMode(7,OUTPUT);    
 
	PORT_RC5 &= ~(1<<PIN_MASK_RC5);  //Input
	EIFR   |=  (1<<INTF2);      // Reset Interrupt 2 Flag 
	EICRA |= (1<<ISC20); //Any edge of INT2 
	EIMSK |= (1<<INT2); //External Interrupt 2 enable


  
  // Turn interrupts on.
  sei();  
}

void rc5_read(unsigned char *toggle, unsigned char *address, unsigned char *command)
{	
	if(cnt_bits >= 28)
	{
		*command = rc5_command;
		*address = rc5_address;
		*toggle = toggle_bit;
	}
	
#if 0
      Serial.print("command: ");       
      Serial.print(*command ,DEC);  

      Serial.print(" address: "); 
      Serial.print(*address ,DEC);  
      
      Serial.print(" toggle_bit: "); 
      Serial.println(*toggle,HEX);              
#endif
}

SIGNAL(INT2_vect) {
unsigned long period;
    
  // digitalWrite(7,!digitalRead(7));
   

  time_stamp = micros();                 // Zeitstempel in µs lesen
  period = time_stamp - time_stamp_save;
  
  if(period > TIME_OUT)
  {
    if(!(PIN_RC5 & PIN_MASK_RC5))  //Start Bit Wurde erkannt        
    {                                 
          rc5_code =  0;    
          cnt_bits = 1;
    }
  }
 

  if(cnt_bits >= 28) //nicht zulässig anzahl bits bzw. ende eines Frame  
    return;
    

 
  if (period >= MIN_SHORT && period <= MAX_SHORT) {
        cnt_bits++;        
        if(PIN_RC5 & PIN_MASK_RC5)   
        {     
          rc5_code = (rc5_code << 1) | 1 ;        
        }
        else        
        {
            rc5_code =  (rc5_code << 1) | 0 ;
        }
    } else if (period >= MIN_LONG && period <= MAX_LONG) {      
        cnt_bits+=2;        
        if(PIN_RC5 & PIN_MASK_RC5)
        {
          rc5_code = (rc5_code << 2) | 0x01;
        }
        else
        {
            rc5_code =  (rc5_code << 2) | 0x02;
        }
    } else {       
      
    }
          
    if((cnt_bits == 28) || (cnt_bits == 27))  // End Bit
    {
      unsigned int temp;
      cnt_bits = 28;

      filed_bit = (rc5_code >> 24) & 0x01;  //extended bit dazu hängen      
      temp = rc5_code & 0xFFF;  //decoder von 12bits zu 6bits command
      rc5_command =   (filed_bit << 6) | 
                      ((temp & 0x800) >> 6) | ((temp & 0x200) >> 5) | 
                      ((temp & 0x80) >> 4) | ((temp & 0x20) >> 3) | 
                      ((temp & 0x08) >> 2) | ((temp & 0x02) >> 1 );                

      temp = (rc5_code >> 12) & 0x3FF;   //decoder von 10bits zu 5bits address
      rc5_address = ((temp & 0x200) >> 5) | ((temp & 0x80) >> 4) | ((temp & 0x20) >> 3) | ((temp & 0x08) >> 2) | ((temp & 0x02) >> 1 );
      toggle_bit = (rc5_code >> 22) & 0x01;      //read toggel bit 
#if 0
      Serial.print("RC code: ");  
      Serial.print(rc5_code,HEX);  

      Serial.print(" command: ");       
      Serial.print(rc5_command ,DEC);  

      Serial.print(" address: "); 
      Serial.print(rc5_address ,DEC);  
      
      Serial.print(" toggle_bit: "); 
      Serial.print(toggle_bit,HEX);              

      Serial.print(" filed_bit: "); 
      Serial.println(filed_bit,HEX);   
#endif                 


    } 
    time_stamp_save = previousMicros = time_stamp;


}





 