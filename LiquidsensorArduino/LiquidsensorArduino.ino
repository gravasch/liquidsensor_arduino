/*
 * 
 * Add the Lcd display and Sim900 module.
 * Lcd display used for showing messages to the user.
 * The Sim900 is a module that helps to send SMS messages.  
 */

// include the librarys code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7, 8); 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean layer_Low = 0;
boolean layer_Normal = 0; 
boolean layer_High = 0;
String layerText="";

void sendSMS();




void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(" Liquid Sensor");
  
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
  // Give time to your GSM shield log on to network
  //delay(20000);   
  
  // Send the SMS

}


void loop() {
  layer_Low = digitalRead(13);
  layer_Normal = digitalRead(10);
  layer_High = digitalRead(9);
  

    if (layer_Low== 0 && layer_Normal == 0 && layer_High == 0){
      
        lcd.setCursor(0, 0);
        lcd.print(" Liquid Sensor");      
        lcd.setCursor(0, 1);
        lcd.print("Liquid layer Low");
      }
    if (layer_Low== 1 && layer_Normal == 0 && layer_High == 0){ 
      
        lcd.setCursor(0, 0);
        lcd.print(" Liquid Sensor");
        lcd.setCursor(0, 1);
        lcd.print("Liquid layer Normal");
      }
    if (layer_Low== 1 && layer_Normal == 1 && layer_High == 0){
      
        lcd.setCursor(0, 0);
        lcd.print(" Liquid Sensor");
        lcd.setCursor(0, 1);
        lcd.print("Liquid layer High");
      }
    if (layer_Low== 1 && layer_Normal == 1 && layer_High == 1){
      
        
        lcd.setCursor(0, 0);
        lcd.print("     Danger!    ");        
        lcd.setCursor(0, 1);
        lcd.print("Very High layer ");
      }

}




void sendSMS() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+306949165881\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("Message example from Arduino Uno."); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 

}
