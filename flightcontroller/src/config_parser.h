#ifndef _COMM_H_
#define _COMM_H_

#define CONFIG_TOKEN "<C>"

struct Config
{
    //controls for roll
    float ki_r_v, kd_r_v, kp_r_v, kaw_r_v, min_r_v, max_r_v;
    float ki_r_t, kd_r_t, kp_r_t, kaw_r_t, min_r_t, max_r_t;
    //controls for pitch
    float ki_p_v, kd_p_v, kp_p_v, kaw_p_v, min_p_v, max_p_v;
    float ki_p_t, kd_p_t, kp_p_t, kaw_p_t, min_p_t, max_p_t;
    //controls for yaw
    float ki_y_t, kd_y_t, kp_y_t, kaw_y_t, min_y_t, max_y_t;
    //esc
    int esc_min, esc_max, pin_lb, pin_lf, pin_rf, pin_rb;
    bool calib_esc = false;
};

class ConfigParser {
    private:
        void parsePID(Config *c, char *msg, const char *delim);
        void parseESC(Config *c, char *msg, const char *delim);
        void parsePIDLine(char *line, float *vals, const char *delim);

    public:
        bool parse(Config *c, char *msg, const char *delim1, const char *delim2);
};


#endif /* _COMM_H_ */