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


#define RC_MOTOR_2  1
#define RC_MOTOR_3  2
#define RC_MOTOR_4  3
#define RC_MOTOR_5  4
#define RC_MOTOR_6  5
#define RC_MOTOR_7  6
#define RC_MOTOR_8  7

#define OMNI_MODE_STOP    0
#define OMNI_MODE_AUTO    1
#define OMNI_MODE_MANUAL  2

const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


radio_receiver graupner_fst;  // create graupner_remote_control object

Motor motor;  //create motor object

struct STRUCT_RC5 {
uint8_t       toggle, 
              address, 
              command;
}maerklin_fst_current, maerklin_fst_previous;

uint8_t omni_mode = 1;

bool straight = false;
bool side = false;
bool rotate = false;
bool invert = false;

int m1Speed = 0;
int m2Speed = 0;
int m3Speed = 0;

const int demoSpeed = 50; //konstanter Multiplikator (wird nicht verändert)
int delayParam = 3000;
int dir = 1;



//MotorDriverGo Function
void go(int m1Speed, int m2Speed, int m3Speed){
  motor.driverGo(MOTOR_ID_2, m2Speed);
  motor.driverGo(MOTOR_ID_3, m3Speed);
  delay(50);
  motor.driverGo(MOTOR_ID_1, m1Speed);
}


//Demofigure Functions
void spin(int demoSpeed, int dir){
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(2650);
  go(0, 0, 0);
}


void circle(int demoSpeed, int dir){
  m1Speed = demoSpeed * -0.75 * dir;
  m2Speed = demoSpeed * 1 * dir;
  m3Speed = demoSpeed * 0.675 * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(8250);
  go(0, 0, 0);
}


void rectangle(int demoSpeed, int delayParam, int dir){
  m1Speed = demoSpeed * -1 * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = demoSpeed * -0.79 * dir;
  m2Speed = demoSpeed * -0.79 * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * -1 * dir;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  
  m1Speed = demoSpeed * 0.79 * dir;
  m2Speed = demoSpeed * 0.79 * dir;
  m3Speed = demoSpeed * -1 * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  go(0, 0, 0);
}


void triangle(int demoSpeed, int delayParam, int dir) {
  m1Speed = demoSpeed * 0.4 * dir;
  m2Speed = demoSpeed * -1 * dir;
  m3Speed = demoSpeed * 0.78 * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  m1Speed = demoSpeed * -0.4 * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * -0.78 * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  m1Speed = demoSpeed * -0.79 * dir;
  m2Speed = demoSpeed * -0.79 * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  m1Speed = demoSpeed * dir;
  m2Speed = demoSpeed * dir;
  m3Speed = demoSpeed * dir;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  go(0, 0, 0);  
}


void forward(int demoSpeed, int delayParam) {
  m1Speed = demoSpeed * -1;
  m2Speed = demoSpeed;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  go(0, 0, 0);
}


void backward(int demoSpeed, int delayParam) {
  m1Speed = demoSpeed;
  m2Speed = demoSpeed * -1;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParam);
  go(0, 0, 0);
}

void manualDrive(){ //No stable signal between 0 - 1 --> no definition for this range
  switch(graupner_fst.channel(RC_MOTOR_8)){
    case 100: //manualDrive Omni
      lcd.setCursor(0, 1);
      lcd.print(graupner_fst.channel(RC_MOTOR_3));
      lcd.setCursor(4, 1);
      lcd.print(graupner_fst.channel(RC_MOTOR_2));
      lcd.setCursor(8, 1);
      lcd.print(graupner_fst.channel(RC_MOTOR_4));
      
      if(graupner_fst.channel(RC_MOTOR_3) < -1 or graupner_fst.channel(RC_MOTOR_3) > 1) {  //forward/ backward
        m1Speed = graupner_fst.channel(RC_MOTOR_3) * 1;
        m2Speed = graupner_fst.channel(RC_MOTOR_3) * 1;   //is supposed to be inverted (but it works only like that)
        m3Speed = 0;
      }
      if(graupner_fst.channel(RC_MOTOR_2) < -1 or graupner_fst.channel(RC_MOTOR_2) > 1) { //sidewards
        if(straight){
          m1Speed = (m1Speed + (graupner_fst.channel(RC_MOTOR_2)*0.79))/2;  //Speed is already set in straight --> we just add the new speed and divide by 2
          m2Speed = (m2Speed + (graupner_fst.channel(RC_MOTOR_2)*0.79))/2;
          m3Speed = (m3Speed + (graupner_fst.channel(RC_MOTOR_2)*1.00))/2;
        }
        else{
          m1Speed = graupner_fst.channel(RC_MOTOR_2) * -0.79;
          m2Speed = graupner_fst.channel(RC_MOTOR_2) * 0.79;
          m3Speed = graupner_fst.channel(RC_MOTOR_2) * 1;
        }
      }
      if(graupner_fst.channel(RC_MOTOR_4) < -1 or graupner_fst.channel(RC_MOTOR_4) > 1) { //rotategraupner_fst.channel(RC_MOTOR_3)+
        if(straight or side){
          m1Speed = (m1Speed + graupner_fst.channel(RC_MOTOR_4))/2;
          m2Speed = (m2Speed + graupner_fst.channel(RC_MOTOR_4))/2;
          m3Speed = (m3Speed + graupner_fst.channel(RC_MOTOR_4))/2;
        }
        else{
          m1Speed = graupner_fst.channel(RC_MOTOR_4);
          m2Speed = graupner_fst.channel(RC_MOTOR_4);
          m3Speed = graupner_fst.channel(RC_MOTOR_4);
        }
      }
      /*if(graupner_fst.channel(RC_MOTOR_4) < -1) {  //not function
        m1Speed = graupner_fst.channel(RC_MOTOR_4) * -1;
        m2Speed = graupner_fst.channel(RC_MOTOR_4) * -1;
        m3Speed = graupner_fst.channel(RC_MOTOR_4) * -1;
      }*/
      go(-1*m1Speed, -1*m2Speed, -1*m3Speed);
      break;
    case 0: //Arexx-Drive
      m3Speed = 0;
        if(graupner_fst.channel(RC_MOTOR_3) < -1 or graupner_fst.channel(RC_MOTOR_3) > 1) {  //forward/ backward
          if(graupner_fst.channel(RC_MOTOR_3) > 0){
          }
          m1Speed = graupner_fst.channel(RC_MOTOR_3) * 0.7;
          m2Speed = graupner_fst.channel(RC_MOTOR_3) * 0.7;
        }
        if(graupner_fst.channel(RC_MOTOR_2) < -1 or graupner_fst.channel(RC_MOTOR_2) > 1) {   //sidewards
          if (graupner_fst.channel(RC_MOTOR_2) > 0){
            m1Speed = m1Speed + graupner_fst.channel(RC_MOTOR_2) * 0.5;
            
          }
          if (graupner_fst.channel(RC_MOTOR_2) < 0){
            m2Speed = m1Speed + graupner_fst.channel(RC_MOTOR_2) * -0.5;
          }
        }
      m3Speed = 0;
      go(-m1Speed, -m2Speed, m3Speed);

      break;
    case -100: //Driften
      break;
  }
}


