// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVideoFrame>
#include <QImage>

class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QLabel;
class QImage;
class QLabel;

class QTimer;
class QCamera;
class QCameraImageCapture;
class QCameraViewfinder;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void setUI(quint32 type = 0);

private slots:
    void        timerUpdate();
    bool        imgToBuffer(int id, const QVideoFrame &buffer);
    // workaround for https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/issues/4
    bool        imgToFile(int id, const QString &fName);
    void        cvCamUpdate(QImage &image);
    void        cvIRUpdate();

private:
    void setCam();
    void setIR();
    QLayout     *loutMain_;
    QGroupBox   *gbIR_;
    QGroupBox   *gbCam_;
    QGroupBox   *gbCtrl_;

    QImage      *imgIR_;
    QImage      *imgCam_;

    QLabel      *lbIR_;
    QLabel      *lbCam_;

    QVBoxLayout *loutIR_;
    QVBoxLayout *loutCam_;
    QVBoxLayout *loutCtrl_;

    QTimer      *timer_;
    QCamera                 *cam_;
    QCameraImageCapture     *imgCap_;
    QCameraViewfinder       *camViewFinder_;
};

#endif // WIDGET_H
