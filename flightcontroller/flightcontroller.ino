/**
 * @author: Lauritz Gröger
 * developed for the Arduino MKI 1010
 */

#define MSG_SIZE 256

//#include "I2Cdev.h"
#include "Wire.h"
#include "src/mpu_helper.h"
#include "src/ESC.h"
#include "Servo.h"
#include "src/PID.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "src/COMM.h"

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

ESC esc;
MPU6050 mpu;
PID p;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(1);
long last = millis();
int speed = 200;
YPR ypr_struct;
accel accel_struct;
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
float h, t;
char msg[MSG_SIZE];
COMM comm;

void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  
  //esc.init(6, 800, 2100, speed, true);
  
  //initMPU6050(&mpu);
//  if(!bmp.begin())
//  {
//    /* There was a problem detecting the BMP085 ... check your connections */
//    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
//    while(1);
//  }
//  sensors_event_t event;
//  bmp.getEvent(&event);
//  while(!event.pressure){
//    bmp.getEvent(&event);
//  }
//  seaLevelPressure = event.pressure;
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================


bool r;
controls c;
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
 /* Get a new sensor event 
  sensors_event_t event;
  bmp.getEvent(&event);*/ 


   //readString(msg, 256);
   //Serial.println(msg);
   getYPR(&mpu, &ypr_struct, true);
   getAccel(&mpu, &accel_struct);
 
 
  /* Display the results (barometric pressure is measure in hPa) 
  if (event.pressure)
  {
    h = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
    bmp.getTemperature(&t);
    returnData(&ypr, h, t);
  }*/
  comm.sendData(0,0,0, h, t);
  r = comm.readControl(&c, msg, MSG_SIZE);
  if(r) {
    Serial.print("\n-------\n");
    Serial.print("read:\n");
    Serial.print("yaw: ");
    Serial.print(c.yaw_rate);
    Serial.print("\tpitch: ");
    Serial.print(c.pitch_angle);
    Serial.print("\troll: ");
    Serial.print(c.roll_angle);
    Serial.print("\tthrottle: ");
    Serial.print(c.throttle);
    Serial.print("\n-------\n");
  }
  delay(1000);
}
