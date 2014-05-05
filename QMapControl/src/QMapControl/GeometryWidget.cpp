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

#include "GeometryWidget.h"

// STL includes.
#include <cmath>

// Local includes.
#include "GeometryPolygon.h"
#include "Projection.h"

namespace qmapcontrol
{
    GeometryWidget::GeometryWidget(const qreal& longitude, const qreal& latitude, QWidget* widget, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryWidget, zoom_minimum, zoom_maximum),
          m_point_coord(PointWorldCoord(longitude, latitude)),
          m_widget(widget),
          m_alignment_type(AlignmentType::Middle),
          m_base_zoom(-1),
          m_base_size_px(0.0, 0.0),
          m_draw_minimum_px(-1.0, -1.0),
          m_draw_maximum_px(-1.0, -1.0)
    {
        // Do we have a widget?
        if(m_widget != nullptr)
        {
            // Set the visibility of the widget as well.
            m_widget->setVisible(true);
        }
    }

    GeometryWidget::GeometryWidget(const PointWorldCoord& point_coord, QWidget* widget, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryWidget, zoom_minimum, zoom_maximum),
          m_point_coord(point_coord),
          m_widget(widget),
          m_alignment_type(AlignmentType::Middle),
          m_base_zoom(-1),
          m_base_size_px(0.0, 0.0),
          m_draw_minimum_px(-1.0, -1.0),
          m_draw_maximum_px(-1.0, -1.0)
    {
        // Do we have a widget?
        if(m_widget != nullptr)
        {
            // Set the visibility of the widget as well.
            m_widget->setVisible(true);
        }
    }

    PointWorldCoord GeometryWidget::coord() const
    {
        // Return the longitude/latitude coordinate (x/y).
        return m_point_coord;
    }

    void GeometryWidget::setCoord(const PointWorldCoord& point)
    {
        // Has the point change.
        if(m_point_coord != point)
        {
            // Set the new point.
            m_point_coord = point;

            // Emit that we need to redraw to display this change.
            emit requestRedraw();

            // Emit that the position has changed.
            emit positionChanged(this);
        }
    }

    QWidget* GeometryWidget::getWidget() const
    {
        // Return the widget.
        return m_widget;
    }

    void GeometryWidget::setVisible(const bool& enabled)
    {
        // Do we have a widget?
        if(m_widget != nullptr)
        {
            // Set the visibility of the widget as well.
            m_widget->setVisible(enabled);
        }

        // Set the visibility internally.
        Geometry::setVisible(enabled);
    }

    void GeometryWidget::setAlignmentType(const AlignmentType& alignment_type)
    {
        // Set the alignment type.
        m_alignment_type = alignment_type;
    }

    void GeometryWidget::setBaseZoom(const int& zoom)
    {
        // Set the base zoom level.
        m_base_zoom = zoom;
    }

    void GeometryWidget::setDrawMinimumPx(const QSizeF& size_px)
    {
        // Set the minimum size.
        m_draw_minimum_px = size_px;
    }

    void GeometryWidget::setDrawMaximumPx(const QSizeF& size_px)
    {
        // Set the maximum size.
        m_draw_maximum_px = size_px;
    }

    void GeometryWidget::moveWidget(const PointPx& offset_px, const int& controller_zoom)
    {
        // Check the geometry is visible and a widget exists.
        if(isVisible(controller_zoom) && m_widget != nullptr)
        {
            /// @todo move where offset is applied to the setGeometry function!
            // Translate the point into the current world pixel point, and remove the offset.
            const PointWorldPx point_px(projection::get().toPointWorldPx(m_point_coord, controller_zoom) - offset_px);

            // Update the object size for the widget size for this controller zoom.
            const QSizeF widget_size_px(calculateGeometrySizePx(controller_zoom));

            // Calculate the top-left point.
            const PointWorldPx top_left_point_px(calculateTopLeftPoint(point_px, m_alignment_type, widget_size_px));

            // Set the new location of the geometry.
            m_widget->setGeometry(top_left_point_px.x(), top_left_point_px.y(), widget_size_px.width(), widget_size_px.height());
        }
    }

