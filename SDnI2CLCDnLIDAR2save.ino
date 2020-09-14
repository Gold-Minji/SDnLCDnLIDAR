#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h> // Load SPI Library
#include "Wire.h"//imports the wire library for talking over I2C

File mydata;

LiquidCrystal_I2C lcd(0x3F,16,2);

//SoftwareSerial port(TX, RX);
SoftwareSerial portOne(8, 8);
SoftwareSerial portTwo(9, 9);



void getTFminiData(SoftwareSerial* port, int* distance, int* strength, boolean* complete) {
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];

  port->listen();
  if (port->available()) {
    rx[i] = port->read();
    if (rx[0] != 0x59) {
      i = 0;
    } else if (i == 1 && rx[1] != 0x59) {
      i = 0;
    } else if (i == 8) {
      for (j = 0; j < 8; j++) {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256)) {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
        *complete = true;
      }
      i = 0;
    } else {
      i++;
    }
  }
}

void setup() {

  Serial.begin(115200);
  portOne.begin(115200);
  portTwo.begin(115200);
  


  lcd.init();//i2c lcd initialize
  lcd.backlight();//i2c lcd backlight turn on

  

  
}

void loop() {
  int distance1 = 0;
  int strength1 = 0;
  boolean receiveComplete1 = false;

  int distance2 = 0;
  int strength2 = 0;
  boolean receiveComplete2 = false;

  

  if(mydata){
    while (!receiveComplete1) {
      getTFminiData(&portOne, &distance1, &strength1, &receiveComplete1);
      if (receiveComplete1) {
        
        Serial.print("distance1: ");
        Serial.print(distance1);
        Serial.print("cm,  ");
        Serial.print("strength1: ");
        Serial.print(strength1);
        Serial.print("\t  ");
        
        lcd.setCursor(0, 0);
        lcd.print("lidar1: working");

        delay (100);

       
      }
      else {
        lcd.setCursor(0, 0);
        lcd.print("lidar1: stop");
      }
    }
    receiveComplete1 = false;
    delay(1000);
    lcd.clear();

    while (!receiveComplete2) {
      getTFminiData(&portTwo, &distance2, &strength2, &receiveComplete2);
      if (receiveComplete2) {
        
        Serial.print("distance2: ");      
        Serial.print(distance2);
        Serial.print("cm,  ");
        Serial.print("strength2: ");
        Serial.println(strength2);
        
        lcd.setCursor(0, 1);
        lcd.print("lidar2: working");

        delay (100);

       
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("lidar2: stop");
      }
    }
    receiveComplete2 = false;
    delay(1000);
    lcd.clear();  
   
    }
  }
