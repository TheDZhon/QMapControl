#include "linesandpoints.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

/*!
 * \example linesandpoints.cpp
 * This application demonstrates how to use Geometry objects and how to add them to a layer.
 *
 * Here are used three different point types:
 *  - One which displays a image
 *  - One which draws a plain circle
 *  - One which uses a QPen to draw a circle
 *  - One which has no markers
 * Then these Points were added to a LineString
 *
 * Also there is a keylistener.
 *
 * You can find this example here: MapAPI/Samples/LinesAndPoints
 * \image html sample_linesandpoints.png "screenshot"
 */
LinesAndPoints::LinesAndPoints(QWidget *parent)
    : QWidget(parent)
{
    // the size which the QMapControl should fill
    QSizeF size = QSizeF(480,640);

    mc = new QMapControl(size);
    // create layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(mc);
    setLayout(layout);

    // create layer
    std::shared_ptr<Layer> l(std::make_shared<Layer>("Custom Layer"));
    l->addMapAdapter(std::make_shared<MapAdapterOSM>());

    mc->addLayer(l);

    // create a LineString
    std::vector<std::shared_ptr<GeometryPoint>> points;
    // Points with image
    points.emplace_back(std::make_shared<GeometryPointImage>(QPointF(8.259959, 50.001781), "images/bus_stop.png", GeometryPoint::AlignmentType::BottomLeft));
    points.back()->setMetadata("name", "Mainz, Hauptbahnhof");
    points.emplace_back(std::make_shared<GeometryPointImage>(QPointF(8.263758, 49.998917), "images/bus_stop.png", GeometryPoint::AlignmentType::BottomLeft));
    points.back()->setMetadata("name", "Mainz, Münsterplatz");
    points.emplace_back(std::make_shared<GeometryPointImage>(QPointF(8.265812, 50.001952), "images/bus_stop.png", GeometryPoint::AlignmentType::BottomLeft));
    points.back()->setMetadata("name", "Mainz, Neubrunnenplatz");
    // Points with a circle
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.2688, 50.004015), GeometryPoint::AlignmentType::Middle));
    points.back()->setMetadata("name", "Mainz, Bauhofstraße LRP");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.272845, 50.00495), GeometryPoint::AlignmentType::Middle));
    points.back()->setMetadata("name", "Mainz, Landtag");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.280349, 50.008173), GeometryPoint::AlignmentType::Middle));
    points.back()->setMetadata("name", "Mainz, Brückenkopf");
    // A QPen can be used to customize the
//    QPen* pointpen = new QPen(QColor(0,255,0));
//    pointpen->setWidth(3);
    QPen pointpen(QColor(0,255,0));
    pointpen.setWidth(3);
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.273573, 50.016315), 15, GeometryPoint::AlignmentType::Middle, pointpen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Eleonorenstraße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.275145, 50.016992), 15, GeometryPoint::AlignmentType::Middle, pointpen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Johannes-Goßner-Straße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.270476, 50.021426), 15, GeometryPoint::AlignmentType::Middle, pointpen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Ruthof");
    // "Blind" Points
    points.emplace_back(std::make_shared<GeometryPoint>(8.266445, 50.025913));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Mudra Kaserne");
    points.emplace_back(std::make_shared<GeometryPoint>(8.260378, 50.030345));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Amoneburg, Dyckerhoffstraße");

    // A QPen also can use transparency
//    QPen* linepen = new QPen(QColor(0, 0, 255, 100));
//    linepen->setWidth(5);
    QPen linepen(QColor(0, 0, 255, 100));
    linepen.setWidth(5);
    // Add the Points and the QPen to a LineString
    std::shared_ptr<GeometryLineString> ls(std::make_shared<GeometryLineString>(points, linepen));
    ls->setMetadata("name", "Busline 54");

    // Add the LineString to the layer
    l->addGeometry(ls);

    // Connect click events of the layer to this object
    connect(l.get(), SIGNAL(geometryClicked(Geometry*)),
              this, SLOT(geometryClicked(Geometry*)));

    // Sets the view to the interesting area
    std::vector<QPointF> view;
    view.emplace_back(8.24764, 50.0319);
    view.emplace_back(8.28412, 49.9998);
    mc->setMapFocusPoint(view);

    addZoomButtons();
}

void LinesAndPoints::addZoomButtons()
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

void LinesAndPoints::geometryClicked(Geometry* geom)
{
    qDebug() << "Element clicked: " << geom->getMetadata("name");
    if (geom->getGeometryType() == Geometry::GeometryType::GeometryLineString)
    {
        std::vector<std::shared_ptr<GeometryPoint>> pp = static_cast<GeometryLineString*>(geom)->touchedPoints();
        qDebug() << "number of child elements: " << pp.size();
        for(const auto& p : pp)
        {
            QMessageBox::information(this, geom->getMetadata("name").toString(), p->getMetadata("name").toString());
        }
    }
    else if (geom->getGeometryType() == Geometry::GeometryType::GeometryPoint)
    {
        QMessageBox::information(this, geom->getMetadata("name").toString(), "just a point");
    }
}

LinesAndPoints::~LinesAndPoints()
{
}

void LinesAndPoints::keyPressEvent(QKeyEvent* evnt)
{
    if (evnt->key() == 49 || evnt->key() == 17825792)  // tastatur '1'
    {
        mc->zoomIn();
    }
    else if (evnt->key() == 50)
    {
        mc->setMapFocusPointAnimated(QPointF(8.25, 60));
    }
    else if (evnt->key() == 51 || evnt->key() == 16777313)     // tastatur '3'
    {
        mc->zoomOut();
    }
    else if (evnt->key() == 54) // 6
    {
        mc->setMapFocusPoint(QPointF(8,50));
    }
    else if (evnt->key() == 16777234) // left
    {
        mc->scrollViewLeft();
    }
    else if (evnt->key() == 16777236) // right
    {
        mc->scrollViewRight();
    }
    else if (evnt->key() == 16777235 ) // up
    {
        mc->scrollViewUp();
    }
    else if (evnt->key() == 16777237) // down
    {
        mc->scrollViewDown();
    }
    else if (evnt->key() == 48 || evnt->key() == 17825797) // 0
    {
        emit(close());
    }
    else
    {
        qDebug() << evnt->key() << endl;
    }
}
