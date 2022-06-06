#include "mainwindow.h"
#include <QApplication>
#include <iostream>


#include "ui_mainwindow.h"


#include <QDebug>
#include <QSqlDatabase>



using namespace std;




int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
