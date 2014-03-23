/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#pragma once

// Qt includes.
#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QRectF>
#include <QtCore/QTimer>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QWheelEvent>
#include <QtNetwork/QNetworkProxy>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

// STL includes.
#include <chrono>
#include <mutex>

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"
#include "Layer.h"
#include "QProgressIndicator.h"

//! QMapControl namespace
namespace qmapcontrol
{
    /*!
     * This is the main widget.
     * Layers can be added to this control.
     * QMapControl has to be instantiated with a QSizeF which sets the size the widget takes in a layout.
     * The given size is also the size which is assured to be filled with map images.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT QMapControl : public QWidget
    {
        Q_OBJECT
    public:
        //! Declares what actions mouse buttons have on the map.
        enum class MouseButtonMode
        {
            /// @TODO add polygon support!

            /// No action.
            None,
            /// Move the map.
            Pan,
            /// Draw a box.
            DrawBox,
            /// Pan box.
            PanBox,
            /// Selection box.
            SelectBox,
            /// Draw a line.
            DrawLine,
            /// Pan line.
            PanLine,
            /// Selection line.
            SelectLine,
            /// Draw an ellipse.
            DrawEllipse,
            /// Pan ellipse.
            PanEllipse,
            /// Selection ellipse.
            SelectEllipse
        };

    public:
        //! QWidget constructor of QMapControl for use with QtDesigner.
        /*!
         * The QMapControl is the widget which displays the maps.
         * @param parent QObject parent ownership.
         * @param window_flags QWidget window flags.
         */
        QMapControl(QWidget * parent = 0, Qt::WindowFlags window_flags = 0);

        //! QMapControl constructor.
        /*!
         * The QMapControl is the widget which displays the maps.
         * @param size_px The size of the widget in pixels.
         * @param parent QObject parent ownership.
         * @param window_flags QWidget window flags.
         */
        QMapControl(const QSizeF& size_px, QWidget* parent = 0, Qt::WindowFlags window_flags = 0);

        //! Disable copy constructor.
        ///QMapControl(const QMapControl&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///QMapControl& operator=(const QMapControl&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~QMapControl();

        // Settings.
        /*!
         * Set the displayed projection.
         * @param epsg The projection required.
         */
        void setProjection(const projection::EPSG& epsg = projection::EPSG::SphericalMercator);

        /*!
         * Set the tile size used in pxiels.
         * @param tile_size_px The tile size in pixels required.
         */
        void setTileSizePx(const int& tile_size_px = 256);

        /*!
         * Set the background colour of the map control.
         * @param colour The background colour to set.
         */
        void setBackgroundColour(const QColor& colour = Qt::transparent);

        /*!
         * Enable persistent caching of map tiles.
         * Call this method to allow the QMapControl widget to save map tiles persistent (also over application restarts).
         * Default: Images are stored in the subdirectory "QMapControl.cache" within the user's home directory.
         * @param path The path where the images should be stored.
         * @param expiry The max age (in minutes) of an image before its removed and a new one is requested (0 to keep forever).
         */
        void enablePersistentCache(const std::chrono::minutes& expiry = std::chrono::minutes(0), const QDir& path = QDir::homePath() + QDir::separator() + "QMapControl.cache");

        /*!
         * Sets the proxy for HTTP connections.
         * @param proxy The proxy details.
         */
        void setProxy(const QNetworkProxy& proxy);

        /*!
         * Sets the proxy for HTTP connections.
         * @param host The proxy´s hostname or ip.
         * @param port The proxy´s port.
         */
        void setProxy(const std::string& host, const int& port);

        /*!
         * Whether the scaled primary screen image should be displayed during zoom changes in the background.
         * @param visible Whether the scaled background image should be displayed.
         */
        void enableScaledBackground(const bool& visible);

        /*!
         * Set whether the scalebar should be displayed within the widget.
         * @param visible Whether the scalebar should be displayed.
         */
        void enableScalebar(const bool& visible);

        /*!
         * Set whether the crosshairs should be displayed at the center point of the widget.
         * @param visible Whether the crosshairs should be displayed.
         */
        void enableCrosshairs(const bool& visible);

        // Layer management.
        /*!
         * Fetch the layers (Use this instead of the member variable for thread-safety).
         * @return a list of all layers.
         */
        const std::vector< std::shared_ptr<Layer> > getLayers() const;

        /*!
         * Fetch the layer with the given name.
         * @param name The name of the wanted layer.
         * @return the layer with the given name.
         */
        std::shared_ptr<Layer> getLayer(const std::string& name) const;

