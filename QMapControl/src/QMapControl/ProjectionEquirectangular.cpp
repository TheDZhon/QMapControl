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

#include "ProjectionEquirectangular.h"

// STL includes.
#include <cmath>

// Local include.
#include "ImageManager.h"

namespace qmapcontrol
{
    int ProjectionEquirectangular::tilesX(const int& zoom) const
    {
        // Return the number of tiles for the x-axis.
        return std::pow(2, zoom) * 2;
    }

    int ProjectionEquirectangular::tilesY(const int& zoom) const
    {
        // Return the number of tiles for the y-axis.
        return std::pow(2, zoom);
    }

    int ProjectionEquirectangular::epsg() const
    {
        // Return the recognised EPSG number - 4326.
        return 4326;
    }

    PointWorldPx ProjectionEquirectangular::toPointWorldPx(const PointWorldCoord& point_coord, const int& zoom) const
    {
        // Convert from coordinate to pixel by - top/left delta, then ratio of coords per pixel.
        const qreal x_px((point_coord.longitude() + 180.0) * (tilesX(zoom) * ImageManager::get().tileSizePx()) / 360.0);
        const qreal y_px(-(point_coord.latitude() - 90.0) * (tilesY(zoom) * ImageManager::get().tileSizePx()) / 180.0);

        // Return the converted point (x/y pixel point - 0,0 is screen top left).
        return PointWorldPx(x_px, y_px);
    }

    PointWorldCoord ProjectionEquirectangular::toPointWorldCoord(const PointWorldPx& point_px, const int& zoom) const
    {
        // Convert pixel into coordinate by * against ratio of pixels per coord, then + top/left delta offset.
        const qreal longitude((point_px.x() * 360.0 / (tilesX(zoom) * ImageManager::get().tileSizePx())) - 180.0);
        const qreal latitude(-(point_px.y() * 180.0 / (tilesY(zoom) * ImageManager::get().tileSizePx())) + 90.0);

        // Return the converted coordinate (longitude/latitude coordinate - 0,0 is screen middle).
        return PointWorldCoord(longitude, latitude);
    }
}
