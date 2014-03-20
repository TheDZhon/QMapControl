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
    Geometry::Geometry(const GeometryType& geometry_type, const QPen& pen, const int& zoom_minimum, const int& zoom_maximum)
        : m_visible(true),
          m_geometry_type(geometry_type),
          m_pen(pen),
          m_zoom_minimum(zoom_minimum),
          m_zoom_maximum(zoom_maximum)
    {

    }

    Geometry::GeometryType Geometry::getGeometryType() const
    {
        // Return the geometry type.
        return m_geometry_type;
    }

    QVariant Geometry::getMetadata(const std::string& key) const
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

    void Geometry::setMetadata(const std::string& key, const QVariant& value)
    {
        // Set the meta-data.
        m_metadata[key] = value;
    }

    const QPen& Geometry::getPen() const
    {
        // Get the pen to draw with.
        return m_pen;
    }

    void Geometry::setPen(const QPen& pen)
    {
        // Set the pen to draw with.
        m_pen = pen;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
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
}
