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

#define OMNI_MODE_STOP    0
#define OMNI_MODE_AUTO    1
#define OMNI_MODE_MANUAL  2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


radio_receiver graupner_fst;  // create fernsteuerung object 
 
Motor motor;                    // create motor object 

struct STRUCT_RC5 {
uint8_t       toggle, 
              address, 
              command;
}maerklin_fst_current, maerklin_fst_previous;

uint8_t omni_mode = 0;

int8_t m1Speed = 0;
int8_t m2Speed = 0;
int8_t m3Speed = 0;

char transmit_buf[10];
int8_t tx_adr,tx_cmd; 

int speed = 20;

void spin(int speed, int delayParm) {
   m1Speed = speed * 1; //Multiplikator nach Vorlage anpassen
   m2Speed = speed * 1;
   m3Speed = speed * 1;
   
   motor.driverGo(MOTOR_ID_1,m1Speed);
   motor.driverGo(MOTOR_ID_2,m2Speed);
   motor.driverGo(MOTOR_ID_3,m3Speed);
   
   delay(delayParm);
}
void circle(int speed) {
  m1Speed = speed * -1.5;
  m2Speed = speed * 1;
  m3Speed = speed * -2.75;
}

/*void rectangle(){
  m1Speed = speed * ;
  m2Speed = speed * ;
  m3Speed = speed * ;
  delay();
  spin();
}*/
    
void setup() {
  /* put your setup code here, to run once */
  
  //functions


  
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("S I A / 20 / 21");
  delay(1000);            // delay 500 milliseconds
  lcd.clear();
  
  // open the serial port at 9600 bps:
  Serial.begin(9600);      
  Serial.println("Omni car: Start demo Prog."); 
    
  graupner_fst.init(); // init. Graupner fernsteuerung
  rc5_init();  // init. Maerklin fernsteuerung

    // join i2c bus (address optional for master)
  Wire.begin(); 
  
}


