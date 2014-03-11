#pragma once

// Qt includes.
#include <QtCore/QObject>
#include <QtCore/QPointF>

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */

class GPS_Modul : public QObject
{
    Q_OBJECT
public:
    //! Multidemo constructor
    /*!
     * This is used to construct a Multidemo QWidget.
     * @param parent QObject parent ownership.
     */
    GPS_Modul(QObject* parent = 0);

    //! Disable copy constructor.
    ///GPS_Modul(const GPS_Modul&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///GPS_Modul& operator=(const GPS_Modul&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~GPS_Modul() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    /*!
     * Starts the GPS Modul.
     */
    void start();

    /*!
     * Stops the GPS Modul.
     */
    void stop();

public slots:
    /**
     * Moves the position of the point when called.
     */
    void tick();

signals:
    /*!
     * Signal emitted when the position changes.
     * @param point The new position.
     */
    void positionChanged(QPointF point);

private:
    /// Whether the GPS Modul is running.
    bool m_running;
};
