#ifndef CEEPROMCLICK_H
#define CEEPROMCLICK_H

#include <QFile>

#define EEPROM_READ_WRITE "/sys/bus/i2c/devices/2-0050/eeprom"

class CEepromClick
{
public:
    CEepromClick();
    ~CEepromClick();

public:
    void setModbusID(quint32 value);
    void getModbusID(quint32 &value);

private:
    QFile   *m_eepromFile;

};

#endif // CEEPROMCLICK_H
