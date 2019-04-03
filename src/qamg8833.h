#ifndef QAMG8833_H
#define QAMG8833_H

#include <QObject>
#include <QVector>

class QTimer;

//address
#define AMG8833_PCTL                0x00
#define AMG8833_RST                 0x01
#define AMG8833_FPS                 0x02
#define AMG8833_INT                 0x03

//values
#define AMG8833_FPS_10              0x00
#define AMG8833_FPS_1               0x01

#define AMG8833_NORMAL_MODE         0x00

#define AMG8833_INITIAL_RESET       0x3F
#define AMG8833_FLAG_RESET          0x30
#define AMG8833_DISABLE_INT         0x00

#define AMG8833_DATA_OFFSET         0x80
#define AMG8833_RES_X               0x08
#define AMG8833_RES_Y               0x08
#define AMG8833_DATA_SIZE           64


class QAmg8833 : public QObject
{
    Q_OBJECT
public:
    explicit QAmg8833(QObject *parent = nullptr);
    ~QAmg8833();
    void set10fps();
    void set1fps();
    void getData(QVector<quint8> &dataExt);


signals:

public slots:

private slots:
    void updateData();
private:
    float s12tofp32(int value);
    int             fd_;
    int             rslt_;
    QTimer          *timer_;
    QVector<quint8> data;
};

#endif // QAMG8833_H
