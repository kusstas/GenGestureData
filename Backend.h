#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QSize>
#include <QPoint>
#include <QVector2D>

#include <QtConcurrent/QtConcurrent>
#include <QRunnable>
#include <QThreadPool>

#include <QPixmap>

class Drawer : public QRunnable
{
public:
    void run() override;

    QString name;

    QString type;
    QSize size;
    int fps;
    bool negative;
    int diameter;
    QPoint center;
    int value;
    int startAngle;
    int chaotic;

    void rotate(QDir dir, int direct);
    void fall(QDir dir, int direct);
};

class Backend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
public:
    explicit Backend(QObject *parent = nullptr);

    Q_INVOKABLE void setType(QString type);
    Q_INVOKABLE void setSize(QString size);
    Q_INVOKABLE void setFps(int fps);
    Q_INVOKABLE void setNegative(bool negative);
    Q_INVOKABLE void setDiameter(QString diameter);
    Q_INVOKABLE void setCenter(QString center);
    Q_INVOKABLE void setMaxShift(QString maxShift);
    Q_INVOKABLE void setValue(QString value);
    Q_INVOKABLE void setStartAngle(QString startAngle);
    Q_INVOKABLE void setChaotic(int chaotic);
    Q_INVOKABLE void setCount(int count);
    Q_INVOKABLE void generate();
    Q_INVOKABLE void cancel();

    bool running() const;
    bool busy() const;

signals:

    void busyChanged(bool busy);

public slots:
    void setBusy(bool busy);

private:
    void process();
    int randomInRange(int l, int r);

    QThreadPool m_pool;

    QString m_type;
    QSize m_size{};
    QPoint m_center{};
    QPoint m_maxShift{};
    int m_diameterFrom;
    int m_diameterTo;
    int m_fps;
    int m_valueFrom;
    int m_valueTo;
    int m_startAngleFrom;
    int m_startAngleTo;
    int m_chaotic;
    int m_count;
    bool m_negative;
    bool m_busy = false;
    bool m_running = false;
};

#endif // BACKEND_H
