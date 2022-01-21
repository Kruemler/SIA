#include <Arduino.h>
#include <LiquidCrystal.h>
#include <motor_drv.h>
#include <rc5_decoder.h>

byte m1Speed = 50;  //as soon as posible control per IR Remote
byte m2Speed = 50; 
byte m3Speed = 50;

const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
//int speed = 0;
  
  void spin(int speed){                 //spin on one point
    motor.driverGo  (1, speed);
    motor.driverGo  (2, speed);
    motor.driverGo  (4, speed);
    }

  void circle(int speed){
    motor.driverGo  (1, speed*-1.5) //Multiplikator
    motor.driverGo  (2, speed*1)
    motor.driverGo  (4, speed*-2.75)
  }
  Motor motor;

  //copy of Schulungskoffer IR Remote
  struct STRUCT_RC5 {
  uint8_t       toggle, 
                address, 
                command;
  }maerklin_fst_current, maerklin_fst_previous;
  //end of copy

  void spin(int speed) {                 //spin on one point
    motor.driverGo  (1, speed);
    motor.driverGo  (2, speed);
    motor.driverGo  (4, speed);
  }

  void circle(int speed) {
    motor.driverGo  (1, speed*-1.5);
    motor.driverGo  (2, speed*1);
    motor.driverGo  (4, speed*-2.75);
  }


  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Start App.");

  rc5_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  int maerklinInput = 0;

  motor.driverGo (1, m1Speed);
  motor.driverGo (2, m2Speed);
  motor.driverGo (4, m3Speed);
  
  /*switch(maerklinInput){
    case 1:
      spin(speed);
    case 2:
      circle(speed);
    case 3:
      if(speed > 100){
        speed = speed + 5;
      }
    case 4:
      if(speed < -100){
        speed = speed -5
      }
  }*/

  //Copy of Schulungskoffer IR Remote
  uint8_t motor_nr = 1;
  
  rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command); 
   if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle) 
   {
        maerklin_fst_previous = maerklin_fst_current;
        
        lcd.setCursor(0, 0);
        lcd.print("C:");
        lcd.print(maerklin_fst_current.command);

        lcd.setCursor(4, 0);
        lcd.print("A:");
        lcd.print(maerklin_fst_current.address);

        if(maerklin_fst_current.address == 24)
        {
            switch(maerklin_fst_current.command)
            {
              case 80: //Taster *
                      motor.driverStop(1);
                      break;
                   
              case 81: //Taster 1
                      motor_nr = (0<<3) | (0<<2) | (0<<1) | (1<<0);
                      m1Speed = 50; //50%
                      break;
                   
              case 82: //Taster 2
                      motor_nr = (0<<3) | (0<<2) | (0<<1) | (1<<0);
                      m1Speed = -75; //75%
                      break;

             case 83: //Taster 3
                      motor_nr = (0<<3) | (0<<2) | (1<<1) | (1<<0);
                      m1Speed = m2Speed = 100; //100%
                      break;
                      
              case 84: //Taster 4
                      motor_nr = (0<<3) | (0<<2) | (1<<1) | (1<<0);
                      m1Speed = m2Speed = -100; //100%
                      break;
            }
             
        }
   } 

    if(maerklin_fst_current.command != 80)
        motor.driverGo(motor_nr, m1Speed ,m2Speed, 0, 0 );
}
