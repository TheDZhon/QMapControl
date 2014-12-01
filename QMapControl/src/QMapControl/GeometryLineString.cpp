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

#include "GeometryLineString.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    GeometryLineString::GeometryLineString(const std::vector<PointWorldCoord>& points, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryLineString, zoom_minimum, zoom_maximum),
          m_points(points)
    {
    }

    GeometryLineString::GeometryLineString(int zoom_min, int zoom_max)
        : Geometry(Geometry::GeometryType::GeometryLineString, zoom_min, zoom_max),
          m_points()
    {
    }

    std::vector<PointWorldCoord> GeometryLineString::points() const
    {
        // Return the points.
        return m_points;
    }

    void GeometryLineString::addPoint(const PointWorldCoord& point)
    {
        // Add the point.
        m_points.push_back(point);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void GeometryLineString::setPoints(const std::vector<PointWorldCoord>& points)
    {
        // Set the new points.
        m_points = points;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    RectWorldCoord GeometryLineString::boundingBox(const int& /*controller_zoom*/) const
    {
        // Create a polygon of the points.
        QPolygonF polygon_line;

        // Loop through each point to add to the polygon.
        for(const auto& point : m_points)
        {
            // Add the point to be drawn.
            polygon_line.append(point.rawPoint());
        }

        // Return the bounding box.
        return RectWorldCoord::fromQRectF(polygon_line.boundingRect());
    }

    bool GeometryLineString::touches(const Geometry* geometry, const int& controller_zoom) const
    {
        /// @todo change to world coordinates.

        // Default return success.
        bool return_touches(false);

//        // Clear previous touches result.
//        m_touched_points.clear();

//        // Check the geometry is visible.
//        if(isVisible(controller_zoom))
//        {
//            // Loop through each point.
//            for(const auto& point : m_points)
//            {
//                // Does the touch area contain the point?
//                if(point->touches(area_px, controller_zoom))
//                {
//                    // Add the point to the touches list.
//                    m_touched_points.push_back(point);

//                    // Set that we have touched.
//                    return_touches = true;
//                }
//            }

//            // Did we find at least one geometry touching?
//            if(return_touches)
//            {
//                // Emit that the geometry has been clicked.
//                emit geometryClicked(this);
//            }
//        }

        // Return our success.
        return return_touches;
    }

    void GeometryLineString::draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Create a polygon of the points.
            QPolygonF polygon_line;

            // Loop through each point to add to the polygon.
            for(const auto& point : m_points)
            {
                // Add the point to be drawn.
                polygon_line.append(point.rawPoint());
            }

            // Does the polygon intersect with the backbuffer rect?
            if(QPolygonF(backbuffer_rect_coord.rawRect()).intersected(polygon_line).empty() == false)
            {
                // Create a polygon of the points.
                QPolygonF polygon_line_px;

                // Loop through each point to add to the polygon.
                for(const auto& point : m_points)
                {
                    // Add the point to be drawn.
                    polygon_line_px.append(projection::get().toPointWorldPx(point, controller_zoom).rawPoint());
                }

                // Set the pen to use.
                painter.setPen(pen());

                // Draw the polygon line.
                painter.drawPolyline(polygon_line_px);
            }
        }
    }
}
