//<LED>
#include <Wire.h>
#include <rc5_decoder.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
//</LED>

struct STRUCT_RC5 {
uint8_t       toggle, 
              address, 
              command;
}maerklin_fst_current, maerklin_fst_previous;


void ledStop(){
    matrix.clear();
    matrix.drawLine(0,7, 7,0, showLED);
    matrix.drawLine(7,7, 0,0, showLED);
    matrix.drawLine(5,0, 7,0, showLED);
    matrix.drawLine(7,0, 7,2, showLED);
    matrix.drawLine(7,7, 7,5, showLED);
    matrix.drawLine(7,7, 5,7, showLED);
    matrix.drawLine(0,7, 2,7, showLED);
    matrix.drawLine(0,7, 0,5, showLED);
    matrix.drawLine(0,0, 0,2, showLED);
    matrix.drawLine(0,0, 2,0, showLED);
    matrix.writeDisplay();
    delay(500);
}

void cross() {
    matrix.clear();
    matrix.drawLine(0,5, 7,5, showLED);
    matrix.drawLine(0,4, 7,4, showLED);
    matrix.drawLine(0,3, 7,3, showLED);
    matrix.drawLine(0,2, 7,2, showLED);
    matrix.writeDisplay();
    delay(500);
}

void sanduhr(){
    matrix.clear();
    matrix.drawLine(0,7, 7,0, showLED);
    matrix.drawLine(7,7, 0,0, showLED);
    matrix.drawLine(0,7, 7,7, showLED);
    matrix.drawLine(0,0, 7,0, showLED);
    matrix.drawLine(1,1, 6,1, showLED);
    matrix.drawLine(1,6, 6,6, showLED);
    matrix.drawLine(3,7, 3,0, showLED);
    matrix.drawLine(4,7, 4,0, showLED);
    matrix.writeDisplay();
    delay(500);
}

void arrowBottomRight(){
    matrix.clear();
    matrix.drawLine(0,0,7,7, showLED);
    matrix.drawLine(7,4,7,7, showLED);
    matrix.drawLine(7,7,4,7, showLED);
    matrix.writeDisplay();
    delay(500);
}

void arrowBottomLeft(){
    matrix.clear();
    matrix.drawLine(0,7, 7,0, showLED);
    matrix.drawLine(0,7, 3,7, showLED);
    matrix.drawLine(0,7, 0,4, showLED);
    matrix.writeDisplay();
    delay(500);
}

void ueberholverbot(){
    matrix.clear();
    matrix.drawLine(0,0, 0,7, showLED);
    matrix.drawLine(0,0, 7,0, showLED);
    matrix.drawLine(0,7, 7,7, showLED);
    matrix.drawLine(7,0, 7,7, showLED);
    matrix.writeDisplay();
    delay(500);
}

void setup(){
    matrix.begin(0x70)

    bool ledStatus = false;
    String showLED = "";
}

void setLedStatus(){
    if(ledStatus == true){
        showLED = "LED_ON";
    }
    else{
        showLED = "LED_OFF";
    }
}

void loop() {
    rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);

    if(maerklin_fst_current.adress == 24){
        switch(maerklin_fst_current.command){
            case 80:    //Warnbake an <=> Batteriespannung in V
                if(maerklin_fst_previous.command == maerklin_fst_current.command){
                    ledStatus = !ledStatus;
                }
                break;
            case 81:    //links_vorbei
                if(maerklin_fst_previous.command == maerklin_fst_current.command){
                    ledStatus = !ledStatus;
                    setLedStatus();
                }
                arrowBottomLeft();
                break;

            case 82:    //Stopp 
                if(maerklin_fst_previous.command == maerklin_fst_current.command){
                    ledStatus = !ledStatus;
                    setLedStatus();
                }
                break;

            case 83:    //Warnblinker
                break;

            case 84:    //rechts_vorbei
                if(maerklin_fst_previous.command == maerklin_fst_current.command){
                    ledStatus = !ledStatus;
                    setLedStatus();
                }
                break;

            case 16:
                if(maerklin_fst_previous.command == maerklin_fst_current.command){
                    ledStatus = !ledStatus;
                    setLedStatus();
                }
                cross();

            case 13:    //Sanduhr
                ledStatus = !ledStatus;
                setLedStatus();
                sanduhr();
                break;
                
        }
        maerklin_fst_previous = maerklin_fst_current;
    }
}
