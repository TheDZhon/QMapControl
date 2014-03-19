#include "multidemo.h"

// Qt includes.
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

// QMapControl includes.
#include <QMapControl/GeometryLineString.h>
#include <QMapControl/GeometryPointCircle.h>
#include <QMapControl/GeometryPointWidget.h>
#include <QMapControl/GeometryPolygon.h>
#include <QMapControl/MapAdapterGoogle.h>
#include <QMapControl/MapAdapterYahoo.h>
#include <QMapControl/MapAdapterWMS.h>

/*!
 * This is a bit complexer application, which lets you play around.
 * There are the following buttons configured:
 *  - Add Point: adds a Point to the coordinate you click (this point will be clickable).
 *  - Drag Rect: lets to drag a rectangular into which will be zoomed in.
 *  - Move To Click: moves the view middle to the clicked coordinate.
 *  - GPS: starts a "pseudo" GPS receiver which emits new positions, these are connected to the GeometryPointImage.
 *  - Follow Geom: Follows the ImagePoint, when it moves because of new GPS positions.
 *
 * A "mini" map lets you see where you are. You can even click on it to change your position.
 */
Multidemo::Multidemo(QWidget* parent)
    : QWidget(parent)
{
    // Setup the maps.
    setupMaps();

    // Setup the layout
    setupLayout();

    // Create the GPS Modul and connect the appropriate signal/slot to update the associated GeometryPointImage position.
    m_gps_modul = new GPS_Modul();
    QObject::connect(m_gps_modul, &GPS_Modul::positionChanged, m_gps_point.get(), &GeometryPointImage::setCoordinate);
}

