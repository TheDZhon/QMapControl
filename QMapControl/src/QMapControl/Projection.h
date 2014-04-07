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
#include <QtCore/QPoint>

// Local includes.
#include "qmapcontrol_global.h"
#include "Point.h"

/*!
 * @author Chris Stylianou <chris5287@gmail.com>
 */
namespace qmapcontrol
{
    class QMAPCONTROL_EXPORT Projection
    {
    public:
        //! Disable copy constructor.
        ///Projection(const Projection&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///Projection& operator=(const Projection&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~Projection() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * The number of tiles on the x-axis for a given zoom.
         * @param zoom The zoom level.
         * @return number of tiles on the x-axis for a given zoom.
         */
        virtual int tilesX(const int& zoom) const = 0;

        /*!
         * The number of tiles on the y-axis for a given zoom.
         * @param zoom The zoom level.
         * @return number of tiles on the y-axis for a given zoom.
         */
        virtual int tilesY(const int& zoom) const = 0;

        /*!
         * Fetch the recognised EPSG number for the projection.
         * @return the recognised EPSG number for the projection.
         */
        virtual int epsg() const = 0;

        /*!
         * Converts a world coorindate point (longitude/latitude) into the pixel point for a given zoom.
         * @param point_coord The world coordinate point to convert (longitude/latitude).
         * @param zoom The zoom level.
         * @return the converted world pixel point.
         */
        virtual PointWorldPx toPointWorldPx(const PointWorldCoord& point_coord, const int& zoom) const = 0;

        /*!
         * Converts a world pixel point into the coorindate point (longitude/latitude) for a given zoom.
         * @param point_px The world pixel point to convert.
         * @param zoom The zoom level.
         * @return the converted world coorindate point (longitude/latitude).
         */
        virtual PointWorldCoord toPointWorldCoord(const PointWorldPx& point_px, const int& zoom) const = 0;

    protected:
        //! Constuctor.
        /*!
         * Projection constructor.
         */
        Projection() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        Projection(const Projection&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        Projection& operator=(const Projection&); /// @todo remove once MSVC supports default/delete syntax.
    };

    namespace projection
    {
        //! Projection types available.
        enum class EPSG
        {
            /// Equirectangular (EPSG:4326 - lat/long).
            Equirectangular = 4326,
            /// SphericalMercator (EPSG:3857 - meters)
            SphericalMercator = 3857
        };

        /*!
         * Get the singleton instance of the Projection.
         * @return the singleton instance.
         */
        QMAPCONTROL_EXPORT Projection& get();

        /*!
         * Set the projection (Creates new singleton instance).
         * @param type The projection type required.
         */
        QMAPCONTROL_EXPORT void set(const EPSG& type);
    }
}
