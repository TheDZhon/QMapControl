/*!
 * \example citymap.cpp
 * This demo appclication shows more features of the QMapControl.
 * It shows images, which changes its size when changing the zoomlevel.
 * You can display/hide layers and choose different map providers.
 * Also it demonstrates more possibilities for user interaction:<br/>
 * - notes can be added to any coordinate (a QTextEdit is used for editing the note)<br/>
 * - the user can measure distances between two points
 *
 * \image html sample_citymap.png "screenshot"
 */


#include "citymap.h"

#include <QMapControl/GeometryPointWidget.h>

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

// STL includes.
#include <cmath>

Citymap::Citymap(QWidget*)
{
    // create QMapControl
    mc = new QMapControl(QSizeF(380,540));
    mc->enableScalebar(true);
    // display the QMapControl in the application
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mc);

    QWidget* w = new QWidget();
    w->setLayout(layout);
    setCentralWidget(w);


    notepixmap = new QPixmap(QCoreApplication::applicationDirPath() + "/images/note.png");

    coord1 = QPointF();
    coord2 = QPointF();
    mapadapter = std::make_shared<MapAdapterOSM>();

    // create a layer with the mapadapter and type MapLayer
    l = std::make_shared<Layer>("Custom Layer");
    l->addMapAdapter(mapadapter);
    overlay = std::make_shared<Layer>("Overlay");
    overlay->addMapAdapter(std::make_shared<MapAdapterYahoo>(QUrl("http://us.maps3.yimg.com/aerial.maps.yimg.com/png?v=2.2&t=h&s=256&x=%x&y=%y&z=%zoom")));
    overlay->setVisible(false);

    mc->addLayer(l);
    mc->addLayer(overlay);

    notes = std::make_shared<Layer>("Notes");
    notes->addMapAdapter(mapadapter);


    createTours();
    addSights();
    addPubs();
    addMuseums();

    addZoomButtons();
    createActions();
    createMenus();

    mc->addLayer(notes);
    connect(notes.get(), SIGNAL(geometryClicked(Geometry*)),
              this, SLOT(editNote(Geometry*)));


    mc->setMapFocusPoint(QPointF(8.26,50));
    mc->setZoom(13);

    ignoreClicks = false;
    addingNote = false;
    noteID = 0;

    notetextedit = new QTextEdit(mc);
    notetextedit->setGeometry(0,0,200,100);
    notepoint = std::make_shared<GeometryPointWidget>(QPointF(0.0, 0.0), notetextedit, GeometryPoint::AlignmentType::TopLeft);
    notepoint->setVisible(false);
    notes->addGeometry(notepoint);

}

void Citymap::createTours()
{
//    QPen* pen = new QPen(QColor(0,0,255,100));
//    pen->setWidth(5);
    QPen pen(QColor(0,0,255,100));
    pen.setWidth(5);

    std::vector<std::shared_ptr<GeometryPoint>> points;
    points.emplace_back(std::make_shared<GeometryPoint>(8.2606, 50.0051));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2602, 50.0050));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2598, 50.0044));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2569, 50.0057));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2595, 50.0083));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2587, 50.0086));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2589, 50.0100));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2590, 50.0105));
    pub_tour = std::make_shared<GeometryLineString>(points, pen);
    notes->addGeometry(pub_tour);

    points.clear();
    points.emplace_back(std::make_shared<GeometryPoint>(8.25987, 50.0018));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26192, 50.0019));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26301, 50.0031));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26459, 50.0026));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26601, 50.004));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26781, 50.0033));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27052, 50.0054));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2697, 50.0059));
    museum_tour = std::make_shared<GeometryLineString>(points, pen);
    notes->addGeometry(museum_tour);

    points.clear();
    points.emplace_back(std::make_shared<GeometryPoint>(8.26015, 50.0015));
    points.emplace_back(std::make_shared<GeometryPoint>(8.2617, 50.0012));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26423, 50.0002));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26698, 50.0024));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27065, 50.0012));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27152, 50.0016));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27225, 50.0004));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27333, 49.9994));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26946, 49.9983));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27105, 49.9973));
    points.emplace_back(std::make_shared<GeometryPoint>(8.27024, 49.9972));
    points.emplace_back(std::make_shared<GeometryPoint>(8.26833, 49.9958));
    sights_tour = std::make_shared<GeometryLineString>(points, pen);
    notes->addGeometry(sights_tour);
}

