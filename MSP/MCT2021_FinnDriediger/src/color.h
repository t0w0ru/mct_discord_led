#include "yahal_String.h"
#include "stdio.h"
#include <cstring>

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

struct color
{
    int r, g, b;
    color( ) = default;
    color(const int r, const int g, const int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    bool operator==(color c)
    {
        if (c.r == r && c.g == g && c.b == b)
            return true;
        else
            return false;
    }

    static color white  (   ){ return { 255, 255, 255 }; }
    static color black  (   ){ return {   0,   0,   0 }; }


    static color red    (   ){ return { 255,   0,   0 }; }
    static color green  (   ){ return {   0, 255,   0 }; }
    static color blue   (   ){ return {   0,   0, 255 }; }

    static color yellow (   ){ return { 255, 255,   0 }; }
    static color fuchsia(   ){ return { 255,   0, 255 }; }
    static color aqua   (   ){ return {   0, 255, 255 }; }

    static color orange (   ){ return { 255, 128,   0 }; }
    static color pink   (   ){ return { 255,   0, 128 }; }
    static color lime   (   ){ return { 128, 255,   0 }; }
    static color cyan   (   ){ return {   0, 255, 128 }; }
    static color violet (   ){ return { 128,   0, 255 }; }
    static color lblue  (   ){ return {   0, 128, 255 }; }

    //aliases
    static color magenta(   ){ return fuchsia(); }

    static const char* color_to_char(color col)
    {
        if      ( col == color::white( ) )
            return "WHITE          ";
        else if ( col == color::red( ) )
            return "RED            ";
        else if ( col == color::green( ) )
            return "GREEN          ";
        else if ( col == color::blue( ) )
            return "BLUE           ";
        else if ( col == color::yellow( ) )
            return "YELLOW         ";
        else if ( col == color::fuchsia( ) )
            return "FUCHSIA        ";
        else if ( col == color::aqua( ) )
            return "AQUA           ";
        else if ( col == color::orange( ) )
            return "ORANGE         ";
        else if ( col == color::pink( ) )
            return "PINK           ";
        else if ( col == color::lime( ) )
            return "LIME           ";
        else if ( col == color::cyan( ) )
            return "CYAN           ";
        else if ( col == color::violet( ) )
            return "VIOLET         ";
        else if ( col == color::lblue( ) )
            return "L-BLUE         ";
        else if ( col == color::black( ) )
            return "BLACK          ";
        else
        {
            static char ret[30];
            char r_val[4], g_val[4], b_val[4];
            strcpy(ret, "{");
            sprintf(r_val, "%d", col.r);
            sprintf(g_val, "%d", col.g);
            sprintf(b_val, "%d", col.b);
            strcat(ret, r_val);
            strcat(ret, "|");
            strcat(ret, g_val);
            strcat(ret, "|");
            strcat(ret, b_val);
            strcat(ret, "}      ");
            return ret;
         }
    }

    static color hex_to_color(String s)
    {
        char * str = const_cast<char*>(s.substr(1, s.size()).c_str());
        int _r, _g, _b;
        sscanf(str, "%02x%02x%02x", &_r, &_g, &_b);
        return color(_r, _g, _b);
    }

};


#endif /* SRC_COLOR_H_ */
