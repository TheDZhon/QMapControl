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

#include "MapAdapterTile.h"

// STL includes.
#include <cmath>

namespace qmapcontrol
{
    MapAdapterTile::MapAdapterTile(const QUrl& base_url,
                                   const std::set<projection::EPSG>& epsg_projections,
                                   const int& adapter_zoom_minimum,
                                   const int& adapter_zoom_maximum,
                                   const int& adapter_minimum_offset,
                                   const bool& invert_y,
                                   QObject* parent)
            : MapAdapter(base_url, epsg_projections, adapter_zoom_minimum, adapter_zoom_maximum, adapter_minimum_offset, parent),
              m_invert_y(invert_y)
    {

    }

    QUrl MapAdapterTile::tileQuery(const int& x, const int& y, const int& zoom_controller) const
    {
        // Capture inital y-axis tile request.
        int y_axis(y);

        // Do we need to invert the y-axis?
        if(m_invert_y)
        {
            // Inverse-y required.
            y_axis = projection::get().tilesY(zoom_controller - 1) - 1 - y;
        }

        // Return a modified url with the %x, %y and %zoom values replaced.
        /// @note QUrl converts % into %25, so we replace %25x, %25y and %25zoom instead.
        return QUrl(getBaseUrl().toString()
                    .replace("%25x", QString::number(x))
                    .replace("%25y", QString::number(y_axis))
                    .replace("%25zoom", QString::number(toAdapterZoom(zoom_controller))));
    }
}
