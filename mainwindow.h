#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QInputDialog>
#include <QDir>
#include <QClipboard>
#include <QSystemTrayIcon>

#include "bookmark.h"
#include "hotkey.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_newButton_clicked();

    void on_editButton_clicked();

    void on_removeButton_clicked();

    void on_browserButton_clicked();

    void on_inputCategory_currentIndexChanged(int index);

    void on_inputSubcategory_currentIndexChanged(int index);

    void on_pasteButton_clicked();

    void on_actionSave_triggered();

    void on_actionReload_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_filterCategory_currentIndexChanged(int index);

    void on_filterSubcategory_currentIndexChanged(int index);

    void iconActivated(QSystemTrayIcon::ActivationReason);

    void changeEvent(QEvent*);

    void handle_hot_key_event(int);

private:
    Ui::MainWindow *ui;
    QStringList* entries;
    QSystemTrayIcon* trayIcon;
    HotKeyThread* m_hot_key_thread;
};

#endif // MAINWINDOW_H
