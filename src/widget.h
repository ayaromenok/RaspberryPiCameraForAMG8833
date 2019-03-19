// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QLayout;
class QGroupBox;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void setUI(quint32 type = 0);

private:

    QLayout     *loutMain_;
    QGroupBox   *gbAGM8833_;
    QGroupBox   *gbRPiCam_;
    QGroupBox   *gbControl_;
};

#endif // WIDGET_H
