#ifndef CRELAYCLICK_H
#define CRELAYCLICK_H

#include <QFile>

#define RELAY_OUT "/dev/mikrobus/out_RL2"

class CRelayClick : public QObject
{
    Q_OBJECT
public:
    CRelayClick();
    ~CRelayClick();

public:
    void setPosition(bool value);

private:
    QFile   *m_relayFile;
};


#endif // CRELAYCLICK_H
