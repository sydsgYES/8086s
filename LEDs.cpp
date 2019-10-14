#include "LEDs.h"


LEDs::LEDs()
{
    status = 0;
    for(int i=0;i<8;i++){
        pins[i]=inf;
    }

}


void LEDs::setPinVoltage(MicroCom::Pins pin, Voltage value){
    pins[pin] = value;
    emit pinVolChanged(pin);
}


void LEDs::handlePinVolChanges(MicroCom::Pins pin, Voltage value){
    pins[pin] = value;
    status ++;
    if(status!=8){
        return;
    }
    int flag = 0;
    for(int i=8; i>0; i--){
        if(pins[i-1] == high){
            flag += (1 << (i-1));
        }
    }
    switch(flag){
    case 0x3F:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 0         ";
        qDebug()<<"==============================";
        break;
    case 0x06:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 1         ";
        qDebug()<<"==============================";
        break;
    case 0x5B:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 2         ";
        qDebug()<<"==============================";
        break;
    case 0x4F:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 3         ";
        qDebug()<<"==============================";
        break;
    case 0x66:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 4         ";
        qDebug()<<"==============================";
        break;
    case 0x6D:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 5         ";
        qDebug()<<"==============================";
        break;
    case 0x7D:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 6         ";
        qDebug()<<"==============================";
        break;
    case 0x07:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 7         ";
        qDebug()<<"==============================";
        break;
    case 0x7F:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 8         ";
        qDebug()<<"==============================";
        break;
    case 0x6F:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows 9         ";
        qDebug()<<"==============================";
        break;
    case 0x77:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows A         ";
        qDebug()<<"==============================";
        break;
    case 0x7C:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows B         ";
        qDebug()<<"==============================";
        break;
    case 0x39:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows C         ";
        qDebug()<<"==============================";
        break;
    case 0x5E:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows D         ";
        qDebug()<<"==============================";
        break;
    case 0x79:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows E         ";
        qDebug()<<"==============================";
        break;
    default:
        qDebug()<<"==============================";
        qDebug()<<"      The LED shows F         ";
        qDebug()<<"==============================";
        break;
    }
    for(int i=0;i<8;i++){
        pins[i]=inf;
    }
    status = 0;
}


//get the pin's voltage
Voltage LEDs::getPinVoltage(MicroCom::Pins pin){
    return pins[pin];
}
