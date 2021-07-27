#ifndef _SERIAL_READER_H_
#define _SERIAL_READER_H_

#include <Arduino.h>

bool readString(char *msg, int msg_size, char eol) 
{
    memset(msg, 0, msg_size);
    int i = 0;
    if (Serial.available())
    {
        do
        {
            msg[i] = Serial.read();
            ++i;
        } while (Serial.available() && i < msg_size - 1 && msg[i - 1] != eol);
    }
    msg[i] = '\0';
    return i > 0;
}

#endif /* _SERIAL_READER_H_ */

