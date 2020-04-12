#include "citymap.h"

// Qt includes.
#include <QtWidgets/QMenuBar>

// STL includes.
#include <cmath>

// QMapControl includes.
#include <QMapControl/GeometryPointImage.h>
#include <QMapControl/GeometryPointImageScaled.h>
#include <QMapControl/GeometryWidget.h>
#include <QMapControl/LayerMapAdapter.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/MapAdapterYahoo.h>
#include <QMapControl/MapAdapterGoogle.h>
#include <QMapControl/MapAdapterOTM.h>
#include <QMapControl/MapAdapterBing.h>

/*!
 * This demo application shows more features of the QMapControl.
 * It shows images, which changes its size when changing the zoomlevel.
 * You can display/hide layers and choose different map providers.
 * Also it demonstrates more possibilities for user interaction:
 * - notes can be added to any coordinate (a QTextEdit is used for editing the note).
 * - the user can measure distances between two points.
 */
Citymap::Citymap(QWidget* parent)
    : QMainWindow(parent),
      m_mode_note_adding(false),
      m_mode_distance_calculating(false),
      m_coord_start(0.0, 0.0),
      m_coord_end(0.0, 0.0)
{
    // Create a new QMapControl.
    m_map_control = new QMapControl(QSizeF(400.0, 590.0));

    // Show QMapControl in QMainWindow.
    setCentralWidget(m_map_control);

    // Create/add a layer with the default OSM map adapter.
    m_map_control->addLayer(std::make_shared<LayerMapAdapter>("Map", std::make_shared<MapAdapterOSM>()));

    // Create a layer for the Yahoo streets overlay (hide it by default).
    m_layer_yahoo_streets = std::make_shared<LayerMapAdapter>("Yahoo: Street Overlay", std::make_shared<MapAdapterYahoo>(QUrl("http://us.maps3.yimg.com/aerial.maps.yimg.com/png?v=2.2&t=h&s=256&x=%x&y=%y&z=%zoom")));
    m_layer_yahoo_streets->setVisible(false);
    m_map_control->addLayer(m_layer_yahoo_streets);

    // Create a layer for notes/distance measurements.
    m_layer_notes = std::make_shared<LayerGeometry>("Notes");
    m_map_control->addLayer(m_layer_notes);

    // Add sights, pubs, museums and tours.
    addSights();
    addPubs();
    addMuseums();
    addTours();

    // Create actions and menus.
    createActions();
    createMenus();

    // Add signal/slot to connect notes layer to edit method.
    QObject::connect(m_layer_notes.get(), &LayerGeometry::geometryClicked, this, &Citymap::editNote);

    // Set the map focus and zoom to Mainz.
    m_map_control->setMapFocusPoint(PointWorldCoord(8.26, 50.0));
    m_map_control->setZoom(13);

    // Create a text edit geometry widget (and hide for now).
    m_text_edit = new QTextEdit(m_map_control);
    m_text_edit->setGeometry(0,0,200,100);
    m_geometry_text_edit = std::make_shared<GeometryWidget>(PointWorldCoord(0.0, 0.0), m_text_edit);
    m_geometry_text_edit->setAlignmentType(GeometryPoint::AlignmentType::TopLeft);
    m_geometry_text_edit->setVisible(false);
    m_layer_notes->addGeometry(m_geometry_text_edit);
}

void Citymap::addSights()
{
    // Create the sights layer.
    m_layer_sights = std::make_shared<LayerGeometry>("Sights");
    m_map_control->addLayer(m_layer_sights);

    // Create the "Mainzer Dom" sight and add it to the layer.
    std::shared_ptr<GeometryPoint> dom(std::make_shared<GeometryPointImageScaled>(PointWorldCoord(8.274167, 49.998889), ":/resources/images/180-dom.jpg", 17));
    dom->setMetadata("name", "Mainzer Dom");
    m_layer_sights->addGeometry(dom);

    // Create the "St. Stephan" sight and add it to the layer.
    std::shared_ptr<GeometryPoint> stephan(std::make_shared<GeometryPointImageScaled>(PointWorldCoord(8.268611, 49.995556), ":/resources/images/180-stephan.jpg", 17));
    stephan->setMetadata("name", "St. Stephan");
    m_layer_sights->addGeometry(stephan);

    // Create the "St. Quintin" sight and add it to the layer.
    std::shared_ptr<GeometryPoint> quitin(std::make_shared<GeometryPointImageScaled>(PointWorldCoord(8.272222, 50.000833), ":/resources/images/180-quintin.jpg", 17));
    quitin->setMetadata("name", "St. Quintin");
    m_layer_sights->addGeometry(quitin);

    // Connect signal/slot to handle sights being clicked.
    QObject::connect(m_layer_sights.get(), &LayerGeometry::geometryClicked, this, &Citymap::geometryClicked);
}

