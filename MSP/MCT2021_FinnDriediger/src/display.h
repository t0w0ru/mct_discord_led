#include "gpio_msp432.h"
#include "msp.h"
#include "uGUI.h"

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

class display
{

public:
    display();
    virtual ~display();
    void set_mode(char* str);
    const char* get_mode();
    void draw(uGUI gui);
};




#endif /* SRC_DISPLAY_H_ */
