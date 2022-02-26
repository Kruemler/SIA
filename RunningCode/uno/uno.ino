#include <analogOut.h>
#include <define.h>
#include <motor_drv.h>
#include <radio_receiver.h>
#include <rc5_decoder.h>
#include <Servo.h>
#include <ServoTimers.h>
#include <sia_board_io.h>
#include <LiquidCrystal.h>
#include <Wire.h>
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

#define COLOUR_GREEN_POS  0
#define COLOUR_RED_POS    1
#define COLOUR_BLUE_POS   2

#define SLAVE_ADR   0x38

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 125 
#define DELAYVAL_2 100


unsigned colour_selection[8][3]=  {{0,0,0},{0,0,255},{0,255,0},{0,255,255},{255,0,0},{255,0,255},{255,255,0},{255,255,255}};



void setup(){
    pixels.begin();
    pixels.clear();

    rc5_init();
}

void loop() {
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);

  if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle){
    if(maerklin_fst_current.address == 24){
      switch(maerklin_fst_current.command){
        case 80:
          pixels.setPixelColor(13, pixels.Color( 0, 255 , 0));
          pixels.show();   // Send the updated pixel colors to the hardware.
          delay(DELAYVAL); // Pause before next pass through loop
          
          pixels.setPixelColor(22, pixels.Color( 255, 0 , 0));
          pixels.show();   // Send the updated pixel colors to the hardware.
          delay(DELAYVAL); // Pause before next pass through loop
          
          pixels.setPixelColor(12, pixels.Color( 0, 0 , 255));
          pixels.show();   // Send the updated pixel colors to the hardware.
          delay(DELAYVAL); // Pause before next pass through loop
          
          delay(DELAYVAL); // Pause before next pass through loop
          break;
      }
    }
  }

  maerklin_fst_previous = maerklin_fst_current;
}
