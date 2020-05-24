#ifndef RPIDRONE_SUBSCRIBER_H
#define RPIDRONE_SUBSCRIBER_H

namespace rpidrone
{
    template <class T>
    class Subscriber{
        public:
            virtual void update(const T& msg) = 0;
    };
}

#endif