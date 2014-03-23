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

#include "Layer.h"

// STL includes.
#include <cmath>

// Local includes.
#include "GeometryPoint.h"
#include "GeometryLineString.h"
#include "GeometryPolygon.h"
#include "ImageManager.h"
#include "Projection.h"

namespace qmapcontrol
{
    Layer::Layer(const std::string& name, const int& zoom_minimum, const int& zoom_maximum, QObject* parent)
        : QObject(parent),
          m_geometries(50, QRectF(QPointF(-180.0, 90.0), QPointF(180.0, -90.0))),
          m_visible(true),
          m_mouse_events_enabled(true),
          m_name(name),
          m_zoom_minimum(zoom_minimum),
          m_zoom_maximum(zoom_maximum)
    {

    }

    std::string Layer::getName() const
    {
        // Return the layer's name.
        return m_name;
    }

    QVariant Layer::getMetadata(const std::string& key) const
    {
        // Default return value.
        QVariant return_value;

        // Find the key.
        const auto find_itr = m_metadata.find(key);
        if(find_itr != m_metadata.end())
        {
            // Fetch the value.
            return_value = find_itr->second;
        }

        // Return the value.
        return return_value;
    }

    void Layer::setMetadata(const std::string& key, const QVariant& value)
    {
        // Set the meta-data.
        m_metadata[key] = value;
    }

    const std::vector< std::shared_ptr<MapAdapter> > Layer::getMapAdapters() const
    {
        // Gain a read lock to protect the map adapters container.
        QReadLocker locker(&m_mapadapters_mutex);

        // Return the list of map adapters.
        return m_mapadapters;
    }

    void Layer::addMapAdapter(const std::shared_ptr<MapAdapter>& map_adapter)
    {
        // Scope the locker to ensure the mutex is release as soon as possible.
        {
            // Gain a write lock to protect the map adapters container.
            QWriteLocker locker(&m_mapadapters_mutex);

            // Add the map adapter.
            m_mapadapters.push_back(map_adapter);
        }

        // Emit to redraw layer.
        emit requestRedraw();
    }

    bool Layer::isVisible(const int& controller_zoom) const
    {
        // Default visibility.
        bool visibility(m_visible);

        // Check whether the controller zoom is within range.
        if(m_zoom_minimum > controller_zoom || m_zoom_maximum < controller_zoom)
        {
            // Outside of supported zoom levels.
            visibility = false;
        }

        // Return the visibility.
        return visibility;
    }

    void Layer::setVisible(const bool& visible)
    {
        // Set the visibility.
        m_visible = visible;

        // Emit to redraw layer.
        emit requestRedraw();
    }

    void Layer::enableMouseEvents(const bool& enable)
    {
        // Set whether to enable mouse events.
        m_mouse_events_enabled = enable;
    }

    const std::set< std::shared_ptr<Geometry> > Layer::getGeometries(const QRectF& range_coord) const
    {
        // Gain a read lock to protect the geometries container.
        QReadLocker locker(&m_geometries_mutex);

        // The geometries container to return.
        std::set< std::shared_ptr<Geometry> > return_geometries;

        // Populate the geometries container.
        m_geometries.query(return_geometries, range_coord);

        // Return the list of geometries.
        return return_geometries;
    }

    const std::set< std::shared_ptr<GeometryPointWidget> > Layer::getGeometryWidgets() const
    {
        // Gain a read lock to protect the geometry widgets container.
        QReadLocker locker(&m_geometry_widgets_mutex);

        // Return the list of geometry widgets.
        return m_geometry_widgets;
    }

    bool Layer::containsGeometry(const std::shared_ptr<Geometry>& geometry, const int& controller_zoom) const
    {
        // Default return answer.
        bool contains_geometry(false);

        // We only return true for valid geomtries to start with.
        if(geometry != nullptr)
        {
            // Is it a geometry widget?
            if(geometry->getGeometryType() == Geometry::GeometryType::GeometryPointWidget)
            {
                // Fetch a copy of the current geometry widgets.
                const auto geometry_widgets = getGeometryWidgets();

                // Does the list contain the geometry widget?
                contains_geometry = (std::find(geometry_widgets.begin(), geometry_widgets.end(), geometry) != geometry_widgets.end());
            }
            // Else it must be a Geometry object.
            else
            {
                // Fetch a copy of the current geometries.
                const auto geometries = getGeometries(geometry->boundingBox(controller_zoom));

                // Does the list contain the geometry?
                contains_geometry = (std::find(geometries.begin(), geometries.end(), geometry) != geometries.end());
            }
        }

        // Return the success.
        return contains_geometry;
    }

