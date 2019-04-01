// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#include "widget.h"
#include <QtWidgets>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QDebug>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    setGeometry (100, 100, 854, 480);
    setUI(0);
    setCam();
    setIR();
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer_->start(1000); //msec. min is 125 - 8fps - maximum for AGM8833
}

Widget::~Widget()
{
    timer_->stop();
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

    imgIR_ = new QImage(":/res/test/testHotCupColdMeet.jpg");
    lbIR_ = new QLabel("IR image");
    lbIR_->setPixmap(QPixmap::fromImage(*imgIR_).scaled(240, 240));
    loutIR_ = new QVBoxLayout;
    loutIR_->addWidget(lbIR_);
    gbIR_ = new QGroupBox("IR/AGM 8833");
    gbIR_->setLayout(loutIR_);

    imgCam_ = new QImage(":/res/test/testHotCupColdMeetCamFOV160_640x480.jpg");
    lbCam_ = new QLabel("Cam image");
    lbCam_->setPixmap(QPixmap::fromImage(*imgCam_).scaled(320, 240));
    loutCam_ = new QVBoxLayout;
    loutCam_->addWidget(lbCam_);
    gbCam_  = new QGroupBox("Cam");
    gbCam_->setLayout(loutCam_);

    loutCtrl_ = new QVBoxLayout;
    gbCtrl_ = new QGroupBox("Control");
    gbCtrl_->setLayout(loutCtrl_);

    loutMain_->addWidget(gbIR_);
    loutMain_->addWidget(gbCam_);
    loutMain_->addWidget(gbCtrl_);

    setLayout(loutMain_);
}

void
Widget::timerUpdate()
{
    qDebug() << __PRETTY_FUNCTION__;

    cam_->searchAndLock();
#ifdef CAMERA_CAPTURE_VIA_FILE
    imgCap_->capture("./imgCam.jpg");
#else //CAMERA_CAPTURE_VIA_FILE
    imgCap_->capture();
#endif //CAMERA_CAPTURE_VIA_FILE
    cam_->unlock();

    cvIRUpdate();
}

void
Widget::setCam()
{
    qDebug() << __PRETTY_FUNCTION__;
    camViewFinder_  = new QCameraViewfinder;
    loutCtrl_->addWidget(camViewFinder_);

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        qDebug() << "camera: " << cameraInfo.description();
    }
    cam_ = new QCamera(QCameraInfo::defaultCamera());
    cam_->setCaptureMode(QCamera::CaptureStillImage);
    cam_->setViewfinder(camViewFinder_);

    imgCap_ = new QCameraImageCapture(cam_);

#ifdef CAMERA_CAPTURE_VIA_FILE
    //workaround for RPi\OSX\iOS\some Windows builds
    imgCap_->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(imgCap_, SIGNAL(imageSaved(int, const QString&)),
        this, SLOT(imgToFile(int, const QString&)));
    qDebug() << "Capture via File (workaround mode)";
#else //CAMERA_CAPTURE_VIA_FILE
    imgCap_->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    connect(imgCap_, &QCameraImageCapture::imageAvailable,
            this, &Widget::imgToBuffer);
    qDebug() << "Capture via Buffer (preferable mode)";
#endif //CAMERA_CAPTURE_VIA_FILE
    camViewFinder_->show();

    cam_->start();
}

void
Widget::setIR()
{
    qDebug() << __PRETTY_FUNCTION__;
}

bool
Widget::imgToBuffer(int id, const QVideoFrame &buffer)
{
    qDebug() << __PRETTY_FUNCTION__;
    bool result = false;
    QVideoFrame frame(buffer);

    frame.map(QAbstractVideoBuffer::ReadOnly);
    int nbytes = frame.mappedBytes();
    QImage imgIn = QImage::fromData(frame.bits(), nbytes).scaledToWidth(360);
//    qDebug() << "\t\tinput image format" << imgIn.format()
//             << "// 4 - Image::Format_RGB32" << "id" <<id;
    lbCam_->setPixmap(QPixmap::fromImage(imgIn));
    if (imgIn.width() > 0)
        result = true;
    return result;
}

bool
Widget::imgToFile(int id, const QString &fName)
{
    qDebug() << __PRETTY_FUNCTION__;
    bool result = false;
    QImage imgIn(fName);
    QImage img(imgIn.scaled(320,240));
    qDebug() << "\t\tinput image format" << img.format()
             << "// 4 - Image::Format_RGB32" << "id" << id
             << "size" << img.width() << img.height();
    if (!imgIn.isNull())
        cvCamUpdate(img);

    if (imgIn.width() > 0)
        result = true;
    return result;
}

void
Widget::cvCamUpdate(QImage &image){
    qDebug() << __PRETTY_FUNCTION__;

    cv::Mat imgIn(cv::Size(image.width(),image.height()),
                  CV_8UC4, image.bits());
    cv:: Mat gray,grayRes, imgRes;

    cv:Canny(imgIn, imgRes, 50, 100, 3);
    //cv::imshow("res", imgRes);
    QImage imageOut(imgRes.cols, imgRes.rows,  QImage::Format_RGB888);
    cv::Mat imageCvOut(cv::Size(imgRes.cols,imgRes.rows),
                       CV_8UC3, imageOut.bits());
    cv::cvtColor(imgRes, imageCvOut, cv::COLOR_BGR2RGB);

    lbCam_->setPixmap(QPixmap::fromImage(imageOut.scaledToWidth(360)));
}

void
Widget::cvIRUpdate(){
    qDebug() << __PRETTY_FUNCTION__;
}
