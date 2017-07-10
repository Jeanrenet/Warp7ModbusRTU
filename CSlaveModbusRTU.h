#ifndef CSLAVEMODBUSRTU_H
#define CSLAVEMODBUSRTU_H

#include <QModbusRtuSerialSlave>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

#define PORT_NAME   "/dev/ttymxc5"

//pour la température
#define TEMP_RAW    "/sys/bus/iio/devices/iio\:device0/in_temp_raw"
#define TEMP_SCALE  "/sys/bus/iio/devices/iio\:device0/in_temp_scale"


class CEepromClick;
class CFlameClick;
class CRelayClick;

class CSlaveModbusRTU : public QObject
{
    Q_OBJECT
public:
    CSlaveModbusRTU(CEepromClick *eeprom,
                    CFlameClick *flame,
                    CRelayClick *relay);
    ~CSlaveModbusRTU();

protected slots:
    //slots pour le serveur modbus
    void messageReceived(QModbusDataUnit::RegisterType table, int address, int size);
    void stateChanged(QModbusDevice::State);
    void errorOccurred(QModbusDevice::Error);

    //slot pour la flameclick
    void flameDetected(bool value);

    //slot pour la lecture de température
    void readTemperature();

private:
    QModbusRtuSerialSlave   *m_modbusRTUSerialSlave;

    CEepromClick            *m_eepromClick;
    CFlameClick             *m_flameClick;
    CRelayClick             *m_relayClick;

    QTimer                  *m_eventLoop;
};

#endif // CSLAVEMODBUSRTU_H
