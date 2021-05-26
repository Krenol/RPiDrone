#ifndef _MPU_HELPER_H_
#define _MPU_HELPER_H_

#include "I2Cdev.h"
#include "MPU6050/MPU6050_6Axis_MotionApps20.h"

#define INTERRUPT_PIN 2


// ===========
// == MPU Defs
// ============
struct YPR
{
    float yaw = 0, pitch = 0, roll = 0;
};

struct rotation
{
    float x = 0, y = 0, z = 0;
};

//MPU6050 mpu;
// MPU control/status vars
bool dmpReady = false; // set true if DMP init was successful
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
uint8_t fifoBuffer[64]; // FIFO storage buffer
int16_t rotx,roty,rotz;

//uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

volatile bool mpuInterrupt = false;
void dmpDataReady()
{
    mpuInterrupt = true;
}

void getYPR(MPU6050 *mpu, YPR *ypr_struct, bool degrees = true)
{
    if (!dmpReady)
        return;
    if(mpu->dmpGetCurrentFIFOPacket(fifoBuffer)){
        mpu->dmpGetQuaternion(&q, fifoBuffer);
        mpu->dmpGetGravity(&gravity, &q);
        mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
        if (degrees)
        {
            ypr_struct->yaw = ypr[0] * 180 / M_PI;
            ypr_struct->pitch = ypr[1] * 180 / M_PI;
            ypr_struct->roll = ypr[2] * 180 / M_PI;
        }
        else
        {
            ypr_struct->yaw = ypr[0];
            ypr_struct->pitch = ypr[1];
            ypr_struct->roll = ypr[2];
        }
    }
}


void getMeasurements(MPU6050 *mpu, rotation *a, YPR *ypr_struct, bool degrees = true) {
    if (!dmpReady)
        return;
    if(mpu->dmpGetCurrentFIFOPacket(fifoBuffer)){
        mpu->dmpGetQuaternion(&q, fifoBuffer);
        mpu->dmpGetGravity(&gravity, &q);
        mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
        if (degrees)
        {
            ypr_struct->yaw = ypr[0] * 180 / M_PI;
            ypr_struct->pitch = ypr[1] * 180 / M_PI;
            ypr_struct->roll = ypr[2] * 180 / M_PI;
        }
        else
        {
            ypr_struct->yaw = ypr[0];
            ypr_struct->pitch = ypr[1];
            ypr_struct->roll = ypr[2];
        }
        mpu->getRotation(&rotx, &roty, &rotz);
        a->x = rotx;
        a->y = roty;
        a->z = rotz;
    }
}

void initMPU6050(MPU6050 *mpu)
{
    uint8_t devStatus;
    uint8_t mpuIntStatus;
    mpu->initialize();
    Serial.println(mpu->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    // load and configure the DMP
    devStatus = mpu->dmpInitialize(10);

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu->setXGyroOffset(220);
    mpu->setYGyroOffset(76);
    mpu->setZGyroOffset(-85);
    mpu->setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu->CalibrateAccel(6);
        mpu->CalibrateGyro(6);
        mpu->PrintActiveOffsets();
        mpu->setDMPEnabled(true);

        mpuIntStatus = mpu->getIntStatus();
        dmpReady = true;
        // get expected DMP packet size for later comparison
        //packetSize = mpu->dmpGetFIFOPacketSize();
        //pinMode(INTERRUPT_PIN, INPUT);
        //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

#endif /* _MPU_HELPER_H_ */