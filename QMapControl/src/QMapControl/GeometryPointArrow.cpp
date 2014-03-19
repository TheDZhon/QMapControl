/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2010 Jeffery MacEachern
* Based on CirclePoint code by Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will `be useful,
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

#include "GeometryPointArrow.h"

namespace qmapcontrol
{
    GeometryPointArrow::GeometryPointArrow(const QPointF& point_coord, const qreal& heading, const int& size_px, const AlignmentType& alignment_type, const QPen& pen, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPoint(point_coord, QPixmap(size_px, size_px), alignment_type, pen, zoom_minimum, zoom_maximum),
          m_heading(heading)
    {
        // Update the pixmap.
        updatePixmap();
    }

    qreal GeometryPointArrow::getHeading() const
    {
        // Return the heading,
        return m_heading;
    }

    void GeometryPointArrow::setHeading(const qreal& heading)
    {
        /// @todo replace with setRotate in GeometryPoint.

        // Set the heading.
        m_heading = heading;

        // Update the pixmap.
        updatePixmap();
    }

    void GeometryPointArrow::updatePixmap()
    {
        // Fetch the current pixmap.
        QPixmap pixmap = getPixmap();

        // Reset the pixmap.
        pixmap.fill(Qt::transparent);

        // Create a painter for the pixmap.
        QPainter painter(&pixmap);

//#if !defined Q_WS_MAEMO_5 //FIXME Maemo has a bug - it will antialias our point out of existence
        painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
//#endif

        // Set the pen and brush.
        painter.setPen(getPen());
        painter.setBrush(QBrush(getPen().color()));

        // Rotate the painter, so that the arrow drawn is point in the right direction.
        QPointF center_px(pixmap.width() / 2.0, pixmap.height() / 2.0);
        painter.translate(center_px);
        painter.rotate(m_heading);

        // Add points to create arrow shape.
        QPolygonF arrow;
        arrow << QPointF((pixmap.width() / 2.0) - center_px.x(), 0.0 - center_px.y());
        arrow << QPointF(pixmap.width() - center_px.x(), pixmap.height() - center_px.y());
        arrow << QPointF((pixmap.width() / 2.0) - center_px.x(), (pixmap.height() / 2.0) - center_px.y());
        arrow << QPointF(0.0 - center_px.x(), pixmap.height() - center_px.y());

        // Draw the arrow.
        painter.drawPolygon(arrow);

        // Set the new pixmap.
        setPixmap(pixmap);
    }
}
