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
#include "MapAdapterTile.h"

namespace qmapcontrol
{
    //! MapAdapter for Google.
    /*!
     * This is a conveniece class, which extends and configures a MapAdapterTile.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT MapAdapterGoogle : public MapAdapterTile
    {
        Q_OBJECT
    public:
        /// Different Google layer types available.
        enum class GoogleLayerType
        {
            /// Map layer (standard roadmap).
            MAPS,
            /// Satelite layer.
            SATELLITE_ONLY,
            /// Terrain layer.
            TERRAIN_ONLY,
            TERRAIN,
            // Roads layer
            ROADS_ONLY,
            /// Hybrid layer.
            HYBRID,
            /// Ratser layer.
            RASTER
        };

    public:
        //! Constructor.
        /*!
         * This construct a Google MapAdapter.
         * @param layer_type The Google layer type to display.
         * @param parent QObject parent ownership.
         */
        MapAdapterGoogle(const GoogleLayerType& layer_type = GoogleLayerType::MAPS, QObject* parent = 0);

        //! Disable copy constructor.
        ///MapAdapterGoogle(const MapAdapterGoogle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapterGoogle& operator=(const MapAdapterGoogle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~MapAdapterGoogle() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Convert the enum to a 1-character representation.
         * @param layer_type The layer type to convert.
         * @return the 1-character representation.
         */
        QString layerTypeToString(const MapAdapterGoogle::GoogleLayerType& layer_type);

    private:
        //! Disable copy constructor.
        MapAdapterGoogle(const MapAdapterGoogle&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapterGoogle& operator=(const MapAdapterGoogle&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
