/*
case 83:   //Warnblinker - Symbol, Knopf 3
          do{
            pixels.clear();
            pixels.setPixelColor(38, pixels.Color(5, 0, 0));   //Helles Warnblinker-Symbol
            pixels.setPixelColor(30, pixels.Color(5, 0, 0));
            pixels.setPixelColor(22, pixels.Color(5, 0, 0));
            pixels.setPixelColor(14, pixels.Color(5, 0, 0));
            pixels.setPixelColor(6, pixels.Color (5, 0, 0));
            pixels.setPixelColor(29, pixels.Color(5, 0, 0));
            pixels.setPixelColor(21, pixels.Color(5, 0, 0));
            pixels.setPixelColor(13, pixels.Color(5, 0, 0));
            pixels.setPixelColor(20, pixels.Color(5, 0, 0));
            pixels.show();
  
            pixels.setPixelColor(33, pixels.Color(5, 0, 0));   //Blink ding oben
            pixels.setPixelColor(1, pixels.Color(5, 0, 0)); 
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(38, pixels.Color( 255, 0, 0)); //"Animiertes" Warnblinker-Symbol
            pixels.setPixelColor(30, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(22, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(14, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(6, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(29, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(21, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(13, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(20, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL_2);
          } while (83);
            break; 

*/









#include <Rc5Decoder.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

struct STRUCT_RC5 {
uint8_t       toggle, 
              address, 
              command;
}maerklin_fst_current, maerklin_fst_previous;


#define PIN 13 
#define NUMPIXELS 40

#define SLAVE_ADR   0x38

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 150
#define DELAYVAL_2 500
boolean blinker = true;
boolean links = false;
boolean rechts = false;
boolean stopp = false;
boolean sanduhr = true;

unsigned colour_selection[8][3]=  {{0,0,0},{0,0,255},{0,255,0},{0,255,255},{255,0,0},{255,0,255},{255,255,0},{255,255,255}};


void setup() {
   pixels.begin();
   pixels.clear();
   Serial.begin(9600);
}

