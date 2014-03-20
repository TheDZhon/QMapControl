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

#include "MapAdapterWMS.h"

// Qt includes.
#include <QUrlQuery>

// STL includes.
#include <cmath>

// Local includes.
#include "ImageManager.h"

namespace qmapcontrol
{
    MapAdapterWMS::MapAdapterWMS(const QUrl& base_url, const std::set<projection::EPSG>& epsg_projections, QObject* parent)
          : MapAdapter(base_url, epsg_projections, 0, 17, 0, parent)
    {
        // Perform initial query modificiations.
        setBaseUrl(base_url);
    }

    void MapAdapterWMS::setBaseUrl(const QUrl& base_url)
    {
        // Get the url's query details.
        QUrlQuery url_query(base_url);

        // Enforce SERVICE (WMS).
        url_query.removeQueryItem("SERVICE");
        url_query.addQueryItem("SERVICE", "WMS");

        // Enforce REQUEST (GetMap).
        url_query.removeQueryItem("REQUEST");
        url_query.addQueryItem("REQUEST", "GetMap");

        // Enforce TILED (TRUE).
        url_query.removeQueryItem("TILED");
        url_query.addQueryItem("TILED", "TRUE");

        // Enforce WIDTH (the tile size).
        url_query.removeQueryItem("WIDTH");
        url_query.addQueryItem("WIDTH", QString::number(ImageManager::get().tileSizePx()));

        // Enforce HEIGHT (the tile size).
        url_query.removeQueryItem("HEIGHT");
        url_query.addQueryItem("HEIGHT", QString::number(ImageManager::get().tileSizePx()));

        // Is VERSION specified?
        if(url_query.hasQueryItem("VERSION") == false)
        {
            // Set the default version (1.1.1).
            /// @todo default to 1.3.0 instead?
            url_query.addQueryItem("VERSION", "1.1.1");
        }

        // Is TRANSPARENT specified?
        if(url_query.hasQueryItem("TRANSPARENT") == false)
        {
            // Set the default transparent (true).
            url_query.addQueryItem("TRANSPARENT", "TRUE");
        }

        // Is LAYERS specified?
        //if(url_query.hasQueryItem("LAYERS") == false)
        //{
        //    // Set the default layers (TBD).
        //    url_query.addQueryItem("LAYERS", TBD);
        //}

        // Is SRS or CRS specified?
        if(url_query.hasQueryItem("SRS") == false &&
                url_query.hasQueryItem("CRS") == false)
        {
            // Set the default srs (projection system value).
            url_query.addQueryItem("SRS", QString("EPSG:") + QString::number(projection::get().epsg()));
            url_query.addQueryItem("CRS", QString("EPSG:") + QString::number(projection::get().epsg()));

            //url_query.addQueryItem("SRS", "EPSG:4326"); // Equirectangular projection (lat/long)
            //url_query.addQueryItem("SRS", "EPSG:900913"); // Google Mercator projection
            //url_query.addQueryItem("SRS", "EPSG:3857"); // Spherical Mercator projection
        }

        // Is STYLES specified?
        if(url_query.hasQueryItem("STYLES") == false)
        {
            // Set the default styles (blank).
            url_query.addQueryItem("STYLES", QString());
        }

        // Is FORMAT specified?
        if(url_query.hasQueryItem("FORMAT") == false)
        {
            // Set the default format (png).
            url_query.addQueryItem("FORMAT", "IMAGE/PNG");
        }

        // Remove BBOX (is added at time of actually query).
        url_query.removeQueryItem("BBOX");

        // Create a new url with the modified url query.
        QUrl modified_url(base_url);
        modified_url.setQuery(url_query);

        // Quick check that the url has a path.
        if(modified_url.path().isEmpty())
        {
            // Set a default path (/wms/).
            modified_url.setPath("/wms/");
        }

        // Set the url path.
        MapAdapter::setBaseUrl(modified_url);
    }

    QUrl MapAdapterWMS::tileQuery(const int& x, const int& y, const int& controller_zoom) const
    {
        // Get the url's query details.
        QUrlQuery url_query(getBaseUrl());

        // Calculate the number of coordinates per tile.
        const int coord_per_tile_x = 360.0 / projection::get().tilesX(controller_zoom);
        const int coord_per_tile_y = 180.0 / projection::get().tilesY(controller_zoom);

        // Set BBOX (x1,y1,x2,y2).
        url_query.removeQueryItem("BBOX");
        url_query.addQueryItem("BBOX", getBBox(-180 + x * coord_per_tile_x,
                                            90 - (y + 1) * coord_per_tile_y,
                                            (-180 + x * coord_per_tile_x) + coord_per_tile_x,
                                            (90 - (y + 1) * coord_per_tile_y) + coord_per_tile_y));

        // Create a new url with the modified url query.
        QUrl modified_url(getBaseUrl());
        modified_url.setQuery(url_query);

        // Return the modified url.
        return QUrl(modified_url);
    }

    QString MapAdapterWMS::getBBox(const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2) const
    {
        // Return the formatted BBOX values.
        return QString("%1,%2,%3,%4")
                .arg(QString::number(x1, 'f', 6))
                .arg(QString::number(y1, 'f', 6))
                .arg(QString::number(x2, 'f', 6))
                .arg(QString::number(y2, 'f', 6));
    }
}
