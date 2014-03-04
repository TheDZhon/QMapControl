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

#pragma once

// Local includes.
#include "qmapcontrol_global.h"
#include "MapAdapter.h"

namespace qmapcontrol
{
    //! MapAdapter for servers with image tiles.
    /*!
     * Use this derived MapAdapter to display maps from Tiled Map servers (OpenStreetMap/Google/etc...).
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT MapAdapterTile : public MapAdapter
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * This construct a MapAdapter for Tiled Map (Slippymap) services.
         * Sample of a correct initialization of a MapAdapter.
         * std::shared_ptr<MapAdapterTile> mat(std::make_shared<MapAdapterTile>(QUrl("http://192.168.8.1/img/img_cache.php/%zoom/%x/%y.png"), ...));
         * The placeholders available are: %zoom, %x and %y.
         * @param base_url The base url of the map server.
         * @param epsg_projections The supported EPSG projections.
         * @param adapter_zoom_minimum The adapter's minimum zoom level available.
         * @param adapter_zoom_maximum The adapter'smaximum zoom level available.
         * @param adapter_zoom_offset The initial offset from the controller zoom at level 0.
         * @param invert_y Whether the y-axis tile needs to be inverted (ie: y-axis tiles start at bottom-left, instead of top-left).
         * @param parent QObject parent ownership.
         */
        MapAdapterTile(const QUrl& base_url,
                       const std::set<projection::EPSG>& epsg_projections,
                       const int& adapter_zoom_minimum = 0,
                       const int& adapter_zoom_maximum = 17,
                       const int& adapter_minimum_offset = 0,
                       const bool& invert_y = false,
                       QObject* parent = 0);

        //! Disable copy constructor.
        ///MapAdapterTile(const MapAdapterTile&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapterTile& operator=(const MapAdapterTile&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~MapAdapterTile() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Generates the url required to fetch the image tile for the specified x, y and zoom.
         * @param x The x coordinate required.
         * @param y The y coordinate required.
         * @param controller_zoom The current controller zoom.
         * @return the generated url.
         */
        virtual QUrl tileQuery(const int& x, const int& y, const int& controller_zoom) const override;

    private:
        //! Disable copy constructor.
        MapAdapterTile(const MapAdapterTile&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapterTile& operator=(const MapAdapterTile&); /// @todo remove once MSVC supports default/delete syntax.

        /// Whether the y-axis tile needs to be inverted (ie: y-axis tiles start at bottom-left, instead of top-left).
        const bool m_invert_y;
    };
}
