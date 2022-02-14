#include <Wire.h>       
//#include <rc5_decoder.h>    
#include "Adafruit_LEDBackpack.h"       
#include "Adafruit_GFX.h"       
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();      
void setup() {        
 matrix.begin(0x70);        
}

void loop(){
 // stop
 matrix.clear();
 matrix.drawLine(0,7, 7,0, LED_ON);
 matrix.drawLine(7,7, 0,0, LED_ON);
 matrix.drawLine(5,0, 7,0, LED_ON);
 matrix.drawLine(7,0, 7,2, LED_ON);
 matrix.drawLine(7,7, 7,5, LED_ON);
 matrix.drawLine(7,7, 5,7, LED_ON);
 matrix.drawLine(0,7, 2,7, LED_ON);
 matrix.drawLine(0,7, 0,5, LED_ON);
 matrix.drawLine(0,0, 0,2, LED_ON);
 matrix.drawLine(0,0, 2,0, LED_ON);
 matrix.writeDisplay();
delay(500);

//kreuz
 matrix.clear();
 matrix.drawLine(0,5, 7,5, LED_ON);
 matrix.drawLine(0,4, 7,4, LED_ON);
 matrix.drawLine(0,3, 7,3, LED_ON);
 matrix.drawLine(0,2, 7,2, LED_ON);
 matrix.writeDisplay();
delay(500);

//sanduhr
 matrix.clear();
 matrix.drawLine(0,7, 7,0, LED_ON);
 matrix.drawLine(7,7, 0,0, LED_ON);
 matrix.drawLine(0,7, 7,7, LED_ON);
 matrix.drawLine(0,0, 7,0, LED_ON);
 matrix.drawLine(1,1, 6,1, LED_ON);
 matrix.drawLine(1,6, 6,6, LED_ON);
 matrix.drawLine(3,7, 3,0, LED_ON);
 matrix.drawLine(4,7, 4,0, LED_ON);
 matrix.writeDisplay();
delay(500);

//Pfeil unten rechts
 matrix.clear();
 matrix.drawLine(0,0,7,7, LED_ON);
 matrix.drawLine(7,4,7,7, LED_ON);
 matrix.drawLine(7,7,4,7, LED_ON);
 matrix.writeDisplay();
 delay(500);

//Pfeil unten links
 matrix.clear();
 matrix.drawLine(0,7, 7,0, LED_ON);
 matrix.drawLine(0,7, 3,7, LED_ON);
 matrix.drawLine(0,7, 0,4, LED_ON);
 matrix.writeDisplay();
 delay(500);

//Überholverbot
 matrix.clear();
 matrix.drawLine(0,0, 0,7, LED_ON);
 matrix.drawLine(0,0, 7,0, LED_ON);
 matrix.drawLine(0,7, 7,7, LED_ON);
 matrix.drawLine(7,0, 7,7, LED_ON);
 matrix.writeDisplay();
 delay(500);
 }
