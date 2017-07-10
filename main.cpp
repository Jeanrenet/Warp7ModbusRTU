#include <QCoreApplication>

#include "CEepromClick.h"
#include "CFlameClick.h"
#include "CRelayClick.h"
#include "CSlaveModbusRTU.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CEepromClick *eepromClick = new CEepromClick;
    CFlameClick *flameClick = new CFlameClick;
    CRelayClick *relayClick = new CRelayClick;
    CSlaveModbusRTU *modbusServer = new CSlaveModbusRTU(eepromClick,
                                                        flameClick,
                                                        relayClick);

    Q_UNUSED(modbusServer)


    return a.exec();
}
