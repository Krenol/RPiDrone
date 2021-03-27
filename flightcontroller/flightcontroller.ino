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

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

ESC esc;
int speed = 200;
PID p;

void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  
  esc.init(6, 800, 2100, speed, true);
  
  initMPU6050();
  // enable Arduino interrupt detection
  
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
long last = millis();
YPR ypr;

void loop() {
  //print();
  
  if(speed > 0) {
    speed-=10;
    esc.setSpeed(speed);
  }
  print();
  Serial.print("Took me: ");
  Serial.print(millis() - last);
  Serial.print("ms\n");
  last = millis();
 
}