void Multidemo::setupMaps()
{
    // Create a new QMapControl.
    m_map_control = new QMapControl(QSizeF(480.0, 640.0));

    // Set the mouse button functionality required for the QMapControl.
    m_map_control->setMouseButtonLeft(QMapControl::MouseButtonMode::Pan, false);
    m_map_control->setMouseButtonRight(QMapControl::MouseButtonMode::PanBox, false);

    // Enable the zoom controls.
    m_map_control->enableZoomControls(true, false);

    // Create a base map adapter to use.
//    std::set<projection::EPSG> wms_projections;
//    wms_projections.insert(projection::EPSG::Equirectangular);
//    std::shared_ptr<MapAdapter> mapadapter(std::make_shared<MapAdapterWMS>(QUrl("http://www2.demis.nl/wms/wms.asp?wms=WorldMap&LAYERS=Countries,Borders,Cities,Rivers,Settlements,Hillshading,Waterbodies,Railroads,Highways,Roads&FORMAT=image/png&VERSION=1.1.1&SERVICE=WMS&REQUEST=GetMap&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage&SRS=EPSG:4326&TRANSPARENT=FALSE"), wms_projections));
//    std::shared_ptr<MapAdapter> mapadapter(std::make_shared<MapAdapterOSM>());
//    std::shared_ptr<MapAdapter> mapadapter(std::make_shared<MapAdapterYahoo>());
    std::shared_ptr<MapAdapter> map_adapter(std::make_shared<MapAdapterGoogle>(MapAdapterGoogle::GoogleLayerType::MAPS));

    // Create a layer for the map adapter and add it to the QMapControl.
    std::shared_ptr<Layer> layer_map(std::make_shared<Layer>("Map Layer"));
    layer_map->addMapAdapter(map_adapter);
    m_map_control->addLayer(layer_map);

    // Create a layer to store geometries.
    std::shared_ptr<Layer> layer_geometries(std::make_shared<Layer>("Geometry Layer"));
    m_map_control->addLayer(layer_geometries);


    // Create a "mini" QMapControl.
    m_mini_map_control = new QMapControl(QSizeF(150.0, 150.0));
    m_mini_map_control->setBackgroundColour(Qt::white);

    // Disable any zoom/mouse interaction.
    m_mini_map_control->enableZoomControls(false);
    m_mini_map_control->enableLayerMouseEvents(false);
    m_mini_map_control->setMouseButtonLeft(QMapControl::MouseButtonMode::None, false);
    m_mini_map_control->setMouseButtonRight(QMapControl::MouseButtonMode::None, false);

    // Create a layer for the map adapter and add it to the "mini" QMapControl.
    std::shared_ptr<Layer> layer_mini_map(std::make_shared<Layer>("Mini Map Layer"));
    layer_mini_map->addMapAdapter(map_adapter);
    m_mini_map_control->addLayer(layer_mini_map);


    // Create some GeometryPoints to add to a GeometryLineString.
    QPen pen(QColor(255, 0, 0, 100));
    pen.setWidth(5);
    std::vector<std::shared_ptr<GeometryPoint>> points;
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.259959, 50.001781), pen));
    points.back()->setMetadata("name", "Mainz, Hauptbahnhof");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.263758, 49.998917), pen));
    points.back()->setMetadata("name", "Mainz, Münsterplatz");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.265812, 50.001952), pen));
    points.back()->setMetadata("name", "Mainz, Neubrunnenplatz");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.2688, 50.004015), pen));
    points.back()->setMetadata("name", "Mainz, Bauhofstraße LRP");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.272845, 50.00495), pen));
    points.back()->setMetadata("name", "Mainz, Landtag");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.272845, 50.00495), pen));
    points.back()->setMetadata("name", "Mainz, Brückenplatz");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.280349, 50.008173), pen));
    points.back()->setMetadata("name", "Mainz, Brückenkopf");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.273573, 50.016315), pen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Eleonorenstraße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.275145, 50.016992), pen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Johannes-Goßner-Straße");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.270476, 50.021426), pen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Ruthof");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.266445, 50.025913), pen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Kastel, Mudra Kaserne");
    points.emplace_back(std::make_shared<GeometryPointCircle>(QPointF(8.260378, 50.030345), pen));
    points.back()->setMetadata("name", "Wiesbaden-Mainz-Amoneburg, Dyckerhoffstraße");

    // Add the GeometryPoints to GeoemtryLineString.
    pen = QPen(QColor(0, 0, 255, 100));
    pen.setWidth(5);
    //    QBrush brush(Qt::red);
    //    std::shared_ptr<GeometryPolygon> line_string(std::make_shared<GeometryPolygon>(points, brush, pen));
    std::shared_ptr<GeometryLineString> line_string(std::make_shared<GeometryLineString>(points, pen));
    line_string->setMetadata("name", "Busline 54");

    // Add the GeoemtryLineString to the Geometry layer.
    layer_geometries->addGeometry(line_string);


    // Create a GPS GeometryPoint that can dynamically move, and add it to the geometry layer.
    m_gps_point = std::make_shared<GeometryPointImage>(QPointF(0.0, 0.0), ":/resources/images/marker1.png");
    m_gps_point->setAlignmentType(GeometryPoint::AlignmentType::TopRight);
    m_gps_point->setMetadata("name", "image point");
    layer_geometries->addGeometry(m_gps_point);


    // Create a GeometryPoint with a PushButton widget.
    QPushButton* pb = new QPushButton("test button", m_map_control);
    std::shared_ptr<GeometryPoint> widget_point(std::make_shared<GeometryPointWidget>(QPointF(-20.0, -20.0), pb));
    widget_point->setAlignmentType(GeometryPoint::AlignmentType::TopLeft);
    widget_point->setMetadata("name", ".");
    widget_point->setBaseZoom(3);
    layer_geometries->addGeometry(widget_point);


    // Connect the signal/slots are required.
    // Signal/slot to manage geometries being clicked.
    QObject::connect(m_map_control, &QMapControl::geometryClicked, this, &Multidemo::geometryClickEvent);
    // Signal/slot to manage geometries being selected.
    QObject::connect(m_map_control, &QMapControl::geometriesSelected, this, &Multidemo::geometriesSelectedEvent);
    // Signal/slot to manage main map control mouse press events.
    QObject::connect(m_map_control, &QMapControl::mouseEventPressCoordinate, this, &Multidemo::main_mouseEventPressCoordinate);
    // Signal/slot to manage main map control mouse release events.
    QObject::connect(m_map_control, &QMapControl::mouseEventMoveCoordinate, this, &Multidemo::main_mouseEventReleaseCoordinate);
    QObject::connect(m_map_control, &QMapControl::mouseEventReleaseCoordinate, this, &Multidemo::main_mouseEventReleaseCoordinate);
    // Signal/slot to manage "mini" map control mouse press events.
    QObject::connect(m_mini_map_control, &QMapControl::mouseEventPressCoordinate, this, &Multidemo::mini_mouseEventPressCoordinate);
}