    RectWorldCoord GeometryWidget::boundingBox(const int& controller_zoom) const
    {
        // Translate the point into the current world pixel point, and remove the offset.
        const PointWorldPx point_px(projection::get().toPointWorldPx(m_point_coord, controller_zoom));

        // Update the object size for the widget size for this controller zoom.
        const QSizeF widget_size_px(calculateGeometrySizePx(controller_zoom));

        // Calculate the top-left and bottom-right points.
        const PointWorldPx top_left_point_px(calculateTopLeftPoint(point_px, m_alignment_type, widget_size_px));
        const PointWorldPx bottom_right_px(top_left_point_px.x() + widget_size_px.width(), top_left_point_px.y() + widget_size_px.height());

        // Returnt the bounding box in world coordinates.
        return RectWorldCoord(projection::get().toPointWorldCoord(top_left_point_px, controller_zoom), projection::get().toPointWorldCoord(bottom_right_px, controller_zoom));
    }

    bool GeometryWidget::touches(const Geometry* geometry, const int& controller_zoom) const
    {
        // Default return success.
        bool return_touches(false);

        // Check we are visible and the geometry to compare against is valid.
        if(isVisible(controller_zoom) && geometry != nullptr)
        {
            // Switch to the correct geometry type.
            switch(geometry->geometryType())
            {
                case GeometryType::GeometryLineString:
                {
                    /// @todo Line String calculation.

                    // Finished.
                    break;
                }
                case GeometryType::GeometryPoint:
                case GeometryType::GeometryWidget:
                {
                    // Check if the bounding boxes intersect.
                    if(geometry->boundingBox(controller_zoom).rawRect().intersects(boundingBox(controller_zoom).rawRect()))
                    {
                        // Set that we have touched.
                        return_touches = true;
                    }

                    // Finished.
                    break;
                }
                case GeometryType::GeometryPolygon:
                {
                    // Check if the poylgon intersects with our bounding box.
                    if(static_cast<const GeometryPolygon*>(geometry)->toQPolygonF().intersected(boundingBox(controller_zoom).rawRect()).empty() == false)
                    {
                        // Set that we have touched.
                        return_touches = true;
                    }

                    // Finished.
                    break;
                }
            }

            // Have we touched?
            if(return_touches)
            {
                // Emit that the geometry has been clicked.
                emit geometryClicked(this);
            }
        }

        // Return our success.
        return return_touches;
    }

    void GeometryWidget::draw(QPainter& /*painter*/, const RectWorldCoord& /*backbuffer_rect_coord*/, const int& /*controller_zoom*/)
    {
        // Do nothing.
    }

    QSizeF GeometryWidget::calculateGeometrySizePx(const int& controller_zoom) const
    {
        // Get the object size (default to point - ie: no size).
        QSizeF return_size_px(1.0, 1.0);

        // Do we have a pixmap?
        if(m_widget != nullptr && m_widget->size().isEmpty() == false)
        {
            // Get the pixmap size.
            return_size_px = m_widget->size();

            // Do we have a base zoom set?
            if(m_base_zoom > 0)
            {
                // What is the difference between the base and current zoom.
                const int zoom_difference = m_base_zoom - controller_zoom;

                // Calculate the zoom width/height as a ratio of the zoom difference.
                return_size_px.setHeight(m_base_size_px.height() / std::pow(2.0, zoom_difference));
                return_size_px.setWidth(m_base_size_px.width() / std::pow(2.0, zoom_difference));

                // Do we have a minimum height draw size set?
                if(m_draw_minimum_px.height() > -1.0)
                {
                    // Take the highest height.
                    return_size_px.setHeight(std::max(return_size_px.height(), m_draw_minimum_px.height()));
                }

                // Do we have a minimum width draw size set?
                if(m_draw_minimum_px.width() > -1.0)
                {
                    // Take the highest width.
                    return_size_px.setWidth(std::max(return_size_px.width(), m_draw_minimum_px.width()));
                }

                // Do we have a maximum height draw size set?
                if(m_draw_maximum_px.height() > -1.0)
                {
                    // Take the lowest height.
                    return_size_px.setHeight(std::min(return_size_px.height(), m_draw_maximum_px.height()));
                }

                // Do we have a maximum width draw size set?
                if(m_draw_maximum_px.width() > -1.0)
                {
                    // Take the lowest width.
                    return_size_px.setWidth(std::min(return_size_px.width(), m_draw_maximum_px.width()));
                }
            }
        }

        // Return the size.
        return return_size_px;
    }

}
