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

// Qt includes.
#include <QtCore/QObject>
#include <QtCore/QUrl>

// STL includes.
#include <set>

// Local includes.
#include "qmapcontrol_global.h"
#include "Projection.h"

namespace qmapcontrol
{
    //! Used to fit map servers into QMapControl.
    /*!
     * There are two ready-made MapAdapters:
     *  - MapAdapterTile, which is ready to use for OpenStreetMap or Google (Mercator projection).
     *  - MapAdapterWMS, which could be used for the most WMS-Server (some servers show errors, because of image ratio).
     *
     * MapAdapters are also needed to form the HTTP-Queries to load the map tiles.
     * The maps from WMS Servers are also divided into tiles, because those can be better cached.
     *
     * @see MapAdapterTile, @see MapAdapterWMS
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT MapAdapter : public QObject
    {
        Q_OBJECT
    public:
        //! Disable copy constructor.
        ///MapAdapter(const MapAdapter&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapter& operator=(const MapAdapter&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~MapAdapter() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Get the base url.
         * @return the base url.
         */
        QUrl getBaseUrl() const;

        /*!
         * Change the base url post-initialisation.
         * @param base_url The new base url to set.
         */
        virtual void setBaseUrl(const QUrl& base_url);

        /*!
         * Indicates whether a given x, y and controller zoom would provide a valid image tile.
         * @param x The x coordinate required.
         * @param y The y coordinate required.
         * @param controller_zoom The current controller zoom.
         * @return whether their would be a valid image tile.
         */
        bool isTileValid(const int& x, const int& y, const int& controller_zoom) const;

        /*!
         * Generates the url required to fetch the image tile for the specified x, y and zoom.
         * @param x The x coordinate required.
         * @param y The y coordinate required.
         * @param controller_zoom The current controller zoom.
         * @return the generated url.
         */
        virtual QUrl tileQuery(const int& x, const int& y, const int& controller_zoom) const = 0;

    protected:
        //! Constructor.
        /*!
         * This construct a Map Adapter.
         * @param base_url The base url of the map server.
         * @param epsg_projections The supported EPSG projections.
         * @param adapter_zoom_minimum The adapter's minimum zoom level available.
         * @param adapter_zoom_maximum The adapter'smaximum zoom level available.
         * @param adapter_zoom_offset The initial offset from the controller zoom at level 0.
         * @param parent QObject parent ownership.
         */
        MapAdapter(const QUrl& base_url,
                   const std::set<projection::EPSG>& epsg_projections,
                   const int& adapter_zoom_minimum,
                   const int& adapter_zoom_maximum,
                   const int& adapter_zoom_offset,
                   QObject* parent = 0);

        /*!
         * Translates the controller zoom in the map adapters zoom.
         * @param controller_zoom The controller's zoom.
         * @return the map adapters zoom.
         */
        int toAdapterZoom(const int& controller_zoom) const;

    private:
        //! Disable copy constructor.
        MapAdapter(const MapAdapter&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapter& operator=(const MapAdapter&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The base url path of the map server.
        QUrl m_base_url;

        /// The supported EPSG projections.
        const std::set<projection::EPSG> m_epsg_projections;

        /// The minimum adapter zoom level available.
        const int m_adapter_zoom_minimum;

        /// The maximum adapter zoom level available.
        const int m_adapter_zoom_maximum;

        /// The initial offset from the controller zoom at level 0.
        const int m_adapter_zoom_offset;
    };
}
