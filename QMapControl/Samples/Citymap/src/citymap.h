#pragma once

// Qt includes.
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>

// QMapControl includes.
#include <QMapControl/QMapControl.h>
#include <QMapControl/GeometryLineString.h>
#include <QMapControl/GeometryWidget.h>
#include <QMapControl/Layer.h>
#include <QMapControl/LayerGeometry.h>

// Local includes.
#include "dialogs.h"

using namespace qmapcontrol;

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class Citymap: public QMainWindow
{
    Q_OBJECT
public:
    //! Citymap constructor
    /*!
     * This is used to construct a Citymap Widget.
     * @param parent QWidget parent ownership.
     */
    Citymap(QWidget* parent = 0);

    //! Disable copy constructor.
    ///Citymap(const Citymap&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///Citymap& operator=(const Citymap&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~Citymap() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

private:
    /*!
     * Adds sight geometries to QMapControl.
     */
    void addSights();

    /*!
     * Adds pub geometries to QMapControl.
     */
    void addPubs();

    /*!
     * Adds museum geometries to QMapControl.
     */
    void addMuseums();

    /*!
     * Adds tour geometries to QMapControl.
     */
    void addTours();

    /*!
     * Creates actions.
     */
    void createActions();

    /*!
     * Adds menu actions to QMainWindow.
     */
    void createMenus();

private slots:
    /*!
     * Slot called when a geometry is clicked.
     * @param geometry The geometry that was clicked.
     */
    void geometryClicked(const Geometry* geometry);

    /*!
     * Slot called when a pub geometry is clicked.
     * @param geometry The geometry that was clicked.
     */
    void geometryClickedPub(const Geometry* geometry);

    /*!
     * Slot called to start adding a note.
     */
    void addNote();

    /*!
     * Slot called to add a note at the provided coordinates.
     * @param mouse_event The QMouseEvent.
     * @param press_coordinate The location of note required.
     */
    void writeNote(QMouseEvent* mouse_event, PointWorldCoord press_coordinate);

    /*!
     * Slot called to finish adding the note.
     * @param mouse_event The QMouseEvent.
     * @param press_coordinate The location of note required.
     */
    void hideNote(QMouseEvent* mouse_event, PointWorldCoord press_coordinate);

    /*!
     * Slot called to start editing a geometry.
     * @param geometry The geometry to edit.
     */
    void editNote(const Geometry* geometry);

    /*!
     * Slot called to start calculating distances.
     */
    void calculateDistance();

    /*!
     * Slot called to calculate a distance after a mouse press.
     * @param mouse_event The QMouseEvent.
     * @param press_coordinate The location of mouse press required.
     */
    void calculateDistanceClick(QMouseEvent* mouse_event, PointWorldCoord press_coordinate);

    /*!
     * Slot called to change the map provider.
     * @param action The QAction.
     */
    void mapProviderSelected(QAction* action);

private:
    /// The main map control.
    QMapControl* m_map_control;

    /// The layer to display yahoo streets overlay.
    std::shared_ptr<Layer> m_layer_yahoo_streets;

    /// The layer to display/store notes.
    std::shared_ptr<LayerGeometry> m_layer_notes;

    /// The layer to display/store sights.
    std::shared_ptr<LayerGeometry> m_layer_sights;

    /// The layer to display/store pubs.
    std::shared_ptr<LayerGeometry> m_layer_pubs;

    /// The layer to display/store museums.
    std::shared_ptr<LayerGeometry> m_layer_museum;

    /// The layer to display/store tours.
    std::shared_ptr<LayerGeometry> m_layer_tours;

    /// The geometry to represent a sight tour.
    std::shared_ptr<GeometryLineString> m_tour_sights;

    /// The geometry to represent a pub tour.
    std::shared_ptr<GeometryLineString> m_tour_pubs;

    /// The geometry to represent a museum tour.
    std::shared_ptr<GeometryLineString> m_tour_museums;

    /// Menu action to toggle sights.
    QAction* m_action_sights;

    /// Menu action to toggle pubs.
    QAction* m_action_pubs;

    /// Menu action to toggle museums.
    QAction* m_action_museums;

    /// Menu action to toggle sight tours.
    QAction* m_action_tour_sights;

    /// Menu action to toggle pub tours.
    QAction* m_action_tour_pubs;

    /// Menu action to toggle museum tours.
    QAction* m_action_tour_museums;

    /// Menu action to toggle adding notes.
    QAction* m_action_add_note;

    /// Menu action to toggle calculating distances.
    QAction* m_action_calculate_distance;

    /// Menu action to toggle Google maps.
    QAction* m_action_google_map;

    /// Menu action to toggle OSM maps.
    QAction* m_action_osm;

    /// Menu action to toggle Yahoo maps.
    QAction* m_action_yahoo_map;

    /// Menu action to toggle Yahoo satellite maps.
    QAction* m_action_yahoo_satellite;

    /// Menu action to toggle Yahoo street overlay map.
    QAction* m_action_yahoo_streets;

    /// Indicates if we are in note adding mode.
    bool m_mode_note_adding;

    /// Indicates if we are in distance calculating mode.
    bool m_mode_distance_calculating;

    /// The distance calculating start point.
    PointWorldCoord m_coord_start;

    /// The distance calculating end point.
    PointWorldCoord m_coord_end;

    /// Text edit widget.
    QTextEdit* m_text_edit;

    /// Geometry to manage text edit widget.
    std::shared_ptr<GeometryWidget> m_geometry_text_edit;

    /// The currently selected geometry.
    Geometry* m_selected_geometry;
};
