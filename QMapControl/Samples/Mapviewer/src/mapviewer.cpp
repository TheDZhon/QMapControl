#include "mapviewer.h"

// QMapControl includes.
#include <QMapControl/LayerMapAdapter.h>
#include <QMapControl/MapAdapterOSM.h>

/*!
 * This application is just a simple map viewer. A Mapadapter is created (OpenStreetMaps)
 * and added to a layer. The layer is given to the QMapControl.
 *
 * If the window is resized the map widget will adjust its size.
 */

Mapviewer::Mapviewer(QWidget* parent)
    : QMainWindow(parent)
{
    // Create a new QMapControl.
    m_map_control = new QMapControl(QSizeF(380.0, 540.0));

    // Create/add a layer with the default OSM map adapter.
    m_map_control->addLayer(std::make_shared<LayerMapAdapter>("OpenStreetMap-Layer", std::make_shared<MapAdapterOSM>()));

    // Show QMapControl in QMainWindow.
    setCentralWidget(m_map_control);
}

void Mapviewer::resizeEvent(QResizeEvent * resize_event)
{
    // Set the new viewport size.
    m_map_control->setViewportSize(resize_event->size());
}
