#pragma once

// Qt includes.
#include <QtCore/QObject>
#include <QtCore/QPointF>

// QMapControl includes.
#include <QMapControl/Point.h>
#include <QMapControl/GPS_Position.h>

using namespace qmapcontrol;

//! A parser for the NMEA data format.
/*!
 * This class parses gps data from the Neo´s gllin service, which you have to start manually.
 * It reads the device file every second and emits a signal which contains a GPS_Position.
 * @see http://3rdparty.downloads.openmoko.org
 *
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class GPS_Neo: public QObject
{
    Q_OBJECT

public:
    //! GPS_Neo constructor
    /*!
     * This is used to construct a GPS_Neo.
     * @param parent QObject parent ownership.
     */
    explicit GPS_Neo(QObject* parent = 0);

    //! Disable copy constructor.
    ///GPS_Neo(const GPS_Neo&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///GPS_Neo& operator=(const GPS_Neo&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~GPS_Neo() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    /*!
     * Starts the gps data collection.
     */
    void start();

    /*!
     * Stops the gps data collection.
     */
    void stop();

public slots:
    /**
     * Check for the next position from the GPS device.
     */
    void tick();

signals:
    /*!
     * Signal emitted when the position changes.
     * @param time The GPS time.
     * @param point The GPS position.
     */
    void new_position(float time, PointWorldCoord point);

private:
    /*!
     * Parse a GPS data line.
     * @param line The line to process.
     * @return the populate GPS details.
     */
    GPS_Position process_line(QByteArray line);

private:
    /// Whether the GPS collection is running.
    bool m_running;
};
