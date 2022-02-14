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


#define RC_MOTOR_1  1    
#define RC_MOTOR_2  2
#define RC_MOTOR_3  3

Motor motor;  //create motor object

radio_receiver graupner_fst;  // create graupner_remote_control object

const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int m1Speed = 0;
int m2Speed = 0;
int m3Speed = 0;

int demoSpeed = 20;
int delayParam = 3000;
String remote = "autom";


struct STRUCT_RC5 {
uint8_t       toggle, 
              address, 
              command;
}maerklin_fst_current, maerklin_fst_previous;


//MotorDriverGo Function
void go(int m1Speed, int m2Speed, int m3Speed){
  motor.driverGo(MOTOR_ID_1, m1Speed);
  motor.driverGo(MOTOR_ID_2, m2Speed);
  motor.driverGo(MOTOR_ID_3, m3Speed);
}


//Demofiguren init
void spin(int demoSpeed, int delayParam){
  m1Speed = demoSpeed * 1;
  m2Speed = demoSpeed * 1;
  m3Speed = demoSpeed * 1;
  go(50, 50, 50);
  delay(delayParam);
}

void circle(int demoSpeed, int delayParam){
  m1Speed = demoSpeed * -1.5;
  m2Speed = demoSpeed * 1;
  m3Speed = demoSpeed * -2.75;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
}


void rectangle(int demoSpeed, int delayParam){
  m1Speed = 50;
  m2Speed = -50;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = -38;
  m2Speed = -38;
  m3Speed = 59;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = -50;
  m2Speed = 50;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = 38;
  m2Speed = 38;
  m3Speed = -59;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
}

void setup(){
  rc5_init(); //init Maerklin rmote control
  graupner_fst.init(); // init. Graupner remote control
  //graupner_fst_init(); //init Graupner Remote Control
  lcd.print("upload complete");
  delay(5000);
  lcd.clear();
}

void loop(){
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);

        m1Speed = graupner_fst.channel(RC_MOTOR_1);
        m2Speed = graupner_fst.channel(RC_MOTOR_2);
        m3Speed = graupner_fst.channel(RC_MOTOR_3);
        lcd.setCursor(0,0);
        lcd.print("Graupner Remote");
        lcd.setCursor(0,1);
        lcd.print(graupner_fst.channel(RC_MOTOR_1));


  if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle){
  
    switch(maerklin_fst_current.address)  //Switch Case though the adresses (1/2/3/4) = () 
    {
      case 24:  //Channel 1 //reserved Warnbrarke --> needs to be changed
        switch(maerklin_fst_current.command){
          case 16:
            lcd.clear();
            bool test = true;
            test = !test;
            lcd.print(test);
          case 81:
            lcd.clear();
            remote = "manuell";
            lcd.print(remote);
        }
        
      case 26:  //Channel 2
        lcd.clear();
        switch(maerklin_fst_current.command){
          case 80: //Button *
  
            break;
          case 81: //Button 1
  
            break;
          case 82: //Button 2
  
            break;
          case 83: //Button 3
  
            break;
          case 84: //Button 4
  
            break;
          case 16: //Button +
          
            break;
          case 17: //Button -
  
            break;
          case 13: //Button <.>
            break;
          }
      
      case 27:  //Channel 3 Demofiguren
        switch(maerklin_fst_current.command)
        {
          case 80:  //Button *
            go(0,0,0);
          case 81:  //Button 1
            spin(demoSpeed, delayParam);
            break;
          case 82:  //Button 2
            circle(demoSpeed, delayParam);
            break;
          case 83:  //Button 3
            rectangle(demoSpeed, delayParam);
            break;
          case 84:  //Button 4
            break;
          case 16:  //+ -Button
            if(demoSpeed < 10000){
              delayParam = delayParam + 500;
            }
            break;
          case 17:  //- -Button
            if(demoSpeed > 0){
              delayParam = delayParam - 500;
            }
            break;
          case 13: //Button <.>
            break;
        }
      case 28:  //Channel 4
        switch(maerklin_fst_current.command){
          case 80: //Button *
  
            break;
          case 81: //Button 1
  
            break;
          case 82: //Button 2
  
            break;
          case 83: //Button 3
  
            break;
          case 84: //Button 4
  
            break;
          case 16: //Button +
          
            break;
          case 17: //Button -
  
            break;
          case 13: //Button <.>
            break;
          }
      
    }
  }
  maerklin_fst_previous = maerklin_fst_current;
  go(0,0,0);
}
  
