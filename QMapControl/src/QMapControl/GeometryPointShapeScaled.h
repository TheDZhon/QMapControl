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
#include "GeometryPointShape.h"

namespace qmapcontrol
{
    //! Draws a scaled shape at a specific point onto the map.
    /*!
     * This class draws a scaled shape at a specific point onto the map. GeometryPointShape is an abstract (non-instantiable) class.
     *
     * To create scaled shape (shape that increases/decreases size on zooming) a base zoom level has to be set.
     * The base level is the zoom level on which the shape gets displayed at its real size.
     * On lower zoom levels it gets displayed smaller and on higher zoom levels larger.
     * A minimal size can be set as well as a maximum size.
     * @see setBaselevel, setDrawMinimumPx, setDrawMaximumPx.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointShapeScaled : public GeometryPointShape
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * This constructor creates a scaled shape at a specific point to be displayed.
         * @param longitude The longitude of the point to be displayed (world coordinates).
         * @param latitude The latitude of the point to be displayed (world coordinates).
         * @param base_size_px The size of the shape (pixels).
         * @param base_zoom The zoom level where the shape will be drawn at its real size (use -1 to disable).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointShapeScaled(const qreal& longitude, const qreal& latitude, const QSizeF& base_size_px, const int& base_zoom, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point to be displayed.
         * @param point_coord The point to be displayed (world coordinates).
         * @param base_size_px The size of the shape (pixels).
         * @param base_zoom The zoom level where the shape will be drawn at its real size (use -1 to disable).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointShapeScaled(const PointWorldCoord& point_coord, const QSizeF& base_size_px, const int& base_zoom, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointShapeScaled(const GeometryPointShapeScaled&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointShapeScaled& operator=(const GeometryPointShapeScaled&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointShapeScaled() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointShapeScaled(const GeometryPointShapeScaled&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointShapeScaled& operator=(const GeometryPointShapeScaled&); /// @todo remove once MSVC supports default/delete syntax.

    public:
        /*!
         * Fetches the base zoom level.
         * @return the base zoom level.
         */
        const int& baseZoom() const;

        /*!
         * Sets the zoom level that the shape will be drawn at its real size.
         * - At lower zoom levels, the shape will be drawn smaller.
         * - At higher zoom levels, the shape will be drawn larger.
         * @see setDrawMinimumPx, setDrawMaximumPx
         * @param base_zoom The zoom level where the shape will be drawn at its real size (use -1 to disable).
         */
        void setBaseZoom(const int& base_zoom = -1);

        /*!
         * Fetches the draw minimum size (pixels).
         * @return the draw minimum size (pixels).
         */
        const QSizeF& drawMinimumPx() const;

        /*!
         * Set the minimum size a shape can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The minimum size to draw a shape.
         */
        void setDrawMinimumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Fetches the draw maximum size (pixels).
         * @return the draw maximum size (pixels).
         */
        const QSizeF& drawMaximumPx() const;

        /*!
         * Set the maximum size a shape can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The maximal size to draw a shape.
         */
        void setDrawMaximumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

    public:
        /*!
         * Fetches the bounding box (world coordinates).
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        RectWorldCoord boundingBox(const int& controller_zoom) const final;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_coord Only draw geometries that are contained in the backbuffer rect (world coordinates).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom);

    private:
        /*!
         * Calculates the shape size in pixels for the given controller zoom.
         * @param controller_zoom The current controller zoom.
         * @return the shape size in pixels.
         */
        const QSizeF calculateGeometrySizePx(const int& controller_zoom) const;

    protected:
        /*!
         * \brief drawShape Draws the shape in a transformed painter according to zoom/translate/rotate status
         * A derived class must implement this method
         * \param painter
         * \param rect The rect in pixel coordinates
         */
        virtual void drawShape(QPainter &painter, const RectWorldPx &rect) = 0;

    private:
        /// The base zoom level.
        int m_base_zoom;

        /// The minimum size a shape can be drawn in pixels.
        QSizeF m_draw_minimum_px;

        /// The maximum size a shape can be drawn in pixels.
        QSizeF m_draw_maximum_px;
    };
}