        /*!
         * Adds a layer.
         * If multiple layers are added, they are painted in the added order (the last added wil be the top layer).
         * @param layer The layer which should be added.
         * @param index The index position the layer should added (-1 means the layer is added to the top).
         * @note 'index 0' is the base layer, 'index 1 layer' will be painted on top of 'index 0 layer', and so on...
         */
        void addLayer(const std::shared_ptr<Layer>& layer, const int& index = -1);

        /*!
         * Removes a layer.
         * @param name The name of the layer which should be removed.
         */
        void removeLayer(const std::string& name);

        // Geometry management.
        /*!
         * Whether a geometry is currently visible in the viewport.
         * @param geometry The geometry object to check.
         * @param partial Whether a geometry can be partially visible, or must be totally visible.
         * @return whether the geometry is visible.
         */
        bool isGeometryVisible(const std::shared_ptr<Geometry> geometry, const bool& partial) const;

        /*!
         * Keeps the center of the map on the Geometry, even when it moves.
         * To stop following the geometry, call stopFollowing().
         * @param geometry The Geometry which should stay centered.
         */
        void followGeometry(const std::shared_ptr<Geometry> geometry);

        /*!
         * Stop following a Geometry.
         * @param geometry The Geometry which should not followed anymore
         */
        void stopFollowingGeometry();

        // Viewport management.
        /*!
         * Set the viewport size in pixels.
         * @param size_px The viewport (visible-part of each layer) size in pixels.
         */
        void setViewportSize(const QSizeF& size_px);

        /*!
         * Fetches the visible viewport rect in world coordinates.
         * @return the visible viewport rect in world coordinates.
         */
        QRectF getViewportRect() const;

        /*!
         * Check whether all coordinates are visible in the current viewport.
         * @param points_coord The coordinates to check.
         * @return Whether all the coordinates are visible in the viewport.
         */
        bool viewportContainsAll(const std::vector<QPointF>& points_coord) const;

        /*!
         * Reset limited viewport rect (ie: disable it).
         */
        void resetLimitedViewportRect();

        /*!
         * Set a limited viewport rect.
         * @param top_left_coord The top-left coordinate of the limited viewport rect.
         * @param bottom_right_coord The bottom-right coordinate of the limited viewport rect.
         */
        void setLimitedViewportRect(const QPointF& top_left_coord, const QPointF& bottom_right_coord);

        // Map management.
        /*!
         * Fetches the map focus point in world coordinates (location on map in relation to the center of the screen).
         * @return The map focus point in world coordinates.
         */
        QPointF mapFocusPointCoord() const;

        /*!
         * Set the map focus point to the given coordinate.
         * @param point_coord The coordinate which the view´s middle should be set to
         */
        void setMapFocusPoint(const QPointF& point_coord);

        /*!
         * Sets the map focus point, based on the mean average of all the coordinates.
         * If request, will also automatically calculate best zoom level and zoom to it.
         * @param points_coord The coorinates to based the map focus point on.
         * @param auto_zoom Whether to automatically zoom to the best level.
         */
        void setMapFocusPoint(const std::vector<QPointF>& points_coord, const bool& auto_zoom = false);

        /*!
         * Smoothly moves the center of the view to the given coordinate.
         * @param coordinate The coordinate which the center of the view should moved to.
         * @param steps The number of steps to use in the animation.
         * @param step_interval The number of millseconds to wait between each step.
         */
        void setMapFocusPointAnimated(const QPointF& coordinate, const int& steps = 25, const std::chrono::milliseconds& step_interval = std::chrono::milliseconds(50));

        /*!
         * Scrol the view left by required pixels.
         * @param delta_px The required number of pixels to scroll by.
         */
        void scrollViewLeft(const double& delta_px = 10.0);

        /*!
         * Scrol the view right by required pixels.
         * @param delta_px The required number of pixels to scroll by.
         */
        void scrollViewRight(const double& delta_px = 10.0);

        /*!
         * Scrol the view up by required pixels.
         * @param delta_px The required number of pixels to scroll by.
         */
        void scrollViewUp(const double& delta_px = 10.0);

        /*!
         * Scrol the view down by required pixels.
         * @param delta_px The required number of pixels to scroll by.
         */
        void scrollViewDown(const double& delta_px = 10.0);

        // Zoom management.
        /*!
         * Set the minimum zoom allowed.
         * @param zoom The minimum zoom allowed.
         */
        void setZoomMinimum(const int& zoom);

        /*!
         * Set the maximum zoom allowed.
         * @param zoom The maximum zoom allowed.
         */
        void setZoomMaximum(const int& zoom);

