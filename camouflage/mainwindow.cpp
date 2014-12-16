#include "mainwindow.h"
#include "resultwindow.h"
#include "ui_mainwindow.h"
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

using namespace std;
QString fileLocal1;
QString fileLocal2;

QImage modificar;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



// actions

void MainWindow::initImages(int a)
{
    QWidget window;
    window.resize(320, 240);
    window.show();
    QImage original (fileLocal1);
    QImage modified (fileLocal2);

    if (a == 1)
    {
        showImageO(original);
        setOriginal(original);
    }else{
        showImageM(modified);
        setModified(modified);
    }
}



void MainWindow::showImageO(QImage m)
{
    QImage resized;
    if (ui->fitCheck->isChecked()==1)
        resized = m.scaled (ui->modifiedGV->width()-3,ui->modifiedGV->height()-3,Qt::KeepAspectRatio);
    else
        resized = m;

    QGraphicsScene *toBeShown = new QGraphicsScene();
    toBeShown->addPixmap(QPixmap::fromImage(resized));
    ui->originalGV->setScene(toBeShown);
}

void MainWindow::showImageM(QImage m)
{
    QImage resized;

    if (ui->fitCheck2->isChecked()==1)
        resized = m.scaled (ui->modifiedGV->width()-3,ui->modifiedGV->height()-3,Qt::KeepAspectRatio);
    else

        resized = m;

    QGraphicsScene *toBeShown = new QGraphicsScene();
    toBeShown->addPixmap(QPixmap::fromImage(resized));
    ui->modifiedGV->setScene(toBeShown);
}

// buttons


void MainWindow::on_openButton_clicked()
{
    fileLocal1 = QFileDialog::getOpenFileName(this, tr("Open File"),
                                              "/home",
                                              tr("Images (*.png *.xpm *.jpg)"));
    initImages(1);
}

void MainWindow::on_openButton2_clicked()
{
    fileLocal2 = QFileDialog::getOpenFileName(this, tr("Open File"),
                                              "/home",
                                              tr("Images (*.png *.xpm *.jpg)"));
    initImages(2);
}



void MainWindow::setModified(QImage m)
{
    modified = m;
}

QImage MainWindow::getModified()
{
    return modified;
}

void MainWindow::setOriginal(QImage o)
{
    original = o;
}

QImage MainWindow::getOriginal()
{
    return original;
}

//modifiedImage vai conter o fundo branco com a cara do animal
void MainWindow::on_camouflageButton_clicked()
{

    float camouflageBox = ui->camouflageBox->value();
    QImage originalImage = getOriginal();
    QImage modifiedImage = getModified();

    QImage newImage (originalImage.width(), originalImage.height(), QImage::Format_RGB32);
    QImage passaAltas (modifiedImage.width(), modifiedImage.height(), QImage::Format_RGB32);
    QImage grayScaleImage (modifiedImage.width(), modifiedImage.height(), QImage::Format_RGB32);


    grayScaleImage = grayScale(modifiedImage);

    //float highpass[9] = {0,-1,0,-1,4,-1,0,-1,0};
    float lowpass[9] = {0.0625,0.125,0.0625,0.125,0.25, 0.125, 0.0625,0.125,0.0625};


    grayScaleImage = convolve(lowpass, grayScaleImage);




    for (int x = 0 ; x < modifiedImage.width() - 1 ; x++){
        for (int y = 0 ; y < modifiedImage.height() - 1 ; y++){

            QRgb pixel = modifiedImage.pixel(x,y);
            int red, green, blue;
            int redOriginal, greenOriginal, blueOriginal;
            int newRed, newGreen, newBlue;

            if(qRed(pixel) < 250 && qGreen(pixel) < 250 && qBlue(pixel) < 250){

                red = qRed(grayScaleImage.pixel(x,y));
                green = qGreen(grayScaleImage.pixel(x,y));
                blue = qBlue(grayScaleImage.pixel(x,y));

                redOriginal = qRed(originalImage.pixel(x,y));
                greenOriginal = qGreen(originalImage.pixel(x,y));
                blueOriginal = qBlue(originalImage.pixel(x,y));

                newRed = camouflageBox*red + (1 - camouflageBox)*redOriginal ;
                if(newRed > 255){
                    newRed = 255;
                }else if(newRed < 0){
                    newRed = 0;
                }
                newGreen = camouflageBox*green + (1 - camouflageBox)*greenOriginal  ;
                if(newGreen > 255){
                    newGreen = 255;
                }else if(newGreen < 0){
                    newGreen = 0;
                }
                newBlue =  camouflageBox*blue + (1 - camouflageBox)*blueOriginal ;
                if(newBlue > 255){
                    newBlue = 255;
                }else if(newBlue < 0){
                    newBlue = 0;
                }

                QRgb newPixel = qRgb (newRed, newGreen, newBlue);
                newImage.setPixel(x,y, newPixel);
            }else{


                red = qRed(originalImage.pixel(x,y));
                green = qGreen(originalImage.pixel(x,y));
                blue = qBlue(originalImage.pixel(x,y));
                QRgb newPixel = qRgb (red, green, blue);
                newImage.setPixel(x,y, newPixel);

            }
        }
    }

    modificar = newImage;

    this->result();
    this->applyResult(newImage);
    //this->applyResult(newImage);
    //resultWindow r;
    //r.show();
    //r.showImageR(newImage);
    //resultWindow::showImageR(newImage);

}



