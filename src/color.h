#ifndef COLOR_H
#define COLOR_H

namespace os {

//The available colors
enum class color : unsigned int
{
    black         = 0x0,
    blue          = 0x1,
    green         = 0x2,
    cyan          = 0x3,
    red           = 0x4,
    magenta       = 0x5,
    brown         = 0x6,
    light_gray    = 0x7,
    dark_gray     = 0x8,
    light_blue    = 0x9,
    light_green   = 0xA,
    light_cyan    = 0xB,
    light_red     = 0xC,
    light_magenta = 0xD,
    yellow        = 0xE,
    white         = 0xF
};

//Foreground and background color
class fbcolor
{
public:
    unsigned int _fore : 4;
    unsigned int _back : 4;
    
public:
    fbcolor(color f, color b):
        _fore(static_cast<uint8_t>(f)),
        _back(static_cast<uint8_t>(b))
    { }
        
    explicit fbcolor(uint8_t val):
        _fore(val & 0x0f),
        _back((val & 0xf0) >> 4)
    { }
        
    uint8_t val() const { return _fore | (_back << 4); }
    
    color fore() const { return static_cast<color>(_fore); }
    color back() const { return static_cast<color>(_back); }
    
    color fore(color c) { _fore = static_cast<unsigned int>(c); return fore(); }
    color back(color c) { _back = static_cast<unsigned int>(c); return back(); }
} __attribute__((packed));

} //namespace os

#endif
