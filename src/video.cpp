#include "video.h"

#include "outb.h"
#include "memcpy.h"

namespace os {

namespace { //anonymous
    //Helper functions that replace some STL functions
    //TODO: Separate them out
    template<class It, class T>
    It fill_n(It begin, size_t n, const T& value)
    {
        for(size_t i = 0; i < n; ++i)
            *begin++ = value;
        return begin;
    }

    /*template<class InputIt, class OutputIt>
    OutputIt copy_(InputIt begin, InputIt end, OutputIt out)
    {
        while(begin != end)
            *out++ = *begin++;
        return out;
    }*/

    template<class T>
    const T& min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template<class T>
    const T& max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }
}

video scr;

video::video(fbcolor col):
    tcolor(col),
    cx(0),
    cy(0)
{
    clear();
}

void video::update_cursor()
{
    uint16_t pos = cols*cy + cx;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void video::clear()
{
    fill_n(textmem, cols*lines, letter{' ', tcolor});
    
    cx = 0;
    cy = 0;
}

void video::scroll()
{
    //Copy the contents of the screen one line up
    //copy(textmem + cols, textmem + cols*lines, textmem);
    memcpy(textmem, textmem + cols, cols*lines * sizeof(letter));

    //Overwrite the last line with blank spaces
    fill_n(textmem + cols*(lines-1), cols, letter{' ', tcolor});

    //Adapt the cursor position
    if(cy == 0)
    {
        cx = 0;
        cy = 0;
    }
    else
        cy--;
}

void video::newline()
{
    if(cy >= lines-1)
        scroll();

    cx = 0;
    cy++;
}

void video::write(const char* s, unsigned int n)
{
    for(unsigned int i = 0; i < n; ++i)
    {
        setch(s[i]);
        advance();
    }
}

void video::copy(const letter* lstr, unsigned int n)
{
    for(unsigned int i = 0; i < n; ++i)
    {
        here() = lstr[i];
        advance();
    }
}


void video::putn(long i, unsigned int base, unsigned int padding)
{
    if(i >= 0)
        putu(i, base, padding);
    else
    {
        setch('-');
        advance();
        putu(-i, base, max(padding, 1U) - 1);
    }
}

void video::putu(unsigned long i, unsigned int base, unsigned int padding)
{
    const size_t bufsize = 8*sizeof(i);

    if(base < 2 || base > 36)
        return;
    
    padding = min<size_t>(padding, bufsize);
    
    char buf[8*bufsize];
    char* bufend = buf + bufsize;
    char* p = bufend;
    do
    {
        *--p = "0123456789abcdefghijklmnopqrstuvwxyz"[i % base];
        i /= base;
    } while(i);
    
    while(static_cast<unsigned int>(bufend - p) < padding)
    {
        *--p = '0';
    }
    
    write(p, bufend - p);
    update_cursor();
}

video& video::operator<<(char c)
{
    if(c == '\n')
    {
        newline();
    }
    else
    {
        setch(c);
        advance();
    }
    update_cursor();
    return *this;
}

video& video::operator<<(const char* s)
{
    for(; *s != '\0'; ++s)
    {
        if(*s == '\n')
        {
            newline();
            continue;
        }
        
        setch(*s);
        advance();
    }
    
    update_cursor();
    return *this;
}

video& video::operator<<(const void* p)
{
    write("0x", 2);
    putu(reinterpret_cast<unsigned long>(p), 16, 8);
    return *this;
}

} //namespace os