void Citymap::addPubs()
{
    // Create the pubs layer.
    m_layer_pubs = std::make_shared<LayerGeometry>("Pubs");
    m_map_control->addLayer(m_layer_pubs);

    // Create the "Bagatelle" pub and add it to the layer.
    std::shared_ptr<GeometryPoint> bagatelle(std::make_shared<GeometryPointImage>(PointWorldCoord(8.2606, 50.0052), ":/resources/images/pub.png"));
    bagatelle->setMetadata("name", "Bagatelle");
    m_layer_pubs->addGeometry(bagatelle);

    // Create the "Nirgendwo" pub and add it to the layer.
    std::shared_ptr<GeometryPoint> nirgendwo(std::make_shared<GeometryPointImage>(PointWorldCoord(8.2595, 50.0048), ":/resources/images/pub.png"));
    nirgendwo->setMetadata("name", "Nirgendwo");
    m_layer_pubs->addGeometry(nirgendwo);

    // Create the "Krokodil" pub and add it to the layer.
    std::shared_ptr<GeometryPoint> krokodil(std::make_shared<GeometryPointImage>(PointWorldCoord(8.2594, 50.0106), ":/resources/images/pub.png"));
    krokodil->setMetadata("name", "Krokodil");
    m_layer_pubs->addGeometry(krokodil);

    // Connect signal/slot to handle pubs being clicked.
    QObject::connect(m_layer_pubs.get(), &LayerGeometry::geometryClicked, this, &Citymap::geometryClickedPub);
}

void Citymap::addMuseums()
{
    // Create the museums layer.
    m_layer_museum = std::make_shared<LayerGeometry>("Museums");
    m_map_control->addLayer(m_layer_museum);

    // Create the "rgzm" pub and add it to the layer.
    std::shared_ptr<GeometryPoint> rgzm(std::make_shared<GeometryPointImageScaled>(PointWorldCoord(8.269722, 50.006111), ":/resources/images/180-rgzm.jpg", 17));
    rgzm->setMetadata("name", "rgzm");
    m_layer_museum->addGeometry(rgzm);

    // Create the "lm" pub and add it to the layer.
    std::shared_ptr<GeometryPoint> lm(std::make_shared<GeometryPointImageScaled>(PointWorldCoord(8.26778, 50.00385), ":/resources/images/180-lm.jpg", 17));
    lm->setMetadata("name", "lm");
    m_layer_museum->addGeometry(lm);

    // Connect signal/slot to handle sights being clicked.
    QObject::connect(m_layer_museum.get(), &LayerGeometry::geometryClicked, this, &Citymap::geometryClicked);
}

