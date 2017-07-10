#include "CSlaveModbusRTU.h"

#include "CEepromClick.h"
#include "CFlameClick.h"
#include "CRelayClick.h"

#include <QDebug>

CSlaveModbusRTU::CSlaveModbusRTU(CEepromClick *eeprom, CFlameClick *flame, CRelayClick *relay) :
    m_eepromClick(eeprom),
    m_flameClick(flame),
    m_relayClick(relay)
{

    //Nouvelles instances
    m_eventLoop = new QTimer();
    connect(m_eventLoop, SIGNAL(timeout()), this, SLOT(readTemperature()));

    m_modbusRTUSerialSlave = new QModbusRtuSerialSlave();

    //connexion vers des slots
    connect(m_modbusRTUSerialSlave,
            SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)),
            this,
            SLOT(messageReceived(QModbusDataUnit::RegisterType,int,int)));

    connect(m_modbusRTUSerialSlave,
            SIGNAL(stateChanged(QModbusDevice::State)),
            this,
            SLOT(stateChanged(QModbusDevice::State)));

    connect(m_modbusRTUSerialSlave,
            SIGNAL(errorOccurred(QModbusDevice::Error)),
            this,
            SLOT(errorOccurred(QModbusDevice::Error)));

    //création de notre table de données
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils,              { QModbusDataUnit::Coils,               0, 1  });
    reg.insert(QModbusDataUnit::DiscreteInputs,     { QModbusDataUnit::DiscreteInputs,      0, 1  });
    reg.insert(QModbusDataUnit::InputRegisters,     { QModbusDataUnit::InputRegisters,      0, 1  });
    reg.insert(QModbusDataUnit::HoldingRegisters,   { QModbusDataUnit::HoldingRegisters,    0, 1  });
    m_modbusRTUSerialSlave->setMap(reg);

    //paramètres
    m_modbusRTUSerialSlave->setConnectionParameter(QModbusDevice::SerialPortNameParameter,  PORT_NAME);
    m_modbusRTUSerialSlave->setConnectionParameter(QModbusDevice::SerialParityParameter,    QSerialPort::NoParity);
    m_modbusRTUSerialSlave->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,  QSerialPort::Baud115200);
    m_modbusRTUSerialSlave->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,  QSerialPort::Data8);
    m_modbusRTUSerialSlave->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,  QSerialPort::OneStop);

    //initialisation des données
    flameDetected(m_flameClick->getStatus());

    //on récupère l'ID modbus
    quint32 modbusID;
    m_eepromClick->getModbusID(modbusID);

    //on spécifie l'identifiant
    m_modbusRTUSerialSlave->setServerAddress(modbusID);

    //connexion
    m_modbusRTUSerialSlave->connectDevice();

    //création d'une connexion avec la FlameClick
    connect(m_flameClick, SIGNAL(flameDetected(bool)), this, SLOT(flameDetected(bool)));

    //on démarre le timer de lecture de la température
    m_eventLoop->start(1000);
}

CSlaveModbusRTU::~CSlaveModbusRTU()
{
    m_eventLoop->stop();
    delete m_eventLoop;
    delete m_modbusRTUSerialSlave;
}

void CSlaveModbusRTU::stateChanged(QModbusDevice::State state)
{
    qDebug() << state;
}

void CSlaveModbusRTU::errorOccurred(QModbusDevice::Error error)
{
    qDebug() << error;
}

void CSlaveModbusRTU::flameDetected(bool value)
{
    m_modbusRTUSerialSlave->setData(QModbusDataUnit::DiscreteInputs,    //code fonction
                                    0,                                  //adresse
                                    value);                             //valeur
}

void CSlaveModbusRTU::readTemperature()
{
    quint32 rawValue = 0;
    float scaleValue = 0;

    QFile raw(TEMP_RAW);
    if (raw.open(QIODevice::ReadOnly))
    {
        //on lit et on retire le dernier caractère
        QByteArray data =  raw.readAll();
        rawValue = data.remove(data.length() - 1, 1).toInt();

    }
    QFile scale(TEMP_SCALE);
    if (scale.open(QIODevice::ReadOnly))
    {
        //on lit et on retire le dernier caractère
        QByteArray data =  scale.readAll();
        scaleValue = data.remove(data.length() - 1, 1).toFloat();
    }

    m_modbusRTUSerialSlave->setData(QModbusDataUnit::InputRegisters,    //code fonction
                                    0,                                  //adresse
                                    rawValue * scaleValue);             //valeur
}

void CSlaveModbusRTU::messageReceived(QModbusDataUnit::RegisterType table, int address, int size)
{
    for (int i = 0; i < size; i++)
    {
        quint16 value;

        switch(table)
        {
        case QModbusDataUnit::Coils:
            m_modbusRTUSerialSlave->data(QModbusDataUnit::Coils, address + i, &value);

            //on teste l'addresse
            if (address + i == 0)
                m_relayClick->setPosition(value);
            break;
        case QModbusDataUnit::HoldingRegisters:
            m_modbusRTUSerialSlave->data(QModbusDataUnit::HoldingRegisters, address + i, &value);

            //on teste l'addresse
            if (address + i == 0)
                m_eepromClick->setModbusID(value);
            break;
        default:
            break;
        }
    }
}
