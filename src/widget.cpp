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

    imgIR_ = new QImage(":/res/test/min20max50.jpg");
    lbIR_ = new QLabel("IR image");
    lbIR_->setPixmap(QPixmap::fromImage(*imgIR_).scaled(256, 256));
    QVBoxLayout *loutIR = new QVBoxLayout;
    loutIR->addWidget(lbIR_);
    gbIR_ = new QGroupBox("IR/AGM 8833");
    gbIR_->setLayout(loutIR);

    imgCam_ = new QImage(":/res/test/candle.jpg");
    lbCam_ = new QLabel("Cam image");
    lbCam_->setPixmap(QPixmap::fromImage(*imgCam_).scaled(256, 256));
    QVBoxLayout *loutCam = new QVBoxLayout;
    loutCam->addWidget(lbCam_);
    gbCam_  = new QGroupBox("Cam");
    gbCam_->setLayout(loutCam);

    gbControl_ = new QGroupBox("Control");

    loutMain_->addWidget(gbIR_);
    loutMain_->addWidget(gbCam_);
    loutMain_->addWidget(gbControl_);

    setLayout(loutMain_);
}
