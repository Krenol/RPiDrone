#ifndef _COMM_H_
#define _COMM_H_

struct Config
{
    float ki;
};

class ConfigParser {
    private:
        char *str, *sptr; 
    public:
        void parse(Config *c, char *msg, const char delim);
};


#endif /* _COMM_H_ */