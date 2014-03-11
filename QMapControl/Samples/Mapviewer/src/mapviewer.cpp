#include "mapviewer.h"

// QMapControl includes.
#include <QMapControl/Layer.h>
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

    // Create a layer.
    std::shared_ptr<Layer> layer(std::make_shared<Layer>("OpenStreetMap-Layer"));

    // Add a map adapter (OSM) to the layer.
    layer->addMapAdapter(std::make_shared<MapAdapterOSM>());

    // Add the layer to the QMapControl
    m_map_control->addLayer(layer);

    // Show QMapControl in QMainWindow.
    setCentralWidget(m_map_control);
}

void Mapviewer::resizeEvent(QResizeEvent * resize_event)
{
    // Set the new viewport size.
    m_map_control->setViewportSize(resize_event->size());
}
