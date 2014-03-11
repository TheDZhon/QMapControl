#pragma once

// Qt includes.
#include <QtWidgets/QMainWindow>

// QMapControl includes.
#include <QMapControl/QMapControl.h>

// Local includes.
#include "gps_neo.h"

using namespace qmapcontrol;

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class GPS: public QMainWindow
{
    Q_OBJECT

public:
    //! GPS constructor
    /*!
     * This is used to construct a GPS Widhet.
     * @param parent QObject parent ownership.
     */
    GPS(QWidget* parent = 0);

    //! Disable copy constructor.
    ///GPS(const GPS&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///GPS& operator=(const GPS&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~GPS() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

public slots:
    /*!
     * Called when a new GPS position received.
     * @param time The GPS time.
     * @param point The GPS position.
     */
    void updatePosition(float time, QPointF point);

private:
    /// The main map control.
    QMapControl* m_map_control;

    /// The GPS Nep.
    GPS_Neo m_gps_neo;

};

