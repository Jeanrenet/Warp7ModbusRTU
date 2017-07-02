#include "CFlameClick.h"

#include <QDebug>

CFlameClick::CFlameClick() :
    m_watcherTimer(new QTimer),
    m_flameFile(new QFile(FLAME_CLICK_OUT)),
    m_systemIsOnFire(false)
{
    connect(m_watcherTimer,
            SIGNAL(timeout()),
            this,
            SLOT(checkFile()));

    m_watcherTimer->start(10);
}

CFlameClick::~CFlameClick()
{
    m_watcherTimer->stop();
    delete m_watcherTimer;

    delete m_flameFile;
}

void CFlameClick::checkFile()
{
    if (m_flameFile->open(QIODevice::ReadOnly))
    {
        bool value = m_flameFile->readAll().remove(1, 1).toInt();

        if (value != m_systemIsOnFire)
        {
            m_systemIsOnFire = value;

            Q_EMIT flameDetected(m_systemIsOnFire);

            qDebug() << QString(m_systemIsOnFire?"System is on Fire !!":"YEAH WE ARE ALL SAVED");
        }
        m_flameFile->close();
    }
}
