#ifndef _SERIAL_COMM_H_
#define _SERIAL_COMM_H_

#include <Arduino.h>
#include "sensors.h"

bool readString(char *arr, int arr_size, char delimiter = '\n')
{
    memset(arr, 0, arr_size);
    int i = 0;
    if (Serial.available())
    {
        do
        {
            arr[i] = Serial.read();
            ++i;
        } while (Serial.available() && i < arr_size - 1 && arr[i - 1] != delimiter);
    }
    arr[i] = '\0';
    return i > 0;
}

bool readConfig(char *arr, int arr_size, char delimiter = '\n'){
    readString(arr, arr_size, delimiter);
    
}

bool readControl(char *arr, int arr_size, char delimiter = '\n') {
    //pitch angle, yaw rate, roll angle, throttle
}

void returnData(YPR* ypr, float h, float t) {
    Serial.print("Y");
    Serial.print(ypr->yaw);
    Serial.print(";");
    Serial.print("P");
    Serial.print(ypr->pitch);
    Serial.print(";");
    Serial.print("R");
    Serial.print(ypr->roll);
    Serial.print(";");
    Serial.print("H");
    Serial.print(h);
    Serial.print(";");
    Serial.print("T");
    Serial.print(t);
    Serial.print("\n");
}


#endif /* _SERIAL_COMM_H_ */