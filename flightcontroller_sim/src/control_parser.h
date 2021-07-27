#ifndef _CONTROL_PARSER_H_
#define _CONTROL_PARSER_H_

#define CONTROL_TOKEN "<S>"

class ControlParser {
    private:
        char *str, *sptr; 

    public:
        void parse(float* ypr, int *throttle, char *msg, const char *delim);
};

#endif /* _CONTROL_PARSER_H_ */