        /*!
         * Fetches the current zoom level.
         * @return the current zoom level.
         */
        int getCurrentZoom() const;

        /*!
         * Set whether the zoom controls should be displayed.
         * @param enable Whether the zoom control should be displayed.
         * @param align_left Whether to align the zoom controls to left or right of the widget.
         */
        void enableZoomControls(const bool& enable, const bool& align_left = true);

        // Mouse management.
        /*!
         * Set whether the layers should handle mouse events.
         * @param enable Whether the layers should handle mouse events.
         */
        void enableLayerMouseEvents(const bool& enable);

        /*!
         * Fetches the left mouse button mode.
         * @return the left mouse button mode.
         */
        QMapControl::MouseButtonMode getMouseButtonLeftMode();

        /*!
         * Sets the left mouse button settings of the QMapControl.
         * @param mode The mouse mode required.
         * @param origin_center Whether the origin is centered to the starting mouse point.
         */
        void setMouseButtonLeft(const MouseButtonMode& mode, const bool& origin_center);

        /*!
         * Fetches the right mouse button mode.
         * @return the right mouse button mode.
         */
        QMapControl::MouseButtonMode getMouseButtonRightMode();

        /*!
         * Sets the right mouse button settings of the QMapControl.
         * @param mode The mouse mode required.
         * @param origin_center Whether the origin is centered to the starting mouse point.
         */
        void setMouseButtonRight(const MouseButtonMode& mode, const bool& origin_center);

        /*!
         * Called when a mouse button is pressed down.
         * @param mouse_event The mouse event.
         */
        void mousePressEvent(QMouseEvent* mouse_event);

        /*!
         * Called when a mouse button is released.
         * @param mouse_event The mouse event.
         */
        void mouseReleaseEvent(QMouseEvent* mouse_event);

        /*!
         * Called when a mouse button is double clicked.
         * @param mouse_event The mouse event.
         */
        void mouseDoubleClickEvent(QMouseEvent* mouse_event);

        /*!
         * Called when a mouse is moved.
         * @param mouse_event The mouse event.
         */
        void mouseMoveEvent(QMouseEvent* mouse_event);

        /*!
         * Called when a mouse's wheel is scrolled.
         * @param wheel_event The mouse wheel event.
         */
        void wheelEvent(QWheelEvent* wheel_event);

        // Keyboard management.
        /*!
         * Called when a keyboard key is pressed.
         * @param key_event The keyboard key event.
         */
        void keyPressEvent(QKeyEvent* key_event);

        // Drawing management.
        /*!
         * Fetch the primary screen pixmap.
         * @return the primary screen pixmap.
         */
        QPixmap getPrimaryScreen() const;

    public slots:
        // Zoom management.
        /*!
         * Zoom in one level.
         */
        void zoomIn();

        /*!
         * Zoom out one level.
         */
        void zoomOut();

        /*!
         * Set the zoom level required.
         * @param zoom The zoom level requested.
         */
        void setZoom(int zoom);

        // Drawing management.
        /*!
         * Called when something requires the view to be redrawn.
         */
        void requestRedraw();

    private:
        // Map management.
        /*!
         * Converts a mouse click point in pixels to map point in pixels (uses the current map focus point).
         * @param click_point_px The mouse click point in pixels to convert.
         * @return the map point in pixels.
         */
        QPointF toPointPx(const QPointF& click_point_px) const;

        /*!
         * Converts a mouse click point in pixels to map point in pixels.
         * @param click_point_px The mouse click point in pixels to convert.
         * @param map_focus_point_px The map focus point in pixels to use.
         * @return the map point in pixels.
         */
        QPointF toPointPx(const QPointF& click_point_px, const QPointF& map_focus_point_px) const;

        /*!
         * Converts a mouse click point in pixels to map point in coordinates (uses the current map focus point).
         * @param click_point_px The mouse click point in pixels to convert.
         * @return the map point in coordinates.
         */
        QPointF toPointCoord(const QPointF& click_point_px) const;

        /*!
         * Converts a mouse click point in pixels to map point in coordinates.
         * @param click_point_px The mouse click point in pixels to convert.
         * @param map_focus_point_px The map focus point in pixels to use.
         * @return the map point in coordinates.
         */
        QPointF toPointCoord(const QPointF& click_point_px, const QPointF& map_focus_point_px) const;

        /*!
         * Fetches the map focus point in pixels (location on map in relation to the center of the screen).
         * @return the map focus point in pixels.
         */
        QPointF mapFocusPointPx() const;

