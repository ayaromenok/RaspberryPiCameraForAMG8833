#include "qamg8833.h"

#include <QDebug>
#include <QTimer>

#include <wiringPiI2C.h>

QAmg8833::QAmg8833(QObject *parent) : QObject(parent)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__<< "Debug";
    fd_ = 0;
#else //DEBUG_PC
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
#endif //DEBUG_PC

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateData()));
    data.fill(0,64);
}

QAmg8833::~QAmg8833()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC
}

float
QAmg8833::s12tofp32(int value)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC

    int absValue = (value & 0x7FF);
    if (value & 0x8000){
        return (0 - float(absValue));
    }
    return float(absValue);
}

void
QAmg8833::updateData()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC

    int raw, adr;
    for (int i=0; i<AMG8833_RES_X; i++) {
        for (int j=0; j<AMG8833_RES_Y; j++) {
            adr = AMG8833_DATA_OFFSET+(i*AMG8833_RES_X +j)*2; //2byte data
#ifdef DEBUG_PC
            raw = 63;
            if ((i>3) & (i <5))
                raw += 63;
            if ((j>3) & (j <5))
                raw += 63;
#else //DEBUG_PC
            rslt_ = wiringPiI2CWriteReg8(fd_, adr, 0);
            raw = wiringPiI2CReadReg16(fd_, adr);
#endif //DEBUG_PC
            data[i*AMG8833_RES_X +j] = (quint8) raw;
         }
    }
}

void
QAmg8833::set10fps()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC

    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_FPS, AMG8833_FPS_10);
    timer_->start(100);  //msec

}

void
QAmg8833::set1fps()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC

    rslt_ = wiringPiI2CWriteReg8(fd_, AMG8833_FPS, AMG8833_FPS_1);
    timer_->start(1000); //msec
}

void
QAmg8833::getData(QVector<quint8> &dataExt)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;;
#endif //DEBUG_PC

    updateData();
    if (dataExt.length() == AMG8833_DATA_SIZE){
        quint8 *dataPtr = dataExt.data();
        for (int i=0; i<AMG8833_DATA_SIZE; i++){
            dataPtr[i] = data.at(i);
        }
    } else {
        qDebug() << "wrong data size";
    }
}
