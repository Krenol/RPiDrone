/**
 * @author: Lauritz Gröger
 * developed for the Arduino MKI 1010
 */


#include "I2Cdev.h"
#include "src/sensors.h"
#include "src/ESC.h"
#include "Wire.h"
#include "Servo.h"
#include "src/PID.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "src/serial_comm.h"

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

ESC esc;
PID p;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(1);
long last = millis();
int speed = 200;
YPR ypr;
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
char msg[256];
float h, t;

void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  
  //esc.init(6, 800, 2100, speed, true);
  
  //initMPU6050();
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  sensors_event_t event;
  bmp.getEvent(&event);
  while(!event.pressure){
    bmp.getEvent(&event);
  }
  seaLevelPressure = event.pressure;
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================




void loop() {
  //print();
  
//  if(speed > 0) {
//    speed-=10;
//    esc.setSpeed(speed);
//  }
//  print();
//  Serial.print("Took me: ");
//  Serial.print(millis() - last);
//  Serial.print("ms\n");
//  last = millis();
 /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);


   //readString(msg, 256);
   //Serial.println(msg);
 
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    h = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
    bmp.getTemperature(&t);
    returnData(&ypr, h, t);
  }
  returnData(&ypr, h, t);
  delay(1000);
}