    void Layer::addGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw)
    {
        // Check the geometry is valid.
        if(geometry == nullptr)
        {
            qDebug() << "Cannot added invalid geometry!";
        }
        else
        {
            // Handle the different geometry types.
            switch(geometry->getGeometryType())
            {
                // Is it a GeometryPoint.
                case Geometry::GeometryType::GeometryPoint:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Add the geometry.
                    m_geometries.insert(std::static_pointer_cast<GeometryPoint>(geometry)->coordinate(), geometry);

                    // Finished.
                    break;
                }

                // Is it a GeometryPointWidget.
                case Geometry::GeometryType::GeometryPointWidget:
                {
                    // Gain a write lock to protect the geometry widget container.
                    QWriteLocker locker(&m_geometry_widgets_mutex);

                    // Add the geometry widget.
                    m_geometry_widgets.insert(std::static_pointer_cast<GeometryPointWidget>(geometry));

                    // Finished.
                    break;
                }

                // Is it a GeometryLineString.
                case Geometry::GeometryType::GeometryLineString:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Loop through each GeometryLineString point and add it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryLineString>(geometry)->points())
                    {
                        // Add the geometry.
                        m_geometries.insert(point->coordinate(), geometry);
                    }

                    // Finished.
                    break;
                }

                // Is it a GeometryPolygon.
                case Geometry::GeometryType::GeometryPolygon:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Loop through each GeometryPolygon point and add it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryPolygon>(geometry)->points())
                    {
                        // Add the geometry.
                        m_geometries.insert(point->coordinate(), geometry);
                    }

                    // Finished.
                    break;
                }
            }

            // Should we redraw?
            if(disable_redraw == false)
            {
                // Emit to redraw layer.
                emit requestRedraw();
            }

            // Geometries can request a redraw, e.g. when its position has been changed.
            // Connect the redraw signal to promulgate up as required.
            QObject::connect(geometry.get(), &Geometry::requestRedraw, this, &Layer::requestRedraw);
        }
    }

    void Layer::removeGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw)
    {
        // Check the geometry is valid.
        if(geometry == nullptr)
        {
            qDebug() << "Cannot remove invalid geometry!";
        }
        else
        {
            // Handle the different geometry types.
            switch(geometry->getGeometryType())
            {
                // Is it a GeometryPoint.
                case Geometry::GeometryType::GeometryPoint:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Remove the geometry from the list.
                    m_geometries.erase(std::static_pointer_cast<GeometryPoint>(geometry)->coordinate(), geometry);
                }

                // Is it a GeometryPointWidget.
                case Geometry::GeometryType::GeometryPointWidget:
                {
                    // Gain a write lock to protect the geometry widgets container.
                    QWriteLocker locker(&m_geometry_widgets_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Find the object in the container.
                    const auto itr_find = m_geometry_widgets.find(std::static_pointer_cast<GeometryPointWidget>(geometry));
                    if(itr_find != m_geometry_widgets.end())
                    {
                        // Remove the geometry from the list.
                        m_geometry_widgets.erase(itr_find);
                    }

                    // Finished.
                    break;
                }

                // Is it a GeometryLineString.
                case Geometry::GeometryType::GeometryLineString:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Loop through each GeometryLineString point and remove it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryLineString>(geometry)->points())
                    {
                        // Remove the geometry.
                        m_geometries.erase(point->coordinate(), geometry);
                    }

                    // Finished.
                    break;
                }

                // Is it a GeometryPolygon.
                case Geometry::GeometryType::GeometryPolygon:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Loop through each GeometryPolygon point and remove it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryPolygon>(geometry)->points())
                    {
                        // Remove the geometry.
                        m_geometries.erase(point->coordinate(), geometry);
                    }

                    // Finished.
                    break;
                }
            }

            // Should we redraw?
            if(disable_redraw == false)
            {
                // Emit to redraw layer.
                emit requestRedraw();
            }
        }
    }

    void Layer::clearGeometries()
    {
        // Gain a write lock to protect the geometries and geometry widgets container.
        QWriteLocker locker(&m_geometries_mutex);
        QWriteLocker locker_widgets(&m_geometry_widgets_mutex);

        // Remove all geometries from the list.
        m_geometries.clear();
        m_geometry_widgets.clear();
    }

    void Layer::mousePressEvent(const QMouseEvent* mouse_event, const QPointF& mouse_point_px, const int& controller_zoom)
    {
        // Are mouse events enabled, is the layer visible and is it a mouse press event?
        if(m_mouse_events_enabled && isVisible(controller_zoom) && mouse_event->type() == QEvent::MouseButtonPress)
        {
            // Is this a left-click event?
            if(mouse_event->button() == Qt::LeftButton)
            {
                /// @TODO expose the fuzzy factor as a setting.
                const qreal fuzzy_factor_px = 5.0;

                // Calculate a rect around the mouse point with a 'fuzzy-factor' around it in pixels.
                const QGraphicsRectItem mouse_area_px(mouse_point_px.x() - fuzzy_factor_px, mouse_point_px.y() - fuzzy_factor_px, (fuzzy_factor_px * 2.0), (fuzzy_factor_px * 2.0));

                // Calculate a rect around the mouse point with a 'fuzzy-factor' around it in coordinates.
                const QRectF mouse_area_coord(projection::get().toCoordinatePoint(mouse_area_px.rect().topLeft(), controller_zoom), projection::get().toCoordinatePoint(mouse_area_px.rect().bottomRight(), controller_zoom));

                // Check each geometry to see it is contained in our touch area.
                for(const auto& geometry : getGeometries(mouse_area_coord))
                {
                    // Does it touch? (Will emit if it does).
                    if(geometry->touches(mouse_area_px, controller_zoom))
                    {
                        // Emit that the geometry has been clicked.
                        emit geometryClicked(geometry.get());
                    }
                }
            }
        }
    }

    void Layer::draw(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Check the layer is visible.
        if(isVisible(controller_zoom))
        {
            // Draw map adapters.
            drawMapAdapters(painter, backbuffer_rect_px, controller_zoom);

            // Draw geometries.
            drawGeometries(painter, backbuffer_rect_px, controller_zoom);
        }
    }

    void Layer::moveGeometryWidgets(const QPointF& offset_px, const int& controller_zoom) const
    {
        // Check the layer is visible.
        if(isVisible(controller_zoom))
        {
            // Loop through each geometry widget.
            for(const auto& geometry : getGeometryWidgets())
            {
                // Set the widgets new location.
                geometry->moveWidget(offset_px, controller_zoom);
            }
        }
    }

    void Layer::drawMapAdapters(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) const
    {
        // For each map in the layer.
        for(const auto& map_adapter : getMapAdapters())
        {
            // Check we have a base url to fetch map!
            if(map_adapter->getBaseUrl().isEmpty())
            {
                // We cannot fetch map tiles as no base url set!
                qDebug() << "Map adapater base url is empty!";
            }
            else
            {
                // The current tile size.
                const QSizeF tile_size_px(ImageManager::get().tileSizePx(), ImageManager::get().tileSizePx());

                // Calculate the tiles to draw.
                const int furthest_tile_left = std::floor(backbuffer_rect_px.left() / tile_size_px.width());
                const int furthest_tile_top = std::floor(backbuffer_rect_px.top() / tile_size_px.height());
                const int furthest_tile_right = std::floor(backbuffer_rect_px.right() / tile_size_px.width());
                const int furthest_tile_bottom = std::floor(backbuffer_rect_px.bottom() / tile_size_px.height());

                // Loop through the tiles to draw (left to right).
                for(int i = furthest_tile_left; i <= furthest_tile_right; ++i)
                {
                    // Loop through the tiles to draw (top to bottom).
                    for(int j = furthest_tile_top; j <= furthest_tile_bottom; ++j)
                    {
                        // Check the tile is valid.
                        if(map_adapter->isTileValid(i, j, controller_zoom))
                        {
                            // Calculate the tile rect.
                            const QRectF tile_rect_px(QPointF(i * tile_size_px.width(), j * tile_size_px.height()), tile_size_px);

                            // Draw the tile.
                            painter->drawPixmap(tile_rect_px.topLeft(), ImageManager::get().getImage(map_adapter->tileQuery(i, j, controller_zoom)));
                        }
                    }
                }

                // Prefetch the next set of rows/column tiles (ready for when the user starts panning).
                const int prefetch_tile_left = furthest_tile_left - 1;
                const int prefetch_tile_top = furthest_tile_top - 1;
                const int prefetch_tile_right = furthest_tile_right + 1;
                const int prefetch_tile_bottom = furthest_tile_bottom + 1;

                // Fetch the top/bottom rows.
                for (int i = prefetch_tile_left; i <= prefetch_tile_right; ++i)
                {
                    // Top row - check the tile is valid.
                    if(map_adapter->isTileValid(i, prefetch_tile_top, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(map_adapter->tileQuery(i, prefetch_tile_top, controller_zoom));
                    }

                    // Bottom row - check the tile is valid.
                    if(map_adapter->isTileValid(i, prefetch_tile_bottom, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(map_adapter->tileQuery(i, prefetch_tile_bottom, controller_zoom));
                    }
                }

                // Fetch the left/right columns.
                for (int j = prefetch_tile_top; j <= prefetch_tile_bottom; ++j)
                {
                    // Left column - check the tile is valid.
                    if(map_adapter->isTileValid(prefetch_tile_left, j, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(map_adapter->tileQuery(prefetch_tile_left, j, controller_zoom));
                    }

                    // Right column - check the tile is valid.
                    if(map_adapter->isTileValid(prefetch_tile_right, j, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(map_adapter->tileQuery(prefetch_tile_right, j, controller_zoom));
                    }
                }
            }
        }
    }

    void Layer::drawGeometries(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Save the current painter's state.
        painter->save();

        // Calculate the range (convert backbuffer rect to coordinates).
        const QRectF range(projection::get().toCoordinatePoint(backbuffer_rect_px.topLeft(), controller_zoom), projection::get().toCoordinatePoint(backbuffer_rect_px.bottomRight(), controller_zoom));

        // Loop through each geometry and draw it.
        for(const auto& geometry : getGeometries(range))
        {
            // Draw the geometry (this will not move widgets).
            geometry->draw(painter, backbuffer_rect_px, controller_zoom);
        }

        // Restore the painter's state.
        painter->restore();
    }
}
