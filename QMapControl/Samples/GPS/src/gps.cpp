#include "gps.h"

#include "QMapControl/Layer.h"

/*!
 * \example gps.cpp
 * This sample application uses GPS data from the gllin service, which you have to start manually before using this application.
 * The file /tmp/nmeaNP is read in for the posiitons.
 *
 * The application receives positions from the GPS_Neo class and writes it into a label.
 * You can toggle a button to set the view to the received coordinate.
 *
 * You can find this example here: MapAPI/Samples/GPS
 * \image html sample_gps.png "screenshot"
 * @see http://3rdparty.downloads.openmoko.org
 */

#include <QtWidgets/QHBoxLayout>
#include <QMapControl/MapAdapterOSM.h>

GPS::GPS()
{
    // create QMapControl
    mc = new QMapControl(QSizeF(480,640));
// 	ImageManager::instance()->setProxy("www-cache", 8080);

    // create a layer with the mapadapter and type MapLayer
    std::shared_ptr<Layer> l(std::make_shared<Layer>("Custom Layer"));
    l->addMapAdapter(std::make_shared<MapAdapterOSM>());

    // add Layer to the QMapControl
    mc->addLayer(l);

    // display the QMapControl in the application
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(mc);
    setLayout(layout);

    // create buttons as controls for zoom
    QPushButton* zoomin = new QPushButton("+");
    QPushButton* zoomout = new QPushButton("-");
    followgps = new QPushButton("F");
    followgps->setCheckable(true);
    gpsposition = new QLabel();
    zoomin->setMaximumWidth(50);
    zoomout->setMaximumWidth(50);
    followgps->setMaximumWidth(50);
    gpsposition->setFont(QFont("Arial", 10));

    connect(zoomin, SIGNAL(clicked(bool)),
              mc, SLOT(zoomIn()));
    connect(zoomout, SIGNAL(clicked(bool)),
              mc, SLOT(zoomOut()));

    // add zoom buttons to the layout of the QMapControl
    QVBoxLayout* innerlayout = new QVBoxLayout;
    innerlayout->addWidget(zoomin);
    innerlayout->addWidget(zoomout);
    innerlayout->addWidget(followgps);
    innerlayout->addWidget(gpsposition);
    mc->setLayout(innerlayout);

    GPS_Neo* gm = new GPS_Neo();
    connect(gm, SIGNAL(new_position(float, QPointF)),
              this, SLOT(updatePosition(float, QPointF)));
    gm->start();
}

GPS::~GPS()
{
}

void GPS::updatePosition(float time, QPointF coordinate)
{
    gpsposition->setText(QString::number(time) + " / " + QString::number(coordinate.x()) + " / " + QString::number(coordinate.y()));
    if (followgps->isChecked())
    {
        mc->setMapFocusPoint(coordinate);
    }
}
