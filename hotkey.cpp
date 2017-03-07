#include "hotkey.h"

#include <QDebug>
#include <process.h>

#define WM_END_THREAD (WM_USER+2)

HotKeyThread::HotKeyThread(QObject *parent)
    : QThread(parent)
{
    this->m_thread_id = 0;
}

HotKeyThread::~HotKeyThread()
{

}

void HotKeyThread::stop()
{
    if(this->m_thread_id != 0)
        ::PostThreadMessage(this->m_thread_id, WM_END_THREAD, 0, 0);
}

//
void HotKeyThread::run()
{
    // store a thread id so we can exit later
    m_thread_id = ::GetCurrentThreadId();

    //qDebug() << "ThreadIDs" << QString::number(m_thread_id, 16) << QString::number((int) this->currentThreadId(), 16);

    // register an atom, and a hotkey
    BOOL retVal;
    int counter = 0;
    int magic_num = 1128;
    ATOM id = ::GlobalAddAtom(MAKEINTATOM(magic_num + counter++));

    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    int modifier = MOD_ALT | MOD_CONTROL; // 0x0;// modify this line
    int key = VK_NUMPAD0;// modify this line
    if(QSysInfo::windowsVersion() > QSysInfo::WV_VISTA)
    {
        retVal = ::RegisterHotKey(NULL, id, modifier | MOD_NOREPEAT, key);
    }
    else
    {
        // No repeat is only supported in 7 and later
        retVal = ::RegisterHotKey(NULL, id, modifier, key);
    }

    if(retVal)
    {
        //qDebug() << "Successfully added a HotKey!";
    }
    else
    {
        //qDebug() << "Failed to add a hotkey!";
        return;
    }

    // wait on hotkeys
    MSG msg = {0};
    while (0 < ::GetMessage(&msg, NULL, 0, 0))
    {
        if(msg.message == WM_HOTKEY)
        {
            bool control = LOWORD(msg.lParam) & MOD_CONTROL;
            bool shift = LOWORD(msg.lParam) & MOD_SHIFT;
            bool alt = LOWORD(msg.lParam) & MOD_ALT;
            bool win = LOWORD(msg.lParam) & MOD_WIN;
            /*qDebug() << "HotKey!" << (control ? "Ctrl +": "")
                << (alt ? "Alt +": "")
                << (shift ? "Shift +":"")
                << (win ? "Win +":"") << QString::number(HIWORD(msg.lParam),16);*/
            // TODO Notify MainWindow of the event
            emit hot_key_event(msg.lParam);
        }
        else if(msg.message == WM_END_THREAD)
        {
            // exit
            break;
        }
    }

    // Clean up Hotkey
    ::UnregisterHotKey(NULL, id);
    ::GlobalDeleteAtom(id);
}
