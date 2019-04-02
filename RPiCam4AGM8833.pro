# Copyright(C) 2019 Andrey Yaromenok, ayaromenok@gmail.com
# MIT License
# https://github.com/ayaromenok/RaspberryPiCameraForAMG8833/blob/master/LICENSE

QT       += core gui widgets
QT       += multimedia multimediawidgets

TARGET = RPiCam4AGM8833

DEFINES += CAMERA_CAPTURE_VIA_FILE
DEFINES += CAMERA_CAPTURE_CV

TEMPLATE = app

CONFIG += c++11

SOURCES += \
        src/main.cpp \
        src/widget.cpp

HEADERS += \
        src/widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

CVLINUX = /usr/local
linux:!android {
    message("* OpenCV settings for Unix/Linux.")
    INCLUDEPATH += $${CVLINUX}/include/opencv4
    LIBS += -L$${CVLINUX}/lib
    LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
    LIBS += -lopencv_highgui -lopencv_videoio
}
