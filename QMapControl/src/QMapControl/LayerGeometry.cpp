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

#include "LayerGeometry.h"

// Local includes.
#include "GeometryPoint.h"
#include "GeometryLineString.h"
#include "GeometryPolygon.h"
#include "Projection.h"

namespace qmapcontrol
{
    LayerGeometry::LayerGeometry(const std::string& name, const int& zoom_minimum, const int& zoom_maximum, QObject* parent)
        : Layer(LayerType::LayerGeometry, name, zoom_minimum, zoom_maximum, parent),
          m_geometries(50, RectWorldCoord(PointWorldCoord(-180.0, 90.0), PointWorldCoord(180.0, -90.0))),
          mFuzzyFactorPx(5.0)
    {

    }

    const std::set< std::shared_ptr<Geometry> > LayerGeometry::getGeometries(const RectWorldCoord& range_coord) const
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

    const std::set< std::shared_ptr<GeometryWidget> > LayerGeometry::getGeometryWidgets() const
    {
        // Gain a read lock to protect the geometry widgets container.
        QReadLocker locker(&m_geometry_widgets_mutex);

        // Return the list of geometry widgets.
        return m_geometry_widgets;
    }

    bool LayerGeometry::containsGeometry(const std::shared_ptr<Geometry>& geometry, const int& controller_zoom) const
    {
        // Default return answer.
        bool contains_geometry(false);

        // We only return true for valid geomtries to start with.
        if(geometry != nullptr)
        {
            // Is it a geometry widget?
            if(geometry->geometryType() == Geometry::GeometryType::GeometryWidget)
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

    void LayerGeometry::addGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw)
    {
        // Check the geometry is valid.
        if(geometry != nullptr)
        {
            geometry->mLayer = this;
            // Handle the different geometry types.
            switch(geometry->geometryType())
            {
                // Is it a GeometryPoint.
                case Geometry::GeometryType::GeometryPoint:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Add the geometry.
                    m_geometries.insert(std::static_pointer_cast<GeometryPoint>(geometry)->coord(), geometry);

                    // Finished.
                    break;
                }

                // Is it a GeometryPointWidget.
                case Geometry::GeometryType::GeometryWidget:
                {
                    // Gain a write lock to protect the geometry widget container.
                    QWriteLocker locker(&m_geometry_widgets_mutex);

                    // Add the geometry widget.
                    m_geometry_widgets.insert(std::static_pointer_cast<GeometryWidget>(geometry));

                    // Finished.
                    break;
                }

                /// @todo move linestring and polygon into different container type.
                // Is it a GeometryLineString.
                case Geometry::GeometryType::GeometryLineString:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Loop through each GeometryLineString point and add it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryLineString>(geometry)->points())
                    {
                        // Add the geometry.
                        m_geometries.insert(point, geometry);
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
                        m_geometries.insert(point, geometry);
                    }

                    // Finished.
                    break;
                }
            }

            geometry->onAddedToLayer(this);

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

