//#include <cstring>
#include "memcpy.h"

#undef memmove
#undef memcpy
void* memcpy(void* dest, const void* src, size_t n)
{
    for(size_t i = 0; i < n; ++i)
        static_cast<char*>(dest)[i] = static_cast<const char*>(src)[i];
    
    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    if(dest < src)
        return memcpy(dest, src, n);

    size_t end = static_cast<size_t>(-1);
    for(size_t i = n-1; i != end; --i)
        static_cast<char*>(dest)[i] = static_cast<const char*>(src)[i];
    
    return dest;
}
