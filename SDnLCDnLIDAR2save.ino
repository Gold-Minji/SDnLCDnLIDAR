#include <LiquidCrystal.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h> // Load SPI Library
#include "Wire.h"//imports the wire library for talking over I2C

File mydata;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Define lcd

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



  lcd.begin(16, 2);
  lcd.clear();

  pinMode(10, OUTPUT);

  if (!SD.begin(10)) //10번핀에 SD카드를 read했을때 존재하지 않을 때
  {
    lcd.setCursor(0, 0);
    lcd.print("SD card");
    lcd.setCursor(0, 1);
    lcd.print("Error");
    while (1); //todo edit
  }
}

void loop() {
  int distance1 = 0;
  int strength1 = 0;
  boolean receiveComplete1 = false;

  int distance2 = 0;
  int strength2 = 0;
  boolean receiveComplete2 = false;

  mydata = SD.open("lidar.txt", FILE_WRITE);

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

        mydata.print("distance1: ");
        mydata.print(distance1);
        mydata.print("cm,  ");
        mydata.print("strength1: ");
        mydata.print("\t  ");
        mydata.print(strength1);
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

        mydata.print("distance2: ");
        mydata.print(distance2);
        mydata.print("cm,  ");
        mydata.print("strength2: ");
        mydata.println(strength2);
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("lidar2: stop");
      }
    }
    receiveComplete2 = false;
    delay(1000);
    lcd.clear();  
   
    mydata.close();}
  }