void Citymap::addTours()
{
    // Create the tours layer.
    m_layer_tours = std::make_shared<LayerGeometry>("Tours");
    m_map_control->addLayer(m_layer_tours);

    // Create a pen to draw.
    QPen pen(QColor(0, 0, 255, 100));
    pen.setWidth(5);

    // Add the points of the sights tour.
    std::vector<PointWorldCoord> points;
    points.emplace_back(8.26015, 50.0015);
    points.emplace_back(8.2617, 50.0012);
    points.emplace_back(8.26423, 50.0002);
    points.emplace_back(8.26698, 50.0024);
    points.emplace_back(8.27065, 50.0012);
    points.emplace_back(8.27152, 50.0016);
    points.emplace_back(8.27225, 50.0004);
    points.emplace_back(8.27333, 49.9994);
    points.emplace_back(8.26946, 49.9983);
    points.emplace_back(8.27105, 49.9973);
    points.emplace_back(8.27024, 49.9972);
    points.emplace_back(8.26833, 49.9958);

    // Create the sights tour as a Line String and add it to the notes layer.
    m_tour_sights = std::make_shared<GeometryLineString>(points);
    m_tour_sights->setPen(pen);
    m_layer_tours->addGeometry(m_tour_sights);

    // Add the points of the pub tour.
    points.clear();
    points.emplace_back(8.2606, 50.0051);
    points.emplace_back(8.2602, 50.0050);
    points.emplace_back(8.2598, 50.0044);
    points.emplace_back(8.2569, 50.0057);
    points.emplace_back(8.2595, 50.0083);
    points.emplace_back(8.2587, 50.0086);
    points.emplace_back(8.2589, 50.0100);
    points.emplace_back(8.2590, 50.0105);

    // Create the pub tour as a Line String and add it to the notes layer.
    m_tour_pubs = std::make_shared<GeometryLineString>(points);
    m_tour_pubs->setPen(pen);
    m_layer_tours->addGeometry(m_tour_pubs);

    // Add the points of the museum tour.
    points.clear();
    points.emplace_back(8.25987, 50.0018);
    points.emplace_back(8.26192, 50.0019);
    points.emplace_back(8.26301, 50.0031);
    points.emplace_back(8.26459, 50.0026);
    points.emplace_back(8.26601, 50.004);
    points.emplace_back(8.26781, 50.0033);
    points.emplace_back(8.27052, 50.0054);
    points.emplace_back(8.2697, 50.0059);

    // Create the museum tour as a Line String and add it to the notes layer.
    m_tour_museums = std::make_shared<GeometryLineString>(points);
    m_tour_museums->setPen(pen);
    m_layer_tours->addGeometry(m_tour_museums);
}

void Citymap::geometryClicked(const Geometry* geometry)
{
    // Check clicks aren't being ignore and we aren't in the middle of adding a note.
    if(m_mode_distance_calculating == false && m_mode_note_adding == false)
    {
        // Create a new info dialog.
        InfoDialog* info_dialog = new InfoDialog(this);
        info_dialog->setWindowTitle(geometry->metadata("name").toString());

        // Handle the sight/musem.
        if(geometry->metadata("name") == "Mainzer Dom")
        {
            info_dialog->setInfotext("<h1>Mainzer Dom</h1><p><img src=':/resources/images/180-dom.jpg' align='left'/>Der Hohe Dom zu Mainz ist die Bischofskirche der Diözese Mainz und steht unter dem Patrozinium des heiligen Martin von Tours. Der Ostchor ist dem Hl. Stephan geweiht. Der zu den Kaiserdomen zählende Bau ist in seiner heutigen Form eine dreischiffige romanische Säulenbasilika, die in ihren Anbauten sowohl gotische als auch barocke Elemente aufweist.</p>");
        }
        else if(geometry->metadata("name") == "St. Stephan")
        {
            info_dialog->setInfotext("<h1>St. Stephan</h1><p><img src=':/resources/images/180-stephan.jpg' align='left'/>Die katholische Pfarrkirche Sankt Stephan in Mainz wurde 990 von Erzbischof Willigis auf der höchsten Erhebung der Stadt gegründet. Auftraggeberin war höchstwahrscheinlich die Kaiserwitwe Theophanu. Willigis wollte mit ihr die Gebetsstätte des Reiches schaffen. In der Kirche war ursprünglich ein Stift untergebracht. Der Propst des Stiftes verwaltete eines der Archidiakonate (mittelalterliche Organisationseinheit, ähnlich den heutigen Dekanaten) des Erzbistums.</p>");
        }
        else if(geometry->metadata("name") == "St. Quintin")
        {
            info_dialog->setInfotext("<h1>St. Quintin</h1><p><img src=':/resources/images/180-quintin.jpg' align='left'/>Die Kirche St. Quintin in Mainz ist die Pfarrkirche der ältesten nachgewiesenen Pfarrei der Stadt.");
        }
        else if(geometry->metadata("name") == "rgzm")
        {
            info_dialog->setInfotext("<h1>Römisch-Germanisches Zentralmuseum</h1><p><img src=':/resources/images/180-rgzm.jpg' align='left'/>Das Römisch-Germanische Zentralmuseum (RGZM) in Mainz ist ein Forschungsinstitut für Vor- und Frühgeschichte, das von Bund und Ländern getragen wird und zur Leibniz-Gemeinschaft deutscher Forschungseinrichtungen gehört. Gegliedert in mehrere Abteilungen, arbeitet das Institut im Bereich der Alten Welt sowie seiner Kontaktzonen von der Altsteinzeit bis ins Mittelalter.");
        }
        else if(geometry->metadata("name") == "lm")
        {
            info_dialog->setInfotext("<h1>Landesmuseum Mainz</h1><p><img src=':/resources/images/180-lm.jpg' align='left'/>Das Landesmuseum Mainz ist eines der ältesten Museen in Deutschland. Eine seiner Vorgängerinstitutionen, die Städtische Gemäldesammlung, wurde bereits 1803 von Jean-Antoine Chaptal auf Veranlassung Napoléon Bonapartes durch eine Schenkung von 36 Gemälden gegründet. Das Museum, welches sich heute im ehemaligen kurfürstlichen Marstall befindet, gehört zusammen mit dem Römisch-Germanischen Zentralmuseum und dem Gutenbergmuseum zu den bedeutenden Museen in Mainz. Seine kunst- und kulturgeschichtliche Sammlung erstreckt sich von der Vorgeschichte über die römische Zeit, dem Mittelalter und Barock bis hin zur Jugendstilzeit und der Kunst des 20. Jahrhunderts.");
        }
        if(geometry->metadata("name") != "")
        {
            info_dialog->showMaximized();
        }
    }
}

