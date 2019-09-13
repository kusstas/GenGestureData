#include "Backend.h"

#include <QDebug>
#include <QDir>
#include <QRandomGenerator>
#include <QPainter>

Backend::Backend(QObject* parent) : QObject(parent)
{
    m_pool.setMaxThreadCount(8);
}

void Backend::setType(QString type)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    m_type = type;

    qInfo() << "Setup type:" << m_type;
}

void Backend::setSize(QString size)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = size.split("x");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_size = QSize{value, value};
        break;
    }
    case 2:
        m_size = QSize{values.front().toInt(), values.back().toInt()};
        break;
    default:
        qFatal("Wrong format size");
        return;
    }

    if (m_size.width() <= 0 || m_size.height() <= 0)
    {
        qFatal("Wrong format size(cannot be negative value)");
        return;
    }

    qInfo() << "Setup size:" << m_size;
}

void Backend::setFps(int fps)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    if (fps <= 0)
    {
        qFatal("Wrong fps value");
        return;
    }

    m_fps = fps;
    qInfo() << "Setup FPS:" << fps;
}

void Backend::setNegative(bool negative)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    m_negative = negative;

    qInfo() << "Setup Negative:" << negative;
}

void Backend::setDiameter(QString diameter)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = diameter.split("/");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_diameterFrom = value;
        m_diameterTo = value;
        break;
    }
    case 2:
        m_diameterFrom = values.front().toInt();
        m_diameterTo = values.back().toInt();
        if (m_diameterFrom > m_diameterTo)
        {
            std::swap(m_diameterFrom, m_diameterTo);
        }
        break;
    default:
        qFatal("Wrong format diameter");
        return;
    }

    if (m_diameterFrom <= 0 || m_diameterTo <= 0)
    {
        qFatal("Wrong format diameter(cannot be negative value)");
        return;
    }

    qInfo() << "Setup diameter from:" << m_diameterFrom << "to:" << m_diameterTo;
}

void Backend::setCenter(QString center)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = center.split("x");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_center = QPoint{value, value};
        break;
    }
    case 2:
        m_center = QPoint{values.front().toInt(), values.back().toInt()};
        break;
    default:
        qFatal("Wrong format center");
        return;
    }

    qInfo() << "Setup center:" << m_center;
}

void Backend::setMaxShift(QString maxShift)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = maxShift.split("x");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_maxShift = QPoint{value, value};
        break;
    }
    case 2:
        m_maxShift = QPoint{values.front().toInt(), values.back().toInt()};
        break;
    default:
        qFatal("Wrong format max shift");
        return;
    }

    m_maxShift = {qAbs(m_maxShift.x()), qAbs(m_maxShift.y())};

    qInfo() << "Setup max shift:" << m_maxShift;
}

void Backend::setValue(QString value)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = value.split("/");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_valueFrom = value;
        m_valueTo = value;
        break;
    }
    case 2:
        m_valueFrom = values.front().toInt();
        m_valueTo = values.back().toInt();
        if (m_valueFrom > m_valueTo)
        {
            std::swap(m_valueFrom, m_valueTo);
        }
        break;
    default:
        qFatal("Wrong format value");
        return;
    }

    if (m_valueFrom <= 0 || m_valueTo <= 0)
    {
        qFatal("Wrong format value(cannot be negative value)");
        return;
    }

    qInfo() << "Setup value from:" << m_valueFrom << "to:" << m_valueTo;
}

void Backend::setStartAngle(QString startAngle)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    auto values = startAngle.split("/");
    switch (values.size()) {
    case 1:
    {
        auto value = values.front().toInt();
        m_startAngleFrom = value;
        m_startAngleTo = value;
        break;
    }
    case 2:
        m_startAngleFrom = values.front().toInt();
        m_startAngleTo = values.back().toInt();
        if (m_startAngleFrom > m_startAngleTo)
        {
            std::swap(m_startAngleFrom, m_startAngleTo);
        }
        break;
    default:
        qFatal("Wrong format start angle");
        return;
    }

    qInfo() << "Setup start angle from:" << m_startAngleFrom << "to:" << m_startAngleTo;
}

void Backend::setChaotic(int chaotic)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    m_chaotic = qAbs(chaotic);

    qInfo() << "Setup chaotic:" << m_chaotic;
}

void Backend::setCount(int count)
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    m_count = qAbs(count);

    qInfo() << "Setup count:" << m_count;
}

