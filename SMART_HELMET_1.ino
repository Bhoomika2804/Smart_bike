#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Ultrasonic.h>
Ultrasonic ultrasonic(11, 12);   //(trig,echo)
int distance; 
#include <SoftwareSerial.h>
SoftwareSerial ser(0, 1);              // RX, TX
#include <Keypad.h>

// IOT 1
String apiKey = "7K9L7M0WVC48U8X0";    // Edit this API key according to your Account
String Host_Name = "SATURDAY";                                                                                  // Edit Host_Name
String Password = "SATURDAY";                                                                            // Edit Password

String cmd;

#define alco A2
int alco_level=0;

#define limit A3
int limit_state=0;

#define float_sens A1
int fuel_level=0;

#define main_relay 13

//////////KEYPAD
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() 
{

    lcd.init();                  
  lcd.init();
  lcd.backlight();
  
   Serial.begin(115200); 
  ser.begin(115200);
  ser.println("AT+RST");               // Resetting ESP8266
  char inv = '"';
  String cmd = "AT+CWJAP";

  cmd += "=";
  cmd += inv;
  cmd += Host_Name;
  cmd += inv;
  cmd += ",";
  cmd += inv;
  cmd += Password;
  cmd += inv;
  ser.println(cmd);                    // Connecting ESP8266 to your WiFi Router


  Serial.begin(115200);   // Initiate a serial communication  
  pinMode(limit,INPUT_PULLUP);
  pinMode(float_sens,INPUT_PULLUP);
  pinMode(main_relay,OUTPUT);

     lcd.begin(16, 2);
     lcd.print(" SMART VEHICAL");
     lcd.setCursor(0,1);
     lcd.print("SECURITY & SAFETY"); 
     delay(1000);
     lcd.clear();

     lcd.begin(16, 2);                // Set up the LCD's number of columns and rows:
     lcd.setCursor(0,0);              // Position the LCD cursor
     lcd.print("     JAIN ");   // Print a message to the LCD.
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("   COLLEGE "); 
     Serial.begin(115200);              // Initialize the serial communications:
     delay(1000);
     lcd.clear();

     lcd.setCursor(1,0);              // Position the LCD cursor
     lcd.print("     E & C");   // Print a message to the LCD.
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("   DEPARTMENT"); 
       delay(1000);
     lcd.clear();
     
     lcd.setCursor(1,0);              // Position the LCD cursor
     lcd.print("     HOD");   // Print a message to the LCD.
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("CHETAN PATIL"); 
       delay(1000);
     lcd.clear();

     lcd.setCursor(1,0);              // Position the LCD cursor
     lcd.print("  GUIDE");   // Print a message to the LCD.
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("AVINASH. P. B"); 
       delay(1000);
     lcd.clear();
   key_pad();

}

void loop() 
{
  EstablishTCPconnection();
  UpdatetoTS();
 // key_pad();
  fuel();
  alcohol();
  helmet();
  drive();

}

void key_pad()
{
  lcd.clear();
  Serial.println("Enter Your Password");
  lcd.print("Enter password: ");
  String password="12345";
  String input_password;
  char code[5];
  while(1)
  {
    char key = customKeypad.getKey();
    delay(200);
    if (key)
    {
      //Serial.println(key);
      if(key == '*') 
      {
        input_password = ""; // reset the input password
        lcd.clear();
        lcd.print("Enter password: ");
      } 
      else if(key == '#') 
      {
        if(input_password == password) 
        {
            Serial.println("password is correct");
            lcd.clear();
            lcd.print("Authorized");
            lcd.setCursor(0,1);
            lcd.print("User");
            delay(2000);
            lcd.clear();
            helmet();
            break;
        } 
        else 
        {
          lcd.clear();
          lcd.print("Wrong Password!!");
          digitalWrite(main_relay,LOW);
          Serial.println("Entered Password: ");
          Serial.println(input_password);
          Serial.println("password is incorrect, try again");
          delay(2000);
          lcd.clear();
          break;
        }
        input_password = ""; // reset the input password
      } 
      else 
      {
        input_password += key; // append new character to input password string
        Serial.println(input_password);
        lcd.clear();
        digitalWrite(main_relay,LOW);
        lcd.print("Enter password: ");
        lcd.setCursor(0,1);
        lcd.print(input_password);
      }
    }
  }  
  }


