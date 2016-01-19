#include "mainwindow.h"
#include <QApplication>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Create seeds from which a new set of random numbers will be created
    qsrand(static_cast<unsigned int>(QDateTime::currentMSecsSinceEpoch()));

    w.show();

    return a.exec();
}
