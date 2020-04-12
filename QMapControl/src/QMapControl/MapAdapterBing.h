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
    //! MapAdapter for Bing.
    /*!
     * This is a conveniece class, which extends and configures a MapAdapterTile.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     * @author Andrii Shelestov <streamx3@gmail.com>
     *
     * Had to inherit from MapAdapter directly, since Bing has slightly
     * diffetent coordinate calculation.
     * https://docs.microsoft.com/en-us/bingmaps/articles/bing-maps-tile-system
     */
    class QMAPCONTROL_EXPORT MapAdapterBing : public MapAdapter
    {
        Q_OBJECT
    public:
        /// Different Bing layer types available.
        enum class BingLayerType
        {
            /// Map layer.
            MAPS,
            /// Satelite layer.
            SATELLITE,
        };

    public:
        //! Constructor.
        /*!
         * This construct a Bing MapAdapter.
         * @param layer_type The Bing layer type to display.
         * @param parent QObject parent ownership.
         */
        MapAdapterBing(const BingLayerType& layer_type = BingLayerType::SATELLITE, QObject* parent = 0);

        //! Disable copy constructor.
        ///MapAdapterGoogle(const MapAdapterGoogle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapterGoogle& operator=(const MapAdapterGoogle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~MapAdapterBing() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Convert the enum to a 1-character representation.
         * @param layer_type The layer type to convert.
         * @return the 1-character representation.
         */
//        QString layerTypeToString(const MapAdapterBing::BingLayerType& layer_type);

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
        MapAdapterBing(const MapAdapterBing&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapterBing& operator=(const MapAdapterBing&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
