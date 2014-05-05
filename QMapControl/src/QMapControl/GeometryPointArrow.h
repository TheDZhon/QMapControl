/*
 *
 * This file is part of QMapControl,
 * an open-source cross-platform map widget
 *
 * Copyright (C) 2014 Chris Stylianou
 * Copyright (C) 2010 Jeffery MacEachern
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
 */

#pragma once

// Local includes.
#include "qmapcontrol_global.h"
#include "GeometryPointImage.h"

namespace qmapcontrol
{
    //! Draws an arrow at a specific point onto the map.
    /*!
     * This class draws an arrow at a specific point onto the map.
     *
     * @author Jeffery MacEachern <j.maceachern@gmail.com>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointArrow : public GeometryPointImage
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display an image pixmap with an arrow.
         * @param point_coord The point (world coordinates).
         * @param size_px The arrow size in pixels.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointArrow(const PointWorldCoord& point_coord, const QSizeF& size_px, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointArrow(const GeometryPointArrow&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointArrow& operator=(const GeometryPointArrow&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointArrow() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointArrow(const GeometryPointArrow&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointArrow& operator=(const GeometryPointArrow&); /// @todo remove once MSVC supports default/delete syntax.

    protected:
        /*!
         * Updates the shape (draws an arrow on to the image pixmap).
         */
        void updateShape() final;
    };
}
