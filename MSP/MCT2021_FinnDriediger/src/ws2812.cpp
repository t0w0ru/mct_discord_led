//
///*
// * ws2812.cpp
// *
// *  Created on: 28.05.2021
// *      Author: Finn Driediger(additions and changes to suit the project), A. Terstegge(base class)
// */
//
#include "ws2812.h"

// Create a WS2812 object for 'size' LEDs.
// Use the SPI interface _spi
WS2812::WS2812(spi_interface &spi, int size) :
        _spi(spi), _size(size)
{
    // Set clock speed of SPI interface
    _spi.setSpeed(3000000);
    // Do not generate CS signals (we don't need them...)
    _spi.setCS(true);
    // Allocate SPI send buffer
    _txbuff = new uint8_t[size * 12];
    // Initialize colors (set all LEDs to black)
    for (int i = 0; i < size; ++i) {
        setRGB(i, 0, 0, 0);
    }
    sendData();
}

// Set the color of LED with index 'i' (0 < i < size)
// Color is specified with one byte for R, G and B.
void WS2812::setRGB(int led, int r, int g, int b)
{
    yahal_assert(led < _size);
    // SPI values for  '00'  '01'  '10'  '11'
    char values[] = { 0x88, 0x8e, 0xe8, 0xee };
    for (int i = 0; i < 4; ++i)
    {
        _txbuff[3 - i + led * 12] = values[g & 0x3];
        _txbuff[7 - i + led * 12] = values[r & 0x3];
        _txbuff[11- i + led * 12] = values[b & 0x3];
        g >>= 2;
        r >>= 2;
        b >>= 2;
    }
}

void WS2812::setRGB(int led, color color)
{
    setRGB(led, color.r, color.g, color.b);
}

const char* WS2812::getMode()
{
    return this->_mode;
}

void WS2812::setMode(const char* mode)
{
    this->_mode = mode;
}


void WS2812::sendData()
{
    // Send out data via SPI to control all LEDs!
    _spi.spiTx(_txbuff, _size * 12);
    // Make sure MOSI is low after transmission!
    uint8_t c = 0;
    _spi.spiTx(&c, 1);
}

void WS2812::setSize(int size)
{
    this->_size = size;
}

void WS2812::reset()
{
    for (int i = 0; i < _size; ++i)
    {
        setRGB(i, 0, 0, 0);
    }
    sendData();
}


//utilities
/////////////////////////////////////////////////////////////////////////////////
//modes


void WS2812::do_static(color col, uGUI gui)
{
    gui.PutString(35, 35, "STATIC    ", true);
    gui.PutString(35, 65, color::color_to_char(col), true);
    for(int i = 0; i < _size; ++i) {
        setRGB(i, col.r, col.g, col.b);
    }
    sendData();
}


void WS2812::do_wave(color col, uGUI gui)
{
    gui.PutString(35, 35, "WAVE      ", true);
    gui.PutString(35, 65, color::color_to_char(col), true);

    //set all LEDs to 10%
    for(int i = 0; i < _size; ++i)
    {
        setRGB(i, col.r/10, col.g/10, col.b/10);
    }
    sendData();


    //leading led zieht vor und hinter sich schwächere leds mit
    for(int leading = -10; leading < (_size + 10); ++leading)
    {
        if(leading < _size && leading > 0) //leider notwendig, da die leds sonst stoppen wuerden, anderen if-abfragen analog.
            setRGB(leading, col.r, col.g, col.b);
        //wir wollen erst updaten wenn die folgenden und fuehrenden leds auch die richtige farbe bekommen haben.
        for(int k = 1; k < 10; k++)
        {
            if(k < 9) // die ersten 8 anhaenger schwaecher machen
            {
                if((leading - k) < _size && (leading - k) > 0)
                    setRGB(leading - k, col.r * (10 - k) / 10, col.g * (10 - k) / 10, col.b * (10 - k) / 10);
                if((leading + k) < _size && (leading + k) > 0)
                    setRGB(leading + k, col.r * (10 - k) / 10, col.g * (10 - k) / 10, col.b * (10 - k) / 10);
            }
            else // den rest normalisieren
            {
                if((leading - k) < _size && (leading - k) > 0)
                    setRGB(leading - k, col.r / 10, col.g / 10, col.b / 10);
                if((leading + k) < _size && (leading + k) > 0)
                    setRGB(leading + k, col.r / 10, col.g / 10, col.b / 10);
            }
        }
        task::sleep(50);
        sendData(); //hier die leading & anhaenger updaten

    }


    //ab hier analog in entgegengesetzte richtung
    for(int i = 0; i < _size; ++i)
    {
        setRGB(i, col.r / 10, col.g / 10, col.b / 10);
    }
    sendData();

    for(int leading = (_size + 10); leading > -10; --leading)
    {
        if(leading > -1 && leading < _size)
            setRGB(leading, col.r, col.g, col.b);

        for(int k = 1; k < 10; k++)
        {
            if(k < 9)
            {
                if((leading - k) > -1 && (leading - k) < _size)
                    setRGB(leading - k, col.r * (10 - k) / 10, col.g * (10 - k) / 10, col.b * (10 - k) / 10);
                if((leading + k) > -1 && (leading +k) < _size)
                    setRGB(leading + k, col.r * (10 - k) / 10, col.g * (10 - k) / 10, col.b * (10 - k) / 10);
            }
            else
            {
                if((leading - k) > -1 && (leading - k) < _size)
                    setRGB(leading - k, col.r / 10, col.g / 10, col.b / 10);
                if((leading + k) > -1 && (leading + k) < _size)
                    setRGB(leading + k, col.r / 10, col.g / 10, col.b / 10);
            }
        }
        task::sleep(50);
        sendData();
    }
}

