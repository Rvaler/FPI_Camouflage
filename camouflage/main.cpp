#include "mainwindow.h"
#include "resultwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    resultWindow r;


    w.show();

    QObject::connect(&w, SIGNAL(result()), &r, SLOT(show()));

    QObject::connect(&w, SIGNAL(applyResult(QImage)), &r, SLOT(showImageR(QImage)));

    return a.exec();
}
