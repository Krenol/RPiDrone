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

struct accel
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

//uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

volatile bool mpuInterrupt = false;
void dmpDataReady()
{
    mpuInterrupt = true;
}

void print(MPU6050 *mpu)
{
    // if programming failed, don't try to do anything
    if (!dmpReady)
        return;
    // orientation/motion vars
    VectorInt16 aa;      // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements

    // read a packet from FIFO
    if (mpu->dmpGetCurrentFIFOPacket(fifoBuffer))
    { // Get the Latest packet

        // display Euler angles in degrees
        mpu->dmpGetQuaternion(&q, fifoBuffer);
        mpu->dmpGetGravity(&gravity, &q);
        mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
        Serial.print("ypr\t");
        Serial.print(ypr[0] * 180 / M_PI);
        Serial.print("\t");
        Serial.print(ypr[1] * 180 / M_PI);
        Serial.print("\t");
        Serial.println(ypr[2] * 180 / M_PI);

        mpu->dmpGetQuaternion(&q, fifoBuffer);
        mpu->dmpGetAccel(&aa, fifoBuffer);
        mpu->dmpGetGravity(&gravity, &q);
        mpu->dmpGetLinearAccel(&aaReal, &aa, &gravity);
        Serial.print("areal\t");
        Serial.print(aaReal.x);
        Serial.print("\t");
        Serial.print(aaReal.y);
        Serial.print("\t");
        Serial.println(aaReal.z);
    }
}

void getYPR(MPU6050 *mpu, YPR *ypr_struct, bool degrees = true)
{
    if (!dmpReady)
        return;
    
    mpu->dmpGetQuaternion(&q, fifoBuffer);
    mpu->dmpGetGravity(&gravity, &q);
    mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
    if (degrees)
    {
        ypr_struct->yaw = ypr[0] * 180 / M_PI;
        ypr_struct->pitch = ypr[0] * 180 / M_PI;
        ypr_struct->roll = ypr[0] * 180 / M_PI;
    }
    else
    {
        ypr_struct->yaw = ypr[0];
        ypr_struct->pitch = ypr[0];
        ypr_struct->roll = ypr[0];
    }
}

void getAccel(MPU6050 *mpu, accel *a)
{
    if (!dmpReady)
        return;

}

void initMPU6050(MPU6050 *mpu)
{
    uint8_t devStatus;
    uint8_t mpuIntStatus;
    mpu->initialize();

    // wait for ready
    while (Serial.available() && Serial.read())
        ; // empty buffer
    while (!Serial.available())
        ; // wait for data
    while (Serial.available() && Serial.read())
        ; // empty buffer again

    // load and configure the DMP
    devStatus = mpu->dmpInitialize();

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
        pinMode(INTERRUPT_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
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