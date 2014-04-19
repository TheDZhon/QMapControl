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
    //! A geometric point to draw widgets into maps.
    /*!
    * This class can be used to draw your QWidgets into maps.
    *
    * But keep in mind, that widgets always are drawn on top of all layers.
    * You also have to handle click events yourself.
    *
    * @author Kai Winter <kaiwinter@gmx.de>
    * @author Chris Stylianou <chris5287@gmail.com>
    */
    class QMAPCONTROL_EXPORT GeometryWidget : public Geometry
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
        * This constructor creates a point which will display the given widget.
        * @note IMPORTANT: You have to set the QMapControl as parent to the widget before this is constructed.
        * @note IMPORTANT: You have to set the size of the widget before this is constructed.
        * @param longitude The longitude coordinate (x).
        * @param latitude The latitude coordinate (y).
        * @param widget The widget to be displayed by this point.
        * @param zoom_minimum The minimum zoom level to show this geometry at.
        * @param zoom_maximum The maximum zoom level to show this geometry at.
        */
        GeometryWidget(const qreal& longitude, const qreal& latitude, QWidget* widget, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
        * This constructor creates a point which will display the given widget.
        * @note IMPORTANT: You have to set the QMapControl as parent to the widget before this is constructed.
        * @note IMPORTANT: You have to set the size of the widget before this is constructed.
        * @param point_coord The longitude/latitude coordinate (x/y).
        * @param widget The widget to be displayed by this point.
        * @param zoom_minimum The minimum zoom level to show this geometry at.
        * @param zoom_maximum The maximum zoom level to show this geometry at.
        */
        GeometryWidget(const PointWorldCoord& point_coord, QWidget* widget, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointWidget(const GeometryPointWidget&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointWidget& operator=(const GeometryPointWidget&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryWidget() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

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
         * Fetches the widget.
         * @return The geometry's widget.
         */
        QWidget* getWidget() const;

        /*!
         * Set the visibility of the geometry.
         * @param enabled Whether to make the geometry visible.
         */
        virtual void setVisible(const bool& enabled) final;

        /*!
         * Set the alignment type to use when drawing the geometry.
         * @param alignment_type The alignment type to set.
         */
        void setAlignmentType(const AlignmentType& alignment_type = AlignmentType::Middle);

        /*!
         * Use this method to set a zoom level on which the pixmap will be drawn at its real size.
         * Set to '-1' to always drawn at its real size.
         * On lower zoom levels, it will be drawn smaller.
         * On higher zoom levels, it will be drawn larger.
         * @see setMinsize, setMaxsize
         * @param zoom The zoom level where the point will be drawn at its real size.
         */
        void setBaseZoom(const int& zoom = -1);

        /*!
         * Set the minimum size a point/widget/pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The minimum size to draw a point/widget/pixmap.
         */
        void setDrawMinimumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Set the maximum size a point/widget/pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The maximal size to draw a point/widget/pixmap.
         */
        void setDrawMaximumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Draws the geometry's widget to the screen.
         * @param offset_px The offset in pixels to remove from the coordinate pixel point.
         * @param controller_zoom The current controller zoom.
         */
        void moveWidget(const PointPx& offset_px, const int& controller_zoom);

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
        /*!
         * Calculates the geometry size (widget/pixmap) in pixels for the given controller zoom.
         * @param controller_zoom The current controller zoom.
         * @return the geometry size (widget/pixmap) in pixels.
         */
        QSizeF calculateGeometrySizePx(const int& controller_zoom) const;

    private:
        //! Disable copy constructor.
        GeometryWidget(const GeometryWidget&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryWidget& operator=(const GeometryWidget&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The x/y coordinate (longitude/latitude).
        PointWorldCoord m_point_coord;

        /// The associated widget to draw.
        QWidget* m_widget;

        /// The alignment type to use when drawing.
        AlignmentType m_alignment_type;

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
