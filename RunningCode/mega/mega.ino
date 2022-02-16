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


#define RC_MOTOR_1  3
#define RC_MOTOR_2  1
#define RC_MOTOR_3  2


Motor motor;  //create motor object

const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int m1Speed = 0;
int m2Speed = 0;
int m3Speed = 0;

int demoSpeed = 40;
int delayParam = 3000;
//int dir = 1;
bool mode = false;


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


//Demofigure Functions
void spin(int demoSpeed, int dir){
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(1100);
}


void circle(int demoSpeed, int dir){
  m1Speed = demoSpeed * -1.5 * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * -2.75 * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(3000);
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
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("SIA 21/22 4Teck");
  delay(3000);
  rc5_init(); //init Maerklin remote control
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A:    C:     M:");
  Serial.begin(9600);
}

void loop(){
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);

  
  if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle){
    switch(maerklin_fst_current.address) {  //Switches between adresses (1/2/3/4)
      case 24:  //Adress 1
        lcd.setCursor(0, 0);
        lcd.print("A:1");
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
        
      case 26:  //Adress 2
        lcd.setCursor(0, 0);
        lcd.print("A:2");
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
      
      case 27:  //Adress 3 Demofigures
        lcd.setCursor(0, 0);
        lcd.print("A:3");
        switch(maerklin_fst_current.command)
        {
          case 80:  //Button * (Stop)
            go(0, 0, 0);
            break;
          case 81:  //Button 1 (Spin)
            spin(demoSpeed, dir);
            break;-
          case 82:  //Button 2 (Circle)
            circle(demoSpeed, delayParam);
            break;
          case 83:  //Button 3 (Rectangle)
            rectangle(demoSpeed, delayParam);
            break;
          case 84:  //Button 4 (Triangle)
            triangle(demoSpeed, delayParam);
            break;
          case 16:  //Button + (Forward)
            forward(demoSpeed, delayParam);
            break;
          case 17:  //Button - (Backward)
            backward(demoSpeed, delayParam);
            break;
          case 13: //Button <.> (R/L Toggle)
            if(dir = 1) {
              dir = -1;
            }
            else {
              dir = 1;
            }
            break;
        }
      case 28:  //Adress 4 Drive
    }
  }
  maerklin_fst_previous = maerklin_fst_current;
  go(0,0,0);
}
  
