#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "mainwindow.h"
#include <QImage>
#include <string>
#include <QString>
#include <QFile>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

resultWindow::resultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resultWindow)
{
    ui->setupUi(this);
}

resultWindow::~resultWindow()
{
    delete ui;
}




void resultWindow::showImageR(QImage m)
{

    QImage resized;
    resized = m.scaled (ui->resultGV->width()-3,ui->resultGV->height()-3,Qt::KeepAspectRatio);
    /*
    if (ui->fitCheck->isChecked()==1)
        resized = m.scaled (ui->modifiedGV->width()-3,ui->modifiedGV->height()-3,Qt::KeepAspectRatio);
    else
        resized = m;
    */



    QGraphicsScene *toBeShown = new QGraphicsScene();
    toBeShown->addPixmap(QPixmap::fromImage(resized));
    ui->resultGV->setScene(toBeShown);
   // ui->originalGV->setScene(toBeShown);
}
