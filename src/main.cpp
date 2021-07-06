//===============================
// Antcelerometer V1
// Another random intern project
// Made by Samson H
//===============================

//Dependencies
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "SPI.h"
#include "SD.h"
#include "Wire.h"

//Object declaration
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

const int leftSwitch = D3;
const int rightSwitch = D4;
const int chipSelect = D8;  // used for ESP8266

//Calibration constants
long xScale = 1;
long yScale = 1.005;
long zScale = 1.02;

void setup() {
  //Begin serial port and set sample speed
  Serial.begin(115200);
  
  //Set pull up resistors for switch
  pinMode(leftSwitch, INPUT_PULLUP);
  pinMode(rightSwitch, INPUT_PULLUP);

  //Attempt to connect to MCU over I2C
  if (!mpu.begin()) {
    while (1)
      yield();
  }

  //Attempt to connect OLED over I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 not found"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  //Attempt to connect to SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    while (1);
  }

  Wire.setClock(400000L);

  //Turn on OLED
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  
}

void loop() {
  sensors_event_t a, g, temp;
  //Store values in pointer 
  mpu.getEvent(&a, &g, &temp);

  display.clearDisplay();
  display.setCursor(0, 10);

  // display.println("Gyroscope (rad/s):");
  // display.print(g.gyro.x, 1);
  // display.print(", ");
  // display.print(g.gyro.y, 1);
  // display.print(", ");
  // display.print(g.gyro.z, 1);
  // display.println("");

  if(digitalRead(leftSwitch) == LOW){
    
    File dataFile = SD.open("Logging.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("A,");
      dataFile.print(a.acceleration.x * xScale);
      dataFile.print(",");
      dataFile.print(a.acceleration.y * yScale);
      dataFile.print(",");
      dataFile.print(a.acceleration.z * zScale);
      dataFile.println("");
      dataFile.close();
    }
    
    display.print("Logging");
  }else{
    display.println("AntCelerometer v1");
    display.println("=================");;
    display.println("Acceleration (m/s^2):");
    display.print(a.acceleration.x * xScale, 1);
    display.print(", ");
    display.print(a.acceleration.y * yScale, 1);
    display.print(", ");
    display.print(a.acceleration.z * zScale, 1);
    display.println("");
  }

  if(digitalRead(rightSwitch) == HIGH){
    display.print("ZERO");
  }

  display.display();

}


// Basic demo for accelerometer readings from Adafruit LIS3DH

// #include <Wire.h>
// #include <SPI.h>
// #include <Adafruit_LIS3DH.h>
// #include <Adafruit_Sensor.h>

// // Used for software SPI
// #define LIS3DH_CLK 13
// #define LIS3DH_MISO 12
// #define LIS3DH_MOSI 11
// // Used for hardware & software SPI
// #define LIS3DH_CS 10

// // software SPI
// //Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// // hardware SPI
// //Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// // I2C
// Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// void setup(void) {
//   Serial.begin(9600);
//   while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

//   Serial.println("LIS3DH test!");

//   if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
//     Serial.println("Couldnt start");
//     while (1) yield();
//   }
//   Serial.println("LIS3DH found!");

//   // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

//   Serial.print("Range = "); Serial.print(2 << lis.getRange());
//   Serial.println("G");

//   // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
//   Serial.print("Data rate set to: ");
//   switch (lis.getDataRate()) {
//     case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
//     case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
//     case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
//     case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
//     case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
//     case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
//     case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

//     case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
//     case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
//     case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
//   }
// }

// void loop() {
//   lis.read();      // get X Y and Z data at once
//   // Then print out the raw data
//   Serial.print("X:  "); Serial.print(lis.x);
//   Serial.print("  \tY:  "); Serial.print(lis.y);
//   Serial.print("  \tZ:  "); Serial.print(lis.z);

//   /* Or....get a new sensor event, normalized */
//   sensors_event_t event;
//   lis.getEvent(&event);

//   /* Display the results (acceleration is measured in m/s^2) */
//   Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
//   Serial.print(" \tY: "); Serial.print(event.acceleration.y);
//   Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
//   Serial.println(" m/s^2 ");

//   Serial.println();

//   delay(200);
// }

// #include "SPI.h"
// #include "SD.h"
// #include "Wire.h"

// const int chipSelect = D8;  // used for ESP8266
// int x = 0;

// void setup() {
//   Serial.begin(9600);
  
//   while (!Serial) {
//   }

//   if (!SD.begin(chipSelect)) {
//     Serial.println("Initialization failed!");
//     while (1);
//   }

// }

// void loop() {

//   String dataString = "Logged:" + String(x);
  
//   File dataFile = SD.open("test.txt", FILE_WRITE);

//   if (dataFile) {
//     dataFile.println(dataString);
//     dataFile.close();
//     Serial.println(dataString);
//     x++;
//   }
  
//   else {
//     Serial.println("error opening datalog.txt");
//   }

//   delay(1);
// }