void Citymap::geometryClickedPub(const Geometry* geometry)
{
    // Check clicks aren't being ignore and we aren't in the middle of adding a note.
    if(m_mode_distance_calculating == false && m_mode_note_adding == false)
    {
        // Create a new info dialog.
        InfoDialog* info_dialog = new InfoDialog(this);

        // Display the pub info.
        info_dialog->setWindowTitle(geometry->metadata("name").toString());
        info_dialog->setInfotext("<h1>" + geometry->metadata("name").toString() + "</h1>");
        info_dialog->showNormal();
    }
}

void Citymap::createActions()
{
    // Create the sights action.
    m_action_sights = new QAction(tr("Show Sights"), this);
    m_action_sights->setCheckable(true);
    m_action_sights->setChecked(true);
    QObject::connect(m_action_sights, &QAction::triggered, m_layer_sights.get(), &Layer::setVisible);

    // Create the pubs action.
    m_action_pubs = new QAction(tr("Show Pubs"), this);
    m_action_pubs->setCheckable(true);
    m_action_pubs->setChecked(true);
    QObject::connect(m_action_pubs, &QAction::triggered, m_layer_pubs.get(), &Layer::setVisible);

    // Create the museums action.
    m_action_museums = new QAction(tr("Show Museums"), this);
    m_action_museums->setCheckable(true);
    m_action_museums->setChecked(true);
    QObject::connect(m_action_museums, &QAction::triggered, m_layer_museum.get(), &Layer::setVisible);


    // Create the sight tour action.
    m_action_tour_sights = new QAction(tr("Show Sight Tour"), this);
    m_action_tour_sights->setCheckable(true);
    m_action_tour_sights->setChecked(true);
    QObject::connect(m_action_tour_sights, &QAction::triggered, m_tour_sights.get(), &Geometry::setVisible);

    // Create the pub tour action.
    m_action_tour_pubs = new QAction(tr("Show Pub Tour"), this);
    m_action_tour_pubs->setCheckable(true);
    m_action_tour_pubs->setChecked(true);
    QObject::connect(m_action_tour_pubs, &QAction::triggered, m_tour_pubs.get(), &Geometry::setVisible);

    // Create the museum tour action.
    m_action_tour_museums = new QAction(tr("Show Museum Tour"), this);
    m_action_tour_museums->setCheckable(true);
    m_action_tour_museums->setChecked(true);
    QObject::connect(m_action_tour_museums, &QAction::triggered, m_tour_museums.get(), &Geometry::setVisible);


    // Create the add note action.
    m_action_add_note = new QAction(tr("Add Note"), this);
    QObject::connect(m_action_add_note, &QAction::triggered, this, &Citymap::addNote);

    // Create the calculate distance action.
    m_action_calculate_distance = new QAction(tr("Calculate Distance"), this);
    QObject::connect(m_action_calculate_distance, &QAction::triggered, this, &Citymap::calculateDistance);


    // Create the map provider actions.
    QActionGroup* map_provider_group = new QActionGroup(this);
    m_action_google_map = new QAction(tr("Google: Map"), map_provider_group);
    m_action_osm = new QAction(tr("OpenStreetMap"), map_provider_group);
    m_action_otm = new QAction(tr("OpenTopoMap"), map_provider_group);
    m_action_bing = new QAction(tr("Bing maps"), map_provider_group);
    m_action_yahoo_map = new QAction(tr("Yahoo: Map"), map_provider_group);
    m_action_yahoo_satellite = new QAction(tr("Yahoo: Satellite"), map_provider_group);

    // Ensure the map provider actions are checkable.
    m_action_google_map->setCheckable(true);
    m_action_osm->setCheckable(true);
    m_action_otm->setCheckable(true);
    m_action_bing->setCheckable(true);
    m_action_yahoo_map->setCheckable(true);
    m_action_yahoo_satellite->setCheckable(true);

    // Default to OSM map.
    m_action_osm->setChecked(true);

    // Connect signal/slot to set the map provider.
    QObject::connect(map_provider_group, &QActionGroup::triggered, this, &Citymap::mapProviderSelected);

    // Create yahoo streets overlay action.
    m_action_yahoo_streets = new QAction(tr("Yahoo: Street Overlay"), this);
    m_action_yahoo_streets->setCheckable(true);
    m_action_yahoo_streets->setEnabled(false);

    // Connect signal/slot to set the visibility of the yahoo streets layer when the action is toggled.
    QObject::connect(m_action_yahoo_streets, &QAction::toggled, m_layer_yahoo_streets.get(), &Layer::setVisible);
}

