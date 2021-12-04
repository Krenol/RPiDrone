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
                int read_queue_size = 100, write_queue_size = 100;

                Queues()
                {
                }

                Queues(int read_queue_size, int write_queue_size)
                {
                    this->read_queue_size = read_queue_size;
                    this->write_queue_size = write_queue_size;
                }

                Queues(const Queues &q)
                {
                    this->read_queue_size = q.read_queue_size;
                    this->write_queue_size = q.write_queue_size;
                }
            };
        }
    }
}
#endif