void Multidemo::setupLayout()
{
    // Create the buttons.
    m_button_add_point = new QPushButton("Enable 'Add Point'");
    m_button_add_point->setCheckable(true);
    m_button_add_point->setMaximumWidth(100);
    m_button_add_point->setMaximumHeight(20);
    m_button_add_point->setFont(QFont("Verdana", 5));
    QObject::connect(m_button_add_point, &QPushButton::toggled, this, &Multidemo::toggleAddPoint);

    m_button_draw_rect = new QPushButton("Enable 'Drag Rect'");
    m_button_draw_rect->setCheckable(true);
    m_button_draw_rect->setMaximumWidth(100);
    m_button_draw_rect->setMaximumHeight(20);
    m_button_draw_rect->setFont(QFont("Verdana", 5));
    QObject::connect(m_button_draw_rect, &QPushButton::toggled, this, &Multidemo::toggleDrawRect);

    m_button_move_click = new QPushButton("Enable 'Move to Click'");
    m_button_move_click->setCheckable(true);
    m_button_move_click->setMaximumWidth(100);
    m_button_move_click->setMaximumHeight(20);
    m_button_move_click->setFont(QFont("Verdana", 5));
    QObject::connect(m_button_move_click, &QPushButton::toggled, this, &Multidemo::toggleMoveClick);

    m_button_gps = new QPushButton("Enable 'GPS Modul'");
    m_button_gps->setCheckable(true);
    m_button_gps->setMaximumWidth(100);
    m_button_gps->setMaximumHeight(20);
    m_button_gps->setFont(QFont("Verdana", 5));
    QObject::connect(m_button_gps, &QPushButton::toggled, this, &Multidemo::toggleGPS);

    m_button_follow_gps = new QPushButton("Enable 'Follow GPS Modul'");
    m_button_follow_gps->setCheckable(true);
    m_button_follow_gps->setMaximumWidth(100);
    m_button_follow_gps->setMaximumHeight(20);
    m_button_follow_gps->setFont(QFont("Verdana", 5));
    QObject::connect(m_button_follow_gps, &QPushButton::toggled, this, &Multidemo::toggleFollowGPS);

    // Set the focus policy of each button.
    m_button_add_point->setFocusPolicy(Qt::NoFocus);
    m_button_draw_rect->setFocusPolicy(Qt::NoFocus);
    m_button_move_click->setFocusPolicy(Qt::NoFocus);
    m_button_follow_gps->setFocusPolicy(Qt::NoFocus);
    m_button_gps->setFocusPolicy(Qt::NoFocus);


    // Create an inner layout to display buttons/"mini" map control.
    QVBoxLayout* layout_inner = new QVBoxLayout;
    layout_inner->addWidget(m_button_add_point);
    layout_inner->addWidget(m_button_draw_rect);
    layout_inner->addWidget(m_button_move_click);
    layout_inner->addWidget(m_button_gps);
    layout_inner->addWidget(m_button_follow_gps);
    layout_inner->addSpacing(300);
    layout_inner->addWidget(m_mini_map_control);

    // Set the main map control to use the inner layout.
    m_map_control->setLayout(layout_inner);

    // Create a main layout for the main map control.
    QHBoxLayout* layout_main = new QHBoxLayout;
    layout_main->addWidget(m_map_control);
    layout_main->setMargin(0);

    // Set the main layout to the QWidget.
    QWidget::setLayout(layout_main);
}

void Multidemo::resizeEvent(QResizeEvent* event)
{
    // Set the new viewport size.
    m_map_control->setViewportSize(event->size());
}

void Multidemo::toggleAddPoint(bool enable)
{
    // Should we enable it?
    if(enable)
    {
        // Update the button text.
        m_button_add_point->setText("Disable 'Add Point'");
    }
    else
    {
        // Update the button text.
        m_button_add_point->setText("Enable 'Add Point'");
    }
}

void Multidemo::toggleDrawRect(bool enable)
{
    // Should we enable it?
    if(enable)
    {
        // Set the left mouse button to draw a rect.
        m_map_control->setMouseButtonLeft(QMapControl::MouseButtonMode::DrawBox, false);

        // Update the button text.
        m_button_draw_rect->setText("Disable 'Drag Rect'");
    }
    else
    {
        // Default the left mouse button to pan.
        m_map_control->setMouseButtonLeft(QMapControl::MouseButtonMode::Pan, true);

        // Update the button text.
        m_button_draw_rect->setText("Enable 'Drag Rect'");
    }
}

void Multidemo::toggleMoveClick(bool enable)
{
    // Should we enable it?
    if(enable)
    {
        // Update the button text.
        m_button_move_click->setText("Disable 'Move to Click'");
    }
    else
    {
        // Update the button text.
        m_button_move_click->setText("Enable 'Move to Click'");
    }
}

