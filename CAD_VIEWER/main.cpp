/*! @file main.cpp
 *
 *  The file starts the application and displays the mainwindow
 *
 *  Jay Chauhan, Charles Egan and Jacob Moore 2025
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
