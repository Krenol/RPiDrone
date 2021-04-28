#ifndef _OUT_PARSER_H_
#define _OUT_PARSER_H_

#include <Arduino.h>

class OutParser {
    private:
        char *str, *sptr; 

    public:
        void parse(char *msg, int msg_size, const char eol, const char sep, float yaw, float pitch, float roll, int exec, float ax, float ay, float az, int rf_t, int rb_t, int lf_t, int lb_t);
};
#endif /* _OUT_PARSER_H_ */