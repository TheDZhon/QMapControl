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

#include "Geometry.h"

namespace qmapcontrol
{
    Geometry::Geometry(const GeometryType& geometry_type, const int& zoom_minimum, const int& zoom_maximum)
        : m_geometry_type(geometry_type),
          m_zoom_minimum(zoom_minimum),
          m_zoom_maximum(zoom_maximum),
          m_z_index(0),
          m_visible(true),
          m_metadata_displayed_key(""),
          m_metadata_displayed_zoom_minimum(10),
          m_metadata_displayed_alignment_type(AlignmentType::TopRight),
          m_metadata_displayed_alignment_offset_px(5.0),
          mAncillaryData(0)
    {

    }

    int Geometry::zIndex() const { return m_z_index; }

    void Geometry::setZIndex(int z_index) { m_z_index = z_index; }

    const Geometry::GeometryType& Geometry::geometryType() const
    {
        // Return the geometry type.
        return m_geometry_type;
    }

    bool Geometry::isVisible(const int& controller_zoom) const
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

    void Geometry::setVisible(const bool& enabled)
    {
        // Only update visibility if it has changed.
        if(m_visible != enabled)
        {
            // Set the visibility.
            m_visible = enabled;

            // Emit that we need to redraw to display this change.
            emit requestRedraw();
        }
    }

    const QPen& Geometry::pen()
    {
        // Do we have a pen?
        if(m_pen == nullptr)
        {
            // Create a default pen.
            m_pen = std::make_shared<QPen>();
        }

        // Get the pen to draw with.
        return *(m_pen.get());
    }

    void Geometry::setPen(const std::shared_ptr<QPen>& pen)
    {
        // Set the pen to draw with.
        m_pen = pen;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void Geometry::setPen(const QPen& pen)
    {
        // Set the pen to draw with.
        m_pen = std::make_shared<QPen>(pen);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    const QBrush& Geometry::brush()
    {
        // Do we have a brush?
        if(m_brush == nullptr)
        {
            // Create a default brush.
            m_brush = std::make_shared<QBrush>();
        }

        // Get the brush to draw with.
        return *(m_brush.get());
    }

    void Geometry::setBrush(const std::shared_ptr<QBrush>& brush)
    {
        // Set the brush to draw with.
        m_brush = brush;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void Geometry::setBrush(const QBrush& brush)
    {
        // Set the brush to draw with.
        m_brush = std::make_shared<QBrush>(brush);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    QVariant& Geometry::metadata(const std::string& key) const
    {
        // Return the value for the key.
        return m_metadata[key];
    }

    void Geometry::setMetadata(const std::string& key, const QVariant& value)
    {
        // Set the meta-data.
        m_metadata[key] = value;
    }

    void Geometry::setMetadataDisplayed(const std::string& key, const int& zoom_minimum, const AlignmentType& alignment_type, const double& alignment_offset_px)
    {
        // Set the meta-data key to use.
        m_metadata_displayed_key = key;
        m_metadata_displayed_zoom_minimum = zoom_minimum;
        m_metadata_displayed_alignment_type = alignment_type;
        m_metadata_displayed_alignment_offset_px = alignment_offset_px;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    PointWorldPx Geometry::calculateTopLeftPoint(const PointWorldPx& point_px, const AlignmentType& alignment_type, const QSizeF& geometry_size_px) const
    {
        // Default world point to return.
        PointWorldPx top_left_point_px(point_px);

        // Check the alignment type and apply the required deltas to move the point to top-left.
        switch(alignment_type)
        {
            case AlignmentType::Middle:
            {
                // Move x/y from the middle to top-left.
                top_left_point_px.setX(point_px.x() - (geometry_size_px.width() / 2.0));
                top_left_point_px.setY(point_px.y() - (geometry_size_px.height() / 2.0));
                break;
            }
            case AlignmentType::TopLeft:
            {
                // Already at top-left.
                break;
            }
            case AlignmentType::TopRight:
            {
                // Move x from the top-right to top-left.
                top_left_point_px.setX(point_px.x() - geometry_size_px.width());
                break;
            }
            case AlignmentType::TopMiddle:
            {
                // Move x from the top-center to top-left.
                top_left_point_px.setX(point_px.x() - (geometry_size_px.width() / 2.0));
                break;
            }
            case AlignmentType::BottomLeft:
            {
                // Move y from the bottom-left to top-left.
                top_left_point_px.setY(point_px.y() - geometry_size_px.height());
                break;
            }
            case AlignmentType::BottomRight:
            {
                // Move x/y from the bottom-right to top-left.
                top_left_point_px.setX(point_px.x() - geometry_size_px.width());
                top_left_point_px.setY(point_px.y() - geometry_size_px.height());
                break;
            }
            case AlignmentType::BottomMiddle:
            {
                // Move x/y from the bottom-middle to top-left.
                top_left_point_px.setX(point_px.x() - (geometry_size_px.width() / 2.0));
                top_left_point_px.setY(point_px.y() - geometry_size_px.height());
                break;
            }
        }

        // Return the top-left point.
        return top_left_point_px;
    }
}
