#include "gps_neo.h"

GPS_Neo::GPS_Neo(QObject *parent)
 : QObject(parent)
{
    running = false;
}


GPS_Neo::~GPS_Neo()
{
}

void GPS_Neo::start()
{
    if (!running)
    {
        running = true;
        QTimer::singleShot(1000, this, SLOT(tick()));
    }
}
void GPS_Neo::stop()
{
    running = false;
}

void GPS_Neo::tick()
{
    QFile file("/tmp/nmeaNP");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
// 		qDebug() << file.error();
        return;
    }

    QByteArray line;
    while (!file.atEnd())
    {
        line = file.readLine();
        if (line.contains("GPRMC"))
        {
            break;
        }
    }
    file.close();

    GPS_Position pos = process_line(line);

    emit(new_position(pos.getTime(), QPointF(pos.getLongitudeCoord(), pos.getLatitudeCoord())));

    if (running)
    {
        QTimer::singleShot(1000, this, SLOT(tick()));
    }
}

GPS_Position GPS_Neo::process_line(QByteArray line)
{
    line.chop(1);

    QList<QByteArray> elems = line.split(',');

    const qreal time = QString(elems.at(1)).toDouble();
    const qreal latitude = elems.at(3).toDouble() / 100.0;
    const std::string latitude_dir = QString(elems.at(4)).toStdString();
    const qreal longitude = elems.at(5).toDouble() / 100.0;
    const std::string longitude_dir = QString(elems.at(6)).toStdString();

    return GPS_Position(time, longitude, longitude_dir, latitude, latitude_dir);
}
