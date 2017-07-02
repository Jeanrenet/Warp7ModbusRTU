#ifndef CFLAMECLICK_H
#define CFLAMECLICK_H

#include <QTimer>
#include <QFile>

#define FLAME_CLICK_OUT "/dev/mikrobus/in_INT_FLAME"

class CFlameClick : public QObject
{
    Q_OBJECT
public:
    CFlameClick();
    ~CFlameClick();

public:
    Q_SIGNAL void flameDetected(bool);

protected slots:
    void checkFile();

private:
    QTimer  *m_watcherTimer;
    QFile   *m_flameFile;
    bool    m_systemIsOnFire;
};

#endif // CFLAMECLICK_H
