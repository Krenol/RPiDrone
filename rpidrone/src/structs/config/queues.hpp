#ifndef DRONE_STRUCT_CONFIG_QUEUES_H
#define DRONE_STRUCT_CONFIG_QUEUES_H

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct Queues
            {
                int read_size = 100, write_size = 100;

                Queues()
                {
                }

                Queues(int read_size, int write_size)
                {
                    this->read_size = read_size;
                    this->write_size = write_size;
                }

                Queues(const Queues &q)
                {
                    this->read_size = q.read_size;
                    this->write_size = q.write_size;
                }
            };
        }
    }
}
#endif