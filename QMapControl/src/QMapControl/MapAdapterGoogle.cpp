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

#include "MapAdapterGoogle.h"

namespace qmapcontrol
{
    namespace
    {
        /// @todo remove once MSVC supports initializer lists.
        std::set<projection::EPSG> supportedProjections()
        {
            std::set<projection::EPSG> projections;
            projections.insert(projection::EPSG::SphericalMercator);
            return projections;
        }
    }

    MapAdapterGoogle::MapAdapterGoogle(const MapAdapterGoogle::GoogleLayerType& layer_type,
                                       QObject* parent)
        ///: MapAdapterTile("http://mt1.google.com/vt/v=ap.106&hl=en&x=%x&y=%y&zoom=%zoom&lyrs=" +
        ///layerTypeToString(layer_type), { projection::EPSG::SphericalMercator }, 17, 0, 0, false,
        ///parent) @todo re-add once MSVC supports initializer lists.
        : MapAdapterTile("http://mt.google.com/vt/v=ap.106&hl=en&x=%x&y=%y&zoom=%zoom&lyrs="
                             + layerTypeToString(layer_type),
                         supportedProjections(),
                         17,
                         0,
                         0,
                         false,
                         parent) /// @todo remove once MSVC supports initializer lists.
    {

    }

    QString MapAdapterGoogle::layerTypeToString(const MapAdapterGoogle::GoogleLayerType& layer_type)
    {
        // Convert the enum to a 1-character representation.
        switch (layer_type)
        {
            case GoogleLayerType::SATELLITE_ONLY:
                return "s";
            case GoogleLayerType::TERRAIN_ONLY:
                return "t";
            case GoogleLayerType::TERRAIN:
                return "p";
            case GoogleLayerType::ROADS_ONLY:
                return "h";
            case GoogleLayerType::HYBRID:
                return "y";
            case GoogleLayerType::RASTER:
                return "r";
            case GoogleLayerType::MAPS:
            default:
                return "m";
        }
    }
}
