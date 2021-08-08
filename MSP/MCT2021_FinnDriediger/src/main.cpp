#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include "ws2812.h"
#include "display.h"

#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_6x10.h"
#include "st7735s_drv.h"

#include "posix_io.h"
#include "uart_msp432.h"
#include "gpio_msp432.h"
#include "task.h"
#include "yahal_String.h"


int main(void)
{
    //my led-strips have a total of 100 leds, unfortunately some functions e.g gradient
    //need a little too much energy for all 100 leds to work
    //since i currently dont own a motor or similar delivering enough energy
    //without damaging the board, i'll only enable 50 of them.
    //this value can be changed free at will and will not affect the performance of the programm in any form
    int total_leds{ 50 };



    // Define the SPI interface(s)

    // Interface      MOSI
    // -------------+-----
    // EUSCI_A0_SPI | P1.3
    // EUSCI_A1_SPI | P2.3
    // EUSCI_A2_SPI | P3.3
    // EUSCI_A3_SPI | P9.7 // UART which is connected to the ESP8266
    // EUSCI_B0_SPI | P1.6 // used for the display
    // EUSCI_B1_SPI | P6.4 // used for LEDs
    // EUSCI_B2_SPI | P3.6
    // EUSCI_B3_SPI | P10.2

    // Define a CS pin for SPI
    // (this is only a dummy and not needed)
    gpio_msp432_pin cs(PORT_PIN(2, 0));

//led setup
    // MOSI has to be connected to the IN signal
    // of the first LED
    spi_msp432 spi_B1(EUSCI_B1_SPI, cs);



    // Create a new WS2812 controller
    WS2812 leds(spi_B1, total_leds);
    leds.reset();
    leds.setSize(total_leds);


//screen setup
    display screen;

    // Setup SPI interface
    gpio_msp432_pin lcd_cs(PORT_PIN(5, 0));
    spi_msp432 spi_B0(EUSCI_B0_SPI, lcd_cs);
    spi_B0.setSpeed(24000000);

    // Setup LCD driver
    gpio_msp432_pin lcd_rst(PORT_PIN(5, 7));
    gpio_msp432_pin lcd_dc(PORT_PIN(3, 7));
    st7735s_drv lcd(spi_B0, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128);

    // Setup uGUI
    uGUI gui(lcd);
    lcd.clearScreen(0x0);
    gui.SetForecolor(C_WHITE);
    gui.FontSelect(&FONT_6X10);



// esp setup
    // The backchannel UART (default 115200 baud)
    uart_msp432 uart;
    // Register backchannel UART for printf etc.
    posix_io::inst.register_stdout( uart );

    // UART which is connected to the ESP8266
    uart_msp432 uart_esp(EUSCI_A3,115200);

    // Receive message handler:
    // All received chars are collected in a String.
    // When '\n' is received, the whole message is
    // printed out.
    String msg, last;

    uart_esp.uartAttachIrq([&](char c) {

           if (c != '\n') {
               msg += c; // Add char to message
           } else {
               last = msg;
               printf("Received:\t%s\n", msg.c_str());

               if(msg.c_str()[0] == '0') {
                   leds.do_static(color::black(),gui);
                   gui.PutString(35, 35, "TYPO?     ", true);
                   gui.PutString(35, 65, "TYPO?     ", true);
               } else if(msg.c_str()[0] == '1') {
                   leds.do_static(color::hex_to_color(msg.substr(2, msg.size())), gui);
               } else if(msg.c_str()[0] == '2') {
                       leds.do_pulse(color::hex_to_color(msg.substr(2, msg.size())), gui);
               } else if(msg.c_str()[0] == '3') {
                   leds.do_disco(gui); //ignores color
               } else if(msg.c_str()[0] == '4') {
                   leds.do_gradient(color::hex_to_color(msg.substr(2, msg.size())), gui) ;
               } else if(msg.c_str()[0] == '5') {
                   leds.do_wave(color::hex_to_color(msg.substr(2, msg.size())), gui);
               } else if(msg.c_str()[0] == '6') {
                   leds.do_color_cycle(gui); //rainbow
               } else {
                   gui.PutString(35, 35, "NONE      ", true);
                   gui.PutString(35, 65, "NONE      ", true);
               }
               msg.clear();
           }
       });

    // Reset the ESP8266
    gpio_msp432_pin esp_reset( PORT_PIN(10, 5) );
    esp_reset.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
    esp_reset.gpioWrite( HIGH );

    gui.PutString(35, 35, "NONE      ", true); //default
    gui.PutString(35, 65, "NONE      ", true);//default


// programm loop
    while (true)
    {

        screen.draw(gui);
        // Endless loop which sends lower-case messages
            // to the ESP8266. The answers from ESP8266 are
            // printed in the receive message handler above!

        uart_esp.puts("awaiting message!\n");
        task::sleep(500);
    }
}








