#include "out_parser.h"

void OutParser::parse(char *msg, int msg_size, const char *eol, const char *sep, float yaw, float pitch, float roll, float h, float t) {
    String(String("Y") + yaw + sep + "P" + pitch + sep +  "R" + roll + sep + "H" + h + sep + "T" + t + eol).toCharArray(msg, msg_size);
}