void Citymap::createMenus()
{
    // Create the layers menu.
    QMenu* menu_layer = menuBar()->addMenu(tr("Layers"));
    menu_layer->addAction(m_action_sights);
    menu_layer->addAction(m_action_pubs);
    menu_layer->addAction(m_action_museums);

    // Create the tours menu.
    QMenu* menu_tour = menuBar()->addMenu(tr("Tours"));
    menu_tour->addAction(m_action_tour_sights);
    menu_tour->addAction(m_action_tour_pubs);
    menu_tour->addAction(m_action_tour_museums);

    // Create the tools menu.
    QMenu* menu_tools = menuBar()->addMenu(tr("Tools"));
    menu_tools->addAction(m_action_add_note);
    menu_tools->addAction(m_action_calculate_distance);

    // Create the map providers menu.
    QMenu* menu_map = menuBar()->addMenu(tr("Map Provider"));
    menu_map->addAction(m_action_google_map);
    menu_map->addAction(m_action_osm);
    menu_map->addAction(m_action_otm);
    menu_map->addAction(m_action_bing);
    menu_map->addAction(m_action_yahoo_map);
    menu_map->addAction(m_action_yahoo_satellite);
    menu_map->addSeparator();
    menu_map->addAction(m_action_yahoo_streets);
}

void Citymap::addNote()
{
    // Set that we are currenty adding a note.
    m_mode_note_adding = true;

    // Connect signal/slot to capture the next mouse click to create the note.
    QObject::connect(m_map_control, &QMapControl::mouseEventPressCoordinate, this, &Citymap::writeNote);
}