void loop() {
// put your main code here, to run repeatedly:
  
   //remote_control.debug();
   
   rc5_read(&maerklin_fst_current.toggle,&maerklin_fst_current.address,&maerklin_fst_current.command); 
   if(maerklin_fst_previous.toggle != maerklin_fst_current.toggle) 
   {
        
        Serial.println("toggle bit maerklin fernsteuerung");         
        tx_adr++;
        tx_cmd = ~tx_adr;
        
        Serial.print("ADR: ");
        Serial.print(tx_adr);

        Serial.print(" CMD: ");
        Serial.println(tx_cmd);
                
        lcd.setCursor(0, 0);
        lcd.print("C:");
        lcd.print(maerklin_fst_current.command);

        lcd.setCursor(4, 0);
        lcd.print("A:");
        lcd.print(maerklin_fst_current.address);

        
        if(maerklin_fst_current.address == 27)  //Demofiguren channel (=3)
        {  
          switch(maerklin_fst_current.command)
          {
            case 81:  //Knopf 1
              //circle();
              break;
            case 82:  //Knopf 2
              //spin();
              break;
            case 83:  //Knopf 3
              //rectangle();
              m1Speed = 50;
              m2Speed = -50;
              m3Speed = 0;
              motor.driverGo(MOTOR_ID_1,m1Speed);
              motor.driverGo(MOTOR_ID_2,m2Speed);
              motor.driverGo(MOTOR_ID_3,m3Speed);
              
              delay(2000);
              m1Speed = -38;
              m2Speed = -38;
              m3Speed = 59;
              motor.driverGo(MOTOR_ID_1,m1Speed);
              motor.driverGo(MOTOR_ID_2,m2Speed);
              motor.driverGo(MOTOR_ID_3,m3Speed);
              
              delay(2000);
              m1Speed = -50;
              m2Speed = 50;
              m3Speed = 0;
              motor.driverGo(MOTOR_ID_1,m1Speed);
              motor.driverGo(MOTOR_ID_2,m2Speed);
              motor.driverGo(MOTOR_ID_3,m3Speed);
              
              delay(2000);
              m1Speed = 38;
              m2Speed = 38;
              m3Speed = -59;
              motor.driverGo(MOTOR_ID_1,m1Speed);
              motor.driverGo(MOTOR_ID_2,m2Speed);
              motor.driverGo(MOTOR_ID_3,m3Speed);
              
              delay(2000);
              m1Speed = 0;
              m2Speed = 0;
              m3Speed = 0;
              break;
              
            case 84:  //Knopf 4
              break;

            case 16:  // Knopf +
              break;
            case 17:  //Knopf -
              break;

            case 13:  //letzter Knopf
              break;
          }
        }
        
        if(maerklin_fst_current.address == 24)
        {
            switch(maerklin_fst_current.command)
            {
              case 16: //speed + 1
                      omni_mode = OMNI_MODE_AUTO;
                      if(speed < 100){
                        speed = speed + 10;
                      }
                      break;
             case 17: //speed - 1
                    omni_mode = OMNI_MODE_AUTO;
                    if(speed > -100){
                      speed = speed - 10;
                    }
                    break;
              case 80: //Taster *
                      omni_mode = OMNI_MODE_STOP;
                      m1Speed = m2Speed = m3Speed = 0;
                      motor.driverStop(MOTOR_ALL);
                      break;
                   
              case 81: //Left Taster 1
                      omni_mode = OMNI_MODE_AUTO;
                      m3Speed = -50;
                      break;
                      
                   
              case 82: //spin
                      omni_mode = OMNI_MODE_AUTO;
                      //m1Speed = -50; m2Speed = 50; m3Speed = 0;
                      //spin(speed);
                      break;

             case 83: //circle
                      omni_mode = OMNI_MODE_AUTO;
                      circle(speed);
                      break;
                      
              case 84: //Taster 4
                      omni_mode = OMNI_MODE_MANUAL;
                      break;
            }
        }

       transmit_buf[0] = tx_adr;
       transmit_buf[1] = tx_cmd; 
       transmit_nbyte_to_device(2, transmit_buf);
   } 
   maerklin_fst_previous = maerklin_fst_current;
   
   if(omni_mode == OMNI_MODE_MANUAL)
   {
      m1Speed = graupner_fst.channel(RC_MOTOR_1);
      m2Speed = graupner_fst.channel(RC_MOTOR_2);
      m3Speed = graupner_fst.channel(RC_MOTOR_3);
   }

  #if 0
      motor.driverGo(MOTOR_ID_1,m1Speed);
      motor.driverGo(MOTOR_ID_2,m2Speed);
      motor.driverGo(MOTOR_ID_3,m3Speed);
  #else
      motor.driverGo(MOTOR_ID_1+MOTOR_ID_2+MOTOR_ID_3,m1Speed,m2Speed,m3Speed,0);
      //motor.driverGo(MOTOR_ALL,m1Speed,m2Speed,m3Speed,0);
  #endif

   print_on_lcd_motor_speed(m1Speed,m2Speed,m3Speed);

   delay(10);            // delay 10 milliseconds
}

void print_on_lcd_motor_speed(int8_t m1s, int8_t m2s, int8_t m3s)
{ 
char lcd_text[5]; 

    // Print a message to the LCD.
    lcd.setCursor(10, 0);
    dez_to_char(lcd_text, m1s);
    lcd.print(lcd_text);
    
    lcd.setCursor(0, 1);
    dez_to_char(lcd_text, m2s);
    lcd.print(lcd_text);

    // Print a message to the LCD.
    lcd.setCursor(8, 1);
    dez_to_char(lcd_text, m3s);
    lcd.print(lcd_text);
}

void dez_to_char(char* ptText, int8_t value)
{
char ascii[11] = "0123456789";

   if(value < 0)
   {
      ptText[0] = '-';
      value = -value;
   } 
   else
      ptText[0] = '+';
     
    ptText[1] = ascii[(value/100)%10];
    ptText[2] = ascii[(value/10)%10];
    ptText[3] = ascii[value%10];
    ptText[4] = '\0';
}

void transmit_nbyte_to_device(unsigned char tx_cnt, char *txbuf )
{
  Serial.println("transmit_nbyte_to_device"); 
  
  Wire.beginTransmission(0x38);     // transmit to device adr light state ON/OFF
  
  for(uint8_t i=0; i<tx_cnt; i++)
    Wire.write(byte(txbuf[i]));               // sends byte
    
  Wire.endTransmission();                 // stop transmitting
}
