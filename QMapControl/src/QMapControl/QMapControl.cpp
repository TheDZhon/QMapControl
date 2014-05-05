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

#include "QMapControl.h"

// Qt includes.
#include <QtConcurrent/QtConcurrentRun>
#include <QtWidgets/QStyleOption>

// STL includes.
#include <cmath>
#include <utility>

// Local includes.
#include "GeometryLineString.h"
#include "GeometryPoint.h"
#include "GeometryPolygon.h"
#include "ImageManager.h"
#include "LayerGeometry.h"
#include "Projection.h"

namespace qmapcontrol
{
    QMapControl::QMapControl(QWidget * parent, Qt::WindowFlags window_flags)
        : m_viewport_center_px(0.0, 0.0),
          m_limited_viewport_rect_coord(PointWorldCoord(0.0, 0.0), PointWorldCoord(0.0, 0.0)),
          m_map_focus_coord(0.0, 0.0),
          m_animated_map_focus_point(0.0, 0.0),
          m_mouse_position_pressed_px(0.0, 0.0),
          m_mouse_position_current_px(0.0, 0.0),
          m_primary_screen_map_focus_point_px(0.0, 0.0),
          m_primary_screen_backbuffer_rect_px(PointWorldPx(0.0, 0.0), PointWorldPx(0.0, 0.0)),
          m_primary_screen_scaled_offset(0.0, 0.0)
    {
        /// @TODO understand how QtDesigner uses this so that the correct size can be passed to the constructor.
        QMapControl(parent->size(), parent, window_flags);
    }

    QMapControl::QMapControl(const QSizeF& size_px, QWidget* parent, Qt::WindowFlags window_flags)
        : QWidget(parent, window_flags),
          m_scalebar_enabled(false),
          m_crosshairs_enabled(true),
          m_layer_mouse_events_enabled(true),
          m_viewport_size_px(size_px),
          m_viewport_center_px(size_px.width() / 2.0, size_px.height() / 2.0),
          m_limited_viewport_rect_coord(PointWorldCoord(0.0, 0.0), PointWorldCoord(0.0, 0.0)),
          m_map_focus_coord(0.0, 0.0),
          m_animated_map_focus_point(0.0, 0.0),
          m_animated_steps(0),
          m_animated_interval(0),
          m_zoom_minimum(0),
          m_zoom_maximum(17),
          m_current_zoom(m_zoom_minimum),
          m_mouse_left_pressed(false),
          m_mouse_left_mode(MouseButtonMode::Pan),
          m_mouse_left_origin_center(false),
          m_mouse_right_pressed(false),
          m_mouse_right_mode(MouseButtonMode::DrawBox),
          m_mouse_right_origin_center(false),
          m_mouse_position_pressed_px(0.0, 0.0),
          m_mouse_position_current_px(0.0, 0.0),
          m_primary_screen(size_px.toSize() * 2),
          m_primary_screen_map_focus_point_px(0.0, 0.0),
          m_primary_screen_backbuffer_rect_px(PointWorldPx(0.0, 0.0), PointWorldPx(0.0, 0.0)),
          m_primary_screen_scaled_enabled(false),
          m_primary_screen_scaled(size_px.toSize() * 2),
          m_primary_screen_scaled_offset(0.0, 0.0),
          m_zoom_control_align_left(true),
          m_zoom_control_button_in("+", this),
          m_zoom_control_slider(Qt::Vertical, this),
          m_zoom_control_button_out("-", this),
          m_progress_indicator(this)
    {
        // Register meta types.
        qRegisterMetaType<RectWorldPx>("RectWorldPx");
        qRegisterMetaType<PointWorldPx>("PointWorldPx");

        // Ensure the primary screens are empty.
        m_primary_screen.fill(Qt::transparent);
        m_primary_screen_scaled.fill(Qt::transparent);

        // Connect signal/slot for when the backbuffer is updated, so that primary screen is updated in the main thread.
        QObject::connect(this, &QMapControl::updatedBackBuffer, this, &QMapControl::updatePrimaryScreen);

        // Connect signals from the Image Manager.
        QObject::connect(&ImageManager::get(), &ImageManager::imageUpdated, this, &QMapControl::requestRedraw);
        QObject::connect(&ImageManager::get(), &ImageManager::downloadingFinished, this, &QMapControl::loadingFinished);

        // Default - projection as Spherical Mercator.
        setProjection(projection::EPSG::SphericalMercator);

        // Default - tile size as 256px.
        setTileSizePx(256);

        // Default - zoom controls enabled.
        enableZoomControls(true);

        // Default - allow the map to gain click focus.
        setFocusPolicy(Qt::ClickFocus);

        // Default - enable mouse tracking (all mouse events received - not just clicks).
        setMouseTracking(true);

        // Default - background colour (transparent).
        setBackgroundColour(Qt::transparent);

        // Set QWidget viewport size.
        setViewportSize(size_px);
    }

    QMapControl::~QMapControl()
    {
        // Destroy the image manager instance.
        ImageManager::destory();
    }

    /// Public...
    // Settings.
    void QMapControl::setProjection(const projection::EPSG& epsg)
    {
        // Set the projection.
        projection::set(epsg);
    }

    void QMapControl::setTileSizePx(const int& tile_size_px)
    {
        // Set the tile size used by the Image Manager.
        ImageManager::get().setTileSizePx(tile_size_px);
    }

    void QMapControl::setBackgroundColour(const QColor& colour)
    {
        // Update the QWidget background colour.
        QWidget::setObjectName("QMapControl");
        QWidget::setStyleSheet("QWidget#QMapControl { background-color: " + colour.name() + " }");
    }

    void QMapControl::enablePersistentCache(const std::chrono::minutes& expiry, const QDir& path)
    {
        // Set the Image Manager's persistent cache settings.
        ImageManager::get().enablePersistentCache(expiry, path);
    }

    void QMapControl::setProxy(const QNetworkProxy& proxy)
    {
        // Set the Image Manager's network proxy.
        ImageManager::get().setProxy(proxy);
    }