        /*!
         * Calculates the map focus point from a list of coordinates, based on the x and y mean values.
         * @param points_coord The coorinates to based the map focus point on.
         * @return the map focus point, based on the x and y mean values.
         */
        QPointF calculateMapFocusPoint(const std::vector<QPointF>& points_coord);

        /*!
         * Scrolls the view by the given value in pixels and in display coordinates.
         * @param delta_px The distance which the view should be scrolled in pixels.
         */
        void scrollView(const QPointF& delta_px);

        // Zoom management.
        /*!
         * Check that the zoom settings are valid, and if not fixes them.
         */
        void checkZoom();

        /*!
         * Updates the zoom/progress indicator controls.
         */
        void updateControls();

        // Drawing management.
        /*!
         * Called by QWidget to redraw the current view to the QWidget display.
         * @param paint_event The paint event.
         */
        void paintEvent(QPaintEvent* paint_event);

        /*!
         * Draw the primary screen and "auto-moving" geometries to the pixmap.
         * @param painter The pixmap painter to draw to.
         */
        void drawPrimaryScreen(QPainter* painter) const;

        /*!
         * Check whether we need to redraw the backbuffer (map focus point will have moved).
         * @return whether to backbuffer needs redrawing.
         */
        bool checkBackbuffer() const;

        /*!
         * Redraws the primary screen image.
         * @param force_redraw Whether to force the backbuffer to be redrawn, even if checkBackbuffer() states we do not need to.
         */
        void redrawPrimaryScreen(const bool& force_redraw = false);

        /*!
         * Redraws the backbuffer image, which when ready will emit updatePrimaryScreen() for it to be stored/drawn.
         */
        void redrawBackbuffer();

    private slots:
        // Geometry management.
        /*!
         * Called when a geometry changes its position.
         * @param geometry The geometry that change position.
         */
        void geometryPositionChanged(Geometry* geometry);

        // Map management.
        /*!
         * Called during the animation loop to process the next step.
         */
        void animatedTick();

        // Drawing management.
        /*!
         * Called when the Image Manager has loaded all requested images (removes the zoom image).
         */
        void loadingFinished();

        /*!
         * Called when the backbuffer has been updated, to replace the existing primary screen and request a QWidget::update().
         * @param backbuffer_pixmap The updated backbuffer pixmap.
         * @param backbuffer_rect_px The updated backbuffer rect in pixels.
         * @param backbuffer_map_focus_px The updated backbuffer map foucs point in pixels.
         */
        void updatePrimaryScreen(QPixmap backbuffer_pixmap, QRectF backbuffer_rect_px, QPointF backbuffer_map_focus_px);

    signals:
        // Geometry management.
        /*!
         * Signal emitted when a geometry is clicked.
         * @param geometry The clicked Geometry.
         */
        void geometryClicked(Geometry* geometry);

        /*!
         * Signal emitted when geometries are selected (see MouseButtonModes).
         * @param selected_geometries The selected geometries in each layer.
         */
        void geometriesSelected(std::map<std::string, std::vector<std::shared_ptr<Geometry>>> selected_geometries);

