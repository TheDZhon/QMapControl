#include "mapviewer.h"
/*!
 * \example mapviewer.cpp
 * This application is just a simple map viewer. A Mapadapter is created (OpenStreetmaps)
 * and added to a layer. The layer is given to the QMapControl.
 * Two Buttons are available to adjust the zoom level. If the window is
 * resized the map widget will adjust its size.
 *
 * You can find this example here: QMapControl/Samples/Mapviewer
 * \image html sample_mapviewer.png "screenshot"
 */

#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

Mapviewer::Mapviewer(QWidget *parent)
    : QMainWindow(parent)
{
    // create QMapControl
    mc = new QMapControl(QSizeF(380, 540));
    mc->enableScalebar(true);

    // create mapadapter, for mainlayer and overlay
    // create a layer with the mapadapter and type MapLayer
    mainlayer = std::make_shared<Layer>("OpenStreetMap-Layer");
    mainlayer->addMapAdapter(std::make_shared<MapAdapterOSM>());

    // add Layer to the QMapControl
    mc->addLayer(mainlayer);

    addZoomButtons();

    // show QMapControl in mainwindow
    setCentralWidget(mc);
}
void Mapviewer::addZoomButtons()
{
    // create buttons as controls for zoom
    QPushButton* zoomin = new QPushButton("+");
    QPushButton* zoomout = new QPushButton("-");
    zoomin->setMaximumWidth(50);
    zoomout->setMaximumWidth(50);

    connect(zoomin, SIGNAL(clicked(bool)),
            mc, SLOT(zoomIn()));
    connect(zoomout, SIGNAL(clicked(bool)),
            mc, SLOT(zoomOut()));

    // add zoom buttons to the layout of the QMapControl
    QVBoxLayout* innerlayout = new QVBoxLayout;
    innerlayout->addWidget(zoomin);
    innerlayout->addWidget(zoomout);
    mc->setLayout(innerlayout);
}


Mapviewer::~Mapviewer()
{
}

// resize the widget
void Mapviewer::resizeEvent ( QResizeEvent * event )
{
    mc->setViewportSize(event->size());
}
