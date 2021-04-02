#ifndef _COMM_H_
#define _COMM_H_

#define DELIM ';'
#define EOL '\n'

struct config
{
    float ki;
};

struct controls
{
    float roll_angle, pitch_angle , yaw_rate;
    int throttle;
};

class COMM {
    private:
        char *str, *sptr; 
        const char delim = DELIM;
        bool readString(char *arr, int arr_size);
    public:
        COMM();

        void sendData(float yaw, float pitch, float roll, float h, float t);

        bool readConfig(config *c, char *arr, int arr_size);

        bool readControl(controls *c, char *arr, int arr_size);

};


#endif /* _COMM_H_ */