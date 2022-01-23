#ifndef DRONE_STRUCT_MIDDLEWARE_METADATA_H
#define DRONE_STRUCT_MIDDLEWARE_METADATA_H

namespace drone
{
    namespace structs
    {
        namespace middleware
        {
            struct Metadata
            {
                int fcExecutionTime = 0, throttleLeftFront = 0, throttleLeftBack = 0, throttleRightFront = 0, throttleRightBack = 0;
                Metadata()
                {
                }

                Metadata(int fcExecutionTime, int throttleLeftFront, int throttleLeftBack, int throttleRightFront, int throttleRightBack) 
                {
                    this->fcExecutionTime = fcExecutionTime;
                    this->throttleLeftFront = throttleLeftFront;
                    this->throttleLeftBack = throttleLeftBack;
                    this->throttleRightFront = throttleRightFront;
                    this->throttleRightBack = throttleRightBack;
                }

                Metadata(const Metadata &clientInput) 
                {
                    this->fcExecutionTime = clientInput.fcExecutionTime;
                    this->throttleLeftFront = clientInput.throttleLeftFront;
                    this->throttleLeftBack = clientInput.throttleLeftBack;
                    this->throttleRightFront = clientInput.throttleRightFront;
                    this->throttleRightBack = clientInput.throttleRightBack;
                }
            };
        }
    }
}
#endif