void Citymap::addSights()
{
    sights = std::make_shared<Layer>("Sehenswürdigkeiten");
    sights->addMapAdapter(mapadapter);
    mc->addLayer(sights);
    std::shared_ptr<GeometryPoint> dom(std::make_shared<GeometryPointImage>(QPointF(8.274167, 49.998889), QCoreApplication::applicationDirPath() + "/images/180-dom.jpg"));
    dom->setMetadata("name", "Mainzer Dom");
    dom->setBaseZoom(17);
    sights->addGeometry(dom);

    std::shared_ptr<GeometryPoint> stephan(std::make_shared<GeometryPointImage>(QPointF(8.268611, 49.995556), QCoreApplication::applicationDirPath() + "/images/180-stephan.jpg"));
    stephan->setMetadata("name", "St. Stephan");
    stephan->setBaseZoom(17);
    sights->addGeometry(stephan);

    std::shared_ptr<GeometryPoint> quitin(std::make_shared<GeometryPointImage>(QPointF(8.272222, 50.000833), QCoreApplication::applicationDirPath() + "/images/180-quintin.jpg"));
    quitin->setMetadata("name", "St. Quintin");
    quitin->setBaseZoom(17);
    sights->addGeometry(quitin);
    connect(sights.get(), SIGNAL(geometryClicked(Geometry*)),
              this, SLOT(geometryClicked(Geometry*)));
}
void Citymap::addPubs()
{
    pubs = std::make_shared<Layer>("Kneipe");
    pubs->addMapAdapter(mapadapter);
    mc->addLayer(pubs);

    std::shared_ptr<GeometryPoint> bagatelle(std::make_shared<GeometryPoint>(QPointF(8.2606, 50.0052), QPixmap(QCoreApplication::applicationDirPath() + "/images/pub.png")));
    bagatelle->setMetadata("name", "Bagatelle");
    pubs->addGeometry(bagatelle);

    std::shared_ptr<GeometryPoint> nirgendwo(std::make_shared<GeometryPoint>(QPointF(8.2595, 50.0048), QPixmap(QCoreApplication::applicationDirPath() + "/images/pub.png")));
    nirgendwo->setMetadata("name", "Nirgendwo");
    pubs->addGeometry(nirgendwo);

    std::shared_ptr<GeometryPoint> krokodil(std::make_shared<GeometryPoint>(QPointF(8.2594, 50.0106), QPixmap(QCoreApplication::applicationDirPath() + "/images/pub.png")));
    krokodil->setMetadata("name", "Krokodil");
    pubs->addGeometry(krokodil);

    connect(pubs.get(), SIGNAL(geometryClicked(Geometry*)),
              this, SLOT(geometryClickEventKneipe(Geometry*)));
}
void Citymap::addMuseums()
{
    museum = std::make_shared<Layer>("Museen");
    museum->addMapAdapter(mapadapter);
    mc->addLayer(museum);
    std::shared_ptr<GeometryPoint> rgzm(std::make_shared<GeometryPointImage>(QPointF(8.269722, 50.006111), QCoreApplication::applicationDirPath() + "/images/180-rgzm.jpg"));
    rgzm->setMetadata("name", "rgzm");
    rgzm->setBaseZoom(17);
    museum->addGeometry(rgzm);

    std::shared_ptr<GeometryPoint> lm(std::make_shared<GeometryPointImage>(QPointF(8.26778, 50.00385), QCoreApplication::applicationDirPath() + "/images/180-lm.jpg"));
    lm->setMetadata("name", "lm");
    lm->setBaseZoom(17);
    museum->addGeometry(lm);

    connect(museum.get(), SIGNAL(geometryClicked(Geometry*)),
              this, SLOT(geometryClicked(Geometry*)));
}

