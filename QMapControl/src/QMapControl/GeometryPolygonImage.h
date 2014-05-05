/*
 *
 * This file is part of QMapControl,
 * an open-source cross-platform map widget
 *
 * Copyright (C) 2014 Chris Stylianou
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

// Qt includes.
#include <QtGui/QPixmap>

// Local includes.
#include "qmapcontrol_global.h"
#include "GeometryPolygon.h"

namespace qmapcontrol
{
    //! Draws a fixed image into the map.
    /*!
     * This class draws a image onto the map.
     * The image's top-left and bottom-right corners will always lay on the given world coordinates.
     *
     * @author Chris Stylianou <chris5287@gmail.com>
     * @author Kai Winter <kaiwinter@gmx.de>
     */
    class QMAPCONTROL_EXPORT GeometryPolygonImage : public GeometryPolygon
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a polygon with an image file to be displayed.
         * @param top_left_coord The top-left point of the polygon (world coordinates).
         * @param bottom_right_coord The bottom-right point of the polygon (world coordinates).
         * @param filename The image file to draw.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPolygonImage(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord, const std::string& filename, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a polygon with an image to be displayed.
         * @param top_left_coord The top-left point of the polygon (world coordinates).
         * @param bottom_right_coord The bottom-right point of the polygon (world coordinates).
         * @param image The image pixmap to draw.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPolygonImage(const PointWorldCoord& point_top_left, const PointWorldCoord& bottom_right_coord, const QPixmap& image, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointImageFixed(const GeometryPointImageFixed&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointImageFixed& operator=(const GeometryPointImageFixed&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPolygonImage() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPolygonImage(const GeometryPolygonImage&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPolygonImage& operator=(const GeometryPolygonImage&); /// @todo remove once MSVC supports default/delete syntax.

    public:
        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_coord Only draw geometries that are contained in the backbuffer rect (world coordinates).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom) final;

    private:
        /// The image pixmap to draw.
        QPixmap m_image;
    };
}
