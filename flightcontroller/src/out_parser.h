#ifndef _OUT_PARSER_H_
#define _OUT_PARSER_H_

#include <Arduino.h>

class OutParser {
    private:
        char *str, *sptr; 

    public:
        void parse(char *msg, int msg_size, char eol, char sep, float yaw, float pitch, float roll, float h, float t);
};
#endif /* _OUT_PARSER_H_ */