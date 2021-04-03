/**
 * @author: Lauritz Gröger
 * developed for the Arduino MKI 1010
 */

#define MSG_SIZE 128
#define EOL '\n'
#define DELIM ';'

//#include "I2Cdev.h"
#include "Wire.h"
#include "src/mpu_helper.h"
#include "src/ESC.h"
#include "Servo.h"
#include "src/PID.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "src/config_parser.h"
#include "src/control_parser.h"
#include "src/serial_reader.h"
#include "src/out_parser.h"

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

// ESCs
ESC lf, ef, lb, rb;
int speed = 0;
// MPU
MPU6050 mpu;
YPR ypr_struct;
accel accel_struct;
// controls
PID roll_ang, roll_vel, pitch_ang, pitch_vel, yaw_vel;
// BMP
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(1);
float h, t, seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
// COMM
ControlParser cntrlPrs;
char msg[MSG_SIZE];
Controls cntrls;
bool dataReceived;
OutParser outPrs;
String out;


void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  ConfigParser cfgPrs;
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
  
  outPrs.parse(msg, MSG_SIZE, EOL, DELIM, 0,0,0, h, t);
  Serial.print(msg);
  dataReceived = readString(msg, MSG_SIZE, EOL);
  if(dataReceived) {
    cntrlPrs.parse(&cntrls, msg, DELIM);
    Serial.print("\n-------\n");
    Serial.print("read:\n");
    Serial.print("yaw: ");
    Serial.print(cntrls.yaw_rate);
    Serial.print("\tpitch: ");
    Serial.print(cntrls.pitch_angle);
    Serial.print("\troll: ");
    Serial.print(cntrls.roll_angle);
    Serial.print("\tthrottle: ");
    Serial.print(cntrls.throttle);
    Serial.print("\n-------\n");
  }
  delay(1000);
}
