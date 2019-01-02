#pragma once

// Qt includes.
#include <QtWidgets/QPushButton>

// QMapControl includes.
#include <QMapControl/GeometryLineString.h>
#include <QMapControl/QMapControl.h>
#include <QMapControl/GeometryPointImage.h>

// Local includes.
#include "gps_modul.h"

using namespace qmapcontrol;

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class Multidemo : public QWidget
{
    Q_OBJECT
public:
    //! Multidemo constructor
    /*!
     * This is used to construct a Multidemo QWidget.
     * @param parent QWidget parent ownership.
     */
    Multidemo(QWidget* parent = 0);

    //! Disable copy constructor.
    ///Multidemo(const Multidemo&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///Multidemo& operator=(const Multidemo&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~Multidemo() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

private:
    /*!
     * Setup map control.
     */
    void setupMaps();

    /**
     * Setups layout.
     */
    void setupLayout();

    /**
     * Handles QWidget resizing.
     * @param resize_event The QResizeEvent that occured.
     */
    void resizeEvent(QResizeEvent* event);

private slots:
    void positionChanged(const PointWorldCoord& point);
    /*!
     * Toggle whether to add points
     * @param enable Whether to add points.
     */
    void toggleAddPoint(bool enable);

    /*!
     * Toggle between draw rect and pan modes.
     * @param enable Whether to enable draw rect mode.
     */
    void toggleDrawRect(bool enable);

    /*!
     * Toggle whether a click moves the map.
     * @param enable Whether a click moves the map.
     */
    void toggleMoveClick(bool enable);

    /*!
     * Toggle whether GPS Modul is running.
     * @param enable Whether the GPS Modul should be running.
     */
    void toggleGPS(bool enable);

    /*!
     * Toggle whether to follow the GPS Modul point.
     * @param enable Whether to follow the GPS Modul point.
     */
    void toggleFollowGPS(bool enable);
    /*!
     * Slot to handle geometry selection events.
     * @param selected_geometries The geometries that have been selected.
     */
    void geometriesSelectedEvent(std::map<std::string, std::vector<std::shared_ptr<Geometry>>> selected_geometries);

    /*!
     * Slot to handle a geometry that has been clicked.
     * @param geometry The geometry that has been clicked.
     */
    void geometryClickEvent(const Geometry* geometry);

    /*!
     * Slot to handle mouse press events in the main map control.
     * @param mouse_event The QMouseEvent that occured.
     * @param press_coordinate The corresponding world coordinate of the mouse press.
     * @param release_coordinate The corresponding world coordinate of the mouse release.
     */
    void main_mouseEventReleaseCoordinate(QMouseEvent* mouse_event, PointWorldCoord press_coordinate, PointWorldCoord release_coordinate);

    /*!
     * Slot to handle mouse release events in the main map control.
     * @param mouse_event The QMouseEvent that occured.
     * @param press_coordinate The corresponding world coordinate of the mouse press.
     */
    void main_mouseEventPressCoordinate(QMouseEvent* mouse_event, PointWorldCoord press_coordinate);

    /*!
     * Slot to handle mouse press events in the "mini" map control.
     * @param mouse_event The QMouseEvent that occured.
     * @param press_coordinate The corresponding world coordinate of the mouse press.
     */
    void mini_mouseEventPressCoordinate(QMouseEvent* mouse_event, PointWorldCoord press_coordinate);

private:
    std::shared_ptr<GeometryLineString> m_line_string;
    /// The main map control.
    QMapControl* m_map_control;

    /// The "mini" map control.
    QMapControl* m_mini_map_control;

    /// The GPS Modul GeometryPoint.
    std::shared_ptr<GeometryPointImage> m_gps_point;

    /// The GPS Modul.
    GPS_Modul* m_gps_modul;

    /// Push button to toggle new points being added on a mouse click.
    QPushButton* m_button_add_point;

    /// Push button to toggle the mouse button drawing a rect.
    QPushButton* m_button_draw_rect;

    /// Push button to toggle the mouse click moving the map.
    QPushButton* m_button_move_click;

    /// Push button to toggle the GPS Modul.
    QPushButton* m_button_gps;

    /// Push button to toggle the map to follow a GPS Modul point.
    QPushButton* m_button_follow_gps;
};
