#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QTableWidget>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>

/*
class Bookmark
{
public:
    Bookmark();
};
*/

void readBookmarkFile(QString, QStringList*);

void saveBookmarkFile(QString, QStringList*);

void populateTable(QStringList*, QComboBox*, QComboBox*, QTableWidget*, QLabel*);

int findMatches(QStringList*, QString);

void populateBoxes(QStringList*, QComboBox*, QComboBox*, QComboBox*, QComboBox*);

#endif // BOOKMARK_H
