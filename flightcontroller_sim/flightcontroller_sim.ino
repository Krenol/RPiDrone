/**
  /**
   @author: Lauritz Gröger
   developed for the Arduino MKI 1010
*/

#define MSG_SIZE 256
#define THROTTLE_MAX 100
#define ESC_MAX 120 // we need buffer to still be able to control the drone at top speed
#define ACK_TOKEN "<A>"

//#include "I2Cdev.h"
#include "Wire.h"
#include "src/imu.h"
#include "src/ESC.h"
#include "Servo.h"
#include "src/PID.h"
#include "src/config_parser.h"
#include "src/control_parser.h"
#include "src/serial_reader.h"
#include "src/out_parser.h"
#include "src/misc.h"
#include <string.h>




// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
// ESCs
ESC lf, rf, lb, rb;
int throttle = 0, lb_t, rb_t, lf_t, rf_t;

// MPU
ICM_20948_I2C imu;
YPR ypr_struct;
rotation accel_struct;

// controls
PID roll_vel, roll_t, pitch_vel, pitch_t, yaw_t;
float roll_out, pitch_out, yaw_out, roll_rate, pitch_rate;

// COMM
ControlParser cntrlPrs;
char msg[MSG_SIZE];
float ypr_arr[3];
bool dataReceived;
OutParser outPrs;
const char DELIM = ';', DELIM_D = '&', EOL = '\n';

//timing
int timestamp;


void getConf(Config *conf) {
  ConfigParser cfgPrs;
  char conf_msg[256];
  Serial.println(CONFIG_TOKEN);
  dataReceived = readString(conf_msg, 256, EOL);
  while (!dataReceived || cfgPrs.parse(conf, conf_msg, &DELIM_D, &DELIM) == false) {
    Serial.println(CONFIG_TOKEN);
    delay(200);
    dataReceived = readString(conf_msg, 256, EOL);
  }
}

void initPIDs(Config *conf) {
  roll_vel.set(conf->kp_r_v, conf->kd_r_v, conf->ki_r_v, conf->kaw_r_v, conf->min_r_v, conf->max_r_v);
  roll_t.set(conf->kp_r_t, conf->kd_r_t, conf->ki_r_t, conf->kaw_r_t, conf->min_r_t, conf->max_r_t);
  pitch_vel.set(conf->kp_p_v, conf->kd_p_v, conf->ki_p_v, conf->kaw_p_v, conf->min_r_v, conf->max_p_v);
  pitch_t.set(conf->kp_p_t, conf->kd_p_t, conf->ki_p_t, conf->kaw_p_t, conf->min_p_t, conf->max_p_t);
  yaw_t.set(conf->kp_y_t, conf->kd_y_t, conf->ki_y_t, conf->kaw_y_t, conf->min_y_t, conf->max_y_t);
}

void initESCs(Config *conf) {
  lf.init(conf->pin_lf, conf->esc_min, conf->esc_max, ESC_MAX, conf->calib_esc);
  rf.init(conf->pin_rf, conf->esc_min, conf->esc_max, ESC_MAX, conf->calib_esc);
  lb.init(conf->pin_lb, conf->esc_min, conf->esc_max, ESC_MAX, conf->calib_esc);
  rb.init(conf->pin_rb, conf->esc_min, conf->esc_max, ESC_MAX, conf->calib_esc);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock
  Serial.begin(115200);
  while (!Serial);
  Config conf;
  getConf(&conf);
  Serial.println(ACK_TOKEN);
  initESCs(&conf);
  initPIDs(&conf);
  Serial.println("<A1>");
  initIMU(&imu);
  Serial.println("<A2>");
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  imu.resetFIFO();
  Serial.println(CONTROL_TOKEN);
  timestamp = millis();
}




// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
  dataReceived = readString(msg, MSG_SIZE, EOL);
  if (dataReceived) {
    if (strncmp(CONFIG_TOKEN, msg, 3) == 0 && throttle == 0) {
      Config conf;
      ConfigParser cfgPrs;
      if(cfgPrs.parse(&conf, msg, &DELIM_D, &DELIM)) {
        Serial.println("<A>");
        initPIDs(&conf);
        delay(100);
        Serial.println(CONTROL_TOKEN);
      }
    } else {
      cntrlPrs.parse(ypr_arr, &throttle, msg, &DELIM);
      throttle = BOUND<int>(throttle, 0, THROTTLE_MAX);
    }
  }

  getMeasurements(&imu, &accel_struct, &ypr_struct);
  pitch_rate = pitch_vel.control(ypr_struct.pitch, ypr_arr[1]);
  roll_rate = roll_vel.control(ypr_struct.roll, ypr_arr[2]);

  roll_out = roll_t.control(accel_struct.x, roll_rate);
  pitch_out = pitch_t.control(accel_struct.y, pitch_rate);
  yaw_out = yaw_t.control(accel_struct.z, ypr_arr[0]);

  rf_t = BOUND<int>(throttle + roll_out - pitch_out + yaw_out, 0, ESC_MAX);
  lf_t = BOUND<int>(throttle + -roll_out - pitch_out - yaw_out, 0, ESC_MAX);
  rb_t = BOUND<int>(throttle + roll_out + pitch_out - yaw_out, 0, ESC_MAX);
  lb_t = BOUND<int>(throttle + -roll_out + pitch_out + yaw_out, 0, ESC_MAX);

  rf.setSpeed(rf_t);
  lf.setSpeed(lf_t);
  rb.setSpeed(rb_t);
  lb.setSpeed(lb_t);

  outPrs.parse(msg, MSG_SIZE, EOL, DELIM, ypr_struct.yaw, ypr_struct.pitch, ypr_struct.roll, millis() - timestamp, accel_struct.x, accel_struct.y, accel_struct.z, rf_t, rb_t, lf_t, lb_t, throttle, ypr_arr);
  Serial.print(msg);
  timestamp = millis();
}
