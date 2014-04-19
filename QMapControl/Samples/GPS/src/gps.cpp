#include "gps.h"

// Qt includes.
#include <QtWidgets/QHBoxLayout>

// QMapControl includes.
#include "QMapControl/LayerMapAdapter.h"
#include <QMapControl/MapAdapterOSM.h>

/*!
 * This sample application uses GPS data from the gllin service, which you have to start manually before using this application.
 * The file /tmp/nmeaNP is read in for the posiitons.
 *
 * The application receives positions from the GPS_Neo class and writes it into a label.
 * You can toggle a button to set the view to the received coordinate.
 *
 * @see http://3rdparty.downloads.openmoko.org
 */
GPS::GPS(QWidget* parent)
    : QMainWindow(parent)
{
    // Create a new QMapControl.
    m_map_control = new QMapControl(QSizeF(480.0, 640.0));

    // Create/add a layer with the default OSM map adapter.
    m_map_control->addLayer(std::make_shared<LayerMapAdapter>("Custom Layer", std::make_shared<MapAdapterOSM>()));

    // Connect the GPS Neo signal/slot and start it.
    QObject::connect(&m_gps_neo, &GPS_Neo::new_position, this, &GPS::updatePosition);
    m_gps_neo.start();

    // Show QMapControl in QMainWindow.
    setCentralWidget(m_map_control);
}

void GPS::updatePosition(float /*time*/, PointWorldCoord point)
{
    // Update the map focus point to the GPS location.
    m_map_control->setMapFocusPoint(point);
}
