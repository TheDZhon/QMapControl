#ifndef CITYMAP_H
#define CITYMAP_H

#include <QtWidgets/QMainWindow>

#include <QMapControl/QMapControl.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/MapAdapterYahoo.h>
#include <QMapControl/MapAdapterGoogle.h>
#include <QMapControl/Layer.h>
#include <QMapControl/GeometryLineString.h>
#include <QMapControl/GeometryPointImage.h>

#include "dialogs.h"

using namespace qmapcontrol;
class Citymap: public QMainWindow
{
        Q_OBJECT
        public:
                Citymap(QWidget* parent = 0);

                ~Citymap();

        private:
                QMapControl* mc;
                std::shared_ptr<MapAdapter> mapadapter;

                QPixmap* notepixmap;

                std::shared_ptr<Layer> sights;
                std::shared_ptr<Layer> museum;
                std::shared_ptr<Layer> pubs;
                std::shared_ptr<Layer> notes;

                void addZoomButtons();

                void createTours();
                void createActions();
                void createMenus();

                QMenu* layerMenu;
                QMenu* tourMenu;
                QMenu* toolsMenu;
                QMenu* mapMenu;

                QAction* toggleSights;
                QAction* togglePub;
                QAction* toggleMuseum;

                QAction* togglePubTour;
                QAction* toggleMuseumTour;
                QAction* toggleSightTour;

                QAction* addNoteAction;
                QAction* toolsDistance;

                QAction* osmAction;
                QAction* yahooActionMap;
                QAction* yahooActionSatellite;
                QAction* yahooActionOverlay;
                QAction* googleActionMap;

                bool ignoreClicks;
                bool addingNote;

                void addSights();
                void addPubs();
                void addMuseums();

                QPointF coord1;
                QPointF coord2;

                std::shared_ptr<Layer> l;
                std::shared_ptr<Layer> overlay;

                std::shared_ptr<GeometryLineString> pub_tour;
                std::shared_ptr<GeometryLineString> museum_tour;
                std::shared_ptr<GeometryLineString> sights_tour;

                QTextEdit* notetextedit;
                std::shared_ptr<GeometryPoint> notepoint;
                int noteID;
                int currentnoteID;
                QHash<int, QString> notestext;

        public slots:
                void hideNote(QMouseEvent* evnt, QPointF coordinate);
                void geometryClicked(Geometry* geometry);
                void geometryClickEventKneipe(Geometry* geometry);
                void addNote();
                void writeNote(QMouseEvent*, QPointF);
                void calcDistance();
                void calcDistanceClick(QMouseEvent*, QPointF);

                void mapproviderSelected(QAction*);
                void editNote(Geometry* geom);
};

#endif
