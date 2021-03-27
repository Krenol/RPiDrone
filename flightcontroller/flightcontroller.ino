#include "I2Cdev.h"
#include "src/sensors.h"
#include "src/ESC.h"
#include "Wire.h"
#include "Servo.h"

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

ESC esc;
int speed = 100;
void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

  
  //initMPU6050();
  // enable Arduino interrupt detection
  //pinMode(INTERRUPT_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
  esc.init(6, 700, 2000, 180, false);
  //esc.setSpeed(speed);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
  //print();
  
  while(speed > 0) {
    Serial.println(speed);
    speed-=10;
    esc.setSpeed(speed);
    delay(1000);
  }
  return;
}