void fuel()
{
  lcd.clear();
  fuel_level=digitalRead(float_sens);
  if(fuel_level==0)
  {
    Serial.println("LOW FUEL!!!!!");
    lcd.print("  LOW FUEL");
     lcd.setCursor(0,1 );
     lcd.print("  FILL FUEL");
     digitalWrite(main_relay,LOW);
    delay(2000);
   while(1)
   {
  }
  }

  else
  {
    digitalWrite(main_relay,HIGH);
    lcd.print("IGNITION ON");
    delay(2000);
    lcd.clear();
    lcd.print("Drive Safely ;)");
    delay(1000);
  }
}

void alcohol()
{
  lcd.clear();
  Serial.print("Alcohol level: ");
  alco_level=analogRead(alco);
  Serial.println(alco_level);
  if(alco_level>200)
  {
    Serial.println("Alcohol level exceeded");
    lcd.print("Alcohol Level");
    digitalWrite(main_relay,LOW);
    lcd.setCursor(0,1);
    lcd.print("Exceeded:");
     lcd.print(alco_level);
    delay(2000);
    lcd.clear();
    lcd.print("Not Safe");
    lcd.setCursor(0,1);
    lcd.print("        To Drive");
   while(1)
   {
     }
   }

  else
  {
    lcd.setCursor(1,0);
    lcd.print("Alcohol Level:    ");
     lcd.setCursor(0,1);
     lcd.print("NO Alcohol:");
    lcd.print(alco_level);
    delay(2000);
   lcd.clear();
   
    fuel();
    drive();
    EstablishTCPconnection();
    UpdatetoTS();
  }
}

void helmet()
{
  lcd.clear();
  limit_state=digitalRead(limit);
  while(digitalRead(limit)==1)
  {
    lcd.print("WEAR HELMET!!!!!!");
    digitalWrite(main_relay,LOW);
    delay(500);
    lcd.clear();
    delay(500);
  }
  lcd.print("HELMET ON");
  delay(2000);
  alcohol();
  lcd.clear();

}

void drive()
{
      int distance=ultrasonic.read();
      if(distance<10)    
    
    {
     lcd.setCursor(1, 0);
     lcd.print("NOT SAFE:  ");
       lcd.print(distance);
        delay(3000);   
        lcd.clear();
        Serial.println("not safe");    
        
                                                       Serial.println(distance);
    }
  else
  {
     lcd.setCursor(0, 1);                                 Serial.println("safe");
    lcd.print("Safe:");
   lcd.print(distance);
   delay(3000);
   lcd.clear();
  }

}
void EstablishTCPconnection()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection
  cmd += "184.106.153.149";                       // api.thingspeak.com
  cmd += "\",80";                                 // port 80

  ser.println(cmd);
  Serial.println(cmd);
  if (ser.find("Error")) 
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

void UpdatetoTS()
 {

lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connecting to");
  lcd.setCursor(0, 1);
  lcd.print("HOTSPOT");
   delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("USER:SATURDAY");
  lcd.setCursor(0, 1);
  lcd.print("PASS:SATURDAY");
   delay(5000);
   
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DATA SEND TO");
  lcd.setCursor(0, 1);
  lcd.print("   API / IOT");
  delay(2000);

  int distance=ultrasonic.read();
  int limit_state=digitalRead(limit);
  int alco_level=analogRead(alco);
  int fuel_level=digitalRead(float_sens);
  
  String state1=String(distance);                              
  String state2=String(limit_state);                
  String state3=String(alco_level);    
  String state4=String(fuel_level);    
                    
                                                              //   
                                                              //  String cmd = "AT+CIPSTART=\"TCP\",\"";          
                                                              //  cmd += "184.106.153.149";                       
                                                              //  cmd += "\",80";                                 
                                                              //  ser.println(cmd);
                                                              //  Serial.println(cmd);
                                                              //  if(ser.find("Error"))
                                                              //  {
                                                              //    Serial.println("AT+CIPSTART error");
                                                              //    //return;
                                                              //  }
  String getStr = "GET /update?api_key=";         
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(state1);                     
  getStr +="&field2=";
  getStr += String(state2);                    
  getStr +="&field3=";
  getStr += String(state3);                       
    getStr +="&field4=";
  getStr += String(state4);    
  
  getStr += "\r\n\r\n";
  
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());   
             
  ser.println(cmd);
  Serial.println(cmd);
  
  if(ser.find(">"))
  {
      ser.print(getStr);
      Serial.print(getStr);
  }
  else
  {
      ser.println("AT+CIPCLOSE");                 
      // alert user
      Serial.println("AT+CIPCLOSE");
   }
  delay(1000);                                 
}
