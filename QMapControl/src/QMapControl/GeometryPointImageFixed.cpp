/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2009 Kai Winter
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

#include "GeometryPointImageFixed.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    GeometryPointImageFixed::GeometryPointImageFixed(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord, const std::string& filename, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointImage(top_left_coord, QPixmap(filename.c_str()), zoom_minimum, zoom_maximum),
          m_bottom_right_coord(bottom_right_coord)
    {
        // Fixed image requires the alignment type set to the top-left.
        setAlignmentType(AlignmentType::TopLeft);
    }

    GeometryPointImageFixed::GeometryPointImageFixed(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord, const QPixmap& pixmap, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointImage(top_left_coord, pixmap, zoom_minimum, zoom_maximum),
          m_bottom_right_coord(bottom_right_coord)
    {
        // Fixed image requires the alignment type set to the top-left.
        setAlignmentType(AlignmentType::TopLeft);
    }

    RectWorldCoord GeometryPointImageFixed::boundingBox(const int& /*controller_zoom*/) const
    {
        // Return the bound box.
        return RectWorldCoord(coord(), m_bottom_right_coord);
    }

    void GeometryPointImageFixed::draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Does the image rect intersect with the backbuffer rect?
            if(backbuffer_rect_coord.rawRect().intersects(boundingBox(controller_zoom).rawRect()))
            {
                // Calculate the image rect pixels.
                const RectWorldPx image_rect_px(projection::get().toPointWorldPx(coord(), controller_zoom), projection::get().toPointWorldPx(m_bottom_right_coord, controller_zoom));

                // Draw the pixmap.
                painter.drawPixmap(image_rect_px.rawRect().toRect(), getPixmap());
            }
        }
    }
}
