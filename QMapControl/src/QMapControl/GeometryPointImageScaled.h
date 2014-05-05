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
#include "GeometryPointShapeScaled.h"

namespace qmapcontrol
{
    //! Draws a scaled image at a specific point onto the map.
    /*!
     * This class draws a scaled image at a specific point onto the map.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPointImageScaled : public GeometryPointShapeScaled
    {
    public:
        //! Constructor.
        /*!
         * This constructor creates a point which will display an image pixmap.
         * @param point_coord The point to draw the image at (world coordinates).
         * @param image The image to draw.
         * @param base_size_px The size of the shape (pixels).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImageScaled(const PointWorldCoord& point_coord, const std::shared_ptr<QPixmap>& image, const int& base_zoom, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display an image pixmap.
         * @param point_coord The point to draw the image at (world coordinates).
         * @param image The image to draw.
         * @param base_size_px The size of the shape (pixels).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImageScaled(const PointWorldCoord& point_coord, const QPixmap& image, const int& base_zoom, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display an image pixmap from the file.
         * @param point_coord The point to draw the image at (world coordinates).
         * @param filename The image file to draw.
         * @param base_size_px The size of the shape (pixels).
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPointImageScaled(const PointWorldCoord& point_coord, const std::string& filename, const int& base_zoom, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointImageScaled(const GeometryPointImageScaled&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointImageScaled& operator=(const GeometryPointImageScaled&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointImageScaled() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointImageScaled(const GeometryPointImageScaled&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointImageScaled& operator=(const GeometryPointImageScaled&); /// @todo remove once MSVC supports default/delete syntax.

    public:
        /*!
         * Fetches the image pixmap.
         * @return the image pixmap.
         */
        const QPixmap& image() const;

        /*!
         * Set the image pixmap to draw.
         * @param new_image The image pixmap to draw.
         * @param update_shape Whether updateShape() should be called at the end of this function.
         */
        void setImage(const std::shared_ptr<QPixmap>& new_image, const bool& update_shape = true);

        /*!
         * Set the image pixmap to draw.
         * @param new_image The image pixmap to draw.
         * @param update_shape Whether updateShape() should be called at the end of this function.
         */
        void setImage(const QPixmap& new_image, const bool& update_shape = true);

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
        std::shared_ptr<QPixmap> m_image;
    };
}
