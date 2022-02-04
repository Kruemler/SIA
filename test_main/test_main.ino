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

Motor motor;

int m1Speed = 0;
int m2Speed = 0;
int m3Speed = 0;

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
void spin(int delayParm){
  m1Speed = demoSpeed * 1;
  m2Speed = demoSpeed * 1;
  m3Speed = demoSpeed * 1;
  go(m1Speed, m2speed, m3Speed);
  delay(delayParm);
  go(0,0,0);
}

void cirle(int delayParm){
  m1Speed = demoSpeed * -1.5;
  m2Speed = demoSpeed * 1:
  m3Speed = demoSpeed * -2.75;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  go(0,0,0);
}

void rectangel(int delayParm){
  m1Speed = 50;
  m2Speed = -50;
  m3Speed = 0;
  go(m1Speed, m2Speed, m3Speed);
  delay(delayParm);
  
  m1Speed = -38;
  m2Speed = -38;
  m3Speed = 59;
  go(m1Speed, m2Speed, m3Speed): 
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
  graupner_fst_init(); //init Graupner Remote Control
}

void loop(){
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command);
  
  if(maerklin_fst_current.address == 27)
  {
    switch(maerklin_fst_current.command)
    {
      case 81://Top left (1)
        spin(demoSpeed, delayParm);
        break;
      case 82://Top middle left (2)
        circle(demoSpeed, delayParm);
        break;
      case 83://Top middle right (3)
        rectangle(demoSpeed, delayParm);
      case 84://Top right (4)
        break;
      case 16://+ -Button
        if(demoSpeed < 10000){
          delayParm = delayParm + 500;
        }
        break;
      case 17://- -Button
        if(demoSpeed > 0){
          delayParm = delayParm - 500;
        }
        break;
      case 13: //bottom
        break;
    }
  } 
}
  
