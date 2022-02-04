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

Motor motor;

int m1Speed = 0;
int m2Speed = 0;
int m3Speed = 0;

int delayParm = 1000;
int demoSpeed = 1;

//Maerklin init (verstehen...)
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
void spin(int demoSpeed, int delayParm){
  m1Speed = demoSpeed * 1;
  m2Speed = demoSpeed * 1;
  m3Speed = demoSpeed * 1;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  go(0,0,0);
}

void circle(int demoSpeed, int delayParm){
  m1Speed = demoSpeed * -1.5;
  m2Speed = demoSpeed * 1;
  m3Speed = demoSpeed * -2.75;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  go(0,0,0);
}

void rectangle(int demoSpeed, int delayParm){
  m1Speed = 50;
  m2Speed = -50;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  
  m1Speed = -38;
  m2Speed = -38;
  m3Speed = 59;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  
  m1Speed = -50;
  m2Speed = 50;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  
  m1Speed = 38;
  m2Speed = 38;
  m3Speed = -59;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  
  go(0,0,0);
}

void setup(){
  rc5_init(); //init Maerklin Remote Control
  //graupner_fst_init(); //init Graupner Remote Control
}

void loop(){
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);
  
  if(maerklin_fst_current.address == 27) { //Channel 3
    switch(maerklin_fst_current.command) {
      case 80: //Button *
        
      case 81: //Button 1
        spin(demoSpeed, delayParm);
        break;
      case 82: //Button 2
        circle(demoSpeed, delayParm);
        break;
      case 83: //Button 3
        rectangle(demoSpeed, delayParm);
      case 84: //Button 4
        break;
      case 16: //Button +
        if(demoSpeed < 10000){
          delayParm = delayParm + 500;
        }
        break;
      case 17: //Button -
        if(demoSpeed > 0){
          delayParm = delayParm - 500;
        }
        break;
      case 13: //Button <.>
        break;
    }
  } 
}
  
