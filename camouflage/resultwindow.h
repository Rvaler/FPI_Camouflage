#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>

namespace Ui {
class resultWindow;
}

class resultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit resultWindow(QWidget *parent = 0);

    ~resultWindow();

private slots:
    void showImageR(QImage m);
private:
    Ui::resultWindow *ui;


};

#endif // RESULTWINDOW_H