void Citymap::geometryClicked(Geometry* geometry)
{
    if (ignoreClicks || addingNote)
        return;

    InfoDialog* infodialog = new InfoDialog(this);
    infodialog->setWindowTitle(geometry->getMetadata("name").toString());

    if (geometry->getMetadata("name") == "Mainzer Dom")
    {
        infodialog->setInfotext("<h1>Mainzer Dom</h1><p><img src=\"images/180-dom.jpg\" align=\"left\"/>Der Hohe Dom zu Mainz ist die Bischofskirche der Diözese Mainz und steht unter dem Patrozinium des heiligen Martin von Tours. Der Ostchor ist dem Hl. Stephan geweiht. Der zu den Kaiserdomen zählende Bau ist in seiner heutigen Form eine dreischiffige romanische Säulenbasilika, die in ihren Anbauten sowohl gotische als auch barocke Elemente aufweist.</p>");

    } else if (geometry->getMetadata("name") == "St. Stephan")
    {
        infodialog->setInfotext("<h1>St. Stephan</h1><p><img src=\"images/180-stephan.jpg\" align=\"left\"/>Die katholische Pfarrkirche Sankt Stephan in Mainz wurde 990 von Erzbischof Willigis auf der höchsten Erhebung der Stadt gegründet. Auftraggeberin war höchstwahrscheinlich die Kaiserwitwe Theophanu. Willigis wollte mit ihr die Gebetsstätte des Reiches schaffen. In der Kirche war ursprünglich ein Stift untergebracht. Der Propst des Stiftes verwaltete eines der Archidiakonate (mittelalterliche Organisationseinheit, ähnlich den heutigen Dekanaten) des Erzbistums.</p>");
    } else if (geometry->getMetadata("name") == "St. Quintin")
    {
        infodialog->setInfotext("<h1>St. Quintin</h1><p><img src=\"images/180-quintin.jpg\" align=\"left\"/>Die Kirche St. Quintin in Mainz ist die Pfarrkirche der ältesten nachgewiesenen Pfarrei der Stadt.");
    } else if (geometry->getMetadata("name") == "rgzm")
    {
        infodialog->setInfotext("<h1>Römisch-Germanisches Zentralmuseum</h1><p><img src=\"images/180-rgzm.jpg\" align=\"left\"/>Das Römisch-Germanische Zentralmuseum (RGZM) in Mainz ist ein Forschungsinstitut für Vor- und Frühgeschichte, das von Bund und Ländern getragen wird und zur Leibniz-Gemeinschaft deutscher Forschungseinrichtungen gehört. Gegliedert in mehrere Abteilungen, arbeitet das Institut im Bereich der Alten Welt sowie seiner Kontaktzonen von der Altsteinzeit bis ins Mittelalter.");
    } else if (geometry->getMetadata("name") == "lm")
    {
        infodialog->setInfotext("<h1>Landesmuseum Mainz</h1><p><img src=\"images/180-lm.jpg\" align=\"left\"/>Das Landesmuseum Mainz ist eines der ältesten Museen in Deutschland. Eine seiner Vorgängerinstitutionen, die Städtische Gemäldesammlung, wurde bereits 1803 von Jean-Antoine Chaptal auf Veranlassung Napoléon Bonapartes durch eine Schenkung von 36 Gemälden gegründet. Das Museum, welches sich heute im ehemaligen kurfürstlichen Marstall befindet, gehört zusammen mit dem Römisch-Germanischen Zentralmuseum und dem Gutenbergmuseum zu den bedeutenden Museen in Mainz. Seine kunst- und kulturgeschichtliche Sammlung erstreckt sich von der Vorgeschichte über die römische Zeit, dem Mittelalter und Barock bis hin zur Jugendstilzeit und der Kunst des 20. Jahrhunderts.");
    }
    if (geometry->getMetadata("name") != "")
        infodialog->showMaximized();
}

