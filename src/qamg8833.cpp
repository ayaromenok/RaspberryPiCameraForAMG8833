#include "qamg8833.h"

#include <QDebug>
#include <QTimer>

#include <wiringPiI2C.h>

QAmg8833::QAmg8833(QObject *parent) : QObject(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    fd_ = wiringPiI2CSetup(0x69);

    if (0 == fd_) {
        qDebug() << "Can't init AMG8833. Please check wiring and/or "
                    "switch on I2C in raspi-config";
        return;
    }
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_PCTL, AMG8833_NORMAL_MODE );
    qDebug() << "writing 0x00 reg" << rslt_;
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_RST, AMG8833_INITIAL_RESET);
    qDebug() << "writing 0x01 reg" << rslt_;
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_FPS, AMG8833_FPS_10);
    qDebug() << "writing 0x02 reg" << rslt_;
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_INT, AMG8833_DISABLE_INT);
    qDebug() << "writing 0x03 reg" << rslt_;

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateData()));
}

QAmg8833::~QAmg8833()
{
    qDebug() << __PRETTY_FUNCTION__;
}

float
QAmg8833::s12tofp32(int value)
{
    qDebug() << __PRETTY_FUNCTION__;
    int absValue = (value & 0x7FF);
    if (value & 0x8000){
        return (0 - float(absValue));
    }
    return float(absValue);
}

void
QAmg8833::updateData()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void
QAmg8833::set10fps()
{
    qDebug() << __PRETTY_FUNCTION__;
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_FPS, AMG8833_FPS_10);
    timer_->start(100);  //msec

}

void
QAmg8833::set1fps()
{
    qDebug() << __PRETTY_FUNCTION__;
    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_FPS, AMG8833_FPS_1);
    timer_->start(1000); //msec
}
