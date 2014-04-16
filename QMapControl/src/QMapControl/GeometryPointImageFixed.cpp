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

    void GeometryPointImageFixed::draw(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Calculate the image rect pixels.
            const QRectF image_rect_px(projection::get().toPointWorldPx(coord(), controller_zoom).rawPoint(), projection::get().toPointWorldPx(m_bottom_right_coord, controller_zoom).rawPoint());

            // Does the image rect intersect with the backbuffer rect?
            if(backbuffer_rect_px.intersects(image_rect_px))
            {
                // Draw the pixmap.
                painter->drawPixmap(image_rect_px, getPixmap(), QRectF());
            }
        }
    }
}
