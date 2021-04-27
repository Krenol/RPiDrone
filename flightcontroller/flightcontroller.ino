/**
 * @author: Lauritz Gröger
 * developed for the Arduino MKI 1010
 */

#define MSG_SIZE 128


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
#include "src/misc.h"

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
// ESCs
ESC lf, rf, lb, rb;
int throttle = 0, lb_t, rb_t, lf_t, rf_t;

// MPU
MPU6050 mpu;
YPR ypr_struct;
accel accel_struct;

// controls
PID roll_vel, roll_t, pitch_vel, pitch_t, yaw_t;
float roll_out, pitch_out, yaw_out, roll_rate, pitch_rate;

// BMP
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(1);
float h, t, seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
sensors_event_t event;

// COMM
ControlParser cntrlPrs;
char msg[MSG_SIZE];
float ypr_arr[3];
bool dataReceived;
OutParser outPrs;
const char DELIM = ';', DELIM_D = '&', EOL = '\n';


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(9600);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  ConfigParser cfgPrs;
  Config conf;
  char conf_msg[256];
  Serial.println(CONFIG_TOKEN);
  dataReceived = readString(conf_msg, 256, EOL);
  while(!dataReceived || cfgPrs.parse(&conf, conf_msg, &DELIM_D, &DELIM) == false){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(CONFIG_TOKEN);
    delay(1000);
    dataReceived = readString(conf_msg, 256, EOL);
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println("<A>");
  lf.init(conf.pin_lf, conf.esc_min, conf.esc_max, 100, conf.calib_esc);
  rf.init(conf.pin_rf, conf.esc_min, conf.esc_max, 100, conf.calib_esc);
  lb.init(conf.pin_lb, conf.esc_min, conf.esc_max, 100, conf.calib_esc);
  rb.init(conf.pin_rb, conf.esc_min, conf.esc_max, 100, conf.calib_esc);
  roll_vel.set(conf.kp_r_v, conf.kd_r_v, conf.ki_r_v, conf.kaw_r_v, conf.min_r_v, conf.max_r_v);
  roll_t.set(conf.kp_r_t, conf.kd_r_t, conf.ki_r_t, conf.kaw_r_t, conf.min_r_t, conf.max_r_t);
  pitch_vel.set(conf.kp_p_v, conf.kd_p_v, conf.ki_p_v, conf.kaw_p_v, conf.min_r_v, conf.max_p_v);
  pitch_t.set(conf.kp_p_t, conf.kd_p_t, conf.ki_p_t, conf.kaw_p_t, conf.min_p_t, conf.max_p_t);
  yaw_t.set(conf.kp_y_t, conf.kd_y_t, conf.ki_y_t, conf.kaw_y_t, conf.min_y_t, conf.max_y_t);
  
  initMPU6050(&mpu);
  if(!bmp.begin())
  {
    Serial.println("error");
    while(1);
  }
  bmp.getEvent(&event);
  while(!event.pressure){
    bmp.getEvent(&event);
  }
  seaLevelPressure = event.pressure;
  Serial.println(CONTROL_TOKEN);
  digitalWrite(LED_BUILTIN, HIGH);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================



void loop() {
  dataReceived = readString(msg, MSG_SIZE, EOL);
  if(dataReceived) {
    cntrlPrs.parse(ypr_arr, &throttle, msg, &DELIM);
  }
  
  getYPR(&mpu, &ypr_struct, true);
  pitch_rate = pitch_vel.control(ypr_struct.pitch, ypr_arr[1]);
  roll_rate = roll_vel.control(ypr_struct.roll, ypr_arr[2]);

  getAccel(&mpu, &accel_struct);
  roll_out = roll_t.control(accel_struct.x, roll_rate);
  pitch_out = pitch_t.control(accel_struct.y, pitch_rate);
  yaw_out = yaw_t.control(accel_struct.z, ypr_arr[0]);

  rf_t = BOUND<int>(throttle + roll_out - pitch_out + yaw_out, 0, 100);
  lf_t = BOUND<int>(throttle + -roll_out - pitch_out - yaw_out, 0, 100);
  rb_t = BOUND<int>(throttle + roll_out + pitch_out - yaw_out, 0, 100);
  lb_t = BOUND<int>(throttle + -roll_out + pitch_out + yaw_out, 0, 100);

  rf.setSpeed(rf_t);
  lf.setSpeed(lf_t);
  rb.setSpeed(rb_t);
  lb.setSpeed(lb_t);

   //Get a new sensor event 
  bmp.getEvent(&event);
  if (event.pressure)
  {
    h = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
    bmp.getTemperature(&t);
  }
  
  outPrs.parse(msg, MSG_SIZE, &EOL, &DELIM, ypr_arr[0], ypr_arr[1], ypr_arr[2], h, t);
  Serial.print(msg);
}
