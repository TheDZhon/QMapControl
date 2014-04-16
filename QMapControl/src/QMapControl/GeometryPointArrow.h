/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2010 Jeffery MacEachern
* Based on CirclePoint code by Kai Winter
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
#include "GeometryPoint.h"

namespace qmapcontrol
{
    //! Draws a directed arrow (showing orientation) into the map.
    /*! This is a convenience class for Point.
     * It configures the pixmap of a Point to draw an arrow in a specific direction.
     * A QPen could be used to change the color or line-width of the arrow.
     *
     * @author Jeffery MacEachern <j.maceachern@gmail.com>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointArrow : public GeometryPoint
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap with a circle.
         * @param point_coord The longitude/latitude coordinate (x/y).
         * @param size_px The arrow size in pixels.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointArrow(const PointWorldCoord& point_coord, const qreal& heading, const int& size_px, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointArrow(const GeometryPointArrow&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointArrow& operator=(const GeometryPointArrow&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointArrow() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the arrow heading.
         * @return the arrow heading.
         */
        qreal getHeading() const;

        /*!
         * Set the arrow heading.
         * @param heading The arrow heading.
         */
        void setHeading(const qreal& heading);

    protected:
        /*!
         * Updates the pixmap (draws an arrow to the pixmap).
         */
        void updatePixmap() final;

    private:
        //! Disable copy constructor.
        GeometryPointArrow(const GeometryPointArrow&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointArrow& operator=(const GeometryPointArrow&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The arrow heading.
        qreal m_heading;
    };
}
