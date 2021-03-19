#include "stdlib.h"
#include "stdio.h"
#include "string.h"

namespace drone
{
    namespace logs
    {
        int parseLine(char *line)
        {
            // This assumes that a digit will be found and the line ends in " Kb".
            int i = strlen(line);
            const char *p = line;
            while (*p < '0' || *p > '9')
                p++;
            line[i - 3] = '\0';
            i = atoi(p);
            return i;
        }

        float getVirtualMemoryConsumption()
        {
            //Note: this value is in MB!
            FILE *file = fopen("/proc/self/status", "r");
            int result = -1;
            char line[128];

            while (fgets(line, 128, file) != NULL)
            {
                if (strncmp(line, "VmSize:", 7) == 0)
                {
                    result = parseLine(line);
                    break;
                }
            }
            fclose(file);
            return result * 0.001;
        }

        float getMemoryConsumption()
        {
            //Note: this value is in MB!
            FILE *file = fopen("/proc/self/status", "r");
            int result = -1;
            char line[128];

            while (fgets(line, 128, file) != NULL)
            {
                if (strncmp(line, "VmRSS:", 6) == 0)
                {
                    result = parseLine(line);
                    break;
                }
            }
            fclose(file);
            return result * 0.001;
        }
    }
} // namespace drone