void loop() {


  
  if(blinker == true) {
      pixels.setPixelColor(32, pixels.Color(60, 30, 0));
      pixels.setPixelColor(0, pixels.Color(60, 30, 0)); 
      pixels.show();
      delay(100);
      pixels.setPixelColor(32, pixels.Color(0, 0, 0));
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      delay(50);
      pixels.setPixelColor(32, pixels.Color(60, 30, 0));
      pixels.setPixelColor(0, pixels.Color(60, 30, 0)); 
      pixels.show();
      delay(100);
      pixels.setPixelColor(32, pixels.Color(0, 0, 0));
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVAL_2);
  }
  

  if(links == true) {
      pixels.clear();
      pixels.setPixelColor(2, pixels.Color (6, 3, 0));
      pixels.setPixelColor(11, pixels.Color(6, 3, 0));
      pixels.setPixelColor(20, pixels.Color(6, 3, 0));
      pixels.setPixelColor(22, pixels.Color(6, 3, 0));
      pixels.setPixelColor(29, pixels.Color(6, 3, 0));
      pixels.setPixelColor(36, pixels.Color(6, 3, 0));
      pixels.setPixelColor(30, pixels.Color(6, 3, 0));
      pixels.setPixelColor(37, pixels.Color(6, 3, 0));
      pixels.setPixelColor(38, pixels.Color(6, 3, 0));
      pixels.show();
      
      pixels.setPixelColor(2, pixels.Color (60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(11, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(20, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(22, pixels.Color(60, 30, 0));
      pixels.setPixelColor(29, pixels.Color(60, 30, 0));
      pixels.setPixelColor(36, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(30, pixels.Color(60, 30, 0));
      pixels.setPixelColor(37, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(38, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL_2);
      
      pixels.clear();
      delay(DELAYVAL);
  }
  

  if(rechts == true) {
      pixels.clear(); 
      pixels.setPixelColor(34, pixels.Color (6, 3, 0));
      pixels.setPixelColor(27, pixels.Color(6, 3, 0));
      pixels.setPixelColor(20, pixels.Color(6, 3, 0));
      pixels.setPixelColor(13, pixels.Color(6, 3, 0));
      pixels.setPixelColor(4, pixels.Color(6, 3, 0));
      pixels.setPixelColor(22, pixels.Color(6, 3, 0));
      pixels.setPixelColor(5, pixels.Color(6, 3, 0));
      pixels.setPixelColor(14, pixels.Color(6, 3, 0));
      pixels.setPixelColor(6, pixels.Color(6, 3, 0));
      pixels.show();

      pixels.setPixelColor(34, pixels.Color (60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(27, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(20, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(13, pixels.Color(60, 30, 0));
      pixels.setPixelColor(4, pixels.Color(60, 30, 0));
      pixels.setPixelColor(22, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
  
      pixels.setPixelColor(5, pixels.Color(60, 30, 0));
      pixels.setPixelColor(14, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(6, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL_2);
      
      pixels.clear();
      delay(DELAYVAL);
  }


  if(stopp == true) {
    pixels.clear();
    pixels.setPixelColor(6, pixels.Color (6, 3, 0));
    pixels.setPixelColor(38, pixels.Color(6, 3, 0));
    pixels.setPixelColor(29, pixels.Color(6, 3, 0));
    pixels.setPixelColor(13, pixels.Color(6, 3, 0));
    pixels.setPixelColor(20, pixels.Color(6, 3, 0));
    pixels.setPixelColor(11, pixels.Color(6, 3, 0));
    pixels.setPixelColor(27, pixels.Color(6, 3, 0));
    pixels.setPixelColor(34, pixels.Color(6, 3, 0));
    pixels.setPixelColor(2, pixels.Color(6, 3, 0));
    pixels.show();
    delay(DELAYVAL);
    
    pixels.setPixelColor(6, pixels.Color(60, 30, 0));
    pixels.setPixelColor(38, pixels.Color(60, 30, 0));
    pixels.show();
    delay(DELAYVAL);

    pixels.setPixelColor(13, pixels.Color(60, 30, 0));
    pixels.setPixelColor(29, pixels.Color(60, 30, 0));
    pixels.show();
    delay(DELAYVAL);

    pixels.setPixelColor(20, pixels.Color(60, 30, 0));
    pixels.show();
    delay(DELAYVAL);

    pixels.setPixelColor(11, pixels.Color(60, 30, 0));
    pixels.setPixelColor(27, pixels.Color(60, 30, 0));
    pixels.show();
    delay(DELAYVAL);

    pixels.setPixelColor(2, pixels.Color(60, 30, 0));
    pixels.setPixelColor(34, pixels.Color(60, 30, 0));
    pixels.show();
    delay(DELAYVAL_2);
  }


  if(sanduhr == true){
      pixels.clear();
      pixels.setPixelColor(38, pixels.Color(6, 3, 0));
      pixels.setPixelColor(30, pixels.Color(6, 3, 0));
      pixels.setPixelColor(22, pixels.Color(6, 3, 0));
      pixels.setPixelColor(14, pixels.Color(6, 3, 0));
      pixels.setPixelColor(6, pixels.Color (6, 3, 0));
      pixels.setPixelColor(29, pixels.Color(6, 3, 0));
      pixels.setPixelColor(13, pixels.Color(6, 3, 0));
      pixels.setPixelColor(20, pixels.Color(6, 3, 0));
      pixels.setPixelColor(27, pixels.Color(6, 3, 0));
      pixels.setPixelColor(11, pixels.Color(6, 3, 0));
      pixels.setPixelColor(34, pixels.Color(6, 3, 0));
      pixels.setPixelColor(26, pixels.Color(6, 3, 0));
      pixels.setPixelColor(18, pixels.Color(6, 3, 0));
      pixels.setPixelColor(10, pixels.Color(6, 3, 0));
      pixels.setPixelColor(2, pixels.Color (6, 3, 0));
      pixels.show();

      pixels.setPixelColor(18, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(26, pixels.Color(60, 30, 0));
      pixels.setPixelColor(10, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(34, pixels.Color(60, 30, 0));
      pixels.setPixelColor(2, pixels.Color (60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(27, pixels.Color(60, 30, 0));
      pixels.setPixelColor(11, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(20, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(27, pixels.Color(60, 30, 0));
      pixels.setPixelColor(11, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(13, pixels.Color(60, 30, 0));
      pixels.setPixelColor(29, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(6, pixels.Color(60, 30, 0));
      pixels.setPixelColor(38, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(30, pixels.Color(60, 30, 0));
      pixels.setPixelColor(14, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(22, pixels.Color(60, 30, 0));
      pixels.show();
      delay(DELAYVAL);

//----------------------------------

      pixels.setPixelColor(18, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(26, pixels.Color(6, 3, 0));
      pixels.setPixelColor(10, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(34, pixels.Color(6, 3, 0));
      pixels.setPixelColor(2, pixels.Color (6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(27, pixels.Color(6, 3, 0));
      pixels.setPixelColor(11, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(20, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(27, pixels.Color(6, 3, 0));
      pixels.setPixelColor(11, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(13, pixels.Color(6, 3, 0));
      pixels.setPixelColor(29, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);
      
      pixels.setPixelColor(6, pixels.Color(6, 3, 0));
      pixels.setPixelColor(38, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(30, pixels.Color(6, 3, 0));
      pixels.setPixelColor(14, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL);

      pixels.setPixelColor(22, pixels.Color(6, 3, 0));
      pixels.show();
      delay(DELAYVAL_2);
  }
  
  if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle){
      if(maerklin_fst_current.address == 24){
        switch(maerklin_fst_current.command){
  
  
          case 80:   //Anschalten, Knopf *
            pixels.setPixelColor(0, pixels.Color(255, 0 , 0));
            pixels.show();
            delay(500);
            pixels.setPixelColor(0, pixels.Color(0, 0, 0));
            pixels.show();
            break;
  
  
          case 81:   //Links vorbei, Knopf 1
            if(links == false) {
              links = true;
            }
            else{
              links == false;
            }
            break;
            
          case 82:   //STOPP, Knopf 2 
            if(stopp == false) {
              stopp = true;
            }
            else{
              stopp == false;
            }
            break;

  
          case 83:    //Blinken, Knopf 3
            if(blinker == false) {
              blinker = true;
            }
            else{
              blinker == false;
            }
            break;


  
          case 84:   //Rechts vorbei, Knopf 4
            if(blinker == false) {
              blinker = true;
            }
            else{
              blinker == false;
            }
            break;
             
  
          case 13:   //Sanduhr, Knopf <->
            if(sanduhr == false) {
              sanduhr = true;
            }
            else{
              sanduhr == false;
            }
            break;

          case 16:   //Einfahrt verboten, Knopf +
          do {
            pixels.clear();
            pixels.setPixelColor(38, pixels.Color(5, 0, 0));   //Helles Einfahrt verboten-Symbol
            pixels.setPixelColor(30, pixels.Color(5, 0, 0));
            pixels.setPixelColor(22, pixels.Color(5, 0, 0));
            pixels.setPixelColor(14, pixels.Color(5, 0, 0));
            pixels.setPixelColor(6, pixels.Color (5, 0, 0));
            pixels.setPixelColor(37, pixels.Color(5, 0, 0));
            pixels.setPixelColor(5, pixels.Color (5, 0, 0));
            pixels.setPixelColor(36, pixels.Color(5, 0, 0));
            pixels.setPixelColor(4, pixels.Color (5, 0, 0));
            pixels.setPixelColor(35, pixels.Color(5, 0, 0));
            pixels.setPixelColor(3, pixels.Color (5, 0, 0));
            pixels.setPixelColor(34, pixels.Color(5, 0, 0));
            pixels.setPixelColor(26, pixels.Color(5, 0, 0));
            pixels.setPixelColor(18, pixels.Color(5, 0, 0));
            pixels.setPixelColor(10, pixels.Color(5, 0, 0));
            pixels.setPixelColor(2, pixels.Color (5, 0, 0));
            pixels.show();
  
            pixels.setPixelColor(33, pixels.Color(5, 0, 0));   //Blink ding oben
            pixels.setPixelColor(1, pixels.Color(5, 0, 0)); 
            pixels.show();
            delay(DELAYVAL);
            
            pixels.setPixelColor(22, pixels.Color( 255, 0, 0));   //"Animiertes" Einfahr verboten-Symbol
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(14, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(30, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(38, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(6, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(37, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(5, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(36, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(4, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(35, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(3, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(34, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(2, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(26, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(10, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
  
            pixels.setPixelColor(18, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL_2);
          } while (16);
            break;
  
  
          case 17:   //Überholverboten, Knopf -
          do{
            pixels.clear(); 
            pixels.setPixelColor(29, pixels.Color(5, 0, 0));//Helles Überholverboten-Symbol
            pixels.setPixelColor(21, pixels.Color(5, 0, 0));
            pixels.setPixelColor(13, pixels.Color(5, 0, 0));
            pixels.setPixelColor(5, pixels.Color (5, 0, 0));
            pixels.setPixelColor(36, pixels.Color(5, 0, 0));
            pixels.setPixelColor(28, pixels.Color(5, 0, 0));
            pixels.setPixelColor(20, pixels.Color(5, 0, 0));
            pixels.setPixelColor(12, pixels.Color(5, 0, 0));
            pixels.setPixelColor(4, pixels.Color (5, 0, 0));
            pixels.setPixelColor(35, pixels.Color(5, 0, 0));
            pixels.setPixelColor(27, pixels.Color(5, 0, 0));
            pixels.setPixelColor(19, pixels.Color(5, 0, 0));
            pixels.setPixelColor(11, pixels.Color(5, 0, 0));
            pixels.setPixelColor(3, pixels.Color (5, 0, 0));
            pixels.show();
  
            pixels.setPixelColor(33, pixels.Color(5, 0, 0));   //Blink ding oben
            pixels.setPixelColor(1, pixels.Color(5, 0, 0)); 
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(19, pixels.Color( 255, 0, 0)); //"Animiertes" Überholverboten-Symbol
            pixels.setPixelColor(20, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(21, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(11, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(12, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(13, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(27, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(28, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(29, pixels.Color( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL);
    
            pixels.setPixelColor(35, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(36, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(37, pixels.Color( 255, 0, 0));
            pixels.setPixelColor(3, pixels.Color ( 255, 0, 0));
            pixels.setPixelColor(4, pixels.Color ( 255, 0, 0));
            pixels.setPixelColor(5, pixels.Color ( 255, 0, 0));
            pixels.show();
            delay(DELAYVAL_2);
          } while (17);
            break;
        }
      }
    }
}
