#ifndef VIDEO_H
#define VIDEO_H

#include "types.h"
#include "color.h"

namespace os {

class video
{
public:
    //A character together with colors
    struct letter
    {
        char ch;
        fbcolor col;
    } __attribute__((packed));
    
    //The current color
    fbcolor tcolor;
    //The cursor position
    unsigned int cx, cy;
    
    //Constructor
    video(fbcolor col = {color::light_gray, color::black});
    
    //Returns a reference to the letter at the given position
    letter& at(size_t x, size_t y) { return textmem[cols*y + x]; }
    const letter& at(size_t x, size_t y) const { return textmem[cols*y + x]; }
    //Returns a reference to the letter at the cursor position
    letter& here() { return at(cx, cy); }
    const letter& here() const { return at(cx, cy); }
    
    //The following functions do not send a signal to update the cursor position on screen:
    
    //Scrolls one line down
    void scroll();
    //Sets a character at the given position
    void setch(char c, size_t x, size_t y, fbcolor col) { at(x, y) = {c, col}; }
    void setch(char c, size_t x, size_t y) { setch(c, x, y, tcolor); }
    //Sets a character at the cursor position
    void setch(char c) { setch(c, cx, cy, tcolor); }
    //Breaks the line
    void newline();
    //Advances the cursor
    void advance() { if(cx >= cols-1) newline(); else cx++; }
    //Moves the cursor to the given position
    void mvcursor(unsigned int x, unsigned int y) { cx = x; cy = y; }
    //Writes a string of characters (without checking for newlines or null terminators)
    void write(const char* str, unsigned int n);
    //Writes a string of letters
    void copy(const letter* lstr, unsigned int n);
    
    //The following functions will update the cursor position on screen:
    
    //Sends a signal to the hardware to update the cursor position
    void update_cursor();
    
    //Clears the screen
    void clear();
    //Writes a signed number
    void putn(long i, unsigned int base = 10, unsigned int padding = 0);
    //Writes an unsigned number
    void putu(unsigned long i, unsigned int base = 10, unsigned int padding = 0);
    
    //Output functions similar to streams
    video& operator<<(char c);
    video& operator<<(const char* str);
    video& operator<<(int i) { putn(i, 10); return *this; }
    video& operator<<(unsigned int i) { putu(i, 10); return *this; }
    video& operator<<(long i) { putn(i, 10); return *this; }
    video& operator<<(unsigned long i) { putu(i, 10); return *this; }
    video& operator<<(const void* p);
    //Sets the fore- and background color
    video& operator<<(fbcolor col) { tcolor = col; return *this; }
    //Sets the foreground color
    video& operator<<(color fore) { tcolor.fore(fore); return *this; }
    
private:
    //Location of the text video memory
    static constexpr letter* textmem = reinterpret_cast<letter*>(0xb8000);
    //Screen size
    static constexpr size_t lines = 25, cols = 80;
};

//The screen instance
extern video scr;

} //namespace os

#endif
