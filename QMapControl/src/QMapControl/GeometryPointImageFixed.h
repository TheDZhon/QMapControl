/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2009 Kai Winter
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
#include "GeometryPointImage.h"

namespace qmapcontrol
{
    //! Draws a fixed image into the map.
    /*!
     * This class draws a image overlay onto a map, whose upper left and lower
     * right corners lay always on the given coordinates. The methods
     * setBaselevel, setMaxsize and setMinsize have no effect for this class.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointImageFixed : public GeometryPointImage
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap from the image file to specifed fixed points.
         * @param top_left_coord The top-left x/y coordinate (longitude/latitude).
         * @param bottom_right_coord The bottom-right x/y coordinate (longitude/latitude).
         * @param filename The image file to draw.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImageFixed(const QPointF& top_left_coord, const QPointF& bottom_right_coord, const std::string& filename, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display a pixmap to specifed fixed points.
         * @param top_left_coord The top-left x/y coordinate (longitude/latitude).
         * @param bottom_right_coord The bottom-right x/y coordinate (longitude/latitude).
         * @param pixmap The pixmap to draw.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImageFixed(const QPointF& point_top_left, const QPointF& bottom_right_coord, const QPixmap& pixmap, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointImageFixed(const GeometryPointImageFixed&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointImageFixed& operator=(const GeometryPointImageFixed&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointImageFixed() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    public:
        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) override;

    private:
        //! Disable copy constructor.
        GeometryPointImageFixed(const GeometryPointImageFixed&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointImageFixed& operator=(const GeometryPointImageFixed&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The bottom-right x/y coordinate (longitude/latitude).
        QPointF m_bottom_right_coord;
    };
}