void setup(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("SIA 21/22 4Teck");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A:    C:     M:A");
  Serial.begin(9600);

  rc5_init(); //init Maerklin remote control
  graupner_fst.init(); // init. Graupner remote control

  Wire.begin();
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
        lcd.setCursor(0, 1);
        lcd.print("R");
        switch(maerklin_fst_current.command) {
          case 80:  //Button * (Stop)
            lcd.setCursor(6, 0);
            lcd.print("C:80");
            lcd.setCursor(2, 1);
            lcd.print("Stop     ");
            go(0, 0, 0);
            break;
          case 81:  //Button 1 (Spin)
            lcd.setCursor(6, 0);
            lcd.print("C:81");
            lcd.setCursor(2, 1);
            lcd.print("Spin     ");
            spin(demoSpeed, dir);
            break;
          case 82:  //Button 2 (Circle)
            lcd.setCursor(6, 0);
            lcd.print("C:82");
            lcd.setCursor(2, 1);
            lcd.print("Circle   ");
            circle(demoSpeed, dir);
            break;
          case 83:  //Button 3 (Rectangle)
            lcd.setCursor(6, 0);
            lcd.print("C:83");
            lcd.setCursor(2, 1);
            lcd.print("Rectangle");
            rectangle(demoSpeed, delayParam, dir);
            break;
          case 84:  //Button 4 (Triangle)
            lcd.setCursor(6, 0);
            lcd.print("C:84");
            lcd.setCursor(2, 1);
            lcd.print("Triangle ");
            triangle(demoSpeed, delayParam, dir);
            break;
          case 16:  //Button + (Forward)
            lcd.setCursor(6, 0);
            lcd.print("C:16");
            lcd.setCursor(0, 1);
            lcd.print("  Forward  ");
            forward(demoSpeed, delayParam);
            break;
          case 17:  //Button - (Backward)
            lcd.setCursor(6, 0);
            lcd.print("C:17");
            lcd.setCursor(0, 1);
            lcd.print("  Backward  ");
            backward(demoSpeed, delayParam);
            break;
          case 13: //Button <.> (R/L Toggle)
            /* lcd.setCursor(6, 0);
            lcd.print("C:18");
            if(dir = 1) {
              lcd.setCursor(0, 1);
              lcd.print("L");
              dir = -1;
            }
            else {
              lcd.setCursor(0, 1);
              lcd.print("R");
              dir = 1;
            } */
            break;
        }
      case 28:  //Adress 4 Drive
        lcd.setCursor(0, 0);
        lcd.print("A:4");
        lcd.setCursor(0, 1);
        lcd.print("                ");
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
            lcd.setCursor(6, 0);
            lcd.print("C:13");
            if(omni_mode == OMNI_MODE_AUTO) {
              omni_mode = OMNI_MODE_MANUAL;
              lcd.print("                ");
              lcd.setCursor(13, 0);
              lcd.print("M:M");
              lcd.setCursor(0, 1);
            }
            else {
              omni_mode = OMNI_MODE_AUTO;
              lcd.print("                ");
              lcd.setCursor(13, 0);
              lcd.print("M:A");
            }
            break;
          }
      
    }
  }
  maerklin_fst_previous = maerklin_fst_current;

  //Manual Function
  if(omni_mode == OMNI_MODE_MANUAL) {
    manualDrive();
  }

  delay(10);
  lcd.setCursor(0, 1);
    lcd.print("                ");
}
  
