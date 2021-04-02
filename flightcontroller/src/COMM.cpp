#include "COMM.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void COMM::sendData(float yaw, float pitch, float roll, float h, float t) 
{
    Serial.print("Y");
    Serial.print(yaw);
    Serial.print(";");
    Serial.print("P");
    Serial.print(pitch);
    Serial.print(";");
    Serial.print("R");
    Serial.print(roll);
    Serial.print(";");
    Serial.print("H");
    Serial.print(h);
    Serial.print(";");
    Serial.print("T");
    Serial.print(t);
    Serial.print(EOL);
}

bool COMM::readConfig(config *c, char *arr, int arr_size) 
{
    
}

bool COMM::readControl(controls *c, char *arr, int arr_size) 
{
    //pitch angle, yaw rate, roll angle, throttle
    bool read = readString(arr, arr_size);
    if(!read) return false;
    
    c->yaw_rate = 0.0f;
    c->pitch_angle = 0.0f;
    c->roll_angle = 0.0f;
    c->throttle = 0;
    for (;; arr = NULL)
    {
        str = strtok_r(arr, &delim, &sptr);

        if(str == NULL) break;
        switch (str[0])
        {
        case 'Y':
            c->yaw_rate = atof(str + 1);
            break;
        case 'P':
            c->pitch_angle = atof(str + 1);
            break;
        case 'R':
            c->roll_angle = atof(str + 1);
            break;
        case 'S':
            c->throttle = atoi(str + 1);
            break;
        default:
            break;
        }
    }
    return true;
}

bool COMM::readString(char *arr, int arr_size) 
{
    memset(arr, 0, arr_size);
    int i = 0;
    if (Serial.available())
    {
        do
        {
            arr[i] = Serial.read();
            ++i;
        } while (Serial.available() && i < arr_size - 1 && arr[i - 1] != EOL);
    }
    arr[i] = '\0';
    return i > 0;
}

COMM::COMM() 
{
    
}
