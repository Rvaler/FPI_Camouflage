#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initImages(int a);
    void showImageO(QImage m);
    void showImageM(QImage m);
    void setModified(QImage m);
    QImage getModified();
    void setOriginal(QImage o);
    QImage getOriginal();
    QImage grayScale(QImage m);
    QImage convolve(float *a, QImage image);

private slots:
    void on_openButton_clicked();
    void on_openButton2_clicked();

    void on_camouflageButton_clicked();

private:
    Ui::MainWindow *ui;


    QImage modified;
    QImage original;

signals:
    bool result();
    bool applyResult(QImage m);
};

#endif // MAINWINDOW_H