QImage MainWindow::grayScale(QImage m)
{
    QImage modifiedImage = m;
    QRgb pixel, grayPixel;
    int gray;
    for(int linha = 0; linha < modifiedImage.height() - 1; linha++){
        for(int coluna = 0; coluna < modifiedImage.width() - 1; coluna++){
            pixel = modifiedImage.pixel(coluna,linha);
            gray = qRed(pixel)*0.299 + qGreen(pixel)*0.114 + qBlue(pixel)*0.587;
            grayPixel = qRgb(gray, gray, gray);
            modifiedImage.setPixel(coluna,linha,grayPixel);
        }
    }
    return modifiedImage;
}

QImage MainWindow::convolve(float *a, QImage image)
{
    //recebe os valores e aplica a rotaçao e o filtro na imagem

    QImage newImage = image;


    for(int x = 1; x < image.width() - 2; x++){
        for(int y =1; y < image.height() - 2; y++){
            int valueRed = 0, valueGreen = 0, valueBlue = 0;

            valueRed += qRed(image.pixel(x-1,y+1)) *a[8];
            valueRed += qRed(image.pixel(x,y+1)) *a[7];
            valueRed += qRed(image.pixel(x+1,y+1)) *a[6];
            valueRed += qRed(image.pixel(x-1,y)) *a[5];
            valueRed += qRed(image.pixel(x,y)) *a[4];
            valueRed += qRed(image.pixel(x+1,y)) *a[3];
            valueRed += qRed(image.pixel(x-1,y-1)) *a[2];
            valueRed += qRed(image.pixel(x,y-1)) *a[1];
            valueRed += qRed(image.pixel(x+1,y-1)) *a[0];

            valueGreen += qGreen(image.pixel(x-1,y+1)) *a[8];
            valueGreen+= qGreen(image.pixel(x,y+1)) *a[7];
            valueGreen += qGreen(image.pixel(x+1,y+1)) *a[6];
            valueGreen += qGreen(image.pixel(x-1,y)) *a[5];
            valueGreen += qGreen(image.pixel(x,y)) *a[4];
            valueGreen += qGreen(image.pixel(x+1,y)) *a[3];
            valueGreen += qGreen(image.pixel(x-1,y-1)) *a[2];
            valueGreen += qGreen(image.pixel(x,y-1)) *a[1];
            valueGreen += qGreen(image.pixel(x+1,y-1)) *a[0];

            valueBlue += qBlue(image.pixel(x-1,y+1)) *a[8];
            valueBlue += qBlue(image.pixel(x,y+1)) *a[7];
            valueBlue += qBlue(image.pixel(x+1,y+1)) *a[6];
            valueBlue += qBlue(image.pixel(x-1,y)) *a[5];
            valueBlue += qBlue(image.pixel(x,y)) *a[4];
            valueBlue += qBlue(image.pixel(x+1,y)) *a[3];
            valueBlue += qBlue(image.pixel(x-1,y-1)) *a[2];
            valueBlue += qBlue(image.pixel(x,y-1)) *a[1];
            valueBlue += qBlue(image.pixel(x+1,y-1)) *a[0];

            if(valueRed > 255)
            {
                valueRed = 255;
            }
            if(valueRed < 0)
            {
                valueRed = 0;
            }


            if(valueGreen > 255)
            {
                valueGreen = 255;
            }
            if(valueGreen < 0)
            {
                valueGreen = 0;
            }


            if(valueBlue > 255)
            {
                valueBlue = 255;
            }
            if(valueBlue < 0)
            {
                valueBlue = 0;
            }

            QRgb newPixel = qRgb(valueRed, valueGreen, valueBlue);
            newImage.setPixel(x,y, newPixel);


        }
    }

    return newImage;
}
