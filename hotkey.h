#ifndef HOTKEY_H
#define HOTKEY_H

#include <QThread>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class HotKeyThread : public QThread
{
    Q_OBJECT

public:
    HotKeyThread(QObject *parent);
    ~HotKeyThread();
signals:
    void hot_key_event(int);
public slots:
    void run();
    void stop();
private:
    volatile bool m_stopped;
    DWORD m_thread_id;
};

#endif // HOTKEY_H
