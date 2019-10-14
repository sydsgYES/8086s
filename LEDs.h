#ifndef LEDS_H
#define LEDS_H
#include "Hardwares.h"


class LEDs : public Hardwares
{

private:
    Q_OBJECT
    Voltage pins[8];
    int status;
public:
    LEDs();

    //set the pin's voltage
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage value);
    //get the pin's voltage
    Voltage getPinVoltage(MicroCom::Pins pin);

};

#endif // LEDS_H