void Citymap::geometryClickEventKneipe(Geometry* geometry)
{
    if (ignoreClicks || addingNote)
        return;
    InfoDialog* infodialog = new InfoDialog(this);
    infodialog->setWindowTitle(geometry->getMetadata("name").toString());
    infodialog->setInfotext("<h1>" + geometry->getMetadata("name").toString() + "</h1>");
    infodialog->showNormal();
}

void Citymap::addZoomButtons()
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

void Citymap::createActions()
{
    toggleSights = new QAction(tr("Show Sights"), this);
    toggleSights->setCheckable(true);
    toggleSights->setChecked(true);
    connect(toggleSights, SIGNAL(triggered(bool)),
              sights.get(), SLOT(setVisible(bool)));

    togglePub = new QAction(tr("Show Pubs"), this);
    togglePub->setCheckable(true);
    togglePub->setChecked(true);
    connect(togglePub, SIGNAL(triggered(bool)),
              pubs.get(), SLOT(setVisible(bool)));

    toggleMuseum = new QAction(tr("Show Museums"), this);
    toggleMuseum->setCheckable(true);
    toggleMuseum->setChecked(true);
    connect(toggleMuseum, SIGNAL(triggered(bool)),
              museum.get(), SLOT(setVisible(bool)));


    toggleSightTour = new QAction(tr("Show Sight Tour"), this);
    toggleSightTour->setCheckable(true);
    toggleSightTour->setChecked(true);
    connect(toggleSightTour, SIGNAL(triggered(bool)),
              sights_tour.get(), SLOT(setVisible(bool)));

    togglePubTour = new QAction(tr("Show Pub Tour"), this);
    togglePubTour->setCheckable(true);
    togglePubTour->setChecked(true);
    connect(togglePubTour, SIGNAL(triggered(bool)),
              pub_tour.get(), SLOT(setVisible(bool)));

    toggleMuseumTour = new QAction(tr("Show Museum Tour"), this);
    toggleMuseumTour->setCheckable(true);
    toggleMuseumTour->setChecked(true);
    connect(toggleMuseumTour, SIGNAL(triggered(bool)),
              museum_tour.get(), SLOT(setVisible(bool)));

    addNoteAction = new QAction(tr("Add Note"), this);
    connect(addNoteAction, SIGNAL(triggered(bool)),
              this, SLOT(addNote()));

    toolsDistance = new QAction(tr("Calculate Distance"), this);
    connect(toolsDistance, SIGNAL(triggered(bool)),
              this, SLOT(calcDistance()));

    QActionGroup* mapproviderGroup = new QActionGroup(this);
    osmAction = new QAction(tr("OpenStreetMap"), mapproviderGroup);
    yahooActionMap = new QAction(tr("Yahoo: Map"), mapproviderGroup);
    yahooActionSatellite = new QAction(tr("Yahoo: Satellite"), mapproviderGroup);
    googleActionMap = new QAction(tr("Google: Map"), mapproviderGroup);
    osmAction->setCheckable(true);
    yahooActionMap->setCheckable(true);
    yahooActionSatellite->setCheckable(true);
    googleActionMap->setCheckable(true);
    osmAction->setChecked(true);
    connect(mapproviderGroup, SIGNAL(triggered(QAction*)),
              this, SLOT(mapproviderSelected(QAction*)));

    yahooActionOverlay = new QAction(tr("Yahoo: street overlay"), this);
    yahooActionOverlay->setCheckable(true);
    yahooActionOverlay->setEnabled(false);
    connect(yahooActionOverlay, SIGNAL(toggled(bool)),
              overlay.get(), SLOT(setVisible(bool)));
}

