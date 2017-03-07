#include "bookmark.h"

/*
Bookmark::Bookmark()
{

}
*/

void readBookmarkFile(QString fileName, QStringList* list) {
    list->clear();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug("Error reading file."); //file.errorString()
    } else {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            if(!line.isEmpty()) {
                if(line.split("|").count() == 1) {
                    line += "|||";
                }
                list->append(line);
            }
        }
        file.close();
    }
}

void saveBookmarkFile(QString fileName, QStringList* list) {
    QString tmpName = fileName + "tmp";
    QFile file(tmpName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error, unable to open" << tmpName << "for output";
    } else {
        QTextStream out(&file);
        for(QStringList::Iterator it = list->begin(); it != list->end(); it++) {
            out << *it << "\n";
        }
        file.close();
    }
    QFile::remove(fileName);
    QFile::rename(tmpName, fileName);
}

void populateTable(QStringList* list, QComboBox* category, QComboBox* subcategory, QTableWidget* table, QLabel* label) {
    int count = table->rowCount();
    int n = 0;
    int num = 0;
    for(int i = 0; i < count; i++)
    {
        table->removeRow(i - n++);
    }
    QStringList fields;
    fields.clear();
    QTableWidgetItem* item;
    QString str, cat, subcat;
    for(QStringList::Iterator it = list->begin(); it != list->end(); it++) {
        str = *it;
        fields = str.split("|");

        if(category->currentText().compare(QString("All")) == 0 || (fields.count() > 1 && category->currentText().compare(fields.at(1)) == 0)) {
            if(subcategory->currentText().compare(QString("All")) == 0 || (fields.count() > 2 && subcategory->currentText().compare(fields.at(2)) == 0)) {
                int row = table->rowCount();
                table->insertRow(row);
                num++;

                item = new QTableWidgetItem;
                item->setText(fields.at(0));
                table->setItem(row, 0, item);

                if(fields.count() > 1) {
                    item = new QTableWidgetItem;
                    item->setText(fields.at(1));
                    table->setItem(row, 1, item);
                    item = new QTableWidgetItem;
                    item->setText(fields.at(2));
                    table->setItem(row, 2, item);
                    item = new QTableWidgetItem;
                    item->setText(fields.at(3));
                    table->setItem(row, 3, item);
                }
            }
        }
        fields.clear();
    }
    label->setText(QString::number(num) + QString(" Entries"));
}

int findMatches(QStringList* entries, QString urlText) {
    int r = 0;
    for(int i = 0; i < entries->count(); i++) {
        if(entries->at(i).split("|").at(0).compare(urlText) == 0) {
            r++;
        }
    }
    return r;
}

void populateBoxes(QStringList* list, QComboBox* category, QComboBox* subcategory, QComboBox* filterCat, QComboBox* filterSubcat) {
    QStringList fields;
    fields.clear();
    QString str;
    for(QStringList::Iterator it = list->begin(); it != list->end(); it++) {
        str = *it;
        fields = str.split("|");

        while(category->count() != 2) {
            category->removeItem(1);
        }
        if(category->findText(fields.at(1), Qt::MatchExactly) == -1 && fields.at(1).compare("") != 0) {
            category->insertItem(category->count() - 1, fields.at(1));
        }
        while(subcategory->count() != 2) {
            subcategory->removeItem(1);
        }
        if(subcategory->findText(fields.at(2), Qt::MatchExactly) == -1 && fields.at(2).compare("") != 0) {
            subcategory->insertItem(subcategory->count() - 1, fields.at(2));
        }

        while(filterCat->count() != 1) {
            filterCat->removeItem(1);
        }
        if(filterCat->findText(fields.at(1), Qt::MatchExactly) == -1 && fields.at(1).compare("") != 0) {
            filterCat->addItem(fields.at(1));
        }
        while(filterSubcat->count() != 1) {
            filterSubcat->removeItem(1);
        }
        if(filterSubcat->findText(fields.at(2), Qt::MatchExactly) == -1 && fields.at(2).compare("") != 0) {
            filterSubcat->addItem(fields.at(2));
        }

        fields.clear();
    }
}
