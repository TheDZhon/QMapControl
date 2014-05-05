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
#include "GeometryPoint.h"

namespace qmapcontrol
{
    //! Draws a shape at a specific point onto the map.
    /*!
     * This class draws a shape at a specific point onto the map. GeometryPointShape is an abstract (non-instantiable) class.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointShape : public GeometryPoint
    {
        Q_OBJECT
    protected:
        //! Constructor.
        /*!
         * This constructor creates a shape at a specific point to be displayed.
         * @param longitude The longitude of the point to be displayed (world coordinates).
         * @param latitude The latitude of the point to be displayed (world coordinates).
         * @param size_px The size of the shape (pixels).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointShape(const qreal& longitude, const qreal& latitude, const QSizeF& size_px, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a shape at a specific point to be displayed.
         * @param point_coord The point to be displayed (world coordinates).
         * @param size_px The size of the shape (pixels).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointShape(const PointWorldCoord& point_coord, const QSizeF& size_px, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

    public:
        //! Disable copy constructor.
        ///GeometryPointShape(const GeometryPointShape&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointShape& operator=(const GeometryPointShape&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointShape() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointShape(const GeometryPointShape&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointShape& operator=(const GeometryPointShape&); /// @todo remove once MSVC supports default/delete syntax.

    public:
        /*!
         * Sets the pen to draw the shape with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPen(const std::shared_ptr<QPen>& pen) final;

        /*!
         * Sets the pen to draw the shape with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPen(const QPen& pen) final;

        /*!
         * Sets the brush to draw the shape with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrush(const std::shared_ptr<QBrush>& brush) final;

        /*!
         * Sets the brush to draw the shape with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrush(const QBrush& brush) final;

        /*!
         * Fetches the size of the shape (pixels).
         * @return the size of the shape (pixels).
         */
        const QSizeF& sizePx() const;

        /*!
         * Sets the size of the shape (pixels).
         * @param size_px The size of the shape to set (pixels).
         * @param update_shape Whether updateShape() should be called at the end of this function.
         */
        void setSizePx(const QSizeF& size_px, const bool& update_shape = true);

        /*!
         * Fetches the alignment type to use when drawing the shape.
         * @return the alignment type to use when drawing the shape.
         */
        const AlignmentType& alignmentType() const;

        /*!
         * Sets the alignment type to use when drawing the shape.
         * @param alignment_type The alignment type to set.
         * @param update_shape Whether updateShape() should be called at the end of this function.
         */
        void setAlignmentType(const AlignmentType& alignment_type = AlignmentType::Middle, const bool& update_shape = true);

        /*!
         * Fetches the rotation of the shape (degrees).
         * @return the rotation of the shape (degrees).
         */
        const qreal& rotation() const;

        /*!
         * Sets the rotation of the shape (degrees).
         * @param rotation The rotation to set (degrees).
         * @param update_shape Whether updateShape() should be called at the end of this function.
         * @note Rotation will be applied to the center of the shape.
         */
        void setRotation(const qreal& rotation, const bool& update_shape = true);

    public:
        /*!
         * Fetches the bounding box (world coordinates).
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        virtual RectWorldCoord boundingBox(const int& controller_zoom) const override;

    protected:
        /*!
         * Updates the shape.
         */
        virtual void updateShape();

    private:
        /// The shape size (pixels).
        QSizeF m_size_px;

        /// The alignment type to use when drawing the shape.
        AlignmentType m_alignment_type;

        /// The rotation of the shape (applied to the center of the shape).
        qreal m_rotation;
    };
}
