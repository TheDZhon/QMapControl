#include "gps_modul.h"

// Qt includes.
#include <QtCore/QTimer>

// STL includes.
#include <random>

namespace
{
    /// The current x point.
    qreal m_x = 1.0;

    /// The current y point.
    qreal m_y = 1.0;

    /// A random device.
    std::random_device m_rd;

    /// A random engine.
    std::mt19937 m_mt(m_rd());

    /// A uniform distribution.
    std::uniform_int_distribution<int> m_dist(0, 10);
}

GPS_Modul::GPS_Modul(QObject *parent)
    : QObject(parent),
      m_running(false)
{

}

void GPS_Modul::start()
{
    // Check we are not already running.
    if(m_running == false)
    {
        // Set that we are running.
        m_running = true;

        // Set a timer to start the 'ticking' (1/25 of a second).
        QTimer::singleShot(1000 / 25, this, SLOT(tick()));
    }
}
void GPS_Modul::stop()
{
    // Set to stop runnings.
    m_running = false;
}

void GPS_Modul::tick()
{
    // Perform a random check for point change (1 in 10 chance of happening).
    if(m_dist(m_mt) == 0)
    {
        // Move the point by +0.1
        m_x += 0.1;
        m_y += 0.1;

        // Emit that we have moved the point.
        emit positionChanged(QPointF(m_x ,m_y));
    }

    // Are we still running?
    if(m_running)
    {
        // Schedule the next tick (1/25 of a second).
        QTimer::singleShot(1000 / 25, this, SLOT(tick()));
    }
}
