#include "out_parser.h"

void OutParser::parse(char *msg, int msg_size, const char *eol, const char *sep, float yaw, float pitch, float roll, float h, float t) {
    String(yaw + sep + pitch + sep + roll + sep + h + sep + t + eol).toCharArray(msg, msg_size);
}