void Multidemo::toggleGPS(bool enable)
{
    // Should we enable it?
    if(enable)
    {
        // Start the GPS Modul.
        m_gps_modul->start();

        // Update the button text.
        m_button_gps->setText("Disable 'GPS Modul'");
    }
    else
    {
        // Stop the GPS Modul.
        m_gps_modul->stop();

        // Update the button text.
        m_button_gps->setText("Enable 'GPS Modul'");
    }
}

void Multidemo::toggleFollowGPS(bool enable)
{
    // Should we follow the GPS Modul point?
    if(enable)
    {
        // Follow the GPS Modul point.
        m_map_control->followGeometry(m_gps_point);

        // Update the button text.
        m_button_follow_gps->setText("Disable 'Follow GPS Modul'");
    }
    else
    {
        // Make sure we are not following any geometry.
        m_map_control->stopFollowingGeometry();

        // Update the button text.
        m_button_follow_gps->setText("Enable 'Follow GPS Modul'");
    }
}

void Multidemo::geometriesSelectedEvent(std::map<std::string, std::vector<std::shared_ptr<Geometry>>> selected_geometries)
{
    // Loop through the layers.
    for(const auto& layers : selected_geometries)
    {
        // Loop through the geometries selected in the layer.
        for(const auto& geometry : layers.second)
        {
            // Process the clicked geometry.
            geometryClickEvent(geometry.get());
        }
    }
}

void Multidemo::geometryClickEvent(Geometry* geometry)
{
    // Is it a GeometryLineString.
    if(geometry->getGeometryType() == Geometry::GeometryType::GeometryLineString)
    {
        // GeometryLineString contain multi points, use touchedPoints() to find which points were actually touched.
        std::vector<std::shared_ptr<GeometryPoint>> touched_points = static_cast<GeometryLineString*>(geometry)->touchedPoints();

        // Loop through each point.
        for(const auto& point : touched_points)
        {
            // Display a message box with the point's details.
            QMessageBox::information(this, geometry->getMetadata("name").toString(), point->getMetadata("name").toString());
        }
    }
    // Is it a GeometryPolygon.
    if(geometry->getGeometryType() == Geometry::GeometryType::GeometryPolygon)
    {
        // GeometryPolygon contain multi points, use touchedPoints() to find which points were actually touched.
        std::vector<std::shared_ptr<GeometryPoint>> touched_points = static_cast<GeometryPolygon*>(geometry)->touchedPoints();

        // Loop through each point.
        for(const auto& point : touched_points)
        {
            // Display a message box with the point's details.
            QMessageBox::information(this, geometry->getMetadata("name").toString(), point->getMetadata("name").toString());
        }
    }
    // Is it a GeometryPoint.
    else if(geometry->getGeometryType() == Geometry::GeometryType::GeometryPoint)
    {
        // Display a message box with the point's details.
        QString message = QString("Position: ").append(QString().setNum(((GeometryPoint*)geometry)->x())).append(QString("/")).append(QString().setNum(((GeometryPoint*)geometry)->y()));
        QMessageBox::information(this, geometry->getMetadata("name").toString(), message);
    }
}

void Multidemo::main_mouseEventPressCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate)
{
    // Was there a left mouse button press?
    if(mouse_event->button() == Qt::MouseButton::LeftButton)
    {
        // Is the "Move to click" button checked?
        if(m_button_move_click->isChecked())
        {
            // Move both the "mini" and main map controls to the click coordinate.
            m_mini_map_control->setMapFocusPointAnimated(press_coordinate);
            m_map_control->setMapFocusPointAnimated(press_coordinate);
        }

        // Is the "Add point" button checked?
        if(m_button_add_point->isChecked())
        {
            // Add a GeometryPointCircle with a radius of 10.0 pixels.
            m_map_control->getLayer("Geometry Layer")->addGeometry(std::make_shared<GeometryPointCircle>(press_coordinate, 10.0));
        }
    }
}

void Multidemo::main_mouseEventReleaseCoordinate(QMouseEvent* /*mouse_event*/, QPointF /*press_coordinate*/, QPointF /*release_coordinate*/)
{
    // Update the location of the "mini" map control.
    m_mini_map_control->setMapFocusPoint(m_map_control->mapFocusPointCoord());
}

void Multidemo::mini_mouseEventPressCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate)
{
    // Was there a left mouse button press?
    if(mouse_event->button() == Qt::MouseButton::LeftButton)
    {
        // Move both the "mini" and main map controls to the click coordinate.
        m_mini_map_control->setMapFocusPointAnimated(press_coordinate);
        m_map_control->setMapFocusPointAnimated(press_coordinate);
    }
}