void WS2812::do_pulse(color col, uGUI gui)
{
    gui.PutString(35, 35, "PULSATE   ", true);
    gui.PutString(35, 65, color::color_to_char(col), true);

    //perform breathe-out
    for (int i = 255; i > 10; i--) //loop over all leds
    {
        for (int j = 0; j < _size; ++j) //used for division
        {
            setRGB(j, col.r * i / 255, col.g * i / 255, col.b * i / 255);
        }
        sendData();
        task::sleep(10);
    }
    //perform breathe-in
    for (int i = 10; i < 256; i++)
    {
        for(int j = 0; j < _size; ++j)
        {
            setRGB(j, col.r * i / 255, col.g * i / 255, col.b * i / 255);
        }
        sendData();
        task::sleep(10);
    }
}


void WS2812::do_disco(uGUI gui)
{
    gui.PutString(35, 35, "DISCO     ", true);
    gui.PutString(35, 65, "RAINBOW", true);

    color arr[] = { color::violet(), color::magenta(), color::pink(), color::red(),
                    color::orange(), color::yellow(), color::lime(), color::green(),
                    color::cyan(), color::aqua(), color::lblue(), color::blue() };

    //wechselt durch alle farben des color arrays
    for(size_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, arr[i].r, arr[i].g, arr[i].b);
    sendData();
    task::sleep(500);
    }

}
#define round(x) ( x - (int)x >= 0.5f ? (int)(x + 1) : (int)x )
/*damit wir nicht die cmath lib einbinden muessen...
 *anmerkung: ich weiß dass im faq steht wie das gemacht wird.
 *habe schließlich auch die arduinojson lib einbinden müssen.
 *für unsere zwecke ist dies allerdings mehr als ausreichend. */
void WS2812::do_gradient(color col, uGUI gui)
{
    color neg = color(255 - col.r, 255 - col.g, 255 - col.b); //inverted color
    gui.PutString(35, 35, "GRADIENT  ", true);
    gui.PutString(35, 65, color::color_to_char(col), true);
    gui.PutString(35, 75, color::color_to_char(neg), true);

    for(int i = 0; i < _size; ++i)
    { //erstellt einen prozent wert "auf einere gerade" zwischen den 2 farben fuer jede led
        float percent = i / (float)_size;
        float inverse = 1 - percent;
        color c = color(round(col.r * inverse + neg.r * percent),
                        round(col.g * inverse + neg.g * percent),
                        round(col.b * inverse + neg.b * percent));
        setRGB(i, c);
    }
    sendData();
}


void WS2812::do_color_cycle(uGUI gui) // rainbow
{
    gui.PutString(35, 35, "RAINBOW   ", true);
    gui.PutString(35, 65, "RAINBOW", true);


    //-> rainbow effect
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, 255, i, 0);
        sendData();
        task::sleep(10);
    }// red + green
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, 255 - i, 255, 0);
        sendData();
        task::sleep(10);
    }// green
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, 0, 255, i);
        sendData();
        task::sleep(10);
    }// green + blue
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, 0, 255 - i, 255);
        sendData();
        task::sleep(10);
    }// blue
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, i, 0, 255);
        sendData();
        task::sleep(10);
    }// blue + red
    for(int i = 0; i < 255; ++i) {
        for(int j = 0; j < _size; ++j)
            setRGB(j, 255, 0, 255 - i);
        sendData();
        task::sleep(10);
    }//red
}
