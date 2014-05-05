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

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"
#include "Point.h"

namespace qmapcontrol
{
    //! Draws a point onto the map.
    /*!
     * This class draws a point onto the map.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPoint : public Geometry
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * This constructor creates a point to be displayed.
         * @param longitude The longitude of the point to be displayed (world coordinates).
         * @param latitude The latitude of the point to be displayed (world coordinates).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const qreal& longitude, const qreal& latitude, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point to be displayed.
         * @param point_coord The point to be displayed (world coordinates).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const PointWorldCoord& point_coord, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPoint(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPoint& operator=(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPoint() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPoint(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPoint& operator=(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

    public:
        /*!
         * Fetches the point to be displayed (world coordinates).
         * @return the point to be displayed (world coordinates).
         */
        const PointWorldCoord& coord() const;

        /*!
         * Set the point to be displayed (world coordinates).
         * @param point The point to set (world coordinates).
         */
        void setCoord(const PointWorldCoord& point);

    public:
        /*!
         * Fetches the bounding box (world coordinates).
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        virtual RectWorldCoord boundingBox(const int& controller_zoom) const override;

        /*!
         * Checks if the geometry touches (intersects) with another geometry.
         * @param geometry The geometry to check against.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometries touch (intersects).
         */
        virtual bool touches(const Geometry* geometry, const int& controller_zoom) const override;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_coord Only draw geometries that are contained in the backbuffer rect (world coordinates).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom) override;

    private:
        /// The point to be displayed (world coordinates).
        PointWorldCoord m_point_coord;

    };
}
