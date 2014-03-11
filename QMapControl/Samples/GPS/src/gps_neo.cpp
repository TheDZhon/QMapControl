#include "gps_neo.h"

// Qt includes.
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTimer>

GPS_Neo::GPS_Neo(QObject* parent)
    : QObject(parent),
      m_running(false)
{

}

void GPS_Neo::start()
{
    // Is it already running?
    if(m_running == false)
    {
        // Set to running.
        m_running = true;

        // Set timer to start ticking.
        QTimer::singleShot(1000, this, SLOT(tick()));
    }
}

void GPS_Neo::stop()
{
    // Set to stop running.
    m_running = false;
}

void GPS_Neo::tick()
{
    // Open a handle to the device.
    QFile file("/tmp/nmeaNP");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        qDebug() << "Unable to open handle to device: " << file.errorString();
    }
    else
    {
        // Read in the data until we reach the end..
        QByteArray line;
        while(file.atEnd() == false)
        {
            // Read the next line in.
            line = file.readLine();

            // Does the line contain "GPRMC"?
            if(line.contains("GPRMC"))
            {
                // Break the loop.
                break;
            }
        }

        // Close the file handle.
        file.close();

        // Create a position from the line fetched.
        const GPS_Position pos = process_line(line);

        // Emit our new position fetched.
        emit new_position(pos.getTime(), QPointF(pos.getLongitudeCoord(), pos.getLatitudeCoord()));

        // Are we still running?
        if(m_running)
        {
            // Set timer for the next tick.
            QTimer::singleShot(1000, this, SLOT(tick()));
        }
    }
}

GPS_Position GPS_Neo::process_line(QByteArray line)
{
    // Remove the last byte from the array.
    line.chop(1);

    // Split the line using ','.
    QList<QByteArray> elems = line.split(',');

    // Fetch the values.
    const qreal time = QString(elems.at(1)).toDouble();
    const qreal latitude = elems.at(3).toDouble() / 100.0;
    const std::string latitude_dir = QString(elems.at(4)).toStdString();
    const qreal longitude = elems.at(5).toDouble() / 100.0;
    const std::string longitude_dir = QString(elems.at(6)).toStdString();

    // Create and return the populated GPS position.
    return GPS_Position(time, longitude, longitude_dir, latitude, latitude_dir);
}
