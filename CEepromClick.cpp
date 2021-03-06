#include "CEepromClick.h"

#include <QDebug>
#include <QDataStream>

CEepromClick::CEepromClick()
{
    m_eepromFile = new QFile(EEPROM_READ_WRITE);

    //initialisation
    quint32 id;
    getModbusID(id);
    if (id > 255)
        setModbusID(1);
}

CEepromClick::~CEepromClick()
{
    delete m_eepromFile;
}

void CEepromClick::setModbusID(quint32 value)
{
    if (m_eepromFile->open(QIODevice::ReadWrite))
    {
        QByteArray data;
        QDataStream str(&data, QIODevice::ReadWrite);
        str << value;
        m_eepromFile->write(data);
        m_eepromFile->close();
    }
}

void CEepromClick::getModbusID(quint32 &value)
{
    if (m_eepromFile->open(QIODevice::ReadOnly))
    {
        QByteArray data;
        data.resize(4);
        m_eepromFile->read(data.data(), 4);

        QDataStream str(data);
        str >> value;
        qDebug() << "Read Modbus ID is : "<< value;
        m_eepromFile->close();
    }
}
