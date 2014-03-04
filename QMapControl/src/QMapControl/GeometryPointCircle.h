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
#include "GeometryPoint.h"

namespace qmapcontrol
{
    //! Draws a circle into the map.
    /*! This is a conveniece class for Point.
     * It configures the pixmap of a Point to draw a circle.
     * A QPen could be used to change the color or line-width of the circle.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointCircle : public GeometryPoint
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap with a circle.
         * @param point_coord The x/y coordinate (longitude/latitude).
         * @param radius_px The circle radius in pixels.
         * @param alignment_type The alignment of the geometry to use when drawing.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointCircle(const QPointF& point_coord, const int& radius_px, const AlignmentType& alignment_type = AlignmentType::Middle, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap with a circle (radius = 10.0 pixels).
         * @param point_coord The x/y coordinate (longitude/latitude).
         * @param alignment_type The alignment of the geometry to use when drawing.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointCircle(const QPointF& point_coord, const AlignmentType& alignment_type = AlignmentType::Middle, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointCircle(const GeometryPointCircle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointCircle& operator=(const GeometryPointCircle&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointCircle() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    protected:
        /*!
         * Updates the pixmap (draws a circle to the pixmap).
         */
        void updatePixmap() final;

    private:
        //! Disable copy constructor.
        GeometryPointCircle(const GeometryPointCircle&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointCircle& operator=(const GeometryPointCircle&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
