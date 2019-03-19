// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#include "widget.h"
#include <QtWidgets>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    setGeometry (100, 100, 854, 480);
    setUI(0);
}

Widget::~Widget()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void
Widget::setUI(quint32 type)
{
    qDebug() << __PRETTY_FUNCTION__ << type;
    if (type == 0){
        loutMain_ = new QHBoxLayout;
    } else {
        loutMain_ = new QVBoxLayout;
    }

    gbAGM8833_ = new QGroupBox("AGM 8833");
    gbRPiCam_  = new QGroupBox("RPi Cam");
    gbControl_ = new QGroupBox("Control");

    loutMain_->addWidget(gbAGM8833_);
    loutMain_->addWidget(gbRPiCam_);
    loutMain_->addWidget(gbControl_);

    setLayout(loutMain_);
}
