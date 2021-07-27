#ifndef _IMU_H_
#define _IMU_H_

#include "icm20948/ICM_20948.h"

#define INTERRUPT_PIN 2

struct YPR
{
    float yaw = 0, pitch = 0, roll = 0;
};

struct rotation
{
    float x = 0, y = 0, z = 0;
};

bool dmpReady = false; // set true if DMP init was successful
icm_20948_DMP_data_t data;
double q0, q1, q2, q3, q2sqr, t0, t1, t2, t3, t4;

//uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

void getMeasurements(ICM_20948_I2C *imu, rotation *a, YPR *ypr_struct, bool degrees = true)
{
    if (!dmpReady)
        return;
    imu->readDMPdataFromFIFO(&data);
    if (imu->status == ICM_20948_Stat_Ok)
    {
        if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
        {

            // Scale to +/- 1
            q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
            q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
            q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

            // Convert the quaternions to Euler angles (roll, pitch, yaw)
            // https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

            q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

            q2sqr = q2 * q2;

            // roll (x-axis rotation)
            t0 = +2.0 * (q0 * q1 + q2 * q3);
            t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
            ypr_struct->roll = atan2(t0, t1);

            // pitch (y-axis rotation)
            t2 = +2.0 * (q0 * q2 - q3 * q1);
            t2 = t2 > 1.0 ? 1.0 : t2;
            t2 = t2 < -1.0 ? -1.0 : t2;
            ypr_struct->pitch = asin(t2);

            // yaw (z-axis rotation)
            t3 = +2.0 * (q0 * q3 + q1 * q2);
            t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
            ypr_struct->yaw = atan2(t3, t4);
        }
   
        a->x = imu->gyrX() * M_PI / 180.0;
        a->y = imu->gyrY() * M_PI / 180.0;
        a->z = imu->gyrZ() * M_PI / 180.0;
    }
}

void initIMU(ICM_20948_I2C *imu, int ado_val = 1)
{
    imu->begin(Wire, ado_val);
    if (imu->status != ICM_20948_Stat_Ok)
    {
        Serial.println(F("Error while initializing IMU"));
        return;
    }
    dmpReady = true;
    dmpReady &= (imu->initializeDMP() == ICM_20948_Stat_Ok);
    // Enable the DMP Game Rotation Vector sensor (Quat6)
    dmpReady &= (imu->enableDMPSensor(INV_ICM20948_SENSOR_GRAVITY) == ICM_20948_Stat_Ok);

    // Enable additional sensors / features
    dmpReady &= (imu->enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE) == ICM_20948_Stat_Ok);
    //dmpReady &= (imu->enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);
    //dmpReady &= (imu->enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED) == ICM_20948_Stat_Ok);
    //dmpReady &= (imu->enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);

    // Configuring DMP to output data at multiple ODRs:
    // DMP is capable of outputting multiple sensor data at different rates to FIFO.
    // Setting value can be calculated as follows:
    // Value = (DMP running rate / ODR ) - 1
    // E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.
    dmpReady &= (imu->setDMPODRrate(DMP_ODR_Reg_Quat6, 0) == ICM_20948_Stat_Ok);        // Set to max

    dmpReady &= (imu->setDMPODRrate(DMP_ODR_Reg_Gyro, 54) == ICM_20948_Stat_Ok);         // Set to 1Hz
    dmpReady &= (imu->setDMPODRrate(DMP_ODR_Reg_Gyro_Calibr, 54) == ICM_20948_Stat_Ok);  // Set to 1Hz
    //dmpReady &= (imu->setDMPODRrate(DMP_ODR_Reg_Cpass, 54) == ICM_20948_Stat_Ok);        // Set to 1Hz
    //dmpReady &= (imu->setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 54) == ICM_20948_Stat_Ok); // Set to 1Hz

    // Enable the FIFO
    dmpReady &= (imu->enableFIFO() == ICM_20948_Stat_Ok);

    // Enable the DMP
    dmpReady &= (imu->enableDMP() == ICM_20948_Stat_Ok);

    // Reset DMP
    dmpReady &= (imu->resetDMP() == ICM_20948_Stat_Ok);

    // Reset FIFO
    dmpReady &= (imu->resetFIFO() == ICM_20948_Stat_Ok);
}

#endif /* _IMU_H_ */