    void LayerGeometry::removeGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw)
    {
        // Check the geometry is valid.
        if(geometry != nullptr)
        {
            // Handle the different geometry types.
            switch(geometry->geometryType())
            {
                // Is it a GeometryPoint.
                case Geometry::GeometryType::GeometryPoint:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Remove the geometry from the list.
                    m_geometries.erase(std::static_pointer_cast<GeometryPoint>(geometry)->coord(), geometry);
                }

                // Is it a GeometryPointWidget.
                case Geometry::GeometryType::GeometryWidget:
                {
                    // Gain a write lock to protect the geometry widgets container.
                    QWriteLocker locker(&m_geometry_widgets_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Find the object in the container.
                    const auto itr_find = m_geometry_widgets.find(std::static_pointer_cast<GeometryWidget>(geometry));
                    if(itr_find != m_geometry_widgets.end())
                    {
                        // Remove the geometry from the list.
                        m_geometry_widgets.erase(itr_find);
                    }

                    // Finished.
                    break;
                }

                /// @todo move linestring and polygon into different container type.
                // Is it a GeometryLineString.
                case Geometry::GeometryType::GeometryLineString:
                {
                    // Gain a write lock to protect the geometries container.
                    QWriteLocker locker(&m_geometries_mutex);

                    // Disconnect any signals that were previously connected.
                    QObject::disconnect(geometry.get(), 0, this, 0);

                    // Loop through each GeometryLineString point and remove it to the container.
                    for(const auto point : std::static_pointer_cast<GeometryLineString>(geometry)->quadtree_points())
                    {
                        // Remove the geometry.
                        m_geometries.erase(point, geometry);
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
                        m_geometries.erase(point, geometry);
                    }

                    // Finished.
                    break;
                }
            }

            geometry->onRemovedFromLayer();

            // Should we redraw?
            if(disable_redraw == false)
            {
                // Emit to redraw layer.
                emit requestRedraw();
            }
        }
    }

    void LayerGeometry::clearGeometries()
    {
        // Gain a write lock to protect the geometries and geometry widgets container.
        QWriteLocker locker(&m_geometries_mutex);
        QWriteLocker locker_widgets(&m_geometry_widgets_mutex);

        // Remove all geometries from the list.
        m_geometries.clear();
        m_geometry_widgets.clear();
    }

    bool LayerGeometry::mousePressEvent(const QMouseEvent* mouse_event, const PointWorldCoord& mouse_point_coord, const int& controller_zoom) const
    {
        // Are mouse events enabled, is the layer visible and is it a mouse press event?
        if(isMouseEventsEnabled() && isVisible(controller_zoom) && mouse_event->type() == QEvent::MouseButtonPress)
        {
            // Is this a left-click event?
            if(mouse_event->button() == Qt::LeftButton)
            {
                // Calculate the mouse press world point in pixels.
                const PointWorldPx mouse_point_px(projection::get().toPointWorldPx(mouse_point_coord, controller_zoom));

                // Calculate a rect around the mouse point with a 'fuzzy-factor' around it in pixels.
                const RectWorldPx mouse_rect_px(PointWorldPx(mouse_point_px.x() - mFuzzyFactorPx, mouse_point_px.y() - mFuzzyFactorPx), PointWorldPx(mouse_point_px.x() + mFuzzyFactorPx, mouse_point_px.y() + mFuzzyFactorPx));

                // Calculate a rect around the mouse point with a 'fuzzy-factor' around it in coordinates.
                const RectWorldCoord mouse_rect_coord(projection::get().toPointWorldCoord(mouse_rect_px.topLeftPx(), controller_zoom), projection::get().toPointWorldCoord(mouse_rect_px.bottomRightPx(), controller_zoom));

                // Create a QGraphicsRectItem to perform touches check, as required.
                const GeometryPolygon touches_rect_coord({ mouse_rect_coord.topLeftCoord(), mouse_rect_coord.bottomRightCoord() });

                // Check each geometry to see it is contained in our touch area.
                for(const auto& geometry : getGeometries(mouse_rect_coord))
                {
                    // Does it touch? (Will emit if it does).
                    if(geometry->touches(&touches_rect_coord, controller_zoom))
                    {
                        // Emit that the geometry has been clicked.
                        emit geometryClicked(geometry.get());
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void LayerGeometry::draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Check the layer is visible.
        if(isVisible(controller_zoom))
        {
            // Calculate the world coordinates.
            const RectWorldCoord backbuffer_rect_coord(projection::get().toPointWorldCoord(backbuffer_rect_px.topLeftPx(), controller_zoom), projection::get().toPointWorldCoord(backbuffer_rect_px.bottomRightPx(), controller_zoom));

            // Save the current painter's state.
            painter.save();

            // Loop through each geometry and draw it.
            for(const auto& geometry : getGeometries(backbuffer_rect_coord))
            {
                // Draw the geometry (this will not move widgets).
                geometry->draw(painter, backbuffer_rect_coord, controller_zoom);
            }

            // Restore the painter's state.
            painter.restore();
        }
    }

    void LayerGeometry::moveGeometryWidgets(const PointPx& offset_px, const int& controller_zoom) const
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
    qreal LayerGeometry::getFuzzyFactorPx() const
    {
        return mFuzzyFactorPx;
    }

    void LayerGeometry::setFuzzyFactorPx(const qreal &value)
    {
        mFuzzyFactorPx = value;
    }


}
