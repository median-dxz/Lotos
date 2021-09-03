#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.connect(&a,SIGNAL(lastWindowClose()),&a,SLOT(quit()));
    return a.exec();
}
