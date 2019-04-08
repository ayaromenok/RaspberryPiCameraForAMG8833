// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVideoFrame>
#include <QImage>
#include <QVector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class QAmg8833;

class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QLabel;
class QImage;
class QLabel;
class QPushButton;

class QTimer;
class QCamera;
class QCameraImageCapture;
class QCameraViewfinder;

namespace cv{
    class VideoCapture;
    class Mat;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void setUI(quint32 type = 0);

private slots:
    void        timerUpdate();

    void        cvCamUpdate(cv::Mat &imgIn);
    void        cvIRUpdate();
    void        quit();

private:
    void setCamCV();
    void setIR();
    QLayout     *loutMain_;
    QGroupBox   *gbIR_;
    QGroupBox   *gbCam_;
    QGroupBox   *gbCtrl_;

    QImage      *imgIR_;
    QImage      *imgCam_;

    QLabel      *lbIR_;
    QLabel      *lbCam_;
    QPushButton *pbCtrlExit_;

    QVBoxLayout *loutIR_;
    QVBoxLayout *loutCam_;
    QVBoxLayout *loutCtrl_;

    QTimer      *timer_;

    cv::VideoCapture        cvCap;
    cv::Mat                 frame;
    QAmg8833                *ir_;
    QVector<quint8>         data;

    quint8                  dataMin;
    quint8                  dataMax;
    quint8                  dataScaleAuto;
};

#endif // WIDGET_H