void Citymap::createMenus()
{
    layerMenu = menuBar()->addMenu(tr("&Layer"));
    layerMenu->addAction(toggleSights);
    layerMenu->addAction(togglePub);
    layerMenu->addAction(toggleMuseum);

    tourMenu = menuBar()->addMenu(tr("T&ours"));
    tourMenu->addAction(toggleSightTour);
    tourMenu->addAction(togglePubTour);
    tourMenu->addAction(toggleMuseumTour);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(addNoteAction);
    toolsMenu->addAction(toolsDistance);

    mapMenu = menuBar()->addMenu(tr("&Map Provider"));
    mapMenu->addAction(osmAction);
    mapMenu->addAction(yahooActionMap);
    mapMenu->addAction(yahooActionSatellite);
    mapMenu->addAction(googleActionMap);
    mapMenu->addSeparator();
    mapMenu->addAction(yahooActionOverlay);

}

void Citymap::addNote()
{
    addingNote = true;
    connect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
              this, SLOT(writeNote(QMouseEvent*, QPointF)));
}

void Citymap::writeNote(QMouseEvent*, QPointF coord)
{
    std::shared_ptr<GeometryPoint> p(std::make_shared<GeometryPoint>(coord, *notepixmap, GeometryPoint::AlignmentType::BottomLeft));
    p->setMetadata("name", ++noteID);
    currentnoteID = noteID;
    p->setBaseZoom(16);
    p->setDrawMinimumPx(QSizeF(12, 10));
    p->setDrawMaximumPx(QSizeF(47, 40));
    notes->addGeometry(p);

    notetextedit->clear();

    notepoint->setCoordinate(coord);
    notepoint->setVisible(true);

    mc->requestRedraw();

    disconnect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
              this, SLOT(writeNote(QMouseEvent*, QPointF)));

    connect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
              this, SLOT(hideNote(QMouseEvent*, QPointF)));
}

void Citymap::hideNote(QMouseEvent* evnt, QPointF)
{
    if (addingNote && evnt->type() == QEvent::MouseButtonDblClick)
    {
        addingNote = false;
        notepoint->setVisible(false);

        mc->requestRedraw();

        // save text
        notestext[currentnoteID] = notetextedit->toPlainText();

        disconnect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
                  this, SLOT(hideNote(QMouseEvent*, QPointF)));
    }
}

void Citymap::editNote(Geometry* geom)
{
    addingNote = true;
    currentnoteID = QVariant(geom->getMetadata("name")).toInt();
    notetextedit->setPlainText(notestext[currentnoteID]);
    notepoint->setCoordinate(static_cast<GeometryPoint*>(geom)->coordinate());
    notepoint->setVisible(true);

    mc->requestRedraw();
    connect(mc, SIGNAL(mouseEventReleaseCoordinate(QMouseEvent*, QPointF)),
              this, SLOT(hideNote(QMouseEvent*, QPointF)));
}

