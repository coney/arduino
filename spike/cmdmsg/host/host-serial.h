#ifndef __HOST_SERIAL_H__
#define __HOST_SERIAL_H__

#include <string.h>
#include <sys/time.h>

#define min(a,b) ((a)<(b)?(a):(b))

// todo - hehe, notice here if you are using string arg
#define strlcpy strncpy

typedef uint8_t byte;

inline static unsigned long millis()
{
    struct timeval ts;
    gettimeofday(&ts, NULL);
    return ts.tv_sec * 1000 + ts.tv_usec / 1000;
}

class Stream{
public:
    Stream() {}
    virtual ~Stream() {}
    int print(const char *data)
    {
        int pos = 0;
        while (*(data + pos))
        {
            print(*(data + pos));
            ++pos;
        }
        return pos;
    }
    int print(byte data) 
    {
        return 0;
    }
    size_t available() 
    {
        return 0;
    }

    size_t readBytes(char *buffer, size_t length) 
    {
        return 0;
    }
    int println()
    {
        return 0;
    }
    byte read()
    {
        return 0;
    }
};

#endif // HOST_SERIAL_H__
