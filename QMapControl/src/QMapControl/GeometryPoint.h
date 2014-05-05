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

// Qt includes.
#include <QWidget>

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"
#include "Point.h"

namespace qmapcontrol
{
    //! A geometric point to draw objects into maps.
    /*!
     * This class can be used to draw your custom QPixmap or other QWidgets (see GeometryPointWidget) into maps.
     * You can instantiate a Point with any Pixmap you want. The objects cares about collision detection (for clickable objects).
     *
     * Points emit click events, if the containing layer receives clickevents (the default).
     *
     * To create "zoomable objects" (objects that increases size on zooming), a base level has to be set.
     * The base level is the zoom level on which the point´s pixmap gets displayed on full size.
     * On lower zoom levels it gets displayed smaller and on higher zoom levels larger.
     * A minimal size can be set as well as a maximum size.
     * @see setBaselevel, setDrawMinimumPx, setDrawMaximumPx.
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
         * This constructor creates a point which will display the given pixmap/point.
         * @param longitude The longitude coordinate (x).
         * @param latitude The latitude coordinate (y).
         * @param pixmap The pixmap to be displayed by this point.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const qreal& longitude, const qreal& latitude, const QPixmap& pixmap = QPixmap(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display the given pixmap/point.
         * @param point_coord The longitude/latitude coordinate (x/y).
         * @param pixmap The pixmap to be displayed by this point.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const PointWorldCoord& point_coord, const QPixmap& pixmap = QPixmap(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPoint(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPoint& operator=(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPoint() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the longitude/latitude coordinate (x/y).
         * @return the longitude/latitude coordinate (x/y).
         */
        PointWorldCoord coord() const;

        /*!
         * Set the longitude/latitude coordinate (x/y).
         * @param point The longitude/latitude coordinate (x/y) to set.
         */
        void setCoord(const PointWorldCoord& point);

        /*!
         * Fetches the geometry's pixmap.
         * @return the geometry's pixmap.
         */
        QPixmap& getPixmap() const;

        /*!
         * Set the pixmap to draw.
         * @param pixmap The pixmap to draw.
         */
        void setPixmap(const std::shared_ptr<QPixmap>& pixmap);

        /*!
         * Set the pixmap to draw.
         * @param pixmap The pixmap to draw.
         */
        void setPixmap(const QPixmap& pixmap);

        /*!
         * Sets the pen to draw the geometry with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPen(const std::shared_ptr<QPen>& pen) final;

        /*!
         * Sets the pen to draw the geometry with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPen(const QPen& pen) final;

        /*!
         * Sets the brush to draw the geometry with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrush(const std::shared_ptr<QBrush>& brush) final;

        /*!
         * Sets the brush to draw the geometry with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrush(const QBrush& brush) final;

        /*!
         * Set the alignment type to use when drawing the geometry pixmap.
         * @param alignment_type The alignment type to set.
         */
        void setAlignmentType(const AlignmentType& alignment_type = AlignmentType::Middle);

        /*!
         * Fetches the rotation of the point/pixmap (degrees).
         * @return the rotation of the point/pixmap (degrees).
         */
        qreal getRotation() const;

        /*!
         * Set the rotation of the point/pixmap (degrees).
         * @note Rotation will be applied to the center of the pixmap.
         * @param rotation The rotation to set (degrees).
         */
        void setRotation(const qreal& rotation);

        /*!
         * Use this method to set a zoom level on which the pixmap will be drawn at its real size.
         * Set to '-1' to always drawn at its real size.
         * On lower zoom levels, it will be drawn smaller.
         * On higher zoom levels, it will be drawn larger.
         * @see setMinsize, setMaxsize
         * @param zoom The zoom level where the pixmap will be drawn at its real size.
         */
        void setBaseZoom(const int& zoom = -1);

        /*!
         * Set the minimum size a pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The minimum size to draw a pixmap.
         */
        void setDrawMinimumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Set the maximum size a pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The maximal size to draw a pixmap.
         */
        void setDrawMaximumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

    public:
        /*!
         * The bounding box in world coordinates.
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        virtual RectWorldCoord boundingBox(const int& controller_zoom) const override;

        /*!
         * Checks if the geometry touches (intersects) with another geometry.
         * @param geometry_coord The geometry to check against.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometries touch (intersects).
         */
        virtual bool touches(const Geometry* geometry_coord, const int& controller_zoom) const override;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_coord Only draw geometries that are contained in the backbuffer rect (coord).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom) override;

    protected:
        /*!
         * Updates the pixmap.
         */
        virtual void updatePixmap();

    private:
        //! Disable copy constructor.
        GeometryPoint(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPoint& operator=(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

        /*!
         * Calculates the geometry size (widget/pixmap) in pixels for the given controller zoom.
         * @param controller_zoom The current controller zoom.
         * @return the geometry size (widget/pixmap) in pixels.
         */
        QSizeF calculateGeometrySizePx(const int& controller_zoom) const;

    private:
        /// The x/y coordinate (longitude/latitude).
        PointWorldCoord m_point_coord;

        /// The associated pixmap to draw.
        std::shared_ptr<QPixmap> m_pixmap;

        /// The alignment type to use when drawing.
        AlignmentType m_alignment_type;

        /// The rotation of the point/pixmap.
        qreal m_rotation;

        /// The base zoom level.
        int m_base_zoom;

        /// The base zoom size (must be set when widget/pixmap is set!).
        QSizeF m_base_size_px;

        /// The minimum size a point/widget/pixmap can be drawn in pixels.
        QSizeF m_draw_minimum_px;

        /// The maximum size a point/widget/pixmap can be drawn in pixels.
        QSizeF m_draw_maximum_px;
    };
}