void Citymap::writeNote(QMouseEvent* /*mouse_event*/, PointWorldCoord press_coordinate)
{
    // Create a point to represent the note.
    std::shared_ptr<GeometryPointImageScaled> point(std::make_shared<GeometryPointImageScaled>(press_coordinate, ":/resources/images/note.png", 16));
    point->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
    point->setDrawMinimumPx(QSizeF(12, 10));
    point->setDrawMaximumPx(QSizeF(47, 40));
    m_layer_notes->addGeometry(point);

    // Record the geometry as the currently selected.
    m_selected_geometry = point.get();

    // Clears the textedit note.
    m_text_edit->clear();

    // Show the text edit geometry and move to the press coordinates.
    m_geometry_text_edit->setCoord(press_coordinate);
    m_geometry_text_edit->setVisible(true);

    // Disconnect the signal/slot to create a note.
    QObject::disconnect(m_map_control, &QMapControl::mouseEventPressCoordinate, this, &Citymap::writeNote);

    // Connect signal/slot to capture the next mouse double click to hide the note.
    QObject::connect(m_map_control, &QMapControl::mouseEventDoubleClickCoordinate, this, &Citymap::hideNote);
}

void Citymap::hideNote(QMouseEvent* mouse_event, PointWorldCoord /*press_coordinate*/)
{
    // Check we are adding a note and the user has double clicked.
    if(m_mode_note_adding && mouse_event->type() == QEvent::MouseButtonDblClick)
    {
        // Set that we have finished adding a note.
        m_mode_note_adding = false;

        // Hide the text edit geometry.
        m_geometry_text_edit->setVisible(false);

        // Check we have a selected geometry.
        if(m_selected_geometry != nullptr)
        {
            // Save the text.
            m_selected_geometry->setMetadata("notes", m_text_edit->toPlainText());
        }

        // Disconnect the signal/slot to hide the note.
        QObject::disconnect(m_map_control, &QMapControl::mouseEventDoubleClickCoordinate, this, &Citymap::hideNote);
    }
}

void Citymap::editNote(const Geometry* geometry)
{
    // Record the geometry as the currently selected.
    m_selected_geometry = const_cast<Geometry*>(geometry);

    // Did we get a GeometryPoint?
    if(m_selected_geometry->geometryType() == Geometry::GeometryType::GeometryPoint)
    {
        // Set that we are editing the note.
        m_mode_note_adding = true;

        // Set the text edit to the current note contents.
        m_text_edit->setPlainText(m_selected_geometry->metadata("notes").toString());

        // Show the text edit geometry and move to the geometry coordinates.
        m_geometry_text_edit->setCoord(static_cast<GeometryPoint*>(m_selected_geometry)->coord());
        m_geometry_text_edit->setVisible(true);

        // Connect signal/slot to capture the next mouse double click to hide the note.
        QObject::connect(m_map_control, &QMapControl::mouseEventDoubleClickCoordinate, this, &Citymap::hideNote);
    }
}

void Citymap::calculateDistance()
{
    // Set that we are measuring distance.
    m_mode_distance_calculating = true;

    // Connect signal/slot to capture the next mouse click to start measuring.
    QObject::connect(m_map_control, &QMapControl::mouseEventPressCoordinate, this, &Citymap::calculateDistanceClick);
}

