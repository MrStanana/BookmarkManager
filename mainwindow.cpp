#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableBookmark->setColumnWidth(0, 400);
    ui->tableBookmark->setColumnWidth(1, 200);
    ui->tableBookmark->setColumnWidth(2, 200);
    ui->tableBookmark->setColumnWidth(3, 500);

    entries = new QStringList();

    trayIcon = new QSystemTrayIcon(QIcon(":/icon.png"), this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    QString file = QDir::currentPath() + "/bookmarks";
    readBookmarkFile(file, entries);
    populateBoxes(entries, ui->inputCategory, ui->inputSubcategory, ui->filterCategory, ui->filterSubcategory);
    populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);

    m_hot_key_thread = new HotKeyThread(this);
    QObject::connect(m_hot_key_thread, SIGNAL(hot_key_event(int)), this, SLOT(handle_hot_key_event(int)), Qt::QueuedConnection);
    m_hot_key_thread->start();

    ui->statusBar->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    m_hot_key_thread->stop();
    QString file = QDir::currentPath() + "/bookmarks";
    saveBookmarkFile(file, entries);
    delete ui;
}

void MainWindow::on_newButton_clicked()
{
    QString urlText = ui->inputURL->text();
    if(!urlText.isEmpty()) {
        if(findMatches(entries, urlText) == 0) {
            QString str;
            str = urlText + "|";
            if(ui->inputCategory->currentText().compare("All") != 0) {
                str += ui->inputCategory->currentText();
            }
            str += "|";
            if(ui->inputSubcategory->currentText().compare("All") != 0) {
                str += ui->inputSubcategory->currentText();
            }
            str += "|";
            entries->append(str);
            populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);

            ui->inputURL->clear();
            ui->statusBar->showMessage(QString("Item added: ") + urlText);
        } else {
            QMessageBox::information(this, "Duplicate item", "Item already exists.");
        }
    }
}

void MainWindow::on_editButton_clicked()
{
    QItemSelectionModel* select = ui->tableBookmark->selectionModel();
    if(select->hasSelection()) {
        QModelIndexList list = select->selectedRows();
        if(list.count() == 1) {
            QModelIndex index = list.first();
            ui->statusBar->showMessage("Item edited.");
        } else {
            QMessageBox::information(this, "Item Selection", "Too many rows selected.");
        }
    } else {
        QMessageBox::information(this, "Item Selection", "No rows selected.");
    }
}

void MainWindow::on_removeButton_clicked()
{
    QItemSelectionModel* select = ui->tableBookmark->selectionModel();
    if(select->hasSelection()) {
        QModelIndexList list = select->selectedRows();
        QMessageBox::StandardButton reply;
        QModelIndex index;
        QString url, str;
        int n;
        if(list.count() == 1) {
            reply = QMessageBox::question(this, "Remove Item?", "Are you sure you want to remove this item?", QMessageBox::Yes|QMessageBox::No);
        } else {
            reply = QMessageBox::question(this, "Remove Items?", "Are you sure you want to remove these items?", QMessageBox::Yes|QMessageBox::No);
        }
        if (reply == QMessageBox::Yes) {
            for(QModelIndexList::Iterator it = list.begin(); it != list.end(); it++) {
                index = *it;
                qDebug() << index.row();
                url = ui->tableBookmark->itemAt(index.row(), 0)->text();
                n = 0;
                for(QStringList::Iterator ite = entries->begin(); ite != entries->end(); ite++) {
                    str = *ite;
                    if(str.split("|").at(0).compare(url) == 0) {
                        entries->removeAt(n);
                        break;
                    }
                    n++;
                }
            }
        }
        if(list.count() == 1) {
            ui->statusBar->showMessage("Item removed.");
        } else {
            ui->statusBar->showMessage("Items removed.");
        }
        populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);
    } else {
        QMessageBox::information(this, "Item Selection", "No rows selected.");
    }
}

void MainWindow::on_browserButton_clicked()
{
    QItemSelectionModel* select = ui->tableBookmark->selectionModel();
    if(select->hasSelection()) {
        QModelIndexList list = select->selectedRows();
        if(list.count() == 1) {
            QModelIndex index = list.first();
            int row = index.row();
            QTableWidgetItem* item = ui->tableBookmark->item(row, 0);
            QString link = item->text();
            QUrl url = QUrl::fromUserInput(link);
            QDesktopServices::openUrl(url);
        } else {
            QMessageBox::information(this, "Item Selection", "Too many rows selected.");
        }
    } else {
        QMessageBox::information(this, "Item Selection", "No rows selected.");
    }
}

void MainWindow::on_inputCategory_currentIndexChanged(int index)
{
    static int n = 0;
    if(index == ui->inputCategory->count() - 1 && n == 0) {
        n = 1;
        QString cat = QInputDialog::getText(this, "New Category", "Insert a new category:");
        if(!cat.isEmpty()) {
            ui->inputCategory->insertItem(ui->inputCategory->count() - 1, cat);
            ui->inputCategory->setCurrentIndex(ui->inputCategory->count() - 2);
        }
        n = 0;
    } else if(index == 0) {

    } else {

    }
}

void MainWindow::on_inputSubcategory_currentIndexChanged(int index)
{
    static int n = 0;
    if(index == ui->inputSubcategory->count() - 1 && n == 0) {
        n = 1;
        QString cat = QInputDialog::getText(this, "New Category", "Insert a new category:");
        if(!cat.isEmpty()) {
            ui->inputSubcategory->insertItem(ui->inputSubcategory->count() - 1, cat);
            ui->inputSubcategory->setCurrentIndex(ui->inputSubcategory->count() - 2);
        }
        n = 0;
    } else if(index == 0) {

    } else {

    }
}


void MainWindow::on_pasteButton_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    ui->inputURL->setText(clipboard->text(QClipboard::Clipboard));
}

void MainWindow::on_actionSave_triggered()
{
    QString file = QDir::currentPath() + "/bookmarks";
    saveBookmarkFile(file, entries);
    ui->statusBar->showMessage("Saved.");
}

void MainWindow::on_actionReload_triggered()
{
    QString file = QDir::currentPath() + "/bookmarks";
    readBookmarkFile(file, entries);
    populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);
    ui->statusBar->showMessage("Reloaded.");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "Bookmark Manager", "Developed by João Loureiro\n2015");
}

void MainWindow::on_filterCategory_currentIndexChanged(int index)
{
    populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);
    index;
}

void MainWindow::on_filterSubcategory_currentIndexChanged(int index)
{
    populateTable(entries, ui->filterCategory, ui->filterSubcategory, ui->tableBookmark, ui->label);
    index;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
    {
        if( isVisible() )
        {
            hide();
        }
        else
        {
            show();
            raise();
            setFocus();
        }
        break;
    }
    default:
        ;
    }
}

void MainWindow::changeEvent(QEvent* e)
{
    switch (e->type())
    {
        case QEvent::WindowStateChange:
            {
                if (this->windowState() & Qt::WindowMinimized)
                {
                    iconActivated(QSystemTrayIcon::DoubleClick);
                }

                break;
            }
        default:
            break;
    }

    QMainWindow::changeEvent(e);
}

void MainWindow::handle_hot_key_event(int n) {
    QClipboard* clipboard = QApplication::clipboard();
    if(clipboard->supportsSelection()) {
        ui->inputURL->setText(clipboard->text(QClipboard::Selection));
    } else {
        on_pasteButton_clicked();
    }
    on_newButton_clicked();
    n;
}
