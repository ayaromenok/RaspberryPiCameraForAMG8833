// Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
// MIT License
// https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

#include "widget.h"
#include <QtWidgets>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QDebug>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "qamg8833.h"

#include <algorithm>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << "Debug";
#endif //DEBUG_PC

    dataMin = 255;
    dataMax = 0;
    dataScaleAuto = 1;

    setGeometry (100, 100, 854, 480);
    setUI(0);
    setCamCV();
    setIR();

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer_->start(100); //msec. min is 100 - 10fps - maximum for AGM8833
    //PC - 66 - 15fps minimum for ThinkPad. or 33msec/30fps
    ir_ = new QAmg8833(this);
    data.fill(0,64);;
}

Widget::~Widget()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
    timer_->stop();
}

void
Widget::setUI(quint32 type)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__ << type;
#endif //DEBUG_PC

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
    pbCtrlExit_ = new QPushButton("E&xit", this);
    connect(pbCtrlExit_, SIGNAL(clicked()), this, SLOT(quit()));
    loutCtrl_->addWidget(pbCtrlExit_);
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
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC

    //OpenCV capture here
    cvCap >> frame;
    if (0 == frame.cols){
        qErrnoWarning("CV Camera FRAME capture error");
    } else {
        cvCamUpdate(frame);
    }
    cvIRUpdate();
}



void
Widget::setCamCV()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
    cvCap.open(0); // cv::CAP_V4L);
    cvCap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    cvCap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    //cvCap.set(cv::CAP_PROP_FPS, 30);

    if (!cvCap.isOpened()){
        qErrnoWarning("CV camera capture error");
    } else {
        frame = cv::Mat(320,240, CV_8UC3);
    }
}


void
Widget::setIR()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
}

void
Widget::cvCamUpdate(cv::Mat &imgIn)
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
    cv::Mat imgTmp, imgEdge, imgRes;

    cv::cvtColor(imgIn, imgTmp, cv::COLOR_RGB2GRAY);
#ifdef DEBUG_PC

#else //DEBUG_PC
    //\todo add to settings - required flip for rpi cam v1.3/wide angle
    //don't flip for std RPi cam v1.3
    //cv::flip(imgTmp, imgTmp, -1); //flip by both axis
#endif //DEBUG_PC
    cv::Canny(imgTmp, imgEdge, 50, 100, 3);

    //cut Region Of Interect and show it on original image
    cv::Rect roi;
    roi.x = 40;//96;
    roi.y = 0;//56;
    roi.width = 240;//128;
    roi.height = 240;//128;

    cv::resize(imgEdge(roi), frame, cv::Size(240,240));

    cv::rectangle(imgTmp, roi, cv::Scalar(255,255,255), 2);

    // due to typically different FPS in IR and Cam use separate image for dipslay
    imgRes = imgTmp + imgEdge;

    QImage imageOut(imgRes.cols, imgRes.rows,  QImage::Format_RGB888);
    cv::Mat imageCvOut(cv::Size(imgRes.cols,imgRes.rows),
                       CV_8UC3, imageOut.bits());


    cv::cvtColor(imgRes, imageCvOut, cv::COLOR_BGR2RGB);

    lbCam_->setPixmap(QPixmap::fromImage(imageOut.scaledToWidth(320)));
}


void
Widget::cvIRUpdate()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
    cv::Mat img128;
    ir_->getData(data);

    if (dataMin > *std::min_element(data.begin(), data.end())) {
        dataMin = *std::min_element(data.begin(), data.end());
        dataScaleAuto = (quint8)255/(dataMax-dataMin);
    }
    if (dataMax < *std::max_element(data.begin(), data.end())){
        dataMax = *std::max_element(data.begin(), data.end());
        dataScaleAuto = (quint8)255/(dataMax-dataMin);
    }

    qDebug() << "data: " << dataMin << dataMax << dataScaleAuto;
    for (int i=0; i<data.length(); i++) {
        data[i] = (data.at(i)-dataMin) * dataScaleAuto;
    }

    cv::Mat imgIn(8, 8, CV_8UC1, data.data()), imgTmp;

    cv::flip(imgIn, imgTmp, -1); //\todo - also add rotate\flip settings to config

    //scale to 128 for now
    cv::resize(imgTmp, img128, cv::Size(240,240));

    img128 += frame;// add detected edges from main camera

    QImage imageOut(img128.cols, img128.rows,  QImage::Format_RGB888);
    cv::Mat imageCvOut(cv::Size(img128.cols,img128.rows),
                       CV_8UC3, imageOut.bits());

    cv::cvtColor(img128, imageCvOut, cv::COLOR_GRAY2RGB);

    lbIR_->setPixmap(QPixmap::fromImage(imageOut.scaledToWidth(320)));
 //   qDebug() << data;
}

void
Widget::quit()
{
#ifdef DEBUG_PC
    qDebug() << __PRETTY_FUNCTION__;
#endif //DEBUG_PC
    QApplication::quit();
}
