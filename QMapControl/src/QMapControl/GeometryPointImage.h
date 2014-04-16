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
    //! Draws an image into the map.
    /*! This is a convenience class for Point.
     * It configures the pixmap of a Point to draw the given image.
     * The image will be loaded from the given path and written in the points pixmap.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointImage : public GeometryPoint
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap from the image file.
         * @param point_coord The longitude/latitude coordinate (x/y).
         * @param filename The image file to draw.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImage(const PointWorldCoord& point_coord, const std::string& filename, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap.
         * @param point_coord The longitude/latitude coordinate (x/y).
         * @param pixmap The pixmap to draw.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImage(const PointWorldCoord& point_coord, const QPixmap& pixmap, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointImage(const GeometryPointImage&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointImage& operator=(const GeometryPointImage&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointImage() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointImage(const GeometryPointImage&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointImage& operator=(const GeometryPointImage&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