void Citymap::calculateDistanceClick(QMouseEvent* evnt, PointWorldCoord press_coordinate)
{
    // Have we captured the starting point?
    if(m_coord_start.rawPoint().isNull() && evnt->type() == QEvent::MouseButtonPress)
    {
        // Capture the starting point.
        m_coord_start = press_coordinate;

        // Create a start point flag.
        std::shared_ptr<GeometryPointImage> point_flag(std::make_shared<GeometryPointImage>(m_coord_start, ":/resources/images/flag.png"));
        point_flag->setAlignmentType(GeometryPoint::AlignmentType::BottomRight);

        // Add the starting point to the layer.
        m_layer_notes->addGeometry(point_flag);
    }
    // Else have we captured the finishing point?
    else if(m_coord_end.rawPoint().isNull() && evnt->type() == QEvent::MouseButtonPress)
    {
        // Capture the finishing point.
        m_coord_end = press_coordinate;

        // Convert the coordinate degrees into radians.
        const double start_longitude(m_coord_start.longitude() * (M_PI / 180.0));
        const double start_latitude(m_coord_start.latitude() * (M_PI / 180.0));
        const double end_longitude(m_coord_end.longitude() * (M_PI / 180.0));
        const double end_latitude(m_coord_end.latitude() * (M_PI / 180.0));

        // Calculate the distance between the two points (Spherical law of cosines).
        const double radius(6371.0); // Earth's mean Radius in km.
        const double distance = std::acos(std::sin(start_latitude) * std::sin(end_latitude) + std::cos(start_latitude) * std::cos(end_latitude) * std::cos(end_longitude - start_longitude)) * radius;

        // Draw a line string to show the distance.
        std::vector<PointWorldCoord> points;
        points.push_back(m_coord_start);
        points.push_back(m_coord_end);
        m_layer_notes->addGeometry(std::make_shared<GeometryLineString>(points));
        QPixmap* pixmap = new QPixmap(100,20);
        pixmap->fill(Qt::transparent);
        QPainter painter(pixmap);
        painter.setFont(QFont("Helvetiva", 6));
        painter.drawText(pixmap->rect(), QString().setNum(distance, 'f', 3) + " km");
        painter.end();
        std::shared_ptr<GeometryPointImage> point_end(std::make_shared<GeometryPointImage>(m_coord_end, *pixmap));
        point_end->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
        m_layer_notes->addGeometry(point_end);

        // Reset the points.
        m_coord_start = PointWorldCoord(0.0, 0.0);
        m_coord_end = PointWorldCoord(0.0, 0.0);

        // Set that we have finished measuring.
        m_mode_distance_calculating = false;

        // Disconnect the signal/slot to measure.
        QObject::disconnect(m_map_control, &QMapControl::mouseEventPressCoordinate, this, &Citymap::calculateDistanceClick);
    }
}

void Citymap::mapProviderSelected(QAction* action)
{
    // Disable the yahoo street layer action.
    m_action_yahoo_streets->setEnabled(false);
    m_action_yahoo_streets->setChecked(false);
    m_layer_yahoo_streets->setVisible(false);

    // Create a replacement map layer.
    std::shared_ptr<LayerMapAdapter> map_layer(std::make_shared<LayerMapAdapter>("Map"));

    // Set the map to Google.
    if(action == m_action_google_map)
    {
        // Set the map adapter to Google.
        map_layer->setMapAdapter(std::make_shared<MapAdapterGoogle>());
    }
    // Set the map to OSM.
    else if(action == m_action_osm)
    {
        // Set the map adapter to OSM.
        map_layer->setMapAdapter(std::make_shared<MapAdapterOSM>());
    }
    // Set the map to OTM.
    else if(action == m_action_otm)
    {
        // Set the map adapter to OSM.
        map_layer->setMapAdapter(std::make_shared<MapAdapterOTM>());
    }
    // Set the map to OTM.
    else if(action == m_action_bing)
    {
        // Set the map adapter to OSM.
        map_layer->setMapAdapter(std::make_shared<MapAdapterBing>());
    }
    // Set the map to Yahoo 'map'.
    else if(action == m_action_yahoo_map)
    {
        // Set the map adapter to Yahoo 'map'.
        map_layer->setMapAdapter(std::make_shared<MapAdapterYahoo>());
    }
    // Set the map to Yahoo 'satellite'.
    else if(action == m_action_yahoo_satellite)
    {
        // Set the map adapter to Yahoo 'satellite'.
        map_layer->setMapAdapter(std::make_shared<MapAdapterYahoo>(QUrl("http://us.maps3.yimg.com/aerial.maps.yimg.com/png?v=1.7&t=a&s=256&x=%x&y=%y&z=%zoom")));

        // Enable the yahoo street layer action.
        m_action_yahoo_streets->setEnabled(true);
    }

    // Add the replacement map layer.
    m_map_control->addLayer(map_layer, 0);
}

void Citymap::resizeEvent(QResizeEvent * resize_event)
{
    // Set the new viewport size.
    m_map_control->setViewportSize(resize_event->size());
}