    void QMapControl::setProxy(const std::string& host, const int& port)
    {
        // Set the Image Manager's network proxy.
        ImageManager::get().setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, host.c_str(), port));
    }

    void QMapControl::enableScaledBackground(const bool& visible)
    {
        // Set whether the scaled primary screen should be visible as a background image.
        m_primary_screen_scaled_enabled = visible;
    }

    void QMapControl::enableScalebar(const bool& visible)
    {
        // Set whether the scalebar should be visible.
        m_scalebar_enabled = visible;
    }

    void QMapControl::enableCrosshairs(const bool& visible)
    {
        // Set whether the crosshairs should be visible.
        m_crosshairs_enabled = visible;
    }

    // Layer management.
    const std::vector<std::shared_ptr<Layer> > QMapControl::getLayers() const
    {
        // Gain a read lock to protect the layers container.
        QReadLocker locker(&m_layers_mutex);

        // Return the layers.
        return m_layers;
    }

    std::shared_ptr<Layer> QMapControl::getLayer(const std::string& name) const
    {
        // Default return value.
        std::shared_ptr<Layer> return_layer(nullptr);

        // Fetch a copy of the current layers.
        const auto layers = getLayers();

        // Try to find the layer in question.
        const auto itr_find = std::find_if(layers.begin(), layers.end(), [&](const std::shared_ptr<Layer>& layer) { return layer->getName() == name; });

        // Did we find it?
        if(itr_find != layers.end())
        {
            // Return the layer found.
            return_layer = (*itr_find);
        }

        // Return the layer.
        return return_layer;
    }

    void QMapControl::addLayer(const std::shared_ptr<Layer>& layer, const int& index)
    {
        // Check we have a valid layer.
        if(layer != nullptr)
        {
            // Remove the layer if it already exists.
            removeLayer(layer->getName());

            // Connect signals as required.
            QObject::connect(layer.get(), &Layer::requestRedraw, this, &QMapControl::requestRedraw);

            // Is it a geometry layer?
            if(layer->getLayerType() == Layer::LayerType::LayerGeometry)
            {
                // Connect the geometry clicked signal/slot.
                QObject::connect(static_cast<LayerGeometry*>(layer.get()), &LayerGeometry::geometryClicked, this, &QMapControl::geometryClicked);
            }

            // Scope the locker to ensure the mutex is release as soon as possible.
            {
                // Gain a write lock to protect the layers container.
                QWriteLocker locker(&m_layers_mutex);

                // Is the index == -1 or greater than current vector size.
                if(index == -1 || index >= int(m_layers.size()))
                {
                    // Add the layer to the end.
                    m_layers.push_back(layer);
                }
                else
                {
                    // Insert the layer at the required index.
                    m_layers.insert(m_layers.begin() + index, layer);
                }
            }

            // Force the primary screen to be redrawn.
            redrawPrimaryScreen(true);
        }
    }

    void QMapControl::removeLayer(const std::string& name)
    {
        // Fetch the layer by name.
        const auto layer = getLayer(name);

        // Is the layer valid.
        if(layer != nullptr)
        {
            // Keep track of our success.
            bool success(false);

            // Scope the locker to ensure the mutex is release as soon as possible.
            {
                // Gain a write lock to protect the layers container.
                QWriteLocker locker(&m_layers_mutex);

                // Try to find the layer in question.
                const auto itr_find = std::find(m_layers.begin(), m_layers.end(), layer);

                // Did we find it?
                if(itr_find != m_layers.end())
                {
                    // Disconnect all signals associated with the layer.
                    QObject::disconnect(itr_find->get(), 0, this, 0);

                    // Remove the layer.
                    m_layers.erase(itr_find);

                    // Update our success!
                    success = true;
                }
            }

            // Was we successful in removing the layer?
            if(success)
            {
                // Force the primary screen to be redrawn.
                redrawPrimaryScreen(true);
            }
        }
    }

    // Geometry management.
    bool QMapControl::isGeometryVisible(const std::shared_ptr<Geometry> geometry, const bool& partial) const
    {
        // Default return visibility.
        bool return_visible(false);

        // Check the geometry and viewport are valid.
        if(geometry != nullptr && getViewportRect().rawRect().isValid())
        {
            // Do we want a partial visibile check?
            if(partial)
            {
                // Check whether the geometry bounding box is partially contained by the viewport rect.
                return_visible = getViewportRect().rawRect().intersects(geometry->boundingBox(m_current_zoom).rawRect());
            }
            else
            {
                // Check whether the geometry bounding box is totally contained by the viewport rect.
                return_visible = getViewportRect().rawRect().contains(geometry->boundingBox(m_current_zoom).rawRect());
            }
        }

        // Return the visibility.
        return return_visible;
    }

    void QMapControl::followGeometry(const std::shared_ptr<Geometry> geometry)
    {
        // Check the geometry is valid.
        if(geometry != nullptr)
        {
            // Remove any previous geometry following.
            stopFollowingGeometry();

            // Connect the position changed of the geometry to the controller.
            m_following_geometry = QObject::connect(geometry.get(), &Geometry::positionChanged, this, &QMapControl::geometryPositionChanged);
        }
    }

    void QMapControl::stopFollowingGeometry()
    {
        // Disconnect the signal/slot updates.
        QObject::disconnect(m_following_geometry);
    }

    // Viewport management.
    void QMapControl::setViewportSize(const QSizeF& size_px)
    {
        // Set the inherited QWidget maximum size.
        QWidget::setMaximumSize(size_px.width() + 1, size_px.height() + 1);

        // Set the size of the viewpoint (visible-part of the layer) in pixels.
        m_viewport_size_px = size_px;

        // Calculate the middle point of the viewport (visible-part of the layer) in pixels.
        m_viewport_center_px = PointViewportPx(m_viewport_size_px.width() / 2.0, m_viewport_size_px.height() / 2.0);

        // Create new pixmaps with the new size required (2 x viewport size to allow for panning backbuffer).
        m_primary_screen = QPixmap(m_viewport_size_px.toSize() * 2);
        m_primary_screen.fill(Qt::transparent);
        m_primary_screen_scaled = QPixmap(m_viewport_size_px.toSize() * 2);
        m_primary_screen_scaled.fill(Qt::transparent);
        m_primary_screen_scaled_offset = PointPx(0.0, 0.0);

        // Force the primary screen to be redrawn.
        redrawPrimaryScreen(true);

        // Ensure the zoom controls are updated.
        updateControls();
    }

    RectWorldCoord QMapControl::getViewportRect() const
    {
        // Return the viewport rect converted into the coordinates system.
        return RectWorldCoord(projection::get().toPointWorldCoord(mapFocusPointWorldPx() - m_viewport_center_px, m_current_zoom), projection::get().toPointWorldCoord(mapFocusPointWorldPx() + m_viewport_center_px, m_current_zoom));
    }

    bool QMapControl::viewportContainsAll(const std::vector<PointWorldCoord>& points_coord) const
    {
        // Default return value.
        bool return_contains_all(true);

        // Get the current viewport rect.
        const RectWorldCoord viewport_rect_coord(getViewportRect());

        // Loop through each coordinate and check it is contained by the viewport rect, stop if we find one outside the rect.
        for(size_t i = 0; return_contains_all == true && i < points_coord.size(); i++)
        {
            // Is the point within the rect.
            return_contains_all = viewport_rect_coord.rawRect().contains(points_coord.at(i).rawPoint());
        }

        // Return the result.
        return return_contains_all;
    }

    void QMapControl::resetLimitedViewportRect()
    {
        // Reset limited viewport rect to 0 (ie: disable it).
        m_limited_viewport_rect_coord = RectWorldCoord(PointWorldCoord(0.0, 0.0), PointWorldCoord(0.0, 0.0));
    }

    void QMapControl::setLimitedViewportRect(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord)
    {
        // Set the limited viewport rect.
        m_limited_viewport_rect_coord = RectWorldCoord(top_left_coord, bottom_right_coord);
    }

    // Map management.
    PointWorldCoord QMapControl::mapFocusPointCoord() const
    {
        // Return the current map focus point in coordinates.
        return m_map_focus_coord;
    }

    void QMapControl::setMapFocusPoint(const PointWorldCoord& point_coord)
    {
        // Set the map focus point.
        m_map_focus_coord = point_coord;

        // Request the primary screen to be redrawn.
        redrawPrimaryScreen();
    }

    void QMapControl::setMapFocusPoint(const std::vector<PointWorldCoord>& points_coord, const bool& auto_zoom)
    {
        // Calculate the map focus point and then set the view.
        setMapFocusPoint(calculateMapFocusPoint(points_coord));

        // Has auto zoom been requested?
        if(auto_zoom)
        {
            // Auto zoom go-go-go!

            // While the view cannot fit all the coordinates in and we have not reached the minimum zoom level.
            while(viewportContainsAll(points_coord) == false && m_current_zoom > m_zoom_minimum)
            {
                // Zoom out to the next level.
                zoomOut();
            }

            // While the view fits all the coordinates in and we have not reached the maximum zoom level.
            while(viewportContainsAll(points_coord) == true && m_current_zoom < m_zoom_maximum)
            {
                // Zoom in to the next level.
                zoomIn();
            }

            // Finally, check we haven't accidentally zoomed in too far, meaning the coordinates do not actually fit the display.
            if(viewportContainsAll(points_coord) == false && m_current_zoom > m_zoom_minimum)
            {
                // Zoom out to the next level.
                zoomOut();
            }
        }
    }

    void QMapControl::setMapFocusPointAnimated(const PointWorldCoord& coordinate, const int& steps, const std::chrono::milliseconds& step_interval)
    {
        // Is animation already taking place?
        if(m_animated_mutex.try_lock())
        {
            // Set the new map focus point to scroll to.
            m_animated_map_focus_point = coordinate;

            // Set the number of steps it should take.
            m_animated_steps = steps;

            // Set the interval between each step.
            m_animated_interval = step_interval;

            // Set the animation tick to start.
            QTimer::singleShot(m_animated_interval.count(), this, SLOT(animatedTick()));
        }
        else
        {
            // Warn that we cannot animate as we are already mid-animating.
            qDebug() << "Unable to animate to new map focus, already animating!";
        }
    }

    void QMapControl::scrollViewLeft(const double& delta_px)
    {
        // Scroll the view left by required pixels.
        scrollView(PointPx(-delta_px, 0.0));
    }

    void QMapControl::scrollViewRight(const double& delta_px)
    {
        // Scroll the view right by required pixels.
        scrollView(PointPx(delta_px, 0.0));
    }

    void QMapControl::scrollViewUp(const double& delta_px)
    {
        // Scroll the view up by required pixels.
        scrollView(PointPx(0.0, -delta_px));
    }

    void QMapControl::scrollViewDown(const double& delta_px)
    {
        // Scroll the view down by required pixels.
        scrollView(PointPx(0.0, delta_px));
    }

    // Zoom management.
    void QMapControl::setZoomMinimum(const int& zoom)
    {
        // Set the new zoom minimum.
        m_zoom_minimum = zoom;

        // Check the zoom.
        checkZoom();

        // Ensure the zoom controls are updated.
        updateControls();
    }

    void QMapControl::setZoomMaximum(const int& zoom)
    {
        // Set the new zoom maximum.
        m_zoom_maximum = zoom;

        // Check the zoom.
        checkZoom();

        // Ensure the zoom controls are updated.
        updateControls();
    }

    int QMapControl::getCurrentZoom() const
    {
        // Return the zoom.
        return m_current_zoom;
    }

    void QMapControl::enableZoomControls(const bool& enable, const bool& align_left)
    {
        // Update the required aligment.
        m_zoom_control_align_left = align_left;

        // Set the zoom controls visibility.
        m_zoom_control_button_in.setVisible(enable);
        m_zoom_control_slider.setVisible(enable);
        m_zoom_control_button_out.setVisible(enable);

        // Disconnect previous signals from zoom controls.
        QObject::disconnect(&m_zoom_control_button_in, 0, this, 0);
        QObject::disconnect(&m_zoom_control_slider, 0, this, 0);
        QObject::disconnect(&m_zoom_control_button_out, 0, this, 0);

        // Connect signals from zoom controls.
        QObject::connect(&m_zoom_control_button_in, &QPushButton::clicked, this, &QMapControl::zoomIn);
        QObject::connect(&m_zoom_control_slider, &QSlider::valueChanged, this, &QMapControl::setZoom);
        QObject::connect(&m_zoom_control_button_out, &QPushButton::clicked, this, &QMapControl::zoomOut);

        // Update zoom controls.
        updateControls();
    }

    // Mouse management.
    void QMapControl::enableLayerMouseEvents(const bool& enable)
    {
        // Set whether to enable mouse events for layers.
        m_layer_mouse_events_enabled = enable;
    }

    QMapControl::MouseButtonMode QMapControl::getMouseButtonLeftMode()
    {
        // Return the left mouse button mode.
        return m_mouse_left_mode;
    }

    void QMapControl::setMouseButtonLeft(const MouseButtonMode& mode, const bool& origin_center)
    {
        // Set the left mouse button settings.
        m_mouse_left_mode = mode;
        m_mouse_left_origin_center = origin_center;
    }

    QMapControl::MouseButtonMode QMapControl::getMouseButtonRightMode()
    {
        // Return the right mouse button mode.
        return m_mouse_right_mode;
    }

    void QMapControl::setMouseButtonRight(const MouseButtonMode& mode, const bool& origin_center)
    {
        // Set the right mouse button settings.
        m_mouse_right_mode = mode;
        m_mouse_right_origin_center = origin_center;
    }

    void QMapControl::mousePressEvent(QMouseEvent* mouse_event)
    {
        // Store the mouse location of the current/starting mouse click.
        m_mouse_position_current_px = PointViewportPx(mouse_event->localPos().x(), mouse_event->localPos().y());
        m_mouse_position_pressed_px = m_mouse_position_current_px;

        // Are mouse events enabled for all layers?
        if(m_layer_mouse_events_enabled)
        {
            // Loop through each layer and pass the mouse event on.
            for(const auto& layer : getLayers())
            {
                // Send the mouse press event to the layer.
                layer->mousePressEvent(mouse_event, toPointWorldCoord(m_mouse_position_current_px), m_current_zoom);
            }
        }

        // Left button pressed?
        if(mouse_event->button() == Qt::MouseButton::LeftButton)
        {
            // Capture the left mouse press.
            m_mouse_left_pressed = true;
        }
        // Right button pressed?
        else if(mouse_event->button() == Qt::MouseButton::RightButton)
        {
            // Capture the right mouse press.
            m_mouse_right_pressed = true;
        }

        // Emit the pressed mouse event with the current mouse coordinate.
        emit mouseEventPressCoordinate(mouse_event, toPointWorldCoord(m_mouse_position_current_px));
    }

    void QMapControl::mouseReleaseEvent(QMouseEvent* mouse_event)
    {
        // Store the mouse location of the current mouse click.
        m_mouse_position_current_px = PointViewportPx(mouse_event->localPos().x(), mouse_event->localPos().y());

        // Default mouse mode.
        QMapControl::MouseButtonMode mouse_mode = QMapControl::MouseButtonMode::None;
        bool mouse_origin_center = false;

        // Left button released?
        if(mouse_event->button() == Qt::MouseButton::LeftButton)
        {
            // Capture the left mouse button release.
            m_mouse_left_pressed = false;

            // Capture the left mouse mode.
            mouse_mode = m_mouse_left_mode;
            mouse_origin_center = m_mouse_left_origin_center;
        }
        // Right button released?
        else if(mouse_event->button() == Qt::MouseButton::RightButton)
        {
            // Capture the right mouse button release.
            m_mouse_right_pressed = false;

            // Capture the right mouse mode.
            mouse_mode = m_mouse_right_mode;
            mouse_origin_center = m_mouse_right_origin_center;
        }

        // Are we in pan ... mode?
        if(mouse_mode == QMapControl::MouseButtonMode::PanBox ||
                mouse_mode == QMapControl::MouseButtonMode::PanLine ||
                mouse_mode == QMapControl::MouseButtonMode::PanEllipse)
        {
            // Capture coords to move/zoom with.
            std::vector<PointWorldCoord> coords;

            // Origin at center?
            if(mouse_origin_center)
            {
                // From mouse pressed with offsets applied.
                const PointPx mouse_diff(m_mouse_position_pressed_px - m_mouse_position_current_px);
                coords.push_back(toPointWorldCoord(m_mouse_position_pressed_px - mouse_diff));
                coords.push_back(toPointWorldCoord(m_mouse_position_pressed_px + mouse_diff));
            }
            else
            {
                // From mouse pressed to mouse released.
                coords.push_back(toPointWorldCoord(m_mouse_position_pressed_px));
                coords.push_back(toPointWorldCoord(m_mouse_position_current_px));
            }

            // Tell the map control to move and zoom as required by pan ... mode.
            setMapFocusPoint(coords, true);
        }
        // Else, are we in select ... mode?
        else if(mouse_mode == QMapControl::MouseButtonMode::SelectBox ||
                mouse_mode == QMapControl::MouseButtonMode::SelectLine ||
                mouse_mode == QMapControl::MouseButtonMode::SelectEllipse)
        {
            // From mouse pressed to mouse released.
            PointWorldPx top_left_px(toPointWorldPx(m_mouse_position_pressed_px));
            PointWorldPx bottom_right_px(toPointWorldPx(m_mouse_position_current_px));

            // Origin at center?
            if(mouse_origin_center)
            {
                // From mouse pressed with offsets applied.
                const PointPx mouse_diff(m_mouse_position_pressed_px - m_mouse_position_current_px);
                top_left_px = toPointWorldPx(m_mouse_position_pressed_px - mouse_diff);
                bottom_right_px = toPointWorldPx(m_mouse_position_pressed_px + mouse_diff);
            }

            // Convert to world coordinates.
            const PointWorldCoord top_left_coord(projection::get().toPointWorldCoord(top_left_px, m_current_zoom));
            const PointWorldCoord bottom_right_coord(projection::get().toPointWorldCoord(bottom_right_px, m_current_zoom));

            // Construct geometry to compare against (default is polygon/rect).
            std::unique_ptr<Geometry> geometry_to_compare_coord(new GeometryPolygon({top_left_coord, bottom_right_coord}));
            if(mouse_mode == QMapControl::MouseButtonMode::SelectLine)
            {
                // Set the line with a 'fuzzy-factor' around it using the pen.
                /// @TODO expose the fuzzy factor as a setting.
                const qreal fuzzy_factor_px = 5.0;
                QPen line_pen;
                line_pen.setWidthF(fuzzy_factor_px);

                // Create line string geometry.
                geometry_to_compare_coord.reset(new GeometryLineString({top_left_coord, bottom_right_coord}));
                geometry_to_compare_coord->setPen(line_pen);
            }
            else if(mouse_mode == QMapControl::MouseButtonMode::SelectEllipse)
            {
                /// @todo!
            }

//            // Construct area to check.
//            // Default to rect.
//            std::unique_ptr<QGraphicsItem> area_px(new QGraphicsRectItem(RectWorldPx(top_left_px, bottom_right_px).rawPoint()));
//            if(mouse_mode == QMapControl::MouseButtonMode::SelectLine)
//            {
//                // Line check.
//                area_px.reset(new QGraphicsLineItem(top_left_px.x(), top_left_px.y(), bottom_right_px.x(), bottom_right_px.y()));

//                // Set the line with a 'fuzzy-factor' around it using the pen.
//                /// @TODO expose the fuzzy factor as a setting.
//                const qreal fuzzy_factor_px = 5.0;
//                QPen line_pen(static_cast<QGraphicsLineItem*>(area_px.get())->pen());
//                line_pen.setWidthF(fuzzy_factor_px);
//                static_cast<QGraphicsLineItem*>(area_px.get())->setPen(line_pen);
//            }
//            else if(mouse_mode == QMapControl::MouseButtonMode::SelectEllipse)
//            {
//                // Ellipse check.
//                area_px.reset(new QGraphicsEllipseItem(RectWorldPx(top_left_px, bottom_right_px).rawPoint()));
//            }

            // Collection of selected geometries.
            std::map<std::string, std::vector<std::shared_ptr<Geometry>>> selected_geometries;

            // Loop through each layer to check geometries touches.
            for(const auto& layer : getLayers())
            {
                // Is it a geometry layer and is it visible?
                if(layer->getLayerType() == Layer::LayerType::LayerGeometry && layer->isVisible(m_current_zoom))
                {
                    // Loop through each geometry for the layer.
                    for(const auto& geometry : std::static_pointer_cast<LayerGeometry>(layer)->getGeometries(RectWorldCoord(top_left_coord, bottom_right_coord)))
                    {
                        // Does the geometry touch our area rect?
//                        if(geometry->touches(*(area_px.get()), m_current_zoom))
                        if(geometry->touches(geometry_to_compare_coord.get(), m_current_zoom))
                        {
                            // Add the geometry to the selected collection.
                            selected_geometries[layer->getName()].push_back(geometry);
                        }
                    }
                }
            }

            // Emit the geometries selected.
            emit geometriesSelected(selected_geometries);
        }

        // Schedule a repaint to remove any potential screen artifacts.
        QWidget::update();

        // Emit the released mouse event with the press and release mouse coordinates.
        emit mouseEventReleaseCoordinate(mouse_event, toPointWorldCoord(m_mouse_position_pressed_px), toPointWorldCoord(m_mouse_position_current_px));
    }

    void QMapControl::mouseDoubleClickEvent(QMouseEvent* mouse_event)
    {
        // Store the mouse location of the current mouse click.
        m_mouse_position_current_px = PointViewportPx(mouse_event->localPos().x(), mouse_event->localPos().y());

        // Emit the double click mouse event with the press and current mouse coordinate.
        emit mouseEventDoubleClickCoordinate(mouse_event, toPointWorldCoord(m_mouse_position_pressed_px), toPointWorldCoord(m_mouse_position_current_px));
    }

    void QMapControl::mouseMoveEvent(QMouseEvent* mouse_event)
    {
        // Update the current mouse position.
        m_mouse_position_current_px = PointViewportPx(mouse_event->localPos().x(), mouse_event->localPos().y());

        // Default mouse mode.
        QMapControl::MouseButtonMode mouse_mode = QMapControl::MouseButtonMode::None;

        // Left button still pressed?
        if(mouse_event->buttons() & Qt::MouseButton::LeftButton)
        {
            // Capture the left mouse mode.
            mouse_mode = m_mouse_left_mode;
        }
        // Right button still pressed?
        else if(mouse_event->buttons() & Qt::MouseButton::RightButton)
        {
            // Capture the right mouse mode.
            mouse_mode = m_mouse_right_mode;
        }

        // Are we in panning mode?
        if(mouse_mode == QMapControl::MouseButtonMode::Pan)
        {
            // Move the map by the offset between the last mouse pressed position and the current position.
            scrollView(m_mouse_position_pressed_px - m_mouse_position_current_px);

            // Update the left mouse pressed location.
            m_mouse_position_pressed_px = m_mouse_position_current_px;
        }

        // Schedule a repaint to remove any potential screen artifacts.
        QWidget::update();

        // Emit the moved mouse event with the press and current mouse coordinates.
        emit mouseEventMoveCoordinate(mouse_event, toPointWorldCoord(m_mouse_position_pressed_px), toPointWorldCoord(m_mouse_position_current_px));
    }

    void QMapControl::wheelEvent(QWheelEvent* wheel_event)
    {
        // Is the vertical angle delta positive?
        if(wheel_event->angleDelta().y() > 0)
        {
            // Check the current zoom is less than maximum zoom (as we change the location of the map focus point before we zoom in).
            if(m_current_zoom < m_zoom_maximum)
            {
                // Capture the current wheel point at the current zoom level.
                const PointViewportPx wheel_px(wheel_event->posF().x(), wheel_event->posF().y());
                const PointWorldCoord wheel_coord(toPointWorldCoord(wheel_px));
                const PointPx wheel_delta(mapFocusPointWorldPx() - toPointWorldPx(wheel_px));

                // Update the scaled offset with the current wheel_delta.
                /// @TODO should this add to the offset?
                m_primary_screen_scaled_offset = wheel_delta;

                // Zoom in.
                zoomIn();

                // Google-style zoom...
                setMapFocusPoint(projection::get().toPointWorldCoord(projection::get().toPointWorldPx(wheel_coord, m_current_zoom) + wheel_delta, m_current_zoom));

                // Tell parents we have accepted this events.
                wheel_event->accept();
            }
            else
            {
                // Tell parents we have ignored this events.
                wheel_event->ignore();
            }
        }
        else if(wheel_event->angleDelta().y() < 0)
        {
            // Check the current zoom is greater than minimum zoom (as we change the location of the map focus point before we zoom in).
            if(m_current_zoom > m_zoom_minimum)
            {
                // Capture the current wheel point at the current zoom level.
                const PointViewportPx wheel_px(wheel_event->posF().x(), wheel_event->posF().y());
                const PointWorldCoord wheel_coord(toPointWorldCoord(wheel_px));
                const PointPx wheel_delta(mapFocusPointWorldPx() - toPointWorldPx(wheel_px));

                // Update the scaled offset with the current wheel_delta.
                /// @TODO should this add to the offset?
                /// @TODO not sure if this is correct delta to apply on zoom out!
                m_primary_screen_scaled_offset = wheel_delta;

                // Zoom out.
                zoomOut();

                // Google-style zoom...
                setMapFocusPoint(projection::get().toPointWorldCoord(projection::get().toPointWorldPx(wheel_coord, m_current_zoom) + wheel_delta, m_current_zoom));

                // Tell parents we have accepted this events.
                wheel_event->accept();
            }
            else
            {
                // Tell parents we have ignored this events.
                wheel_event->ignore();
            }
        }
    }

    // Keyboard management.
    void QMapControl::keyPressEvent(QKeyEvent* key_event)
    {
        // Handle the cases we want.
        switch(key_event->key())
        {
            // Up arrow key.
            case Qt::Key_Up:
                scrollViewUp();
                break;

            // Down arrow key.
            case Qt::Key_Down:
                scrollViewDown();
                break;

            // Left arrow key.
            case Qt::Key_Left:
                scrollViewLeft();
                break;

            // Right arrow key.
            case Qt::Key_Right:
                scrollViewRight();
                break;

            // Zoom in.
            case Qt::Key_Plus:
                zoomIn();
                break;

            // Zoom out.
            case Qt::Key_Minus:
                zoomOut();
                break;

            // Else... pass the key press event to the QWidget to process.
            default:
                QWidget::keyPressEvent(key_event);
                break;
        }
    }

    // Drawing management.
    QPixmap QMapControl::getPrimaryScreen() const
    {
        // Return the primary screen (ie: what is currently being displayed).
        // Note: m_viewport_center_px is the same as (m_viewport_size_px / 2)
        return m_primary_screen.copy(QRect((m_viewport_center_px + mapFocusPointWorldPx() - m_primary_screen_map_focus_point_px).rawPoint().toPoint(), m_viewport_size_px.toSize()));
    }


    /// Public slots...
    // Zoom management.
    void QMapControl::zoomIn()
    {
        // Check the current zoom is less than the maximum zoom
        if(m_current_zoom < m_zoom_maximum)
        {
            // Cancel existing image loading.
            ImageManager::get().abortLoading();

            /// @TODO Could we cancel current layer drawing as well?

            // Is the primary screen scaled enabled?
            if(m_primary_screen_scaled_enabled)
            {
                // Paint the primary screen to the scaled screen with a scale.
                QPixmap new_primary_screen_scaled(m_viewport_size_px.toSize() * 2.0);
                new_primary_screen_scaled.fill(Qt::transparent);
                QPainter painter(&new_primary_screen_scaled);
                painter.scale(2, 2);
                painter.drawPixmap(PointWorldPx(0.0, 0.0).rawPoint(), getPrimaryScreen());

                // Store the new scaled primary screen.
                m_primary_screen_scaled = new_primary_screen_scaled;
            }

            // Reset the primary screen, as this is invalid.
            m_primary_screen.fill(Qt::transparent);

            // Increase the zoom!
            m_current_zoom++;

            // Force the primary screen to be redrawn.
            redrawPrimaryScreen(true);

            // Ensure the zoom controls are updated.
            updateControls();
        }
    }

    void QMapControl::zoomOut()
    {
        // Check the current zoom is greater than the minimum zoom.
        if(m_current_zoom > m_zoom_minimum)
        {
            // Cancel existing image loading.
            ImageManager::get().abortLoading();

            /// @TODO Could we cancel current layer drawing as well?

            // Is the primary screen scaled enabled?
            if(m_primary_screen_scaled_enabled)
            {
                // Paint the primary screen to the scaled screen with a scale.
                QPixmap new_primary_screen_scaled(m_viewport_size_px.toSize() * 2.0);
                new_primary_screen_scaled.fill(Qt::transparent);
                QPainter painter(&new_primary_screen_scaled);
                painter.scale(0.5, 0.5);
                painter.drawPixmap(PointWorldPx(m_viewport_size_px.width(), m_viewport_size_px.height()).rawPoint(), m_primary_screen);

                // Store the new scaled primary screen.
                m_primary_screen_scaled = new_primary_screen_scaled;
            }

            // Reset the primary screen, as this is invalid.
            m_primary_screen.fill(Qt::transparent);

            // Decrease the zoom!
            m_current_zoom--;

            // Force the primary screen to be redrawn.
            redrawPrimaryScreen(true);

            // Ensure the zoom controls are updated.
            updateControls();
        }
    }

    void QMapControl::setZoom(int zoom)
    {
        // Check the requested zoom is with the zoom range allowed.
        if(zoom < m_zoom_minimum)
        {
            // Default to minimum zoon instead.
            zoom = m_zoom_minimum;
        }
        else if(zoom > m_zoom_maximum)
        {
            // Default to the maximum zoom instead.
            zoom = m_zoom_maximum;
        }

        // Check the requested zoom isn't already the current zoom.
        if(m_current_zoom != zoom)
        {
            // Is the required zoom less than current zoom?
            if(m_current_zoom > zoom)
            {
                // Keep zooming out until zoom == current zoom.
                for(int i = m_current_zoom; i > zoom; i--)
                {
                    zoomOut();
                }
            }
            else
            {
                // Keep zooming in until zoom == current zoom.
                for(int i = m_current_zoom; i < zoom; i++)
                {
                    zoomIn();
                }
            }
        }
    }

    // Drawing management.
    void QMapControl::requestRedraw()
    {
        // Force the primary screen to be redrawn.
        redrawPrimaryScreen(true);
    }


    /// Private...
    // Map management.
    PointWorldPx QMapControl::toPointWorldPx(const PointViewportPx& click_point_px) const
    {
        // Return the actual position on the map (uses the current map focus point).
        return toPointWorldPx(click_point_px, mapFocusPointWorldPx());
    }

    PointWorldPx QMapControl::toPointWorldPx(const PointViewportPx& click_point_px, const PointWorldPx& map_focus_point_px) const
    {
        // Convert the mouse pixel position into the coordinate system required.
        /*
         * Notes:
         * click_point_px.x() and y() are the mouse pixel points in relation to the viewport (visible-part of the layer).
         * mapFocusPointPx()->x() and y() are the current focus pixel points in relation to the map control (whole layer).
         *
         * Explanation:
         *   0 1 2 3 4 5 6
         * 0 |-----------|  Outside box is Map Control (whole layer) and goes from 0,0 to 6,6
         * 1 | |---------|  Inside box is Viewport (visible-part of the layer) and goes from 0,0 to 4,4 (Center is 2,2)
         * 2 | |         |  Map Focus is 3,3 (ie: Viewport center on Map Control).
         * 3 | | X       |  X is at Viewport position 1,2
         * 4 | |         |
         * 5 | |         |  X at Map Control position = X is at Viewport position - Viewport Center + Map Focus
         * 6 |-|---------|  X at Map Control position = 1,2 - 2,2 + 3,3
         *                  X at Map Control position = 2,3
         */

        // Calculate the actual position on the map (click - viewport center = delta from map focus point).
        return map_focus_point_px + (click_point_px - m_viewport_center_px);
    }

    PointWorldCoord QMapControl::toPointWorldCoord(const PointViewportPx& click_point_px) const
    {
        // Return the point converted into the coordinates system (uses the current map focus point).
        return projection::get().toPointWorldCoord(toPointWorldPx(click_point_px, mapFocusPointWorldPx()), m_current_zoom);
    }

    PointWorldCoord QMapControl::toPointWorldCoord(const PointViewportPx& click_point_px, const PointWorldPx& map_focus_point_px) const
    {
        // Return the point converted into the coordinates system.
        return projection::get().toPointWorldCoord(toPointWorldPx(click_point_px, map_focus_point_px), m_current_zoom);
    }

    PointWorldPx QMapControl::mapFocusPointWorldPx() const
    {
        // Return the current map focus point in pixels.
        return projection::get().toPointWorldPx(m_map_focus_coord, m_current_zoom);
    }

    PointWorldCoord QMapControl::calculateMapFocusPoint(const std::vector<PointWorldCoord>& points_coord)
    {
        // Sum totals.
        double sum_longitudes = 0.0;
        double sum_latitudes = 0.0;

        // Loop through the coordiantes to sum the longitude and latitude's.
        for(const auto& coordinate : points_coord)
        {
            sum_longitudes += coordinate.longitude();
            sum_latitudes += coordinate.latitude();
        }

        // Return the calculated map focus point = mean of longitude and latitude.
        return PointWorldCoord(sum_longitudes / points_coord.size(), sum_latitudes / points_coord.size());
    }

    void QMapControl::scrollView(const PointPx& delta_px)
    {
        // Calculate the new map focus coord.
        const PointWorldCoord new_map_focus_coord(projection::get().toPointWorldCoord(mapFocusPointWorldPx() + delta_px, m_current_zoom));

        // If no limited viewport is set, or if the new map focus point coord is within the limited viewport...
        if(m_limited_viewport_rect_coord.rawRect().isNull() || (m_limited_viewport_rect_coord.rawRect().isValid() && m_limited_viewport_rect_coord.rawRect().contains(new_map_focus_coord.rawPoint())))
        {
            // Update map focus point with delta.
            setMapFocusPoint(new_map_focus_coord);
        }
    }

    // Zoom management.
    void QMapControl::checkZoom()
    {
        // Quick check to ensure min <= max.
        if(m_zoom_maximum > m_zoom_minimum)
        {
            // Swap the zooms.
            std::swap(m_zoom_minimum, m_zoom_maximum);
        }

        // Check the current zoom is not outside the zoom range.
        if(m_current_zoom < m_zoom_minimum)
        {
            // Current zoom is less than the minimum, set to the minimum.
            setZoom(m_zoom_minimum);
        }
        else if(m_current_zoom > m_zoom_maximum)
        {
            // Current zoom is greater than the maximum, set to the maximum.
            setZoom(m_zoom_maximum);
        }
    }

    void QMapControl::updateControls()
    {
        // Default values.
        const int margin = 10;
        const int slider_width = 25;
        const int slider_height = 100;

        // Left-aligned.
        int margin_left = margin;
        // Right-aligned required?
        if(m_zoom_control_align_left == false)
        {
            margin_left = m_viewport_size_px.width() - slider_width - margin;
        }

        // Set the geometries/size of the zoom controls.
        m_zoom_control_button_in.setGeometry(margin_left, margin, 24, 24);
        m_zoom_control_slider.setMinimum(m_zoom_minimum);
        m_zoom_control_slider.setMaximum(m_zoom_maximum);
        m_zoom_control_slider.setValue(m_current_zoom);
        m_zoom_control_slider.setGeometry(margin_left, 33, slider_width, slider_height);
        m_zoom_control_button_out.setGeometry(margin_left, slider_height + 31, 24, 24);

        // Set the default location of the progress indicator to the opposite side.
        if(m_zoom_control_align_left)
        {
            // Place the progress indicator on the left.
            m_progress_indicator.setGeometry(m_viewport_size_px.width() - slider_width - margin, margin, slider_width, slider_width);
        }
        else
        {
            // Place the progress indicator on the left.
            m_progress_indicator.setGeometry(margin, margin, slider_width, slider_width);
        }
    }

    // Drawing management.
    void QMapControl::paintEvent(QPaintEvent* paint_event)
    {
        // Call inherited QWidgets paint event first.
        QWidget::paintEvent(paint_event);

        // Create a painter for this QWidget to draw on.
        QPainter painter(this);

        // Ensure antialiasing is enabled (primitives and pixmaps).
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

        // Apply style sheet options to painter.
        QStyleOption style_options;
        style_options.initFrom(this);
        QWidget::style()->drawPrimitive(QStyle::PE_Widget, &style_options, &painter, this);

        // Draw the current primary screen to the widget.
        drawPrimaryScreen(&painter);

        // Draw a box around the edge of the viewport (useful for debugging).
        painter.drawRect(RectViewportPx(PointViewportPx(0.0, 0.0), PointViewportPx(m_viewport_size_px.width(), m_viewport_size_px.height())).rawRect());

        // Should we draw the scalebar?
        if(m_scalebar_enabled)
        {
            /// @todo This currently only shows the correct scale at the equator when in Spherical Mercator mode!

            // Default list of scalebar distances.
            QList<double> scalebar_distances;
            scalebar_distances << 5000000.0
                            << 2000000.0
                            << 1000000.0
                            << 1000000.0
                            << 1000000.0
                            << 100000.0
                            << 100000.0
                            << 50000.0
                            << 50000.0
                            << 10000.0
                            << 10000.0
                            << 10000.0
                            << 1000.0
                            << 1000.0
                            << 500.0
                            << 200.0
                            << 100.0
                            << 50.0
                            << 25.0;

            // Check we have a scalebar for the current zoom level.
            if(m_current_zoom >= 0 && scalebar_distances.size() > m_current_zoom)
            {
                // Calculate the length of the scalebar line in pixels.
                /// @TODO remove magic numbers!
                const double scalebar_line_length_px = scalebar_distances.at(m_current_zoom) / std::pow(2.0, 18 - m_current_zoom) / 0.597164;

                // Draw the scalebar line.
                painter.setPen(Qt::black);
                const PointViewportPx scale_line_start(10.0, m_viewport_size_px.height() - 20.0);
                const PointViewportPx scale_line_end(scalebar_line_length_px, m_viewport_size_px.height() - 20.0);
                painter.drawLine(scale_line_start.rawPoint(), scale_line_end.rawPoint());
                painter.drawLine(10.0, m_viewport_size_px.height() - 15.0, 10.0, m_viewport_size_px.height() - 25.0);
                painter.drawLine(scalebar_line_length_px, m_viewport_size_px.height() - 15.0, scalebar_line_length_px, m_viewport_size_px.height() - 25.0);

                // Default scalebar text in meters.
                QString scalebar_text(QVariant(scalebar_distances.at(m_current_zoom)).toString() + " m");

                // Over 1000m? Show as km instead.
                if(scalebar_distances.at(m_current_zoom) >= 1000.0)
                {
                    // Set the scalebar text in kilometers.
                    scalebar_text = QVariant(scalebar_distances.at(m_current_zoom) / 1000.0).toString() + " km";
                }

                // Draw the scalebar text.
                painter.drawText(PointViewportPx(scalebar_line_length_px + 10.0, m_viewport_size_px.height() - 15.0).rawPoint(), scalebar_text);
            }
        }

        // Should we draw the crosshairs?
        if(m_crosshairs_enabled)
        {
            // Draw the crosshair at the viewport center.
            // |
            painter.drawLine(m_viewport_center_px.x(), m_viewport_center_px.y() - 10.0,
                             m_viewport_center_px.x(), m_viewport_center_px.y() + 10.0);
            // -
            painter.drawLine(m_viewport_center_px.x() - 10.0, m_viewport_center_px.y(),
                             m_viewport_center_px.x() + 10.0, m_viewport_center_px.y());
        }

        // Is the mouse pressed?
        if(m_mouse_left_pressed || m_mouse_right_pressed)
        {
            // Default to the left mouse button options.
            MouseButtonMode mouse_mode = m_mouse_left_mode;
            bool mouse_origin_center = m_mouse_left_origin_center;

            // Has the right mouse button been pressed?
            if(m_mouse_right_pressed)
            {
                // Set to the right mouse button options.
                mouse_mode = m_mouse_right_mode;
                mouse_origin_center = m_mouse_right_origin_center;
            }

            // Capture the start and end points.
            PointViewportPx start_point_px = m_mouse_position_pressed_px;
            PointViewportPx end_point_px = m_mouse_position_current_px;

            // Draw at center of mouse pressed?
            if(mouse_origin_center)
            {
                // Draw the crosshair at the mouse start point.
                // |
                painter.drawLine(m_mouse_position_pressed_px.x(), m_mouse_position_pressed_px.y() - 1.0,
                                 m_mouse_position_pressed_px.x(), m_mouse_position_pressed_px.y() + 1.0);
                // -
                painter.drawLine(m_mouse_position_pressed_px.x() - 1.0, m_mouse_position_pressed_px.y(),
                                 m_mouse_position_pressed_px.x() + 1.0, m_mouse_position_pressed_px.y());

                // Update the start and end points.
                const PointPx mouse_diff_px(m_mouse_position_pressed_px - m_mouse_position_current_px);
                start_point_px = m_mouse_position_pressed_px - mouse_diff_px;
                end_point_px = m_mouse_position_pressed_px + mouse_diff_px;
            }

            // Save the current painter's state.
            painter.save();

            // Set the pen and brush colours.
            painter.setPen(QPen(QColor(66, 132, 253)));
            painter.setBrush(QBrush(QColor(66, 132, 253)));
            painter.setOpacity(0.4);

            // Is the mouse mode set to draw/pan/select a box.
            if(mouse_mode == QMapControl::MouseButtonMode::DrawBox || mouse_mode == QMapControl::MouseButtonMode::PanBox || mouse_mode == QMapControl::MouseButtonMode::SelectBox)
            {
                // Draw rect.
                painter.drawRect(RectViewportPx(start_point_px, end_point_px).rawRect());
            }
            // Is the mouse mode set to draw/pan/select a line.
            else if(mouse_mode == QMapControl::MouseButtonMode::DrawLine || mouse_mode == QMapControl::MouseButtonMode::PanLine || mouse_mode == QMapControl::MouseButtonMode::SelectLine)
            {
                // Capture the pen.
                QPen line_pen(painter.pen());

                /// @TODO expose the fuzzy factor as a setting.
                const qreal fuzzy_factor_px = 5.0;
                line_pen.setWidthF(fuzzy_factor_px);
                painter.setPen(line_pen);

                // Draw line.
                painter.drawLine(start_point_px.rawPoint(), end_point_px.rawPoint());
            }
            // Is the mouse mode set to draw/pan/select a ellipse.
            else if(mouse_mode == QMapControl::MouseButtonMode::DrawEllipse || mouse_mode == QMapControl::MouseButtonMode::PanEllipse || mouse_mode == QMapControl::MouseButtonMode::SelectEllipse)
            {
                // Draw ellipse from start to current mouse point.
                painter.drawEllipse(RectViewportPx(start_point_px, end_point_px).rawRect());
            }

            // Restore the painter's state.
            painter.restore();
        }
    }

    void QMapControl::drawPrimaryScreen(QPainter* painter) const
    {
        // Is the primary screen scaled enabled?
        if(m_primary_screen_scaled_enabled)
        {
            // Draw the current scaled primary screem image to the pixmap with wheel event offset.
            // Note: m_viewport_center_px is the same as (m_viewport_size_px / 2)
            painter->drawPixmap(-(m_viewport_center_px + mapFocusPointWorldPx() - m_primary_screen_map_focus_point_px - m_primary_screen_scaled_offset).rawPoint(), m_primary_screen_scaled);
        }

        // Draws the primary screen image to the pixmap.
        // Note: m_viewport_center_px is the same as (m_viewport_size_px / 2)
        painter->drawPixmap(-(m_viewport_center_px + mapFocusPointWorldPx() - m_primary_screen_map_focus_point_px).rawPoint(), m_primary_screen);
    }

    bool QMapControl::checkBackbuffer() const
    {
        // Default return success.
        bool return_redraw_required(false);

        // Calculate required viewport rect.
        const RectWorldPx required_viewport_rect_px(toPointWorldPx(PointViewportPx(0.0, 0.0)), toPointWorldPx(PointViewportPx(m_viewport_size_px.width(), m_viewport_size_px.height())));

        // Does the primary screen's backbuffer rect contain the requried viewport rect?
        if(m_primary_screen_backbuffer_rect_px.rawRect().contains(required_viewport_rect_px.rawRect()) == false)
        {
            // Backbuffer rect does not contain the required viewport rect, therefore we need to redraw the backbuffer.
            return_redraw_required = true;
        }

        // Return success.
        return return_redraw_required;
    }

    void QMapControl::redrawPrimaryScreen(const bool& force_redraw)
    {
        // If we are forced to redraw, or current backbuffer does not cover the required viewport.
        if(force_redraw || checkBackbuffer())
        {
            // Schedule the redraw in a background thread.
            QtConcurrent::run(this, &QMapControl::redrawBackbuffer);
        }

        // Loop through the layers to update the Geometries that have widgets as well.
        for(const auto& layer : getLayers())
        {
            // Is it a geometry layer?
            if(layer->getLayerType() == Layer::LayerType::LayerGeometry)
            {
                // Tell the layer to move its geometry widgets.
                std::static_pointer_cast<LayerGeometry>(layer)->moveGeometryWidgets(mapFocusPointWorldPx() - m_viewport_center_px, m_current_zoom);
            }
        }

        // Schedule a repaint.
        QWidget::update();
    }

    void QMapControl::redrawBackbuffer()
    {
        // Get access to the backbuffer's queue mutex.
        if(m_backbuffer_queued_mutex.try_lock())
        {
            // Get access to the backbuffer mutex.
            std::lock_guard<std::mutex> locker(m_backbuffer_mutex);

            // Release the backbuffer queue mutex, so someone else can wait while we redraw.
            m_backbuffer_queued_mutex.unlock();

            // Start the progress indicator as we are going to start the redrawing process
            QTimer::singleShot(0, &m_progress_indicator, SLOT(startAnimation()));

            // Generate a new backbuffer (2 x viewport size to allow for panning backbuffer).
            QImage image_backbuffer(m_viewport_size_px.toSize() * 2, QImage::Format_ARGB32);

            // Clear the backbuffer.
            image_backbuffer.fill(Qt::transparent);

            // Create a painter for the backbuffer.
            QPainter painter_back_buffer(&image_backbuffer);

            // Capture the map focus point we are going to use for this backbuffer.
            PointWorldPx backbuffer_map_focus_px(mapFocusPointWorldPx());

            // Calculate the new backbuffer rect (based on the saved backbuffer map focus point).
            // Note: m_viewport_center_px is the same as (m_viewport_size_px / 2)
            const PointPx viewport_offset_px(m_viewport_size_px.width() / 2.0, m_viewport_size_px.height() / 2.0);
            const RectWorldPx backbuffer_rect_px(toPointWorldPx(PointViewportPx(0, 0) - viewport_offset_px, backbuffer_map_focus_px), toPointWorldPx(PointViewportPx(m_viewport_size_px.width(), m_viewport_size_px.height()) + viewport_offset_px, backbuffer_map_focus_px));

            // Translate to the backbuffer top/left point.
            painter_back_buffer.translate(-backbuffer_rect_px.topLeftPx().rawPoint());

            // Loop through each layer and draw it to the backbuffer.
            for(const auto& layer : getLayers())
            {
                // Draw the layer to the backbuffer.
                layer->draw(painter_back_buffer, backbuffer_rect_px, m_current_zoom);
            }

            // Undo the backbuffer top/left point translation.
            painter_back_buffer.translate(backbuffer_rect_px.topLeftPx().rawPoint());

            // Inform the main thread that we have a new backbuffer.
            emit updatedBackBuffer(QPixmap::fromImage(image_backbuffer), backbuffer_rect_px, backbuffer_map_focus_px);

            // Stop the progress indicator as we have finished the redrawing process.
            QTimer::singleShot(0, &m_progress_indicator, SLOT(stopAnimation()));
        }
    }


    /// Private slots...
    // Geometry management.
    void QMapControl::geometryPositionChanged(const Geometry* geometry)
    {
        // Is it a point geometry?
        if(geometry->geometryType() == Geometry::GeometryType::GeometryPoint)
        {
            // Calculate the delta between the current map focus and the new geometry position.
            const PointWorldPx start_px(projection::get().toPointWorldPx(m_map_focus_coord, m_current_zoom));
            const PointWorldPx dest_px(projection::get().toPointWorldPx(static_cast<const GeometryPoint*>(geometry)->coord(), m_current_zoom));
            const PointPx delta_px(dest_px - start_px);

            // Scroll the view
            scrollView(delta_px);

            // Force the primary screen to be redrawn.
            redrawPrimaryScreen(true);
        }
    }

    // Map management.
    void QMapControl::animatedTick()
    {
        // Do we still have steps to complete?
        if(m_animated_steps > 0)
        {
            // Calculate the delta between the current map focus and the target animated map focus point.
            const PointWorldPx start_px(projection::get().toPointWorldPx(m_map_focus_coord, m_current_zoom));
            const PointWorldPx dest_px(projection::get().toPointWorldPx(m_animated_map_focus_point, m_current_zoom));
            const PointPx delta_px(dest_px - start_px);

            // Scroll to the next point in the step.
            scrollView(delta_px / m_animated_steps);

            // Reduce the number of steps left.
            m_animated_steps--;

            // Schedule the next animation tick.
            QTimer::singleShot(m_animated_interval.count(), this, SLOT(animatedTick()));
        }
        else
        {
            // We have finished our animation, unlock the mutex.
            m_animated_mutex.unlock();
        }
    }

    // Drawing management.
    void QMapControl::loadingFinished()
    {
        // Remove the scaled image, as all new images have been loaded.
        m_primary_screen_scaled.fill(Qt::transparent);

        // Reset the scaled image offset.
        m_primary_screen_scaled_offset = PointPx(0.0, 0.0);

        // Request the primary screen to be redrawn.
        redrawPrimaryScreen();
    }

    void QMapControl::updatePrimaryScreen(QPixmap backbuffer_pixmap, RectWorldPx backbuffer_rect_px, PointWorldPx backbuffer_map_focus_px)
    {
        // Backbuffer image is ready, save it to the primary screen.
        m_primary_screen = backbuffer_pixmap;

        // Update the backbuffer rect that is available.
        m_primary_screen_backbuffer_rect_px = backbuffer_rect_px;

        // Update the primary screen's map focus point.
        m_primary_screen_map_focus_point_px = backbuffer_map_focus_px;

        // Schedule a repaint.
        QWidget::update();
    }
}
