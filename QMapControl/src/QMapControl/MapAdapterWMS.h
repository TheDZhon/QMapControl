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
    //! MapAdapter for WMS servers
    /*!
     * Use this derived MapAdapter to display maps from WMS servers.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT MapAdapterWMS : public MapAdapter
    {
    public:
        //! Constructor.
        /*!
         * This construct a MapAdapter for WMS services.
         * Sample of a correct initialization of a MapAdapter:
         * std::shared_ptr<MapAdapter> mapadapter(std::make_shared<MapAdapterWMS>("http://www2.demis.nl/wms/wms.asp?wms=WorldMap[...]&BBOX=%1,%2,%3,%4&WIDTH=%5&HEIGHT=%5&TRANSPARENT=TRUE", ...));
         * The placeholders %1, %2, %3, %4 creates the bounding box.
         * %5 is where you specify the tile size in pixels.
         * @param base_url The base url of the map server.
         * @param epsg_projections The supported EPSG projections.
         * @param parent QObject parent ownership.
         */
        MapAdapterWMS(const QUrl& base_url, const std::set<projection::EPSG>& epsg_projections, QObject* parent = 0);

        //! Disable copy constructor.
        ///MapAdapterWMS(const MapAdapterWMS&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapterWMS& operator=(const MapAdapterWMS&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~MapAdapterWMS() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Change the base url post initialisation (performs query modifications).
         * @param base_url The new base url to set.
         */
        void setBaseUrl(const QUrl& base_url) override;

        /*!
         * Generates the url required to fetch the image tile for the specified x, y and zoom.
         * @param x The x coordinate required.
         * @param y The y coordinate required.
         * @param controller_zoom The current controller zoom.
         * @return the generated url.
         */
        virtual QUrl tileQuery(const int& x, const int& y, const int& controller_zoom) const override;

    protected:
        /*!
         * Generate a BBOX formatted string of the specified values.
         * @param x1 The x1 value.
         * @param y1 The y1 value.
         * @param x2 The x2 value.
         * @param y2 The y2 value.
         * @return the formatted BBOX string.
         */
        virtual QString getBBox(const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2) const;

    private:
        //! Disable copy constructor.
        MapAdapterWMS(const MapAdapterWMS&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapterWMS& operator=(const MapAdapterWMS&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