void Citymap::calcDistance()
{
    ignoreClicks = true;
    connect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
              this, SLOT(calcDistanceClick(QMouseEvent*, QPointF)));
}
void Citymap::calcDistanceClick(QMouseEvent* evnt, QPointF coord)
{
    if (coord1 == QPointF() && evnt->type() == QEvent::MouseButtonPress)
    {
        coord1 = coord;
        l->addGeometry(std::make_shared<GeometryPointImage>(coord1, QCoreApplication::applicationDirPath() + "/images/flag.png", GeometryPoint::AlignmentType::BottomRight));
        mc->requestRedraw();
    }
    else if (coord2 == QPointF() && evnt->type() == QEvent::MouseButtonPress)
    {
        coord2 = coord;
        double PI = acos(-1.0);
        double a1 = coord1.x()* (PI/180.0);;
        double b1 = coord1.y()* (PI/180.0);;
        double a2 = coord2.x()* (PI/180.0);;
        double b2 = coord2.y()* (PI/180.0);;
        double r = 6378;

        double km = acos(cos(a1)*cos(b1)*cos(a2)*cos(b2) + cos(a1)*sin(b1)*cos(a2)*sin(b2) + sin(a1)*sin(a2)) * r;


        std::vector<std::shared_ptr<GeometryPoint>> points;
        points.emplace_back(std::make_shared<GeometryPoint>(coord1.x(), coord1.y()));
        QPixmap* pixm = new QPixmap(100,20);
        pixm->fill(Qt::transparent);
        QPainter pain(pixm);
        pain.setFont(QFont("Helvetiva", 6));
        pain.drawText(pixm->rect(), QString().setNum(km, 'f', 3) + " km");
        pain.end();
        points.emplace_back(std::make_shared<GeometryPoint>(coord2, *pixm, GeometryPoint::AlignmentType::BottomLeft));
        l->addGeometry(std::make_shared<GeometryLineString>(points));
        mc->requestRedraw();
        coord1 = QPointF();
        coord2 = QPointF();
        ignoreClicks = false;
        disconnect(mc, SIGNAL(mouseEventPressCoordinate(QMouseEvent*, QPointF)),
                      this, SLOT(calcDistanceClick(QMouseEvent*, QPointF)));

    }
}

void Citymap::mapproviderSelected(QAction* action)
{
    if (action == osmAction)
    {
        int zoom = mc->getCurrentZoom();
        mc->setZoom(0);

        mapadapter = std::make_shared<MapAdapterOSM>();
        l->addMapAdapter(mapadapter);
        sights->addMapAdapter(mapadapter);
        museum->addMapAdapter(mapadapter);
        pubs->addMapAdapter(mapadapter);
        notes->addMapAdapter(mapadapter);

        mc->requestRedraw();
        mc->setZoom(zoom);
        yahooActionOverlay->setEnabled(false);
        overlay->setVisible(false);
        yahooActionOverlay->setChecked(false);

    } else if (action == yahooActionMap)
    {
        int zoom = mc->getCurrentZoom();
        mc->setZoom(0);

        mapadapter = std::make_shared<MapAdapterYahoo>();
        l->addMapAdapter(mapadapter);
        sights->addMapAdapter(mapadapter);
        museum->addMapAdapter(mapadapter);
        pubs->addMapAdapter(mapadapter);
        notes->addMapAdapter(mapadapter);

        mc->requestRedraw();
        mc->setZoom(zoom);
        yahooActionOverlay->setEnabled(false);
        overlay->setVisible(false);
        yahooActionOverlay->setChecked(false);
    } else if (action == yahooActionSatellite)
    {
        int zoom = mc->getCurrentZoom();
        mc->setZoom(0);

        mapadapter = std::make_shared<MapAdapterYahoo>(QUrl("http://us.maps3.yimg.com/aerial.maps.yimg.com/png?v=1.7&t=a&s=256&x=%x&y=%y&z=%zoom"));
        l->addMapAdapter(mapadapter);
        sights->addMapAdapter(mapadapter);
        museum->addMapAdapter(mapadapter);
        pubs->addMapAdapter(mapadapter);
        notes->addMapAdapter(mapadapter);

        mc->requestRedraw();
        mc->setZoom(zoom);
        yahooActionOverlay->setEnabled(true);
    } else if (action == googleActionMap)
    {
        int zoom = mc->getCurrentZoom();
        mc->setZoom(0);
        mapadapter = std::make_shared<MapAdapterGoogle>();
        l->addMapAdapter(mapadapter);
        sights->addMapAdapter(mapadapter);
        museum->addMapAdapter(mapadapter);
        pubs->addMapAdapter(mapadapter);
        notes->addMapAdapter(mapadapter);
        mc->requestRedraw();
        mc->setZoom(zoom);
        yahooActionOverlay->setEnabled(false);
        overlay->setVisible(false);
        yahooActionOverlay->setChecked(false);
    }
}

Citymap::~Citymap()
{
    delete mc;
    delete notepixmap;
}

