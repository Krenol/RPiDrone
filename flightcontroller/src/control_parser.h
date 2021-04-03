#ifndef _CONTROL_PARSER_H_
#define _CONTROL_PARSER_H_

struct Controls
{
    float roll_angle, pitch_angle , yaw_rate;
    int throttle;
};

class ControlParser {
    private:
        char *str, *sptr; 

    public:
        void parse(Controls *c, char *msg, const char delim);
};

#endif /* _CONTROL_PARSER_H_ */