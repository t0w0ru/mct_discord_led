/*
 * ws2812.h
 *
 *  Created on: 28.05.2021
 *      Author: Finn Driediger
 */
#include "color.h"
#include "yahal_assert.h"
#include "spi_msp432.h"
#include "task.h"
#include "uGUI.h"
#include "color.h"

#ifndef SRC_WS2812_H_
#define SRC_WS2812_H_


class WS2812 {
private:
    spi_interface & _spi;
    int             _size;
    uint8_t *       _txbuff;
    int             _p;
    char const *_mode { "NONE" };
    color _col { 0, 0, 0 };


public:


    // Create a WS2812 object for 'size' LEDs.
    // Use the SPI interface _spi
    WS2812(spi_interface & spi, int size);

    // Set the color of LED with index 'i' (0 < i < size)
    // Color is specified with one byte for R, G and B.
    void setRGB(int led, int r, int g, int b);

    void setRGB(int led, color color);

    void sendData();

    void reset();

    const char* getMode();

    void setMode(const char* mode);

    void setSize(int size);

    void do_static(color color, uGUI gui);

    void do_pulse(color color, uGUI gui);

    void do_disco(uGUI gui);

    void do_gradient(color col, uGUI gui);

    void do_wave(color color, uGUI gui);

    void do_color_cycle(uGUI gui);

};


#endif /* SRC_WS2812_H_ */
