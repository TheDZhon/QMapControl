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

#include "ProjectionSphericalMercator.h"

// STL includes.
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Local include.
#include "ImageManager.h"

namespace qmapcontrol
{
    int ProjectionSphericalMercator::tilesX(const int& zoom) const
    {
        // Return the number of tiles for the x-axis.
        return std::pow(2, zoom);
    }

    int ProjectionSphericalMercator::tilesY(const int& zoom) const
    {
        // Return the number of tiles for the y-axis.
        return std::pow(2, zoom);
    }

    int ProjectionSphericalMercator::epsg() const
    {
        // Return the recognised EPSG number - 3857.
        return 3857;
    }

    PointWorldPx ProjectionSphericalMercator::toPointWorldPx(const PointWorldCoord& point_coord, const int& zoom) const
    {
        /*!
          Formula basis: http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Lon..2Flat._to_tile_numbers
            n = 2 ^ display_zoom * tile_size (as lat/lon is actual point, we need to multiple by tile size).
            xtile = n * ((lon_deg + 180) / 360) : floor the answer?
            ytile = n * (1 - (log(tan(lat_rad) + sec(lat_rad)) / π)) / 2 : floor the answer?
         */
        const qreal x_px((tilesX(zoom) * ImageManager::get().tileSizePx()) * ((point_coord.longitude() + 180.0) / 360.0));
        const qreal y_px((tilesY(zoom) * ImageManager::get().tileSizePx()) * (1.0 - (std::log(std::tan(point_coord.latitude() * M_PI / 180.0) + (1.0 / std::cos(point_coord.latitude() * M_PI / 180.0))) / M_PI )) / 2.0);

        // Return the converted point (x/y pixel point - 0,0 is screen top left).
        return PointWorldPx(x_px, y_px);
    }

    PointWorldCoord ProjectionSphericalMercator::toPointWorldCoord(const PointWorldPx& point_px, const int& zoom) const
    {
        /*!
          Formula basis: http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Tile_numbers_to_lon..2Flat.
            n = 2 ^ display_zoom * tile_size (as x/y is pixel point and not tile number, we need to multiple by tile size).
            lon_deg = x_point / n * 360.0 - 180.0
            lat_rad = arctan(sinh(π * (1 - 2 * y_point / n)))
            lat_deg = lat_rad * 180.0 / π
         */
        const qreal longitude(qreal(point_px.x()) / qreal(tilesX(zoom) * ImageManager::get().tileSizePx()) * 360.0 - 180.0);
        const qreal latitude(std::atan(std::sinh(M_PI * (1.0 - 2.0 * qreal(point_px.y()) / qreal(tilesY(zoom) * ImageManager::get().tileSizePx())))) * 180.0 / M_PI);

        // Return the converted coordinate (longitude/latitude coordinate - 0,0 is screen middle).
        return PointWorldCoord(longitude, latitude);
    }
}
