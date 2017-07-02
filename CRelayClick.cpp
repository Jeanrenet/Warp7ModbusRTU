#include "CRelayClick.h"

#include <QDebug>

CRelayClick::CRelayClick()
{
    m_relayFile = new QFile(RELAY_OUT);
}

CRelayClick::~CRelayClick()
{
    delete m_relayFile;
}

void CRelayClick::setPosition(bool value)
{
    if (m_relayFile->open(QIODevice::ReadWrite))
    {
        m_relayFile->write(QByteArray::number(value));
        m_relayFile->close();
    }
}