        // Mouse management.
        /*!
         * Signal emitted on MousePressEvents with the additional map coordinates of the mouse press.
         * @param mouse_event The QMouseEvent that occured.
         * @param press_coordinate The corresponding world coordinate of the mouse press.
         */
        void mouseEventPressCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate);

        /*!
         * Signal emitted on MouseReleaseEvents with the additional map coordinates of the mouse press/release.
         * @param mouse_event The QMouseEvent that occured.
         * @param press_coordinate The corresponding world coordinate of the mouse press.
         * @param release_coordinate The corresponding world coordinate of the mouse release.
         */
        void mouseEventReleaseCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate, QPointF release_coordinate);

        /*!
         * Signal emitted on MouseDoubleClickEvents with the additional map coordinates of the mouse press/double press.
         * @param mouse_event The QMouseEvent that occured.
         * @param press_coordinate The corresponding world coordinate of the mouse press.
         * @param double_press_coordinate The corresponding world coordinate of the mouse double press.
         */
        void mouseEventDoubleClickCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate, QPointF double_press_coordinate);

        /*!
         * Signal emitted on MouseMoveEvents with the additional map coordinates of the mouse press/current.
         * @param mouse_event The QMouseEvent that occured.
         * @param press_coordinate The corresponding world coordinate of the mouse press.
         * @param current_coordinate The corresponding world coordinate of the current mouse.
         */
        void mouseEventMoveCoordinate(QMouseEvent* mouse_event, QPointF press_coordinate, QPointF current_coordinate);

        // Drawing management.
        /*!
         * Signal emitted when the backbuffer has been updated.
         * @param backbuffer_pixmap The updated backbuffer pixmap.
         * @param backbuffer_rect_px The updated backbuffer rect in pixels.
         * @param backbuffer_map_focus_px The updated backbuffer map foucs point in pixels.
         */
        void updatedBackBuffer(QPixmap backbuffer_pixmap, QRectF backbuffer_rect_px, QPointF backbuffer_map_focus_px);

    private:
        //! Disable copy constructor.
        QMapControl(const QMapControl&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        QMapControl& operator=(const QMapControl&); /// @todo remove once MSVC supports default/delete syntax.

        /// Whether the scale should be visible.
        bool m_scalebar_enabled;

        /// Whether the crossharis should be visible.
        bool m_crosshairs_enabled;

        /// List of layers (use getLayers() to access this for thread-safe read-only functionality).
        std::vector<std::shared_ptr<Layer>> m_layers;

        /// Mutex to protect layers.
        mutable QReadWriteLock m_layers_mutex;

        /// Whether layer mouse events are enabled.
        bool m_layer_mouse_events_enabled;

        /// The current following geometry signal/slot connection.
        QMetaObject::Connection m_following_geometry;

        /// The viewport (visible-part of each layer) size in pixels.
        QSizeF m_viewport_size_px;

        /// The viewport (visible-part of each layer) center point in pixels.
        QPointF m_viewport_center_px;

        /// The viewing bounding box coord (set to QRectF(0.0, 0.0, 0.0, 0.0) to disable.
        QRectF m_limited_viewport_rect_coord;

        /// The current map focus point in coordinates.
        QPointF m_map_focus_coord;

        /// Mutex to protect the animation loop.
        std::mutex m_animated_mutex;

        /// Animation map focus point target.
        QPointF m_animated_map_focus_point;

        /// Number of animation steps remaining.
        int m_animated_steps;

        /// The number of millseconds to wait between each step.
        std::chrono::milliseconds m_animated_interval;

        /// The minimum zoom allowed.
        int m_zoom_minimum;

        /// The maximum zoom allowed.
        int m_zoom_maximum;

        /// The current zoom.
        int m_current_zoom;

        /// Whether the left mouse button is currently pressed.
        bool m_mouse_left_pressed;

        /// The left mouse button mode.
        MouseButtonMode m_mouse_left_mode;

        /// Whether the left mouse button drawn/pan shape's origin is centered to the starting mouse point.
        bool m_mouse_left_origin_center;

        /// Whether the right mouse button is currently pressed.
        bool m_mouse_right_pressed;

        /// The right mouse button mode.
        MouseButtonMode m_mouse_right_mode;

        /// Whether the right mouse button drawn/pan shape's origin is centered to the starting mouse point.
        bool m_mouse_right_origin_center;

        /// The mouse position when a mouse button was initially pressed in pixels.
        QPointF m_mouse_position_pressed_px;

        /// The current mouse position in pixels (set after every mouse event).
        QPointF m_mouse_position_current_px;

        /// Primary screen pixmap (always 2 x viewport size to allow for panning backbuffer).
        QPixmap m_primary_screen;

        /// The map focus point when the primary screen was created.
        QPointF m_primary_screen_map_focus_point;

        /// Primary screen backbuffer rect in pixels.
        QRectF m_primary_screen_backbuffer_rect_px;

        /// Whether the primary screen scaled is drawn in the background.
        bool m_primary_screen_scaled_enabled;

        /// Primary screen scaled pixmap (zoom in/out).
        QPixmap m_primary_screen_scaled;

        /// Primary screen scaled pixmap offset (wheel events only).
        QPointF m_primary_screen_scaled_offset;

        /// Whether to align the zoom controls to the left (or right).
        bool m_zoom_control_align_left;

        /// The zoom control's '+' zoom in button.
        QPushButton m_zoom_control_button_in;

        /// The zoom control's slider.
        QSlider m_zoom_control_slider;

        /// The zoom control's '-' zoom out button.
        QPushButton m_zoom_control_button_out;

        /// Mutex to protect the backbuffer during the redraw process.
        std::mutex m_backbuffer_mutex;

        /// Mutex to only allow only one other thread to wait for the redraw process.
        std::mutex m_backbuffer_queued_mutex;

        /// Progress indicator to alert user to redrawing progress.
        QProgressIndicator m_progress_indicator;
    };
}