void Backend::generate()
{
    if (busy())
    {
        qWarning("Backend is busy");
        return;
    }

    QDir::current().mkdir(m_type);
    QtConcurrent::run([this] {process();});
}

void Backend::cancel()
{
    m_running = false;
}

bool Backend::running() const
{
    return m_running;
}

bool Backend::busy() const
{
    return m_busy;
}

void Backend::setBusy(bool busy)
{
    if (m_busy == busy)
    {
        return;
    }

    m_busy = busy;
    emit busyChanged(m_busy);
}

void Backend::process()
{
    setBusy(true);
    qInfo() << "Generate data started";

    m_running = true;

    for (int i = 0; i < m_count && running(); i++)
    {
        auto drawer = new Drawer();
        drawer->setAutoDelete(true);
        drawer->name = QString::number(i);
        drawer->type = m_type;
        drawer->size = m_size;
        drawer->fps = m_fps;
        drawer->negative = m_negative;
        drawer->diameter = randomInRange(m_diameterFrom, m_diameterTo);
        drawer->center = m_center + QPoint{randomInRange(-m_maxShift.x(), m_maxShift.x()),
                                           randomInRange(-m_maxShift.y(), m_maxShift.y())};
        drawer->value = randomInRange(m_valueFrom, m_valueTo);
        drawer->startAngle = randomInRange(m_startAngleFrom, m_startAngleTo);
        drawer->chaotic = m_chaotic;
        m_pool.start(drawer);
    }

    m_running = false;
    m_pool.waitForDone();
    qInfo() << "Generate data finished";
    setBusy(false);
}

int Backend::randomInRange(int l, int r)
{
    if (l == r)
    {
        return l;
    }

    return QRandomGenerator::global()->bounded(l, r);
}

void Drawer::run()
{
    qInfo() << "Drawer:" << name << "started." << type
            << "diameter" << diameter
            << "center" << center
            << "value" << value
            << "start angle" << startAngle;

    auto dir = QDir::current();
    dir.cd(type);
    dir.mkdir(name);
    dir.cd(name);

    if (type == "Rotate Left") {
        rotate(dir, -1);
    }
    else if (type == "Rotate Right") {
        rotate(dir, 1);
    }
    else if (type == "To Down") {
        fall(dir, 1);
    }
    else if (type == "To Up") {
        fall(dir, -1);
    }

    qInfo() << "Drawer:" << name << "finished";
}

void Drawer::rotate(QDir dir, int direct)
{
    float angle = (startAngle / 180) * M_PI;
    float step = (2 * M_PI) / fps * -direct;

    for (int i = 0; i < fps; i++)
    {
        QPixmap pixmap(size);
        pixmap.fill(negative ? Qt::black : Qt::white);
        QPainter painter(&pixmap);
        painter.setBrush(negative ? Qt::white : Qt::black);

        float x = std::sin(angle) * value;
        float y = std::cos(angle) * value;

        QPoint point{static_cast<int>(x), static_cast<int>(y)};
        point += center;

        if (chaotic > 0)
        {
            point.setX(point.x() + QRandomGenerator::global()->bounded(-chaotic, chaotic));
            point.setY(point.y() + QRandomGenerator::global()->bounded(-chaotic, chaotic));
        }

        painter.drawEllipse(point, diameter, diameter);

        pixmap.save(dir.filePath(QString::number(i) + ".png"), "PNG");
        angle += step;
    }
}

void Drawer::fall(QDir dir, int direct)
{
    float coord = center.y() - (direct * (value / 2));
    float step = static_cast<float>(value) / fps * direct;

    for (int i = 0; i < fps; i++)
    {
        QPixmap pixmap(size);
        pixmap.fill(negative ? Qt::black : Qt::white);
        QPainter painter(&pixmap);
        painter.setBrush(negative ? Qt::white : Qt::black);

        QPoint point{center.x(), static_cast<int>(coord)};

        if (chaotic > 0)
        {
            point.setX(point.x() + QRandomGenerator::global()->bounded(-chaotic, chaotic));
            point.setY(point.y() + QRandomGenerator::global()->bounded(-chaotic, chaotic));
        }

        painter.drawEllipse(point, diameter, diameter);

        pixmap.save(dir.filePath(QString::number(i) + ".png"), "PNG");
        coord += step;
    }
}
