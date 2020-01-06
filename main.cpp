#include "mainwindow.h"
#include <QApplication>

//CS 3A Final Project by Ian Hinze and James Melford.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    w.show();

    return a.exec();
}
