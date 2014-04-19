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

// STL includes.
#include <memory>

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"
#include "Point.h"

namespace qmapcontrol
{
    //! A collection of Point objects to describe a polygon.
    /*!
     * A Polygon is a collection of Points. Each consecutive pair of Points defines a Line segment.
     *
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPolygon : public Geometry
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * The constructor of a Polygon takes a list of points to form a polygon.
         * @param points The list of Geometry Points.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPolygon(const std::vector<PointWorldCoord>& points, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPolygon(const GeometryPolygon&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPolygon& operator=(const GeometryPolygon&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPolygon() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the list of points that form the polygon.
         * @return the list of points that form the polygon.
         */
        std::vector<PointWorldCoord> points() const;

        /*!
         * Sets the points to use for the Polygon.
         * @param points The points to use for the Polygon.
         */
        void setPoints(const std::vector<PointWorldCoord>& points);

        /*!
         * Fetches the QPolygonF representation of the polygon.
         * @return the QPolygonF representation.
         */
        const QPolygonF toQPolygonF() const;

    public:
        /*!
         * The bounding box in world coordinates.
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        RectWorldCoord boundingBox(const int& controller_zoom) const final;

        /*!
         * Checks if the geometry touches (intersects) with another geometry.
         * @param geometry_coord The geometry to check against.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometries touch (intersects).
         */
        bool touches(const Geometry* geometry_coord, const int& controller_zoom) const final;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_coord Only draw geometries that are contained in the backbuffer rect (coord).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom) final;

    private:
        //! Disable copy constructor.
        GeometryPolygon(const GeometryPolygon&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPolygon& operator=(const GeometryPolygon&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The points that the polygon is made up of.
        std::vector<PointWorldCoord> m_points;

        /// The brush to use (fill).
        QBrush m_brush;
    };
}
