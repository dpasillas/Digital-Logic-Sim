#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Null");
    a.setApplicationName("LogicSim");
    MainWindow w;
    w.show();

    return a.exec();
}
