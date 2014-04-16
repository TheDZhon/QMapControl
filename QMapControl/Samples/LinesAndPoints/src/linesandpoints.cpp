#include "linesandpoints.h"

// Qt includes.
#include <QtWidgets/QMessageBox>

// QMapControl includes.
#include <QMapControl/GeometryLineString.h>
#include <QMapControl/GeometryPointCircle.h>
#include <QMapControl/GeometryPointImage.h>
#include <QMapControl/GeometryPolygon.h>
#include <QMapControl/MapAdapterGoogle.h>

/*!
 * This application demonstrates how to use Geometry objects and how to add them to a layer.
 *
 * Here are used three different point types:
 *  - One which displays a image.
 *  - One which draws a plain circle.
 *  - One which uses a QPen to draw a circle.
 *  - One which has no markers.
 * Then these Points were added to a LineString.
 */
LinesAndPoints::LinesAndPoints(QWidget *parent)
    : QMainWindow(parent)
{
    // Create a new QMapControl.
    m_map_control = new QMapControl(QSizeF(480.0, 640.0));

    // Create a custom layer.
    std::shared_ptr<Layer> custom_layer(std::make_shared<Layer>("Custom Layer"));

    // Add a map adapter (OSM) to the custom layer.
    custom_layer->addMapAdapter(std::make_shared<MapAdapterGoogle>());

    // Add the custom layer.
    m_map_control->addLayer(custom_layer);

    // Create a LineString of different points.
    std::vector<std::shared_ptr<GeometryPoint>> points;

    // Image points.
    points.emplace_back(std::make_shared<GeometryPointImage>(PointWorldCoord(8.259959, 50.001781), ":/resources/images/bus_stop.png"));
    points.back()->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
    points.back()->setMetadata("name", "Mainz, Hauptbahnhof");
    points.emplace_back(std::make_shared<GeometryPointImage>(PointWorldCoord(8.263758, 49.998917), ":/resources/images/bus_stop.png"));
    points.back()->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
    points.back()->setMetadata("name", "Mainz, Münsterplatz");
    points.emplace_back(std::make_shared<GeometryPointImage>(PointWorldCoord(8.265812, 50.001952), ":/resources/images/bus_stop.png"));
    points.back()->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
    points.back()->setMetadata("name", "Mainz, Neubrunnenplatz");

    // Circle points.
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.2688, 50.004015)));
    points.back()->setMetadata("name", "Mainz, Bauhofstraße LRP");
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.272845, 50.00495)));
    points.back()->setMetadata("name", "Mainz, Landtag");
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.280349, 50.008173)));
    points.back()->setMetadata("name", "Mainz, Brückenkopf");

    // Circle points with large green borders.
    QPen point_pen(QColor(0,255,0));
    point_pen.setWidth(3);
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.273573, 50.016315), 15));
    points.back()->setPen(point_pen);
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Eleonorenstraße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.275145, 50.016992), 15));
    points.back()->setPen(point_pen);
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Johannes-Goßner-Straße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(PointWorldCoord(8.270476, 50.021426), 15));
    points.back()->setPen(point_pen);
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Ruthof");

    // Basic Points
    points.emplace_back(std::make_shared<GeometryPoint>(8.266445, 50.025913));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Mudra Kaserne");
    points.emplace_back(std::make_shared<GeometryPoint>(8.260378, 50.030345));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Amoneburg, Dyckerhoffstraße");

    // A QPen also can use transparency.
    QPen line_pen(QColor(0, 0, 255, 100));
    line_pen.setWidth(5);

    // Add the Points and the QPen to a LineString.
    std::vector<PointWorldCoord> raw_points;
    for(const auto& point : points)
    {
        // Add the point.
        raw_points.push_back(point->coord());

        // Also add the point to the custom layer.
        custom_layer->addGeometry(point);
    }
    std::shared_ptr<GeometryLineString> line_string(std::make_shared<GeometryLineString>(raw_points));
    line_string->setPen(line_pen);
    line_string->setMetadata("name", "Busline 54");

    // Add the LineString to the custom layer.
    custom_layer->addGeometry(line_string);

    // Connect click events of the layer to this object.
    QObject::connect(custom_layer.get(), &Layer::geometryClicked, this, &LinesAndPoints::geometryClickEvent);

    // Sets the view to the interesting area.
    std::vector<PointWorldCoord> view_points;
    view_points.emplace_back(PointWorldCoord(8.24764, 50.0319));
    view_points.emplace_back(PointWorldCoord(8.28412, 49.9998));
    m_map_control->setMapFocusPoint(view_points);

    // Show QMapControl in QMainWindow.
    setCentralWidget(m_map_control);
}

void LinesAndPoints::geometryClickEvent(Geometry* geometry)
{
    // Is it a GeometryPoint.
    if(geometry->getGeometryType() == Geometry::GeometryType::GeometryPoint)
    {
        // Display a message box with the point's details.
        QMessageBox::information(this, geometry->getMetadata("name").toString(), "");
    }
}
